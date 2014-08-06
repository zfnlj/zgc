#include "KPlayerTmpBag.h"


void KPlayerTmpBag::Reset()
{
	m_itemList.clear();
}

void KPlayerTmpBag::Add(int tp,int id,int count)
{
	m_itemList.push_back(ItemDef((ItemEnum)tp,id,count));
}