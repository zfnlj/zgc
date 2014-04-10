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
	bool IsUseSkillGood(KCardInst* pCard,int& target);
	virtual void onBattleInit(bool bFirst);
private:
	void ThinkToPlay(float dt);
	virtual void SelectHandCard();
	bool HandCardToField();
	bool SoldierToAttack();
	bool UseSkillCard();
	bool UseHeroSkill();
    bool UseSecretCard();
	float m_thinkElapsed;

	int CalcTotalDamage(KAbilityStatic* pAbility,KCardInstList& arr,int& target);
	int CalcTotalHeal(KAbilityStatic* pAbility,KCardInstList& arr,int& target);
	int CalcTotalHpDouble(KAbilityStatic* pAbility,KCardInstList& lst,int& target);
	int CalcControlBenefit(KAbilityStatic* pAbility,KCardInstList& lst,int& target);
};

#endif // __HELLOWORLD_SCENE_H__
