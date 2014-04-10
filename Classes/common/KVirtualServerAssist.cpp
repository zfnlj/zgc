#include "KVirtualServerAssist.h"

#include "../Item/KBagManager.h"
#include "../Item/KBag.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "../KNet/KNetMsgFacade.h"
using namespace KItemAbout;
namespace KVirtualServerAssist
{

bool AddItem(int itemId,int count)
{
	KItemAbout::KBagNormal* normalBag = KMainPlayer::Instance()->GetBagManagerPtr()->FindNormalBag();
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
		//pBAR->nPlayerID = m_pPlayerOwn->GetID();
		//pBAR->nOperatorType = eOptType;
		//pBAR->nLockType = nLockType;
		pBAR->nPos = slot;
		KDataOutputStream os(szTmp + sizeof(SC_BagAddResponse), sizeof(szTmp) - sizeof(SC_BagAddResponse));
		KItem item(itemId,count);
		item.Serilize(os);
		pBAR->nItemBufferSize = os.m_pos;
		nSendLen = sizeof(SC_BagAddResponse) + os.m_pos;

	}
	KNetMsgFacade::onBagOperation(szTmp,nSendLen);
	return true;
}
}