#include "KCardGroupAssist.h"
#include "../../StaticTable/KGameStatic.h"
#include "KJsonDictMgr.h"
#include "KUIAssist.h"
static KCardGroupAssist::sort_enum g_sortType= KCardGroupAssist::sort_null;
bool comp(const KMiniCardWidget &lhs, const KMiniCardWidget &rhs)
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

bool comp2(const KDBBagItemUnit &lhs, const KDBBagItemUnit &rhs)
{
	KCardStatic* p1 = KGameStaticMgr::getSingleton().GetCard(lhs._id);
	KCardStatic* p2 = KGameStaticMgr::getSingleton().GetCard(rhs._id);
	return p1->GetCost()<p2->GetCost();
}

void KCardGroupAssist::InsetCardGroupElem(int cardId,KMiniCardList& des)
{
	for(KMiniCardList::iterator it=des.begin();it!=des.end();++it)
	{
		if((*it)._cardId==cardId){
			(*it)._count++;
			return;
		}
	}
	
	des.push_back(KMiniCardWidget(cardId,1));
}

void KCardGroupAssist::SortCardItem(KItemUnitList& lst)
{
	lst.sort(comp2);
}

void KCardGroupAssist::SortCardGroup(KIntegerList& src,KMiniCardList& des,sort_enum sortType)
{
	for(KIntegerList::iterator it=src.begin();it!=src.end();++it){
		InsetCardGroupElem(*it,des);
	}
	g_sortType = sortType;
	des.sort(comp);
}

int KCardGroupAssist::GetTotalCardNum(KMiniCardList& lst)
{
	int count=0;
	for(KMiniCardList::iterator it=lst.begin();it!=lst.end();++it)
	{
		count += (*it)._count;
	}
	return count;
}

int KCardGroupAssist::GetDeckMiniCardNum(KMiniCardList& list,int cardId)
{
	for(KMiniCardList::iterator it=list.begin();it!=list.end();++it)
	{
		if((*it)._cardId==cardId){
			return (*it)._count;
		}
	}
	return 0;
}

void KCardGroupAssist::FilterCard(KItemUnitList& src,KItemUnitList& des,int browseId,int raceId,int heroRace,int costId,int skip)
{
	BrowseCard browse = (BrowseCard)browseId;
	for(KItemUnitList::iterator it=src.begin();it!=src.end();++it){
		bool bMatch = false;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(it->_id);
		if(costId==7){
			if(pST->GetCost()<7) continue;
		}else if(costId>0){
			if(pST->GetCost()!=costId) continue;
		}
		if(raceId!=KCardStatic::race_null&& raceId!=pST->GetRace()) continue;
		if(pST->GetRace()!=0 && heroRace>0 && pST->GetRace()!=heroRace) continue;
		if(!pST) continue;
		switch(browse){
		case browse_all:
			bMatch = true;
			break;
		case browse_skill:
			bMatch = (pST->GetType()==KCardStatic::card_skill)?true:false;
			break;
		case browse_soldier:
			bMatch = (pST->GetType()==KCardStatic::card_soldier)?true:false;
			break;
		}
		if(bMatch){
			if(skip>0){
				skip--;
			}else{
				des.push_back(*it);
			}
		}
	}
}

void KCardGroupAssist::SetSlotElem(KCardGroupSlotElem* elem,int id,KCardGroupSlotElem::elemType tp,UIWidget* widget)
{
	if(elem && elem->_widget) elem->_widget->removeFromParent();
	elem->_id = id;
	elem->_type = tp;
	elem->_widget = widget;
}

void KCardGroupAssist::ClearSlotElem(KCardGroupSlotElem* arr,int n)
{
	KCardGroupSlotElem* elem = arr;
	for(int i=0;i<n;i++,elem++){
		if(elem->_type==KCardGroupSlotElem::elem_card||
			elem->_type==KCardGroupSlotElem::elem_hero){
				KJsonDictMgr::getSingleton().OnCardWidgetDestory(elem->_widget);
		}
		if(elem->_widget) elem->_widget->removeFromParent();
		memset(elem,0,sizeof(KCardGroupSlotElem));
	}
}

void KCardGroupAssist::AddMiniCard(KMiniCardList& lst,int cardId,int count)
{
	for(KMiniCardList::iterator it=lst.begin();it!=lst.end();++it){
		KMiniCardWidget& elem = *it;
		if(elem._cardId == cardId){
			elem._count += count;
			if(elem._count<0) elem._count=0;
			if(elem._count>2) elem._count=2;
			if(elem._count==0){
				if(elem._pWidget){
					KJsonDictMgr::getSingleton().OnMiniCardWidgetDestory(elem._pWidget);
					elem._pWidget->removeFromParent();
				}
				lst.erase(it);
			}else{
				if(elem._pWidget) KUIAssist::UpdateMiniCardWidgetNum(elem._pWidget,elem._count);
			}
			return;
		}
	}
	if(count >0){
		for(int i=0;i<count;i++){
			InsetCardGroupElem(cardId,lst);
		}
		lst.sort(comp);
	}
}

int KCardGroupAssist::GetCurDeckRace(KHeroDef& hero,KMiniCardList&lst)
{
	KCardStatic* heroST = KGameStaticMgr::getSingleton().GetCard(hero._cardId);
	if(heroST) return heroST->GetRace();

	for(KMiniCardList::iterator it=lst.begin();it!=lst.end();++it){
		KMiniCardWidget& elem = *it;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(elem._cardId);
		if(pST && pST->GetRace()>0) return pST->GetRace();
	}
	return 0;
}

void KCardGroupAssist::ClearMiniCardList(KMiniCardList&lst)
{
	for(KMiniCardList::iterator it=lst.begin();it!=lst.end();++it){
		KMiniCardWidget& elem = *it;
		if(elem._pWidget){
			KJsonDictMgr::getSingleton().OnMiniCardWidgetDestory(elem._pWidget);
			elem._pWidget->removeFromParent();
		}
	}
	lst.clear();
}

bool KCardGroupAssist::IsMiniCardListMatch(KCardGroupSlotElem& elem,KHeroDef& curHero,KMiniCardList& miniList,KPlayerCardDepot* depot)
{
	if(elem._id==0) return false;
	KCardStatic* pST = NULL;
	if(elem._type==KCardGroupSlotElem::elem_card){
		pST = KGameStaticMgr::getSingleton().GetCard(elem._id);
	}else if(elem._type==KCardGroupSlotElem::elem_hero){
		const KHeroDef* herDef = depot->FindHero(elem._id);
		if(herDef){
			pST = KGameStaticMgr::getSingleton().GetCard(herDef->_cardId);
		}
	}
	if(!pST) return false;
	if(pST->GetRace()==KCardStatic::race_null) return true;

	KCardStatic* pCurHeroST = KGameStaticMgr::getSingleton().GetCard(curHero._cardId);
	if(pCurHeroST && pCurHeroST->GetRace()!=pST->GetRace()) return false;

	for(KMiniCardList::iterator it=miniList.begin();it!=miniList.end();it++){
		KCardStatic* pCardST = KGameStaticMgr::getSingleton().GetCard(it->_cardId);
		if(pCardST->GetRace()==KCardStatic::race_null) continue;
		return (pCardST->GetRace()==pST->GetRace());
	}
	return true;
}

bool KCardGroupAssist::SaveCardGroup(int deckId,KHeroDef& curHero,KMiniCardList& miniList,KPlayerCardDepot* depot)
{
	KIntegerList tmpLst;
	tmpLst.push_back(curHero._id);
	KCardStatic* pHeroST = KGameStaticMgr::getSingleton().GetCard(curHero._cardId);

	for(KMiniCardList::iterator it=miniList.begin();it!=miniList.end();++it){
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(it->_cardId);
		if(pHeroST &&
			pST->GetRace()!= KCardStatic::race_null &&
			pST->GetRace()!= pHeroST->GetRace()) continue;
		for(int i=0;i<it->_count;i++){
			tmpLst.push_back(it->_cardId);
		}
	}
	depot->SaveDeck(deckId,tmpLst);
	return true;
}

void KCardGroupAssist::SmartFillCardGroup(KHeroDef& curHero,KMiniCardList& miniList,KPlayerCardDepot* depot)
{
	if(curHero._id==0) return;
	KItemUnitList tmpList,desList;
	depot->PickStoreCard(tmpList);
	FilterCard(tmpList,desList,browse_all,KCardStatic::race_null,curHero.GetRace(),-1,0);
	int kk=0;
	KIntegerList remainLst;
	for(KItemUnitList::iterator it= tmpList.begin();it!=tmpList.end();++it){
		int remainNum = 2 - GetDeckMiniCardNum(miniList,it->_id);
		while(remainNum>0){
			remainLst.push_back(it->_id);
			remainNum--;
		}
	}
	KIntegerList pickLst;

	int reaminNum = 30 - GetTotalCardNum(miniList);
	_RndPick(remainLst,pickLst,reaminNum);
	for(KIntegerList::iterator it= pickLst.begin();it!=pickLst.end();++it){
		AddMiniCard(miniList,*it,1);
	}
}