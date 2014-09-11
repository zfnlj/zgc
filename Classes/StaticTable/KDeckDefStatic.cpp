#include "KDeckDefStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>
#include "KGameStatic.h"

bool KDeckDefStatic::DynamicCardDef::init(char* buf)
{
	char strBuf[1024];
	strcpy(strBuf,buf);

	char* ss[64];
	int ns = split(strBuf, "*", ss, 64);
	if(ns!=5){
		CCAssert(false , "dynamic deck def error!");
		return false;
	}
	if(strcmp(ss[0],"S")==0){
		_def = KCardStatic::card_soldier;
	}else if(strcmp(ss[0],"C")==0){
		_def = KCardStatic::card_skill;
	}else{
		CCAssert(false , "Parse dynamic def error!");
	}
	_race = KCardStatic::getCardRace(ss[1]);
	_rank = atoi(ss[2]);
	_num = atoi(ss[3]);
	_rate = atoi(ss[4]);
	return true;
}

bool KDeckDefStatic::init()
{
	m_heroID = m_heroHp = m_res = m_heroStrong = m_heroLucky = m_resLucky = 0;
	memset(m_skill,0,sizeof(m_skill));
	return true;
}

KDeckDefStatic* KDeckDefStatic::create()
{
	KDeckDefStatic* pDeck = new KDeckDefStatic;
	pDeck->init();
	return pDeck;
}

void KDeckDefStatic::setDynamic(const char* buf)
{
	char strBuf[1024];
	strcpy(strBuf,buf);

	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);

	for(int i=0; i<ns; i++)
	{
		char* s = _trim(ss[i]);
		if(strlen(s)<2) continue;
		DynamicCardDef* pDef =  DynamicCardDef::create();
		if(!pDef->init(s)){
			delete pDef;
			continue;
		}
		m_defList.push_back(pDef);
	}

}
void KDeckDefStatic::setDeck(const char* buf)
{
	char strBuf[1024];
	strcpy(strBuf,buf);

	m_cardList.clear();
	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);

	for(int i=0; i<ns; i++)
	{
		char* s = _trim(ss[i]);
		int id = atoi(s);
		if(id>0) m_cardList.push_back(id);
	}
}

void KDeckDefStatic::Init(System::File::KTabFile2* fileReader)
{
	char buf[1024];
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetInteger("HeroID", 0, (int*)&m_heroID);
	fileReader->GetInteger("HeroHp", 0, (int*)&m_heroHp);
	fileReader->GetInteger("Res", 0, (int*)&m_res);
	fileReader->GetInteger("Rnd", 0, (int*)&m_rnd);
	fileReader->GetInteger("HeroStrong", 0, (int*)&m_heroStrong);
	fileReader->GetInteger("ResLucky", 0, (int*)&m_resLucky);
	fileReader->GetInteger("HeroLucky", 0, (int*)&m_heroLucky);
	
	fileReader->GetString("DECK", "", buf, 1023);
	setDeck(buf);
	
	fileReader->GetInteger("DrawNum", 0, (int*)&m_drawNum);
	
	fileReader->GetString("HeroSkill", "", buf, 1023);
	setHeroSkill(buf);


	fileReader->GetString("Dynamic", "", buf, 1023);
	setDynamic(buf);
}


void KDeckDefStatic::setHeroSkill(const char* buf)
{
	char strBuf[1024];
	strcpy(strBuf,buf);
	if(strlen(buf)<2) return;
	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);

	int pos = 0;
	for(int i=0; i<ns; i++)
	{
		char* s = _trim(ss[i]);

		char* qq[64];
		int nb = split(s, "*", qq, 64);
		if(nb!=2) continue;
		
		char* p = _trim(qq[0]);
		int id = atoi(p);
		p = _trim(qq[1]);
		int lev =atoi(p);
		m_skill[pos++].init(id,lev);
	}

}

void KDeckDefStatic::GenCardList(KIntegerList& lst,bool bRnd)
{
	KIntegerList tmpLst;
	_CopyIntegerList(m_cardList,tmpLst);
	for(DynamicCardDefList::iterator it=m_defList.begin();it!=m_defList.end();++it){
		DynamicCardDef* def=*it;
		FillOnDynamicCardDef(def,tmpLst);
	}
	if(bRnd){
		_RndIntegerList(tmpLst,lst);
	}else{
		_CopyIntegerList(tmpLst,lst);
	}
}

void KDeckDefStatic::FillOnDynamicCardDef(DynamicCardDef* def,KIntegerList& lst)
{
	KIntegerList tmpLst,tmpLst2;
	KGameStaticMgr::getSingleton().FilterCards(tmpLst,def->_def,def->_race,def->_rank); //选找出符合条件的卡牌
	_RndPick(tmpLst,tmpLst2,def->_num);
	CCAssert(tmpLst2.size()==def->_num , "Parse dynamic def error!");
	_CopyIntegerList(tmpLst2,lst,def->_rate);
}

int KDeckDefStatic::RndPickSummonSoldier(int maxCost)
{
	KIntegerList tmpLst;
	KIntegerList filterLst;
	KGameStaticMgr::getSingleton().FilterCardsOnCost(tmpLst,KCardStatic::card_soldier,
													KCardStatic::race_null,maxCost,1);
	for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();++it)
	{
		KAbilityStatic* pAbility = KGameStaticMgr::getSingleton().GetAbilityOnId((*it)*10);
		if(!pAbility ||
			pAbility->GetWhich()==KAbilityStatic::which_i)
			filterLst.push_back(*it);
	}
	KIntegerList des;
	_RndPick(filterLst,des,1);
	if(des.size()>0){
		return *(des.begin());
	}else{
		return 0;
	}
}

int KDeckDefStatic::GetAtkVal()
{
	float val = 0.0f;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(m_skill[i]._id);
		if(!skill) continue;
		float luckyVal = (m_heroLucky>i*33)?m_heroLucky -i*33:0;
		luckyVal += skill->GetRateVal(m_skill[i]._lev);
		if(luckyVal<=0.0f) luckyVal = 0.0f;
		if(luckyVal>100.0f) luckyVal = 100.0f;
		val += skill->GetPower()*luckyVal;
	}
	val = val*10.0f;
	return (int)(val+0.5f);
}

void KDeckDefStatic::clone(KDeckDefStatic* pDef)
{
	m_Id = -1;
	m_heroID = pDef->m_heroID;
	m_heroHp  = pDef->m_heroHp;
	m_heroStrong = pDef->m_heroStrong;
	m_heroLucky = pDef->m_heroLucky;
	m_res = pDef->m_res;
	m_drawNum = pDef->m_drawNum;
	m_rnd = pDef->m_rnd;
	m_resLucky = pDef->m_resLucky;
	memcpy(&m_skill,pDef->m_skill,sizeof(HeroSkill)*MAX_HERO_SKILL_NUM);

	for(KIntegerList::iterator it=pDef->m_cardList.begin();it!=pDef->m_cardList.end();++it)
	{
		m_cardList.push_back(*it);
	}

	for(DynamicCardDefList::iterator it=pDef->m_defList.begin();it!=pDef->m_defList.end();++it){
		m_defList.push_back(*it);
	}
}