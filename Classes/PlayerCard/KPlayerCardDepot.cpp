#include "KPlayerCardDepot.h"
#include "../StaticTable/KGameStatic.h"
#include "../common/KPlayerRecordAssist.h"
#include "../common/KPlayerBagAssist.h"

bool KPlayerCardDepot::CreateOnDeckDef(int id)
{
	KIntegerList cardList;
	KDeckDefStatic* deck = KGameStaticMgr::getSingleton().GetDeckDef(id);
	if(!deck) return false;

	deck->GenCardList(cardList);
	KPlayerRecordAssist::addStoreCard(m_record,cardList);

	KHeroDef hero;
	hero.Generate(deck->getHero());
	KPlayerRecordAssist::addHero(m_record,&hero);

	if(m_record->GetCurDeck()<0) m_record->SetCurDeck(0);

	cardList.push_front(hero._id);
	KPlayerRecordAssist::updateCardDeck(m_record,cardList);
	return true;
}

int KPlayerCardDepot::CreateHero(int heroId)
{
	KHeroDef hero;
	hero.Generate(heroId);
	KPlayerRecordAssist::addHero(m_record,&hero);
	return hero._id;
}

bool KPlayerCardDepot::FillHeroDef(int id,KHeroDef& hero)
{
	KHeroDef* pCurHero = (KHeroDef*)m_record->_heroData.binData;
	int num = m_record->_heroData.actualLength/sizeof(KHeroDef);
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
	return m_record->_heroData.actualLength/sizeof(KHeroDef);
}

const KHeroDef* KPlayerCardDepot::FindHeroOnIndex(int index)
{
	KHeroDef* pHero = (KHeroDef*)m_record->_heroData.binData;
	return &pHero[index];
}

bool KPlayerCardDepot::PickAllHero(KHeroDefList& lst)
{
	int heroNum = m_record->_heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)m_record->_heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		lst.push_back(pHero);
	}
	return true;
}

KHeroDef* KPlayerCardDepot::FindHero(int id)
{
	int heroNum = m_record->_heroData.actualLength/sizeof(KHeroDef);
	KHeroDef* pHero = (KHeroDef*)m_record->_heroData.binData;
	for(int i=0;i<heroNum;i++,pHero++){
		if(pHero->_id==id) return pHero;
	}
	return NULL;
}

int  KPlayerCardDepot::GetDeckCardNum(int deck,int cardId)
{
	int count=0;
	int cardNum = m_record->_cardDeck[deck].actualLength/sizeof(int);
	int* pCard = (int*)m_record->_cardDeck[deck].binData;
	for(int i=0;i<cardNum;i++,pCard++){
		if(*pCard==cardId) count++;
	}
	return count;
}

bool KPlayerCardDepot::PickCurHero(KHeroDef& hero)
{
	return PickDeckHero(m_record->_curDeck,hero);
}

bool KPlayerCardDepot::PickDeckHero(int index,KHeroDef& hero)
{
	memset(&hero,0,sizeof(KHeroDef));
	if(index<0) return false;
	int cardNum = m_record->_cardDeck[index].actualLength/sizeof(int);
	if(cardNum<1) return false;
	int* pDeck = (int*)m_record->_cardDeck[index].binData;
	const KHeroDef* pDef = FindHero(pDeck[0]);
	if(!pDef) return false;
	memcpy(&hero,pDef,sizeof(KHeroDef));
	return true;
}

bool KPlayerCardDepot::IsDeckReady(int index)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	int cardNum = m_record->_cardDeck[index].actualLength/sizeof(int);
	return (cardNum==31);
}

bool KPlayerCardDepot::IsDeckHero(int heroId)
{
	for(int i=0;i<MAX_DECK_NUM;i++){
		int cardNum = m_record->_cardDeck[i].actualLength/sizeof(int);
		if(cardNum==0) continue;
		int* pDeck = (int*)m_record->_cardDeck[i].binData;
		if(pDeck[0]==heroId) return true;
	}
	return false;
}

bool KPlayerCardDepot::GetCardDeck(int index,KIntegerList& tmpLst,KHeroDef& hero)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	int cardNum = m_record->_cardDeck[index].actualLength/sizeof(int);
	if(cardNum<1) return false;

	int* pDeck = (int*)m_record->_cardDeck[index].binData;
	if(!PickDeckHero(index,hero)) return false;
	int heroId = hero._cardId;
	pDeck++;
	for(int i=0;i<cardNum-1;i++,pDeck++){
		if(*pDeck>0) tmpLst.push_back(*pDeck);
	}
	return true;
}

int KPlayerCardDepot::GetCurDeck()
{
	return m_record->_curDeck;
}

void KPlayerCardDepot::SetCurDeck(int index)
{
	KPlayerRecordAssist::setCurDeck(m_record,index);
}

bool KPlayerCardDepot::PickCurDeck(int& heroCardId,KIntegerList& tmpLst)
{
	if(m_record->_curDeck<0) return false;
	KHeroDef hero;
	bool ret = GetCardDeck(m_record->_curDeck,tmpLst,hero);
	heroCardId = hero._cardId;
	return ret;
}

bool KPlayerCardDepot::PickStoreCard(KItemUnitList& tmpLst)
{
	int cardNum = m_record->_cardStore.actualLength/sizeof(KDBBagItemUnit);
	KDBBagItemUnit* pUnit = (KDBBagItemUnit*)m_record->_cardStore.binData;
	for(int i=0;i<cardNum;i++,pUnit++){
		tmpLst.push_back(KDBBagItemUnit(pUnit->_id,pUnit->_count));
	}
	return true;
}

int  KPlayerCardDepot::GetCardNum(int cardId)
{
	int cardNum = m_record->_cardStore.actualLength/sizeof(KDBBagItemUnit);
	KDBBagItemUnit* pUnit = (KDBBagItemUnit*)m_record->_cardStore.binData;
	for(int i=0;i<cardNum;i++,pUnit++){
		if(pUnit->_id== cardId) return pUnit->_count;
	}
	return 0;
}

bool KPlayerCardDepot::SaveDeck(int deckId,KIntegerList& tmpLst)
{
	return  KPlayerRecordAssist::updateCardDeck(m_record,tmpLst,deckId);
}

bool KPlayerCardDepot::ClearDeck(int deckId)
{
	return KPlayerRecordAssist::ClearCardDeck(m_record,deckId);
}

void KPlayerCardDepot::SaveHero(KHeroDef* hero)
{
	KPlayerRecordAssist::updateHero(m_record,hero);
}

int KPlayerCardDepot::BreakHero(int heroId)
{
	KHeroDef* heroDef = FindHero(heroId);
	if(!heroDef) return 0 ;
	int stoneNum = KGameStaticMgr::getSingleton().GetHeroLevUpExp(heroDef->_lev)*0.6;
	KPlayerBagAssist::AddItem(m_player,heroDef->GetCardId()*10,stoneNum);

	KPlayerRecordAssist::delHero(m_record,heroId);
	return stoneNum;
}