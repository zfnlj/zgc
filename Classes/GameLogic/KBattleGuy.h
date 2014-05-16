#ifndef _KBATTLEGUY_H_
#define _KBATTLEGUY_H_

#include "KBattleDeck.h"
#include "KCardInst.h"
#include "System/Memory/KStepObjectPool.h"
#include <list>
#include "../Facade/FBattleGuy.h"
#include "KCardInst.h"
#include "KBattleGuyAttr.h"
#include "KHeroSkillMgr.h"
class KBattleCtrlBase;
class KMemoryStream;
class KBattleGuy : public System::Memory::KPortableStepPool<KBattleGuy,128>
{
public:
	 KBattleGuy();
	 

	 static KBattleGuy* Alloc()
	 {
		 return System::Memory::KPortableStepPool<KBattleGuy,128>::Alloc();
	 }
	 static void Free(KBattleGuy* p)
	 {
		 p->Clear();
		 System::Memory::KPortableStepPool<KBattleGuy,128>::Free(p);
	 }
	 virtual void free(){ Free(this);}

	 static KBattleGuy* create(void);
	 bool init(void);
	 virtual void onBattleInit(bool bFirst,int deckId,bool bSelectCard=true);
	 bool IsPlayTimeOut();
	 void SetPlayTimeOut();
	 void onTurnBegin(KBattleCtrlBase*,bool bFirstTurn);
	 void onTurnEnd(KBattleCtrlBase*);
	 virtual void onPlayCard(float dt,bool bOK);

	 int  QuerySlotCardNum();
	 int GetCurRes(){ return m_attr.getCurRes();}
	 int GetMaxRes(){ return m_attr.getMaxRes();}
	 KBattleDeck& GetDeck(){ return m_Deck;}
	 void UseRes(int val);
	 FBattleGuy* GetFacade(){ return &m_FacadeObj;}

	 virtual void SelectHandCard(){}
	 KCardInstList* QueryCardSet(int);
	 void onGameEnd();
	 bool IsSelectHandCard(){ return m_bSelectHandCard;}
	 bool IsFirstHand(){ return m_bFirstHand;}
	 int GetFighterNum();
	 UINT64 GetGuyId(){ return m_guyId;}
	 void SetGuyId(UINT64 id){ m_guyId = id;}
	 void AddRes(int val);
	 void AddRes(KBattleCtrlBase* ctrl,KAbilityStatic* pAbility);
	 virtual void update(float dt);
	 void Clear();
	 void DoSelectBeginCard(KCardInstList* arr);
	 void QueryResInfo(char* buf);
	 void QueryValidateHandCards(KCardInstList* arr);
	 void QueryValidateFightCards(KCardInstList* arr);
	 void QueryActiveDefendCards(KCardInstList* arr);
	 void QuerySleepFightCards(KCardInstList* lst);

	 void SetBattleCtrl(KBattleCtrlBase* ctrl){ m_battleCtrl = ctrl;}
	 KBattleCtrlBase* GetBattleCtrl(){ return m_battleCtrl;}

	 size_t serialize(KMemoryStream* so);
	 BOOL deserialize(KMemoryStream* si);
	 size_t serializeDirty(KMemoryStream* so);
	 BOOL deserializeDirty(KMemoryStream* si);

	 bool DoGuyAbility(KBattleCtrlBase* ctrl,KCardInst* pSrc,KAbilityStatic* pAbility,int actor);
	 void RemoveGuyAbility(KAbilityStatic* pAbility);
	 int calcHurtVal(int val);
	 int calcHealVal(int val);
	 int calcMpCost(int val);
	 //evt
	 void onCardEnterCtrl(KBattleCtrlBase*,KCardInst* card);
	 void onCardLeaveCtrl(KBattleCtrlBase*,KCardInst* card);
protected:
	float m_TurnPlayTime;
	UINT64 m_guyId;
	bool m_bSelectHandCard;
	bool m_bFirstHand;//先手
	KBattleDeck m_Deck; //牌管理
	FBattleGuy m_FacadeObj;
	KBattleCtrlBase* m_battleCtrl;
	KBattleGuyAttr m_attr;
	
	KHeroSkillMgr m_heroSkillMgr;
friend class KClientBattleCtrl;
};

typedef std::list<KBattleGuy*> KBattleGuyList;
#endif // __HELLOWORLD_SCENE_H__
