#include "KQuestParamParser.h"
#include <System/Memory/KBuffer64k.h>
#include <System/Misc/StrUtil.h>
#include "KQuestNew.h"
#include "KQuestTarget.h"
#include <System/File/KFile.h>
#include <System/File/KTextReader.h>
#include "../WorldObject/KPlayer.h"
#include "System/File/KFile.h"
#include <System/Cipher/jgencryptor.h>
//#include <JgStreamWrapper.h>

using namespace System::File;

KQuestParamParser::KQuestParamParser()
{
}

KQuestParamParser::~KQuestParamParser()
{
}

KQuestParamParser& KQuestParamParser::GetInstance()
{
	static KQuestParamParser* p = new KQuestParamParser();
	return *p;
}

BOOL KQuestParamParser::Parse(const char* paramStr)
{
	m_paramCount = 0;

	int len = (int)strlen(paramStr);
	memcpy(m_buf, paramStr, len);
	m_buf[len] = '\0';

	char* ss[64];
	char* keyVal[2];

	int ns = split(m_buf, ";", ss, 64);

	for(int i=0; i<ns; i++)
	{
		char* s = _trim(ss[i]);
		if(!s[0]) continue;
		int n = split(s, "=", keyVal, 2);
		if(n != 2) return -1;

		char* key = _trim(keyVal[0]);
		char* val = _trim(keyVal[1]);

		m_params[m_paramCount++] = key;
		m_params[m_paramCount++] = val;
	}

	if (m_paramCount & 1) return FALSE;

	m_paramCount >>= 1;
	return TRUE;
}

char* KQuestParamParser::GetString(const char* name)
{
	for(int i=0; i<m_paramCount; i++)
	{
		char* n = m_params[2*i];
		char* v = m_params[2*i+1];
		if(stricmp(n, name) == 0)
		{
			return v;
		}
	}
	return NULL;
}

int KQuestParamParser::GetInteger(const char* name, int defVal)
{
	char* v = this->GetString(name);
	if(!v) return defVal;
	if(!isnumber3(v)) return defVal;
	return str2int(v);
}

float KQuestParamParser::GetFloat(const char* name, float defVal)
{
	char* v = this->GetString(name);
	if(!v) return defVal;
	if(!isfloat(v)) return defVal;
	return (float)strtod(v, NULL);
}

int KQuestParamParser::GetParamCount() const
{
	return m_paramCount;
}

BOOL KQuestParamParser::GetParameter(int paramIdx, char*& name, char*& val)
{
	if(paramIdx < 0 || paramIdx >= m_paramCount) return FALSE;
	int ndx = paramIdx * 2;
	int vdx = ndx + 1;
	name = m_params[ndx];
	val = m_params[vdx];
	return TRUE;
}

BOOL KQuestParamParser::RemoveAttr(const char* name)
{
	for(int i=0; i<m_paramCount; i++)
	{
		char* n = m_params[2*i];
		char* v = m_params[2*i+1];
		if(stricmp(n, name) == 0)
		{
			void* pDst = &m_params[2*i];
			void* pSrc = (char*)pDst + 2*sizeof(char*);
			int len = (m_paramCount - (i+1)) * 2 * sizeof(char*);
			if(len > 0) memmove(pDst, pSrc, len);
			m_paramCount -= 1;
			return TRUE;
		}
	}
	return FALSE;
}

const char* KQuestParamParser::ToString(char* buf, int len) const
{
	buf[0] = '\0';

	char* p = buf;
	for(int i=0; i<m_paramCount; i++)
	{
		char* n = m_params[2*i];
		char* v = m_params[2*i+1];

		if(i)
		{
			p[0] = ';';
			p += 1;
		}

		int length = sprintf(p, "%s=%s", n, v);
		p += length;
	}

	return buf;
}

LuaString KQuestParamParser::ToString_lua()
{
	char buf[512];
	return LuaString(this->ToString(buf, 512));
}

KQuestLuaParser::KQuestLuaParser():m_pBuffer(NULL)
{

}

KQuestLuaParser::~KQuestLuaParser()
{
	if(m_pBuffer)
	{
		KBuffer64k::Free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

const char* KQuestLuaParser::ParseQuestString(KPlayer* pPlayer, KQuestNew* pQuest, const char* s)
{
	if(!m_pBuffer)
	{
		m_pBuffer = KBuffer64k::Alloc();
	}

	m_pBuffer->Reset();

	int len = strlen(s);
	char* str = m_pBuffer->GetWriteBuffer();
	memcpy(str, s, len+1);

	int pos;
	while(pos = stristr(str, "<lua>"), pos >= 0)
	{
		pos += 5;
		int epos = stristr(str, "</lua>");
		if(epos < 0) return NULL;

		char* luaString = str + pos; str[epos] = '\0';
		this->escapeLuaString(luaString, strlen(luaString));
		const char* ns = this->EvalQuestString(pPlayer, pQuest, luaString);

		ASSERT_I(ns);
		if(!ns) ns = "<NULL>";
		strrep(str, 64*1024, pos-5, epos+6-(pos-5), ns);

		str += pos - 5 + strlen(ns);
	}

	return m_pBuffer->GetReadBuffer();
}

const char* KQuestLuaParser::ParseTargetString(KPlayer* pPlayer, KQuestTarget* pTarget, const char* s)
{
	if(!m_pBuffer)
	{
		m_pBuffer = KBuffer64k::Alloc();
	}

	m_pBuffer->Reset();

	int len = strlen(s);
	char* str = m_pBuffer->GetWriteBuffer();
	memcpy(str, s, len+1);

	int pos;
	while(pos = stristr(str, "<lua>"), pos >= 0)
	{
		pos += 5;
		int epos = stristr(str, "</lua>");
		if(epos < 0) return NULL;

		char* luaString = str + pos; str[epos] = '\0';

		this->escapeLuaString(luaString, strlen(luaString));
		const char* ns = this->EvalTargetString(pPlayer, pTarget, luaString);

		ASSERT_I(ns);
		if(!ns) ns = "<NULL>";
		strrep(str, 64*1024, pos-5, epos+6-(pos-5), ns);

		str += pos - 5 + strlen(ns);
	}
	
	return m_pBuffer->GetReadBuffer();
}

const char* KQuestLuaParser::escapeLuaString(char* str, int len)
{
	int i = 0;
	char prev = '\0';
	BOOL quoted = FALSE;

	while(i < len)
	{
		char chr = str[i];
		if(quoted)
		{
			if(prev != '\\' && chr == '\"')
			{
				quoted = FALSE;
			}
			i++; prev = chr;
			continue;
		}
		else
		{
			if(prev != '\\' && chr == '\"')
			{
				quoted = TRUE;
				i++; prev = chr;
				continue;
			}

			if(prev == '\\' && chr == 'n')
			{
				str[i-1] = ' '; str[i] = '\n';
				prev = '\n';
			}
			else
			{
				prev = chr;
			}

			i++; continue;
		}
	}

	return str;
}

const char* KQuestLuaParser::EvalQuestString(KPlayer* pPlayer, KQuestNew* pQuest, const char* luaString)
{
	/*const char* s = LuaWraper.callAnonymous<const char*,KPlayer*,KQuestNew*>(
		luaString,
		"player", pPlayer,
		"quest", pQuest);
		
	return s;*/
	return "";
}

const char* KQuestLuaParser::EvalTargetString(KPlayer* pPlayer, KQuestTarget* ptarget, const char* luaString)
{
	//const char* s = LuaWraper.callAnonymous<const char*,KPlayer*,KQuestTarget*>(
	//	luaString,
	//	"player", pPlayer,
	//	"target", ptarget);
	//if(!s) return NULL;
	//return s;
	return "";
}

KTraceTextResolve::KTraceTextResolve()
{

}

KTraceTextResolve::~KTraceTextResolve()
{

}

KTraceTextResolve& KTraceTextResolve::GetInstance()
{
	static KTraceTextResolve* m_inst = new KTraceTextResolve();
	return *m_inst;
}

int KTraceTextResolve::Resolve(char* s, int len)
{
	int tokenl;
	char* ss[16];
	int retl = len;

	char* org = s;
	char* pToken = NULL;

	while(tokenl = this->FindTokenString(s, len, pToken), tokenl)
	{
		LuaString token(pToken+4, tokenl-5);
		char* ps = (char*)token.c_str();
		
		int ns = split(ps, ',', ss, 16);
		if(ns < 1)
		{
			s = pToken + tokenl;
			len = strlen(s);
			continue;
		}

		if(stricmp(ss[0], "Tonpc") == 0)
		{
			LuaString srep = this->ResolveToNpc(&ss[1], ns-1);
			if(strrep(s, len, int(pToken-s), tokenl, srep.c_str()))
			{
				retl = retl - tokenl + srep.size();
				s = pToken + srep.size();
				len = strlen(s);
			}
		}
		else if(stricmp(ss[0], "Topname") == 0)
		{
			LuaString srep = this->ResolveToPos(&ss[1], ns-1);
			if(strrep(s, len, int(pToken-s), tokenl, srep.c_str()))
			{
				retl = retl - tokenl + srep.size();
				s = pToken + srep.size();
				len = strlen(s);
			}
		}
		else
		{
			s = pToken + tokenl;
			len = strlen(s);
			continue;
		}
	}

	org[retl] = '\0';
	return retl;
}

LuaString KTraceTextResolve::ResolveToNpc(char** ss, int c)
{
	DWORD npcType = str2int(ss[0]);
	
	// todo:
	//const char* npcName = GetNpcName(npcType, true);
	const char* npcName = "";
	
	if(!npcName)
	{
		LuaString s("Npc:");
		s.append(ss[0]);
		return s;
	}

	return LuaString(npcName);
}

LuaString KTraceTextResolve::ResolveToPos(char** ss, int c)
{
	return LuaString(ss[0]);
}

//<RB=Tonpc,1002,3>
//<RB=Topname,¶¬¶ù,1030,3050>
int KTraceTextResolve::FindTokenString(char* s, int len, char*& pToken)
{
	int pos = stristr(s, "<RB=");
	if(pos < 0) return 0;

	int i = pos;
	while(i < len)
	{
		char ch = s[i];
		if(ch == '>')
		{
			pToken = s + pos;
			return (i + 1) - pos;
		}
		i += 1;
	}
	return 0;
}
