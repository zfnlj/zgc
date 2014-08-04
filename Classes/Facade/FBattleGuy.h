#ifndef _FBATTLEGUY_H_
#define _FBATTLEGUY_H_
//KBattleGuy的包装类，用于表现层访问角色信息

#include "../GameLogic/KCardInst.h"

class KBattleGuy;
class KCardInst;
class FBattleGuy
{
public:
	FBattleGuy(){}
	int  QuerySlotCardNum();
	KCardInstList* QueryCardSet(int slot);
	void SetImpl(KBattleGuy* guy){ m_Impl = guy;}
	bool IsFirstHand();
	KCardInst* GetHero();
	KCardInst* GetCard(int id);
	void Clear();
	void QueryResInfo(char* buf);
	KBattleGuy* GetImp(){ return m_Impl;}
	bool IsSelectHandCard();
	void QueryActiveDefendCards(KCardInstList* arr);
	void QueryValidateHandCards(KCardInstList* arr);
	void QueryValidateFightCards(KCardInstList* arr);
	void QuerySleepFightCards(KCardInstList* lst);
	bool IsEmptyFightSlot(int pos);
	float GetRemainPlayTime();
private:
	KBattleGuy* m_Impl;
};

#endif // __HELLOWORLD_SCENE_H__
