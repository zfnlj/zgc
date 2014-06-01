#include "KCardGroupAssist.h"
#include "../../StaticTable/KGameStatic.h"
#include "KJsonDictMgr.h"

static KCardGroupAssist::sort_enum g_sortType= KCardGroupAssist::sort_null;
bool comp(const KDBBagItemUnit &lhs, const KDBBagItemUnit &rhs)
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


void KCardGroupAssist::InsetCardGroupElem(int cardId,KItemUnitList& des)
{
	for(KItemUnitList::iterator it=des.begin();it!=des.end();++it)
	{
		if((*it)._id==cardId){
			(*it)._count++;
			return;
		}
	}
	
	des.push_back(KDBBagItemUnit(cardId,1));
}

void KCardGroupAssist::SortCardGroup(KIntegerList& src,KItemUnitList& des,sort_enum sortType)
{
	for(KIntegerList::iterator it=src.begin();it!=src.end();++it){
		InsetCardGroupElem(*it,des);
	}
	g_sortType = sortType;
	des.sort(comp);
}

void KCardGroupAssist::FilterCard(KItemUnitList& src,KItemUnitList& des,BrowseCard browse,int skip)
{
	for(KItemUnitList::iterator it=src.begin();it!=src.end();++it){
		bool bMatch = false;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(it->_id);
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