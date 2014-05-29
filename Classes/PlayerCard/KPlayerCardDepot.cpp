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
	KPlayerRecordAssist::addCardDeck(m_record,cardList);
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

bool KPlayerCardDepot::GetCardDeck(int index,int* buf,KHeroDef& hero)
{
	if(index <0 || index >=MAX_DECK_NUM) return false;
	if(m_record->cardDeck[index].actualLength!=31*sizeof(int)) return false;
	int* pDeck = (int*)m_record->cardDeck[index].binData;
	int heroId = *pDeck;
	pDeck++;
	if(!FillHeroDef(heroId,hero)) return false;
	memcpy(buf,pDeck,sizeof(int)*30);
	return true;
}

bool KPlayerCardDepot::FillCurDeck(int* arr,KHeroDef& hero)
{
	return GetCardDeck(m_record->curDeck,arr,hero);
}