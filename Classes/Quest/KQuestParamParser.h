#ifndef _K_QUEST_PARAM_PARSER_H_
#define _K_QUEST_PARAM_PARSER_H_

#include <System/KPlatform.h>
#include <System/Misc/StrUtil.h>
#include <System/Memory/KBuffer64k.h>
#include <System/Collections/KMapByVector.h>
#include "Inc/Lua/KLuaWrap.h"

class KQuestNew;
class KQuestTarget;

namespace KWorldObjAbout
{
	class KPlayer;
}
using namespace KWorldObjAbout;

class KQuestParamParser
{
public:
	KQuestParamParser();
	~KQuestParamParser();
	static KQuestParamParser& GetInstance();

public:
	BOOL Parse(const char* paramStr);

	char* GetString(const char* name);
	int GetInteger(const char* name, int defVal);
	float GetFloat(const char* name, float defVal);

	int GetParamCount() const;
	BOOL GetParameter(int paramIdx, char*& name, char*& val);

	BOOL RemoveAttr(const char* name);
	const char* ToString(char* buf, int len) const;
	LuaString ToString_lua();

public:
	char m_buf[2048];
	int m_paramCount;
	char* m_params[64];

public:
	BeginDefLuaClass(KQuestParamParser)
		DefMemberFunc(GetInteger)
		DefMemberFunc(GetFloat)
		DefMemberFunc(GetString)
		DefMemberFunc(GetParamCount)
		DefMemberFunc(RemoveAttr)
		DefMemberFunc2(ToString_lua,"ToString")
	EndDef
};

class KQuestLuaParser
{
public:
	KQuestLuaParser();
	~KQuestLuaParser();

public:
	const char* ParseQuestString(KPlayer* pPlayer, KQuestNew* pQuest, const char* s);
	const char* ParseTargetString(KPlayer* pPlayer, KQuestTarget* pTarget, const char* s);
	
public:
	const char* escapeLuaString(char* str, int len);
	const char* EvalQuestString(KPlayer* pPlayer, KQuestNew* pQuest, const char* luaString);
	const char* EvalTargetString(KPlayer* pPlayer, KQuestTarget* ptarget, const char* luaString);

public:
	KBuffer64k* m_pBuffer;
};

class KTraceTextResolve
{
public:
	KTraceTextResolve();
	~KTraceTextResolve();

public:
	static KTraceTextResolve& GetInstance();
	int Resolve(char* s, int len);

protected:
	LuaString ResolveToNpc(char** ss, int c);
	LuaString ResolveToPos(char** ss, int c);

public:
	//<RB=Tonpc,1002,3>
	//<RB=Topname,¶¬¶ù,1030,3050>
	int FindTokenString(char* s, int len, char*& pToken);
};

#endif
