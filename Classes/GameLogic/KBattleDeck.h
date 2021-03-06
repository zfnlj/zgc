#ifndef _KBATTLEDECK_H__
#define _KBATTLEDECK_H__

#include "KCardInst.h"
#include "KGameDef.h"
#include "KHeroSkillMgr.h"
#include "../PlayerCard/KPlayerDeck.h"
#define MAX_FIGHT_POS_NUM 5
#define MAX_SECRET_POS_NUM 2

class KCardInst;
class KBattleGuy;
class KMemoryStream;
class KPlayerCardDepot;
struct KHeroDef;
class KBattleDeck
{
public:
	~KBattleDeck();

	bool initGuy(KBattleGuy* guy);
	void initDeck(KDeckDefStatic* pDeckDef,bool bSelectCard);
	bool createTestDeck(void);
	void createDeck(int heroId,KIntegerList& cardLst);
	void createDeck(KDeckDefStatic* pDeckStatic);
	bool createDeck(KPlayerCardDepot* pDepot);
	void Clear();
	void onTurnBegin(KBattleCtrlBase*,bool bFirstTurn);
	void OnTurnEnd(KBattleCtrlBase* ctrl);
	int  DrawCard(int id,KCardInst::CardSlot slot = KCardInst::enum_slot_hand,strCardAbilityResult* result=NULL);
	void CreateCloneCard(KCardInstList& lst,KCardInstList& newLst,KCardInst::CardSlot slot);
	KCardInst* CreateCard(int id,KCardInst::CardSlot slot);
	KCardInst* GetHero(){ return (KCardInst*)m_HeroCardSet.front();}
	KCardInst* GetCard(int id);
	KCardInst* GetCard(KCardInstList* arr,int id);
	KCardInst* GetHandCard(int id) { return GetCard(&m_HandCardSet,id);}

	bool ExistCards();
	KCardInstList* QueryCardSet(int slot);

	void GetDefenderSet(KCardInstList*);
	KCardInstList* QueryCardSet(KCardInst*);
	bool AddCard(KCardInstList* list, KCardInst*);
	void Hand2Fight(KCardInst* pCard,int pos);
	void Hand2Secret(KCardInst* card,int pos);
	void Hand2Tomb(KCardInst* pCard);
	void Fight2Hand(KCardInst* card);
	KCardInst* SummonCard(int id);

	void FindFightingGuider(KCardInstList*);
	void onCard2Tomb(KCardInst* card);
	void PickFighterNearby(KCardInstList* lst,KCardInst* card);
	void PickCard(KCardInstList* arr,KCardInst::CardSlot slot,KCardInst* skip);
	void onSelectHandCardOK(KCardInstList* lst);
	bool IsEmptyFightPos(int pos);
	void UpdateActor(float dt);
	void UpdateActor(KCardInstList* arr,float dt);
	void QueryValidateHandCards(KCardInstList* lst,int curRes);
	void QueryValidateFightCards(KCardInstList* arr,int curRes);
	void QueryActiveDefendCards(KCardInstList* arr);
	void QuerySleepFightCards(KCardInstList* lst);

	
	void updateCardSlot(KCardInst* card);
	void RemoveCard(KCardInst* card);
    int GetEmptyFightSlot();
	int GetEmptyFightSlotNum();
	int GetRndEmptyFightSlot();
	bool IsEmptyFightSlot(int pos);
	int GetEmptySecretSlot();
	void RndPickCard(KCardInstList& lst,int num,KCardInst::CardSlot slot,KCardStatic::CardDef cardDef=KCardStatic::card_null);
	int GetHurtedSoldierNum();
	int GetSoldierNum();
	KHeroSkillMgr m_heroSkillMgr;
	KHeroDef& GetHeroDef(){ return m_heroDef;}
	void SetHeroSkill();
	void TurnBeginDrawCard();
private:
	KCardInstList m_HeroCardSet;
	KCardInstList m_HandCardSet;	//����
	KCardInstList m_FightCardSet;	//������
	KCardInstList m_EquipCardSet; //װ����
	KCardInstList m_SlotCardSet;  //������
	KCardInstList m_TombCardSet;  //�س���
    KCardInstList m_SecretCardSet;
	KBattleGuy* m_Owner;
	KHeroDef m_heroDef;
friend class KSerialize;	
};

#endif // __HELLOWORLD_SCENE_H__
