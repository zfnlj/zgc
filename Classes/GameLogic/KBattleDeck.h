#ifndef _KBATTLEDECK_H__
#define _KBATTLEDECK_H__

#include "KCardInst.h"
#include "KGameDef.h"

#define MAX_FIGHT_POS_NUM 5

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
	void OnTurnEnd();
	int  DrawCard(int id,KCardInst::CardSlot slot = KCardInst::enum_slot_hand);
	void CreateCloneCard(KCardInstList& lst,KCardInstList& newLst,KCardInst::CardSlot slot);
	KCardInst* CreateCard(int id,KCardInst::CardSlot slot);
	KCardInst* GetHero(){ return (KCardInst*)m_HeroCardSet.front();}
    KCardInst* GetSecret();
	KCardInst* GetCard(int id);
	KCardInst* GetCard(KCardInstList* arr,int id);
	KCardInst* GetHandCard(int id) { return GetCard(&m_HandCardSet,id);}

	KCardInstList* QueryCardSet(int slot);

	void GetDefenderSet(KCardInstList*);
	KCardInstList* QueryCardSet(KCardInst*);
	bool AddCard(KCardInstList* list, KCardInst*);
	void Hand2Fight(KCardInst* pCard);
	void Hand2Fight(KCardInst* pCard,int pos);
	void Hand2Secret(KCardInst* card);
	void Hand2Tomb(KCardInst* pCard);
	void Fight2Hand(KCardInst* card);
	KCardInst* SummonCard(int id);

	void FindFightingGuider(KCardInstList*);
	void onCard2Tomb(KCardInst* card);
	void onSecret2Tomb(KCardInst* card);
	void PickCard(KCardInstList* arr,KCardInst::CardSlot slot,KCardInst* skip);
	void onSelectHandCardOK(KCardInstList* lst);
	void UpdateActor(float dt);
	void UpdateActor(KCardInstList* arr,float dt);
	void QueryActiveHandCards(KCardInstList* arr,int curRes);
	void QueryActiveFightCards(KCardInstList* arr,int curRes);
	void QueryActiveDefendCards(KCardInstList* arr);

	size_t serializeDirty(KMemoryStream* so);
	BOOL deserializeDirty(KMemoryStream* si);
	size_t serialize(KMemoryStream* so);
	size_t serializeCardList(KMemoryStream* so,KCardInstList& lst);
	BOOL deserialize(KMemoryStream* si);
	BOOL deserializeCardList(KMemoryStream* si,KCardInstList& lst);
	void updateCardSlot(KCardInst* card);
	void SetCurDeckDB(int* cardStore,int* cardDeck);
	void RemoveCard(KCardInst* card);
    int GetEmptyFightSlot();
	int GetEmptyFightSlotNum();
	int GetRndEmptyFightSlot();
	bool IsEmptyFightSlot(int pos);
	void RndPickCard(KCardInstList& lst,int num,KCardInst::CardSlot slot,KCardStatic::CardDef cardDef=KCardStatic::card_null);
	void GenHandCard(int id);
private:
	KCardInstList m_HeroCardSet;
	KCardInstList	m_HandCardSet;	//����
	KCardInstList	m_FightCardSet;	//������
	KCardInstList	m_EquipCardSet; //װ����
	KCardInstList m_SlotCardSet;  //������
	KCardInstList m_TombCardSet;  //�س���
    KCardInstList m_SecretCardSet;
	KBattleGuy* m_Owner;
	int		m_CurDeckDB[MAX_GAME_PLAY_CARD];
};

#endif // __HELLOWORLD_SCENE_H__
