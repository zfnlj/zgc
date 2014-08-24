#include "KPlayerBagAssist.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"


namespace KPlayerBagAssist
{

int GetPlayerItemNum(KWorldObjAbout::KPlayer* player,DWORD itemId)
{
	KItemAbout::KBagManager* pBagMgr = player->GetBagManagerPtr();
	if(!pBagMgr) return 0;
	KItemAbout::KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return 0;

	return pBag->GetItemSize(itemId);
}

bool UseItem(KWorldObjAbout::KPlayer* player,DWORD itemId,int num)
{
	return true;
}
}