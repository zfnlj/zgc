#include "KUseItemParamParser.h"
#include <System/Memory/KBuffer64k.h>
#include <System/Misc/StrUtil.h>
#include <System/File/KFile.h>
#include <System/File/KTextReader.h>
#include "../WorldObject/KPlayer.h"
#include "System/File/KFile.h"
#include <System/Cipher/jgencryptor.h>
//#include <JgStreamWrapper.h>

using namespace System::File;
namespace KItemAbout
{

KUseItemParamParser::KUseItemParamParser()
{
}

KUseItemParamParser::~KUseItemParamParser()
{
}

KUseItemParamParser& KUseItemParamParser::GetInstance()
{
	static KUseItemParamParser* p = new KUseItemParamParser();
	return *p;
}

char* KUseItemParamParser::GetString(const char* name)
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

BOOL KUseItemParamParser::Parse(const char* paramStr)
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

int KUseItemParamParser::GetInteger(const char* name, int defVal)
{
	char* v = this->GetString(name);
	if(!v) return defVal;
	if(!isnumber3(v)) return defVal;
	return str2int(v);
}

float KUseItemParamParser::GetFloat(const char* name, float defVal)
{
	char* v = this->GetString(name);
	if(!v) return defVal;
	if(!isfloat(v)) return defVal;
	return (float)strtod(v, NULL);
}

int KUseItemParamParser::GetParamCount() const
{
	return m_paramCount;
}

BOOL KUseItemParamParser::GetParameter(int paramIdx, char*& name, char*& val)
{
	if(paramIdx < 0 || paramIdx >= m_paramCount) return FALSE;
	int ndx = paramIdx * 2;
	int vdx = ndx + 1;
	name = m_params[ndx];
	val = m_params[vdx];
	return TRUE;
}

BOOL KUseItemParamParser::RemoveAttr(const char* name)
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

const char* KUseItemParamParser::ToString(char* buf, int len) const
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

LuaString KUseItemParamParser::ToString_lua()
{
	char buf[512];
	return LuaString(this->ToString(buf, 512));
}
};