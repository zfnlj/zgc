#ifndef _KCARDINST_H_
#define _KCARDINST_H_

#include "../StaticTable/KGameStatic.h"
#include "../StaticTable/KAbilityStatic.h"
#include "System/Memory/KStepObjectPool.h"
#include <System/Collections/KList.h>
#include "KCardAttr.h"
#include "Inc/Lua/KLuaWrap.h"
#ifdef _USE_COCOS2DX
#include "cocos2d.h"

USING_NS_CC;
#endif
class KCardStatic;
class KBattleCtrlBase;
class KMemoryStream;
class KBattleGuy;
class KCardInst :public System::Memory::KPortableStepPool<KCardInst,128>
{
public:

	enum CardSlot{  //槽位
		enum_slot_null=0, 
		enum_slot_slot=1, //待抓牌
		enum_slot_select=2, //待选
		enum_slot_hand=3, //手牌
		enum_slot_equip=4, //装备
		enum_slot_fight=5, //战斗
		enum_slot_hero=6,  //英雄
		enum_slot_tomb=7,  //坟场
        enum_slot_secret=8,
	};
	~KCardInst();
	static KCardInst* create();
	static void Free(KCardInst* p)
	{
		p->Clear();
		System::Memory::KPortableStepPool<KCardInst,128>::Free(p);
	}

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	void init(int realId,KBattleGuy* owner,int cardId);
	virtual bool init();
	void clone(KCardInst*);
	int GetRealId(){ return m_attr.getRealID();}
	int GetCardId() const { return m_pST->GetID();}
	int GetCost(); //费用
	int GetRealCost();
	int GetAtk(); //攻击力
	CardSlot GetSlot(){ return (CardSlot)m_attr.getSlot();}
	int GetOldSlot(){ return m_attr.getOldSlot();}
//	
//	CC_SYNTHESIZE(CardSlot,m_slot,Slot);
//	CC_SYNTHESIZE(bool,m_bReady,Ready);//是否就绪，回合开始时战斗牌进入就绪，执行一次攻击或施法，结束就绪
	void EnterFightField(int pos); //进入战斗区
	void onSwitchFightField(int pos);
	void EnterSecretField(int pos);
	void onTurnBegin(KBattleCtrlBase*);     //回合开始
	void OnTurnEnd(KBattleCtrlBase* ctrl);       //回合结束
	void onCard2Tomb();
	bool IsDead();
	int GetHp();
	void AddHp(int val);
	int Heal(KCardInst* pSrc,int val);
	float GetAtkedVal(int damage);
	void HpSet(int val);
	void CurHpSet(int cur);
	void AtkSet(int val);
	int GetLostHp(){ return m_attr.getMaxHp()-m_attr.getCurHp();}
	void AddAtk(int val);
	void HpDouble();
	void DoRush();
	KCardStatic* GetST(){ return m_pST;} //牌的静态信息
	KCardStatic::CardDef GetKind(){ return m_pST->GetType();}
	bool IsKindOf(KCardStatic::CardDef def) { return  (m_pST->GetType()==def);}
	bool IsActiveDefend();
	void AddBuf(KAbilityStatic* pBuf,int loopNum);
	void AddBuf(int id);
	void DelBuf(KAbilityStatic* pBuf);
	void ClearBuf();
	void DispleBuf();
	void DispleGoodBuf();
	bool HasBuf(KAbilityStatic* pBuf);
	int	 CalcGoodBufVal();
	void ReturnHand();
	KAbilityStatic* FindBuf(KAbilityStatic::Enum_What what);
	//KAbilityStatic* FindBufAbility(KAbilityStatic::Enum_What what);
	KAbilityStatic* FindBuf(KAbilityStatic::Enum_When when);
	KAbilityStatic* GetSecretAbility();
	void ReplaceST(int id);
	const char* GetBasePosName(bool bMy);
	bool IsRangeAbility(KAbilityStatic::Enum_When when);
	bool IsTargetLess(KAbilityStatic::Enum_When when);
	void onCardAbility(KBattleCtrlBase*,KAbilityStatic::Enum_When when);
	KBattleGuy* GetOwner(){ return m_Owner;}
	void SetOwner(KBattleGuy* guy){ m_Owner = guy;}
#ifdef _USE_COCOS2DX
	CC_SYNTHESIZE(CCObject*,m_actor,Actor);
	void retainActor(CCObject* actor);
	void releaseActor();
#endif
	void Clear();
	bool IsDirty(){ return m_attr.IsDirty();}
	size_t serialize(KMemoryStream* so);
	size_t serializeDirty(KMemoryStream* so);
	BOOL deserialize(KMemoryStream* si);
	KCardAttr m_attr;
	bool IsClone(){ return m_bClone;}
private:
	KCardStatic* m_pST;
	KBattleGuy* m_Owner;
	bool m_bClone;
public:
BeginDefLuaClass(KCardInst)
	DefMemberFunc(GetHp)
	DefMemberFunc(GetAtk)
	DefMemberFunc(GetRealId)
EndDef
};

typedef std::list<KCardInst*> KCardInstList;

int _getIndexOfCard(KCardInstList* lst,KCardInst* card);
void _copyCardSet(KCardInstList* src,KCardInstList* des,KCardInst* skip=NULL,KCardInst::CardSlot slot=KCardInst::enum_slot_null);
KCardInstList::iterator _findCardIt(KCardInstList* lst,KCardInst* card);
void _clearCardList(KCardInstList* lst);
int _getDirtyCardCount(KCardInstList* lst);
void _fillDirtyCardList(KCardInstList& src,KCardInstList& des);
void _removeFromCardList(KCardInstList& src,KCardInst* card);
void _replaceFromCardList(KCardInstList& lst,KCardInst* card,KCardInst* newcard);
void _rndPickCard(KCardInstList& src,KCardInstList& des,int num,KCardStatic::CardDef cardDef);
#endif // __HELLOWORLD_SCENE_H__
