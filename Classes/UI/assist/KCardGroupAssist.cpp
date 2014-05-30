#include "KCardGroupAssist.h"
#include "../../StaticTable/KGameStatic.h"

static KCardGroupAssist::sort_enum g_sortType= KCardGroupAssist::sort_null;
bool comp(const KCardGroupElem &lhs, const KCardGroupElem &rhs)
{
	KCardStatic* p1 = KGameStaticMgr::getSingleton().GetCard(lhs._cardId);
	KCardStatic* p2 = KGameStaticMgr::getSingleton().GetCard(rhs._cardId);
	switch(g_sortType){
	case KCardGroupAssist::sort_cost:
		return p1->GetCost()<p2->GetCost();
		break;
	default:
		break;
	}
	//return lhs.order < rhs.order;
	return false;
}


void KCardGroupAssist::InsetCardGroupElem(int cardId,CardGroupElemList& des)
{
	for(CardGroupElemList::iterator it=des.begin();it!=des.end();++it)
	{
		if((*it)._cardId==cardId){
			(*it)._num++;
			return;
		}
	}
	
	des.push_back(KCardGroupElem(cardId,1));
}

void KCardGroupAssist::SortCardGroup(KIntegerList& src,CardGroupElemList& des,sort_enum sortType)
{
	for(KIntegerList::iterator it=src.begin();it!=src.end();++it){
		InsetCardGroupElem(*it,des);
	}
	g_sortType = sortType;
	des.sort(comp);
}