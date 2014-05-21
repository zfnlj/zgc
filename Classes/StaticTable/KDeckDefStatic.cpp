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
	if(ns!=4) return false;
	if(strcmp(ss[0],"soldier")==0){
		_def = KCardStatic::card_soldier;
	}else if(strcmp(ss[0],"skill")==0){
		_def = KCardStatic::card_skill;
	}else{
		CCAssert(false , "Parse dynamic def error!");
	}
	_race = KCardStatic::getCardRace(ss[1]);
	_rank = atoi(ss[2]);
	_num = atoi(ss[3]);
	return true;
}

bool KDeckDefStatic::init()
{
	m_heroID = m_heroHp = m_res = 0;
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
	
	fileReader->GetString("DECK", "", buf, 1023);
	setDeck(buf);
	
	fileReader->GetInteger("DrawNum", 0, (int*)&m_drawNum);
	

	fileReader->GetString("Dynamic", "", buf, 1023);
	setDynamic(buf);
}

void KDeckDefStatic::GenCardList(KIntegerList& lst)
{
	KIntegerList tmpLst;
	_CopyIntegerList(m_cardList,tmpLst);
	for(DynamicCardDefList::iterator it=m_defList.begin();it!=m_defList.end();++it){
		DynamicCardDef* def=*it;
		FillOnDynamicCardDef(def,tmpLst);
	}
	if(m_rnd>0){
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
	for(KIntegerList::iterator it=tmpLst2.begin();it!=tmpLst2.end();it++)
	{
		lst.push_back(*it);
		lst.push_back(*it);
	}
}