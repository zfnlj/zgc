#ifndef _KCARDGROUPASSIST_H
#define _KCARDGROUPASSIST_H

#include <list>
#include "../../Common/KCommonObj.h"
#include "../../PlayerCard/KPlayerCardDepot.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
struct KCardGroupSlotElem{
	enum elemType{
		elem_card_group,
		elem_hero,
		elem_card,
		elem_null,
	};
	KCardGroupSlotElem():_id(0),_widget(NULL),_type(elem_null)
	{}
	int _id;
	elemType _type;
	UIWidget* _widget;
};
class KCardGroupAssist
{
public:
	enum sort_enum{
		sort_null,
		sort_cost,
	};
	enum BrowseCard{
		browse_all,
		browse_hero,
		browse_soldier,
		browse_skill,
	};
	static void ClearSlotElem(KCardGroupSlotElem* arr,int n);
	static void SetSlotElem(KCardGroupSlotElem* elem,int id,KCardGroupSlotElem::elemType tp,UIWidget* widget);
	static void SortCardGroup(KIntegerList& src,KItemUnitList& des,sort_enum sortType=sort_cost);
	static void InsetCardGroupElem(int cardId,KItemUnitList& des);
	static void FilterCard(KItemUnitList& src,KItemUnitList& des,BrowseCard browse,int skip);
};

#endif // 	
