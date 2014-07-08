#ifndef _KBATTLEAI_H_
#define _KBATTLEAI_H_

#include "KBattleGuy.h"
#include "System/Memory/KStepObjectPool.h"
#include "KCardInst.h"

class KBattleAI : public KBattleGuy , public System::Memory::KPortableStepPool<KBattleAI,128>
{
public:
	static KBattleAI* Alloc()
	{
		return System::Memory::KPortableStepPool<KBattleAI,128>::Alloc();
	}
	static void Free(KBattleAI* p)
	{
		System::Memory::KPortableStepPool<KBattleAI,128>::Free(p);
	}
	virtual void free(){ Free(this);}
public:
	KBattleAI();
	 static KBattleAI* create(void);
	virtual void onPlayCard(float dt,bool );
	virtual bool init(void);
	float CalcUseRangeSkillGood(KCardInst* pCard,KAbilityStatic*,KCardInst* pDes=NULL);
	float CalcUseTargetSkillGood(KCardInst* pCard,KAbilityStatic* pAbility,int& target);
	float CalcBlessHp(KCardInst* pCard,KAbilityStatic* pAbility,KCardInstList* lstMy);
	bool IsUseSoldierAbilityGood(KCardInst* pCard,int& target);
	virtual void onBattleInit(bool bFirst);
private:
	KCardInst* ThinkUseSkillCard(KCardInst* card,float& retVal);
	KCardInst* SoldierScriptAtk(KCardInst* pAtk,KCardInstList* lst);
	void ThinkToPlay(float dt);
	bool ThinkePlayCard();
	virtual void SelectHandCard();
	bool HandCardToField();
	bool SoldierToAttack();
	bool UseSkillCard();
    bool UseSecretCard();
	float m_thinkElapsed;

};

#endif // __HELLOWORLD_SCENE_H__
