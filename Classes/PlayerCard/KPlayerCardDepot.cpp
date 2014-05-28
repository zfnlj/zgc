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
	return true;
}
