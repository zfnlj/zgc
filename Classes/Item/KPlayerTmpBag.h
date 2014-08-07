#ifndef _KPLAYERTMPBAG_H_
#define _KPLAYERTMPBAG_H_

#include <list>



class KPlayerTmpBag
{
public:
	enum ItemEnum
	{
		item_normal=0,
		item_hero_card=1,
		item_card=2,
	};
	struct ItemDef{
		ItemEnum _type;
		int _id;
		int _count;
		ItemDef(ItemEnum tp,int id,int count):_type(tp),_id(id),_count(count)
		{}
	};
	typedef std::list<ItemDef>  ItemDefList;
public:
	KPlayerTmpBag(){};
	~KPlayerTmpBag(){};

	void Add(int tp,int id,int count);
	void Reset();
	ItemDefList m_itemList;
};


#endif

