#ifndef _KBATTLEFIELDSTATIC_H
#define _KBATTLEFIELDSTATIC_H

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>

class KBattleFieldStatic
{
public:
	enum enum_npc_ai{
		npc_ai_null,
		npc_ai_normal,
	};
    virtual bool init();
	static KBattleFieldStatic* create();
	int GetMyDeck(){ return m_myDeck;}
	int GetYourDeck(){ return m_yourDeck;}
	bool IsSelectCard(){ return m_selectCard>0;}
	bool IsMyFirst(){ return m_myFirst>0;}
	int GetMonster(){ return m_monster;}
	int GetQuestId(){ return m_questId;}
	void Init(System::File::KTabFile2* fileReader);
	bool IsRecEmpty(){ return strlen(m_rec)<2;}
	const char* GetRec(){ return m_rec;}
private:
	int m_Id;
	int m_myDeck;
	int m_yourDeck;
	enum_npc_ai m_yourAI;
	int m_selectCard;
	int m_myFirst;
	int m_monster;
	int m_questId;
	char m_rec[64];
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KBattleFieldStatic*> KBattleFieldStaticMap;

#endif // __HELLOWORLD_SCENE_H__
