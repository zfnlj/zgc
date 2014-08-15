#include "KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"
#include "../StaticTable/StaticData.h"

int KHeroDef::GetLev() const 
{
	return _lev;
}

int KHeroDef::GetRace()
{
	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(_cardId);
	if(pST) return pST->GetRace();
	return 0;
}

int KHeroDef::GetSkillLev(int skillIndex) const
{
	return  _skill[skillIndex]._lev;
}

int KHeroDef::GetSkillLevUpMoney(int skillIndex)
{
	if(_skill[skillIndex]._lev>=_lev) return -1;
	return KGameStaticMgr::getSingleton().GetSkillLevUpExp(skillIndex*10+_skill[skillIndex]._lev+1);
}

int KHeroDef::SkillLevUp(int skillIndex,int money)
{
	if(_skill[skillIndex]._lev>=_lev) return money;
	int needMoney = GetSkillLevUpMoney(skillIndex);
	if(needMoney>money) return money;
	_skill[skillIndex]._lev = _skill[skillIndex]._lev +1;
	return money - needMoney;
}

int KHeroDef::LevUp(int stoneNum)
{
	int needStone = GetLevUpStoneNum();
	if(needStone<0|| needStone>stoneNum) return stoneNum;
	_lev++;

	KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(_lev);
	_skill[_lev-1]._skillId = skill->GetId();
	_skill[_lev-1]._lev = 0;

	_lucky += rndGenLevLucky()+10;
	if(_lucky>100) _lucky = 100;
	return stoneNum-needStone;
}

int KHeroDef::GetLevUpStoneNum()
{
	if(_lev==MAX_HERO_LEV_INDEX) return -1;
	return KGameStaticMgr::getSingleton().GetHeroLevUpExp(_lev+1);
}

void KHeroDef::rndGenerate(int cardId)
{
	_cardId = cardId;
	_id = STATIC_DATA_INC();
	_lev = 0;
	_lucky = 0;
	_strong = 0;
	memset(_skill,0,sizeof(_skill));
	/*for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(i+1);
		_skill[i]._skillId = skill->GetId();
	}*/
}

void KHeroDef::init(KDeckDefStatic* pDef)
{
	_id = 0;
	_lucky = pDef->getHeroLucky();
	_strong = pDef->getHeroStrong();
	_cardId = pDef->getHero();
}

int KHeroDef::rndGenLevLucky()
{
	int rndVal = 0;
	for(int i=0;i<3;i++){
		rndVal  += g_rnd.GetRandom(0,10);
	}
	if(rndVal >24) rndVal = 24;
	return rndVal;
}

int KHeroDef::GetAtkVal() const
{
	float val = 0.0f;

	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(_skill[i]._skillId);
		if(!skill) continue;
		float luckyVal = (_lucky>i*33)?_lucky -i*33:0;
		luckyVal += skill->GetRateVal(GetSkillLev(i));
		if(luckyVal<=0.0f) luckyVal = 0.0f;
		if(luckyVal>100.0f) luckyVal = 100.0f;
		val += skill->GetPower()*luckyVal;
	}
	val = val*10.0f;
	return (int)(val+0.5f);
}

int KHeroDef::GetStrong() const 
{
	if(_strong>0) return _strong;
	int ret = 0;
	switch(_lev){
	case 1:
		ret = 2;
		break;
	case 2:
		ret = 5;
		break;
	case 3: 
		ret = 10;
		break;
	default:
		break;
	}
	return ret;
}


void KPlayerDeck::CreateOnDeckDef(int id)
{
	m_cardList.clear();
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	deck->GenCardList(m_cardList);
	m_heroDef.rndGenerate(deck->getHero());
	
}

