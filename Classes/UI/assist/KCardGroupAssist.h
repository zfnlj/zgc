#ifndef _KCARDGROUPASSIST_H
#define _KCARDGROUPASSIST_H

#include <list>
#include "../../Common/KCommonObj.h"
struct KCardGroupElem{
	int _cardId;
	int _num;
	KCardGroupElem(int cardId,int num):_cardId(cardId),_num(num){}
};

typedef std::list<KCardGroupElem> CardGroupElemList;
class KCardGroupAssist
{
public:
	enum sort_enum{
		sort_null,
		sort_cost,
	};

	static void SortCardGroup(KIntegerList& src,CardGroupElemList& des,sort_enum sortType=sort_cost);
	static void InsetCardGroupElem(int cardId,CardGroupElemList& des);
};

#endif // 	
