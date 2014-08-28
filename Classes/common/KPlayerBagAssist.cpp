#include "KPlayerBagAssist.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"

using namespace KItemAbout;

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

bool ConsumeItem(KWorldObjAbout::KPlayer* player,DWORD itemId,int num)
{
	KItemAbout::KBagManager* pBagMgr = player->GetBagManagerPtr();
	if(!pBagMgr) return false;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return false;
	int pos = pBag->GetFirstPos(itemId);
	if(pos<0) return false;
	pBagMgr->AddBagItem(pBag,pos,-num);
	return true;
}

bool ConsumeMoney(KWorldObjAbout::KPlayer* player,int moneyVal)
{
	if(!player->HasMoney(moneyVal)) return false;

	KMoneyAbout::KMoney curMoney = player->m_money;
	curMoney.Increase(-moneyVal);
	player->syncToRecord();
	return true;
}

}