#include "KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"
#include "../StaticTable/StaticData.h"

const char* KHeroDef::skillDef::GetName() const
{
	KHeroSkillStatic* skillST = KGameStaticMgr::getSingleton().GetHeroSkill(_id);
	return skillST->GetName();
}

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

void KHeroDef::LevUpFate()
{
	_fate++;
}

bool KHeroDef::SkillLevUp(int skillIndex)
{
	if(_skill[skillIndex].IsMaxLev()) return false;
	_skill[skillIndex]._lev = _skill[skillIndex]._lev +1;
	return true;
}

bool KHeroDef::LevUp()
{
	if(IsLevMax()) return false;
	_lev++;
	KHeroSkillStatic* skill = NULL;
	switch(_lev){
	case 2:
		{
			_strong += 1 + rndGenLevStrong();

		}
		break;
	case 3:
		{
			_strong += 1 + rndGenLevStrong();
			_lucky += rndGenLevLucky()+10;
			skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(1);
			_skill[0]._id = skill->GetId();
			_skill[0]._lev = 0;
		}
		break;
	case 4:
		{
			_lucky += rndGenLevLucky()+10;
			_strong += 1;
			skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(2);
			_skill[1]._id = skill->GetId();
			_skill[1]._lev = 0;
		}
		break;
	case 5:
		{
			_lucky += rndGenLevLucky()+10;
			_strong += 1;
			skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(3);
			_skill[2]._id = skill->GetId();
			_skill[2]._lev = 0;
		}
		break;
	}
	if(_lucky>100) _lucky = 100;
	return true;
}

int KHeroDef::GetLevUpStoneNum() const
{
	if(_lev==MAX_HERO_LEV_INDEX) return -1;
	return KGameStaticMgr::getSingleton().GetHeroLevUpExp(_lev+1);
}

void KHeroDef::LevZero()
{
	_lev = 1;
	_lucky = 0;
	_strong = 0;
	_resLucky = 0;
	_fate = 0;
	memset(_skill,0,sizeof(_skill));
}

void KHeroDef::Generate(int cardId)
{
	_cardId = cardId;
	_id = STATIC_DATA_INC();
	_lev = 1;
	_lucky = 0;
	_strong = 0;
	_resLucky = 0;
	_fate = 0;
	memset(_skill,0,sizeof(_skill));
	/*for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetRndHeroSkill(i+1);
		_skill[i]._skillId = skill->GetId();
	}*/
}

void KHeroDef::init(KDeckDefStatic* pDef)
{
	memset(this,0,sizeof(KHeroDef));
	_lucky = pDef->getHeroLucky();
	_strong = pDef->getHeroStrong();
	_cardId = pDef->getHero();
	_resLucky = pDef->getResLucky();
	_fate = pDef->getFate();
	const HeroSkill* skillArr = pDef->GetHeroSkill();
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		_skill[i]._id = skillArr[i]._id;
		_skill[i]._lev = skillArr[i]._lev;
	}
}

int KHeroDef::CalcLev()
{
	if(_lev>0) return _lev;
	int atkVal = GetAtkVal();
	_lev = atkVal/200 + 1;
	if(_lev>5) _lev=5;
	return _lev;
}

int KHeroDef::rndGenLevStrong()
{
	int rndVal = 0;
	for(int i=0;i<2;i++){
		rndVal  += g_rnd.GetRandom(0,3);
	}
	return rndVal;
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
	
	float strongVal = 15.0f*(float)_strong;
	if(strongVal>300.0f) strongVal = 300.0f;

	float skillValDef[MAX_HERO_SKILL_NUM]={100.0f,150.0f,250.0f};
	float skillVal = 0.0f;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(_skill[i]._id);
		if(!skill) continue;
		float luckyVal = (float)_lucky-i*33.0f;
		if(luckyVal<0.0f) luckyVal = 0.0f;
		if(luckyVal>33.0f) luckyVal = 33.0f;
		luckyVal = luckyVal/33.0f;
		luckyVal = 0.3f +0.7f*luckyVal;
		skillVal += luckyVal*(_skill[i]._lev+1)*0.25 * skill->GetPower()*skillValDef[i];
	}
	float resLuckyVal = 1.5f*(float)_resLucky;
	if(resLuckyVal>300.0f) resLuckyVal=300.0f;

	float fateVal = 2.0f*(float)_fate;
	if(fateVal>400.0f) fateVal = 400.0f;
	
	return (int)(strongVal + skillVal + resLuckyVal + fateVal);
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
	m_heroDef.Generate(deck->getHero());
	
}

