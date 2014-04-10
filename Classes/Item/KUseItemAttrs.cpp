#include <System/KPlatform.h>
#include "KUseItemAttrs.h"

#define __str__(x) #x

const char* KUseItemAttr::m_nameTable[KUseItemAttr::attrCount] = 
{
	__str__(itemId),
	__str__(type),
};

const char* KUseItemAttr::m_typeTable[KUseItemAttr::typeCount] = 
{
	__str__(ProfTypeBegin),
	__str__(Explore),
	__str__(Career),
};

const char* KUseItemAttr::GetAttrname(int attrId)
{
	if(attrId < 0 || attrId >= attrCount)
		return NULL;
	return m_nameTable[attrId];
}

int KUseItemAttr::GetAttrId(const char* attrName)
{
	for(int i=0; i<attrCount; i++)
	{
		const char* n = m_nameTable[i];
		if(_stricmp(n, attrName) == 0)
			return i;
	}
	return -1;
}

const char* KUseItemAttr::GetTypeAttrname( int attrId )
{
	return m_typeTable[attrId];
}

int KUseItemAttr::GetTypeAttrId( const char* attrName )
{
	for(int i=0; i<typeCount; i++)
	{
		const char* n = m_typeTable[i];
		if(_stricmp(n, attrName) == 0)
			return i;
	}
	return -1;
}