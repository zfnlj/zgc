#ifndef _K_USEITEM_ATTRS_H_
#define _K_USEITEM_ATTRS_H_

#include "Inc/Lua/KLuaWrap.h"
#include "Inc/KCommonDefine.h"


class KUseItemAttr
{
public:
	enum
	{
		itemId,
		type,
		attrCount,	
	};

	// 任务目标类型, for 宗门任务
	enum {
		ProfTypeBegin,	// 开始
		Explore,		// 探知任务
		Career,			// 职业任务

		typeCount,
	};

	static const char* m_nameTable[attrCount];
	static const char* GetAttrname(int attrId);
	static int GetAttrId(const char* attrName);
	static const char* GetTypeAttrname(int attrId);
	static int GetTypeAttrId(const char* attrName);
	static const char* m_typeTable[typeCount];
};

#endif
