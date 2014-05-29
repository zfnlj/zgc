#ifndef _KHEROSKILLMGR_H
#define _KHEROSKILLMGR_H
#include "../StaticTable/KGameStatic.h"
#include "../Inc/KTypeDef.h"
#include "../PlayerCard/KPlayerDeck.h"

class KBattleCtrlBase;
class KBattleGuy;
struct KHeroDef;
class KHeroSkillMgr
{
	struct HeroSkill{
		int _id;
		int _lev;
		KHeroSkillStatic* _skill;
		HeroSkill():_id(0),_lev(0){}
		void init(int id,int lev){
			_id = id;
			_lev = lev;
			_skill = KGameStaticMgr::getSingleton().GetHeroSkill(id);
			//CCAssert(_skill , "Hero skill is null!");
		}
		KAbilityStatic* GetRndAbility(){ return _skill->GetRndAbility();}
		int getRateVal();
		bool IsEmpty(){ return _id==0;}
		void GenDynAbility(KAbilityStatic& dynAbility);
	};
public:
	KHeroSkillMgr():m_bDone(false){}
	~KHeroSkillMgr(){}

	void onTurnBegin(KBattleCtrlBase* ctrl);
	void onTurnEnd(KBattleCtrlBase*);
	void tmpInit(KBattleGuy* guy);
	void ActiveSkill(KBattleCtrlBase* ctrl,KAbilityStatic::Enum_When when);
	HeroSkill* RndSelectSkill(KAbilityStatic::Enum_When when);

	void SetHero(KHeroDef* pDef);
	size_t serialize(KMemoryStream* so);
	BOOL deserialize(KMemoryStream* si);
	void init(KBattleGuy* guy){ m_Owner = guy;}
private:
	void addSkill(int id,int lev);
	bool m_bDone;   //each turn only one skill actived .
	HeroSkill   m_skill[MAX_HERO_SKILL_NUM];
	KBattleGuy* m_Owner;
	KAbilityStatic m_dynAbility;
};

#endif //
