#ifndef _K_USEITEM_PARAM_PARSER_H_
#define _K_USEITEM_PARAM_PARSER_H_

#include <System/KPlatform.h>
#include <System/Misc/StrUtil.h>
#include <System/Memory/KBuffer64k.h>
#include <System/Collections/KMapByVector.h>
#include "Inc/Lua/KLuaWrap.h"

class KUseItem;

namespace KItemAbout
{

class KUseItemParamParser
{
public:
	KUseItemParamParser();
	~KUseItemParamParser();
	static KUseItemParamParser& GetInstance();

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
	BeginDefLuaClass(KUseItemParamParser)
		DefMemberFunc(GetInteger)
		DefMemberFunc(GetFloat)
		DefMemberFunc(GetString)
		DefMemberFunc(GetParamCount)
		DefMemberFunc(RemoveAttr)
		DefMemberFunc2(ToString_lua,"ToString")
	EndDef
};
};
#endif
