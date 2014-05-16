#ifndef _KBATTLEDECK_H__
#define _KBATTLEDECK_H__

#include "KCardInst.h"
#include "KGameDef.h"


#define MAX_FIGHT_POS_NUM 5
#define MAX_SECRET_POS_NUM 2

class KCardInst;
class KBattleGuy;
class KMemoryStream;
class KBattleDeck
{
public:
	~KBattleDeck();

	bool initGuy(KBattleGuy* guy);
	bool createCards(void);
	void initDeck(KDeckDefStatic* pDeckStatic);
	void Clear();
	void onTurnBegin(KBattleCtrlBase*);
	void OnTurnEnd(KBattleCtrlBase* ctrl);
	int  DrawCard(int id,KCardInst::CardSlot slot = KCardInst::enum_slot_hand,strCardAbilityResult* result=NULL);
	void CreateCloneCard(KCardInstList& lst,KCardInstList& newLst,KCardInst::CardSlot slot);
	KCardInst* CreateCard(int id,KCardInst::CardSlot slot);
	KCardInst* GetHero(){ return (KCardInst*)m_HeroCardSet.front();}
	KCardInst* GetCard(int id);
	KCardInst* GetCard(KCardInstList* arr,int id);
	KCardInst* GetHandCard(int id) { return GetCard(&m_HandCardSet,id);}

	KCardInstList* QueryCardSet(int slot);

	void GetDefenderSet(KCardInstList*);
	KCardInstList* QueryCardSet(KCardInst*);
	bool AddCard(KCardInstList* list, KCardInst*);
	void Hand2Fight(KCardInst* pCard);
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
	void UpdateActor(float dt);
	void UpdateActor(KCardInstList* arr,float dt);
	void QueryValidateHandCards(KCardInstList* lst,int curRes);
	void QueryValidateFightCards(KCardInstList* arr,int curRes);
	void QueryActiveDefendCards(KCardInstList* arr);
	void QuerySleepFightCards(KCardInstList* lst);

	size_t serializeDirty(KMemoryStream* so);
	BOOL deserializeDirty(KMemoryStream* si);
	size_t serialize(KMemoryStream* so);
	size_t serializeCardList(KMemoryStream* so,KCardInstList& lst,bool newCard=false);
	BOOL deserialize(KMemoryStream* si);
	BOOL deserializeCardList(KMemoryStream* si,KCardInstList& lst);
	void updateCardSlot(KCardInst* card);
	void SetCurDeckDB(int* cardStore,int* cardDeck);
	void RemoveCard(KCardInst* card);
    int GetEmptyFightSlot();
	int GetEmptyFightSlotNum();
	int GetRndEmptyFightSlot();
	bool IsEmptyFightSlot(int pos);
	int GetEmptySecretSlot();
	void RndPickCard(KCardInstList& lst,int num,KCardInst::CardSlot slot,KCardStatic::CardDef cardDef=KCardStatic::card_null);
	int GetHurtedSoldierNum();
private:
	KCardInstList m_HeroCardSet;
	KCardInstList m_HandCardSet;	//手牌
	KCardInstList m_FightCardSet;	//场上牌
	KCardInstList m_EquipCardSet; //装备牌
	KCardInstList m_SlotCardSet;  //卡槽牌
	KCardInstList m_TombCardSet;  //坟场牌
    KCardInstList m_SecretCardSet;
	KBattleGuy* m_Owner;

	int		m_CurDeckDB[MAX_GAME_PLAY_CARD];
};

#endif // __HELLOWORLD_SCENE_H__
