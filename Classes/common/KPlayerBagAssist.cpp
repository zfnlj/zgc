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

	player->m_money.Increase(-moneyVal);
	player->syncToRecord();
	return true;
}

bool AddItem(KWorldObjAbout::KPlayer* player,int itemId,int count)
{
	KItemAbout::KBagNormal* normalBag = player->GetBagManagerPtr()->FindNormalBag();
	int slot = normalBag->GetFirstPos(itemId);

	char szTmp[2048] = "";
	int nSendLen = 0;
	SC_BagAddResponse* pBAR = (SC_BagAddResponse*)szTmp;
	if(slot>=0){
		int newNum = normalBag->FindCell(slot)->GetItemStackNumber() + count;
		nSendLen = KBagManager::GenSyncDataUpdateCell(szTmp,enum_item_BagNormal, slot, enum_stackNumber, (BYTE*)(&newNum), sizeof(newNum), 0);
	}else{
		slot = normalBag->FindFirstEmptyCell();
		pBAR->byteSubCmd = enumIOR_ADD;
		pBAR->byteBagID = enum_item_BagNormal;
		pBAR->nPos = slot;
		KDataOutputStream os(szTmp + sizeof(SC_BagAddResponse), sizeof(szTmp) - sizeof(SC_BagAddResponse));
		KItem item(itemId,count);
		item.Serilize(os);
		pBAR->nItemBufferSize = os.m_pos;
		nSendLen = sizeof(SC_BagAddResponse) + os.m_pos;

	}
	player->GetBagManagerPtr()->ItemOperator((const SC_BagOperation*)szTmp);
	player->GetPlayerRecord()->updateMask(tb_player_record::_NORMALITEM);
	return true;
}
}