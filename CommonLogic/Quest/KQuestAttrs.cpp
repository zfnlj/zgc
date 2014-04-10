#include <System/KPlatform.h>
#include "KQuestAttrs.h"

#define __str__(x) #x

const char* KQuestAttr::m_nameTable[KQuestAttr::attrCount] = 
{
	__str__(questId),
	__str__(nameId),
	__str__(talk),
	__str__(effect),
	__str__(type),
	__str__(level),
	__str__(view),
	__str__(abandon),
	__str__(reuse),
	__str__(share),
	__str__(acceptLevel),
	__str__(prevQuest),
	__str__(nextQuest),
	__str__(hardDegree),
	__str__(money),
	__str__(exp),
	__str__(items),
	__str__(questItems),
	__str__(desc),
	__str__(finishDesc),
	__str__(noFinishDesc),
	__str__(achieveId),
	__str__(achievement),
	__str__(userData),
	__str__(timeperiod),
	__str__(activityId),
	__str__(flag),
	__str__(acceptDialogId),
	__str__(noFinishDialogId),
	__str__(finishDialogId),
	__str__(acceptTopLimit),
	__str__(grade),
	__str__(estimateTime),
	__str__(extraDescStr),
	__str__(extraDescBGP),
	__str__(battleField),
};

const char* KQuestAttr::m_typeTable[KQuestAttr::proftypeCount] = 
{
	__str__(ProfTypeBegin),
	__str__(KillMonster),
	__str__(Delivery),
	__str__(Demand),
	__str__(Pet),
	__str__(Explore),
	__str__(Mill),
	__str__(Career),
};

const char* KQuestAttr::GetAttrname(int attrId)
{
	if(attrId < 0 || attrId >= attrCount)
		return NULL;
	return m_nameTable[attrId];
}

int KQuestAttr::GetAttrId(const char* attrName)
{
	for(int i=0; i<attrCount; i++)
	{
		const char* n = m_nameTable[i];
		if(_stricmp(n, attrName) == 0)
			return i;
	}
	return -1;
}

const char* KQuestAttr::GetTypeAttrname( int attrId )
{
	if(attrId < 0 || attrId >= proftypeCount)
		return NULL;
	return m_typeTable[attrId];
}

int KQuestAttr::GetTypeAttrId( const char* attrName )
{
	for(int i=0; i<proftypeCount; i++)
	{
		const char* n = m_typeTable[i];
		if(_stricmp(n, attrName) == 0)
			return i;
	}
	return -1;
}