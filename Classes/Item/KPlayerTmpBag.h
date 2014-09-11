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
	KPlayerTmpBag(){
		Reset();
	};
	~KPlayerTmpBag(){};

	
	void Add(int tp,int id,int count);
	void Reset();
	ItemDefList m_itemList;
	void AddMoney(int val){ m_money += val;}
	void AddExp(int val) { m_exp += val;}
	int GetMoney() { return m_money;}
	int GetExp(){ return m_exp;}
private:
	int m_money;
	int m_exp;
};


#endif

