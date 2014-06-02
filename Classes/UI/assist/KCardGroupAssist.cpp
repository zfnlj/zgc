#include "KCardGroupAssist.h"
#include "../../StaticTable/KGameStatic.h"
#include "KJsonDictMgr.h"
#include "KUIAssist.h"
static KCardGroupAssist::sort_enum g_sortType= KCardGroupAssist::sort_null;
bool comp(const KMiniCardWidget &lhs, const KMiniCardWidget &rhs)
{
	KCardStatic* p1 = KGameStaticMgr::getSingleton().GetCard(lhs._id);
	KCardStatic* p2 = KGameStaticMgr::getSingleton().GetCard(rhs._id);
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


void KCardGroupAssist::InsetCardGroupElem(int cardId,KMiniCardList& des)
{
	for(KMiniCardList::iterator it=des.begin();it!=des.end();++it)
	{
		if((*it)._id==cardId){
			(*it)._count++;
			return;
		}
	}
	
	des.push_back(KMiniCardWidget(cardId,1));
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
		if((*it)._id==cardId){
			return (*it)._count;
		}
	}
	return 0;
}

void KCardGroupAssist::FilterCard(KItemUnitList& src,KItemUnitList& des,int browseId,int costId,int skip)
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
		if(elem->_type==KCardGroupSlotElem::elem_card) KJsonDictMgr::getSingleton().OnCardWidgetDestory(elem->_widget);
		if(elem->_widget) elem->_widget->removeFromParent();
		memset(elem,0,sizeof(KCardGroupSlotElem));
	}
}

void KCardGroupAssist::AddMiniCard(KMiniCardList& lst,int cardId,int count)
{
	for(KMiniCardList::iterator it=lst.begin();it!=lst.end();++it){
		KMiniCardWidget& elem = *it;
		if(elem._id == cardId){
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
				KUIAssist::UpdateMiniCardWidgetNum(elem._pWidget,elem._count);
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