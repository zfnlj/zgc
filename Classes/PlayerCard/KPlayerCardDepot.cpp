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
		bool bNew;
		KPlayerRecordAssist::addStoreCard(m_record,*it,1,bNew);
	}
	KHeroDef hero;
	hero.rndGenerate(deck->getHero());
	KPlayerRecordAssist::addHero(m_record,&hero);

	if(m_record->GetCurDeck()<0) m_record->SetCurDeck(0);

	cardList.push_front(hero._id);
	KPlayerRecordAssist::updateCardDeck(m_record,cardList);
	return true;
}

int KPlayerCardDepot::CreateHero(int heroId)
{
	KHeroDef hero;
	hero.rndGenerate(heroId);
	KPlayerRecordAssist::addHero(m_record,&hero);
	return hero._id;
}

bool KPlayerCardDepot::FillHeroDef(int id,KHeroDef& hero)
{
	KHeroDef* pCurHero = (KHeroDef*)m_record->heroData.binData;
	int num = m_record->heroData.actualLength/sizeof(KHeroDef);
	for(int i=0;i<num;i++,pCurHero++){
		if(pCurHero->_id==id){
			memcpy(&hero,pCurHero,sizeof(KHeroDef));
			return true;
		}
	}
	return false;
}

int  KPlayerCardDepot::GetHeroNum()
{
	return m_record->heroData.actualLength/sizeof(KHeroDef);
}

const KHeroDef* KPlayerCardDepot::FindHeroOnIndex(int index)
{
	KHeroDef* pHero = (KHeroDef*)m_record->heroData.binData;
	return &pHero[index];
}

const KHeroDef* KPlayerCardDepot::FindHero(int id)
{
	int heroNum = m_record->heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)m_record->heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		if(pHero->_id==id) return pHero;
	}
	return NULL;
}

int  KPlayerCardDepot::GetDeckCardNum(int deck,int cardId)
{
	int count=0;
	int cardNum = m_record->cardDeck[deck].actualLength/sizeof(int);
	int* pCard = (int*)m_record->cardDeck[deck].binData;
	for(int i=0;i<cardNum;i++,pCard++){
		if(*pCard==cardId) count++;
	}
	return count;
}

bool KPlayerCardDepot::PickCurHero(KHeroDef& hero)
{
	return PickDeckHero(m_record->curDeck,hero);
}

bool KPlayerCardDepot::PickDeckHero(int index,KHeroDef& hero)
{
	memset(&hero,0,sizeof(KHeroDef));
	if(index<0) return false;
	int cardNum = m_record->cardDeck[index].actualLength/sizeof(int);
	if(cardNum<1) return false;
	int* pDeck = (int*)m_record->cardDeck[index].binData;
	const KHeroDef* pDef = FindHero(pDeck[0]);
	if(!pDef) return false;
	memcpy(&hero,pDef,sizeof(KHeroDef));
	return true;
}

bool KPlayerCardDepot::IsDeckReady(int index)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	int cardNum = m_record->cardDeck[index].actualLength/sizeof(int);
	return (cardNum==31);
}

bool KPlayerCardDepot::GetCardDeck(int index,KIntegerList& tmpLst,KHeroDef& hero)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	int cardNum = m_record->cardDeck[index].actualLength/sizeof(int);
	if(cardNum<1) return false;

	int* pDeck = (int*)m_record->cardDeck[index].binData;
	PickDeckHero(index,hero);
	int heroId = hero._cardId;
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

void KPlayerCardDepot::SetCurDeck(int index)
{
	KPlayerRecordAssist::setCurDeck(m_record,index);
}

bool KPlayerCardDepot::PickCurDeck(int& heroCardId,KIntegerList& tmpLst)
{
	if(m_record->curDeck<0) return false;
	KHeroDef hero;
	bool ret = GetCardDeck(m_record->curDeck,tmpLst,hero);
	heroCardId = hero._cardId;
	return ret;
}

bool KPlayerCardDepot::PickStoreCard(KItemUnitList& tmpLst)
{
	int cardNum = m_record->cardStore.actualLength/sizeof(KDBBagItemUnit);
	KDBBagItemUnit* pUnit = (KDBBagItemUnit*)m_record->cardStore.binData;
	for(int i=0;i<cardNum;i++,pUnit++){
		tmpLst.push_back(KDBBagItemUnit(pUnit->_id,pUnit->_count));
	}
	return true;
}

bool KPlayerCardDepot::SaveDeck(int deckId,KIntegerList& tmpLst)
{
	return  KPlayerRecordAssist::updateCardDeck(m_record,tmpLst,deckId);
}

bool KPlayerCardDepot::ClearDeck(int deckId)
{
	return KPlayerRecordAssist::ClearCardDeck(m_record,deckId);
}

int KPlayerCardDepot::ConsumeExp(int val)
{
	KHeroDef heroDef;
	if(!PickCurHero(heroDef)) return val;
	int oldHeroLev = heroDef.GetLev();
	if(oldHeroLev==9) return val; // max lev
	//heroDef._exp += val;
	//int curHeroLev = heroDef.GetLev();
	//KPlayerRecordAssist::updateHeroExp(m_record,heroDef._id,heroDef._exp);
	
	return 0;
}