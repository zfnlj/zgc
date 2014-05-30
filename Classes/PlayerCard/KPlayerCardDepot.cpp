#include "KPlayerCardDepot.h"
#include "../StaticTable/KGameStatic.h"
#include "../common/KPlayerRecordAssist.h"

bool KPlayerCardDepot::CreateOnDeckDef(int id)
{
	KIntegerList cardList;
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	if(!deck) return false;

	deck->GenCardList(cardList);
	for(KIntegerList::iterator it=cardList.begin();it!=cardList.end();++it){
		KPlayerRecordAssist::addStoreCard(m_record,*it,1);
	}
	KHeroDef hero;
	hero.rndGenerate(deck->getHero());
	KPlayerRecordAssist::addHero(m_record,&hero);

	if(m_record->GetCurDeck()<0) m_record->SetCurDeck(0);

	cardList.push_front(hero._Id);
	cardList.push_back(0); // for 8 size,useless;
	KPlayerRecordAssist::insertCardDeck(m_record,cardList);
	return true;
}

bool KPlayerCardDepot::FillHeroDef(int heroId,KHeroDef& hero)
{
	KHeroDef* pCurHero = (KHeroDef*)m_record->heroData.binData;
	int num = m_record->heroData.actualLength/sizeof(KHeroDef);
	for(int i=0;i<num;i++,pCurHero++){
		if(pCurHero->_Id==heroId){
			memcpy(&hero,pCurHero,sizeof(KHeroDef));
			return true;
		}
	}
	return false;
}

KHeroDef* KPlayerCardDepot::FindHero(int id)
{
	int heroNum = m_record->heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)m_record->heroData.binData;
	for(int i=0;i<heroNum;i++){
		if(pHero->_Id==id) return pHero;
	}
	return NULL;
}

bool KPlayerCardDepot::PickCurHero(KHeroDef& hero)
{
	if(m_record->curDeck<0) return false;
	int cardNum = m_record->cardDeck[m_record->curDeck].actualLength/sizeof(KHeroDef);
	if(cardNum<1) return false;
	int* pDeck = (int*)m_record->cardDeck[m_record->curDeck].binData;
	KHeroDef* pDef = FindHero(pDeck[0]);
	if(!pDef) return false;
	memcpy(&hero,pDef,sizeof(KHeroDef));
	return true;
}

bool KPlayerCardDepot::GetCardDeck(int index,KIntegerList& tmpLst,KHeroDef& hero)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	int cardNum = m_record->cardDeck[index].actualLength/sizeof(int);
	if(cardNum<1) return false;

	int* pDeck = (int*)m_record->cardDeck[index].binData;
	PickCurHero(hero);
	int heroId = hero._heroId;
	pDeck++;
	for(int i=0;i<cardNum-1;i++,pDeck++){
		if(*pDeck>0) tmpLst.push_back(*pDeck);
	}
	return true;
}

int KPlayerCardDepot::GetCurDeck()
{
	return m_record->curDeck;
}

bool KPlayerCardDepot::PickCurDeck(int& heroId,KIntegerList& tmpLst)
{
	if(m_record->curDeck<0) return false;
	KHeroDef hero;
	bool ret = GetCardDeck(m_record->curDeck,tmpLst,hero);
	heroId = hero._heroId;
	return ret;
}