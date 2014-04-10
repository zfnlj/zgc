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

	// ����Ŀ������, for ��������
	enum {
		ProfTypeBegin,	// ��ʼ
		Explore,		// ֪̽����
		Career,			// ְҵ����

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
