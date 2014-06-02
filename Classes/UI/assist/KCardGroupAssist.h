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

struct KMiniCardWidget{
	int _id;
	int _count;
	UIWidget* _pWidget;
	KMiniCardWidget(int id,int count):_id(id),_count(count),_pWidget(NULL){}
};

typedef std::list<KMiniCardWidget> KMiniCardList;

class KCardGroupAssist
{
public:
	enum sort_enum{
		sort_null,
		sort_cost,
	};
	enum BrowseCard{
		browse_hero=0,
		browse_soldier=1,
		browse_skill=2,
		browse_all,
	};
	static void ClearSlotElem(KCardGroupSlotElem* arr,int n);
	static void SetSlotElem(KCardGroupSlotElem* elem,int id,KCardGroupSlotElem::elemType tp,UIWidget* widget);
	static void SortCardGroup(KIntegerList& src,KMiniCardList& des,sort_enum sortType=sort_cost);
	static void InsetCardGroupElem(int cardId,KMiniCardList& des);
	static void FilterCard(KItemUnitList& src,KItemUnitList& des,int browseId,int costId,int skip);
	static void AddMiniCard(KMiniCardList& lst,int cardId,int);
	static int GetDeckMiniCardNum(KMiniCardList& lst,int cardId);
	static int GetTotalCardNum(KMiniCardList& lst);
};

#endif // 	
