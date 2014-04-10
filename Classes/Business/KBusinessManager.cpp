#include "KBusinessManager.h"
#include "Inc/KBusinessDefine.h"
#include "../WorldObject/KPlayer.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "../Item/KItem.h"
#include "../Block/IBlock.h"
#include "../Block/KBlock.h"
#include "../Block/KBlockBox.h"
#include "../Block/KBlockBoxManager.h"
#include "../WorldObject/KMainPlayer.h"
//#include "CommonLogic/Business/KBusinessCheck.h"

using namespace KWorldObjAbout;
using namespace KItemAbout;
using namespace KBlockAbout;

namespace KBusinessAbout
{
	KBusinessManager::KBusinessManager()
	{
		m_bMyLockState = false;
		m_bOtherLockState = false;
	}

	KBusinessManager::~KBusinessManager()
	{
	}
	
	void KBusinessManager::Reset()
	{
		m_bMyLockState = false;
		m_bOtherLockState = false;
		m_moneyMyPutMoney.m_money.ToZero();
	}

	void KBusinessManager::BusinessRequest(const SC_BusinessRequest* pBR)
	{
		//ASSERT_RETURN_VOID(pBR);

		//if(SC_BusinessRequest::enumBR_Suc == pBR->nOpen) // 弹出交易提示框
		//{
		//	ASSERT_RETURN_VOID(pBR->key.IsValid());
		//	UINT64 nOtherPlayerID = pBR->key.GetOtherPlayer(KMainPlayer::Instance()->GetMainPlayerID());
		//	ASSERT_RETURN_VOID(nOtherPlayerID > 0);
		//	BusinessMsgBoxParam BMBP = {0};
		//	BMBP.key1 = pBR->key.GetKey1();
		//	BMBP.key2 = pBR->key.GetKey2();
		//	BMBP.bIsOpen = true;
		//	KPlayer* pRequestPlayer = g_DynamicWorld.GetOnePlayer(nOtherPlayerID);
		//	if(pRequestPlayer)
		//	{
		//		// 1403332	%s请求与您交易
		//		const char* pMsg = KStringManager::Instance()->GetStringByID(1403332);
		//		const char* pRequestName = pRequestPlayer->GetName();
		//		if(pMsg && pRequestName)
		//		{
		//			sprintf_k(BMBP.szMsg, sizeof(BMBP.szMsg), pMsg, pRequestName);
		//		}
		//		g_DynamicWorld.ShowSystemMsg(BMBP.szMsg);
		//		g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_MSGBOX, 0, (unsigned long)(&BMBP));
		//	}
		//	return;
		//}

		//// 失败的处理
		//m_key.Reset();
		//if(SC_BusinessRequest::enumBR_CloseMsgBox == pBR->nOpen)	// 关闭
		//{
		//	BusinessMsgBoxParam BMBP = {0};
		//	BMBP.key1 = 0;
		//	BMBP.key2 = 0;
		//	BMBP.bIsOpen = false;
		//	g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_MSGBOX, 0, (unsigned long)(&BMBP));
		//}
		//else
		//{
		//	const char* pInfo = NULL;
		//	if(SC_BusinessRequest::enumBR_OtherDead == pBR->nOpen)
		//	{
		//		// 1403353	对方处于死亡状态，不可交易
		//		pInfo = KStringManager::Instance()->GetStringByID(1403353);
		//	}
		//	else if(SC_BusinessRequest::enumBR_OtherInFight == pBR->nOpen)
		//	{
		//		// 1403354	对方处于战斗状态，不可交易
		//		pInfo = KStringManager::Instance()->GetStringByID(1403354);
		//	}
		//	else if(SC_BusinessRequest::enumBR_OtherInBusiness == pBR->nOpen)
		//	{
		//		// 1403356	对方正在交易，不可交易
		//		pInfo = KStringManager::Instance()->GetStringByID(1403356);
		//	}
		//	else if(SC_BusinessRequest::enumBR_SeflInFight == pBR->nOpen)
		//	{
		//		// 1406026	您处于战斗状态，不可交易
		//		pInfo = KStringManager::Instance()->GetStringByID(1406026);
		//	}
		//	g_DynamicWorld.ShowHeadInfoMsg(pInfo);
		//}
	}

	void KBusinessManager::BusinessConfirm(const SC_BusinessConfirm* pBC)
	{
		//ASSERT_RETURN_VOID(pBC);

		//ASSERT_RETURN_VOID(pBC->key.IsValid());
		//UINT64 nOtherPlayerID = pBC->key.GetOtherPlayer(KMainPlayer::Instance()->GetMainPlayerID());
		//ASSERT_RETURN_VOID(nOtherPlayerID > 0);

		//if(SC_BusinessConfirm::enumBC_Agree == pBC->nConfirm)	// 同意交易
		//{
		//	char szTmp[1024] = "";
		//	if(pBC->key.GetKey1() == KMainPlayer::Instance()->GetMainPlayerID())
		//	{
		//		// 1403334	%s接受与您交易
		//		const char* pInfo = KStringManager::Instance()->GetStringByID(1403334);
		//		KPlayer* pOther = g_DynamicWorld.GetOnePlayer(pBC->key.GetKey2());
		//		const char* pOtherName = pOther ? pOther->GetName() : NULL;
		//		if(pInfo && pOtherName)
		//		{
		//			sprintf_k(szTmp, sizeof(szTmp), pInfo, pOtherName);
		//			g_DynamicWorld.ShowSystemMsg(szTmp);
		//		}
		//	}
		//	else
		//	{
		//		// 1403336	您开始和%s进行交易
		//		const char* pInfo = KStringManager::Instance()->GetStringByID(1403336);
		//		KPlayer* pOther = g_DynamicWorld.GetOnePlayer(pBC->key.GetKey1());
		//		const char* pOtherName = pOther ? pOther->GetName() : NULL;
		//		if(pInfo && pOtherName)
		//		{
		//			sprintf_k(szTmp, sizeof(szTmp), pInfo, pOtherName);
		//			g_DynamicWorld.ShowSystemMsg(szTmp);
		//		}
		//	}
		//	BusinessDialogParam BDP;
		//	BDP.bIsOpen = true;
		//	BDP.key1 = pBC->key.GetKey1();
		//	BDP.key2 = pBC->key.GetKey2();
		//	g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_DIALOG, nOtherPlayerID, (unsigned long)(&BDP));
		//	StartBusiness(pBC->key);
		//}
		//else // 拒绝交易
		//{
		//	m_key.Reset();
		//	KPlayer* pOtherPlayer = g_DynamicWorld.GetOnePlayer(nOtherPlayerID);
		//	ASSERT_RETURN_VOID(pOtherPlayer);
		//	const char* pOtherName = pOtherPlayer->GetName();
		//	// 1403333	%s拒绝与您交易
		//	const char* pMsg = KStringManager::Instance()->GetStringByID(1403333);
		//	if(pMsg && pOtherName)
		//	{
		//		char szTmp[1024];
		//		sprintf_k(szTmp, sizeof(szTmp), pMsg, pOtherName);
		//		g_DynamicWorld.ShowSystemMsg(szTmp);
		//	}
		//}
	}

	void KBusinessManager::BusinessPutOneItem( const SC_BusinessPutOneItem* pBPOI )
	{
		//ASSERT_RETURN_VOID(pBPOI);

		//KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		//ASSERT_RETURN_VOID(pBagMgr);
		//int nBoxID = pBagMgr->BagId2BoxID(pBPOI->nBagID);
		//ASSERT_RETURN_VOID(-1 != nBoxID);

		//if(CS_BusinessPutOneItem::enumBPOI_Add == pBPOI->nOperaFlag)
		//{
		//	KSingleItemLinkBlock* pSingleItemLink = KSingleItemLinkBlock::Alloc();
		//	pSingleItemLink->SetItemBlock(nBoxID, pBPOI->nBagPos);
		//	pSingleItemLink->SetPos(enumBTID_BusinessSelfBox, pBPOI->nBusinessPos);
		//	KBlockBoxManager::Instance()->PutBlock(enumBTID_BusinessSelfBox, pBPOI->nBusinessPos, pSingleItemLink);
		//	pBagMgr->ChangeItemState(pSingleItemLink->GetLinkBoxID(), pSingleItemLink->GetLinkPos(), true);
		//}
		//else if(CS_BusinessPutOneItem::enumBPOI_Remove == pBPOI->nOperaFlag)
		//{
		//	IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(enumBTID_BusinessSelfBox, pBPOI->nBusinessPos);
		//	if(pIB)
		//	{
		//		KSingleItemLinkBlock* pSingleItemLink = dynamic_cast<KSingleItemLinkBlock*>(pIB);
		//		if(pSingleItemLink)
		//		{
		//			pBagMgr->ChangeItemState(pSingleItemLink->GetLinkBoxID(), pSingleItemLink->GetLinkPos(), false);
		//		}
		//	}

		//	KBlockBoxManager::Instance()->Drop(enumBTID_BusinessSelfBox, pBPOI->nBusinessPos);
		//}
		//else
		//{
		//	ASSERT_I(false);
		//}
	}
	
	void KBusinessManager::BusinessPutMoney(UINT64 nID, const char* pMoney)
	{
		//ASSERT_RETURN_VOID(pMoney);

		//if(KMainPlayer::Instance()->GetMainPlayerID() == nID)
		//{
		//	m_moneyMyPutMoney.m_money = pMoney;
		//}
		//
		//BusinessPutMoneyParam BPMP;
		//strcpy_k(BPMP.szPutMoney, sizeof(BPMP.szPutMoney), pMoney);
		//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_PUTMONEY, nID, (unsigned long)(&BPMP));
	}

	bool KBusinessManager::BusinessShowOneItem(int nBusinessPos, int nOperaFlag, KItemAbout::KItem* pItem)
	{
	/*	ASSERT_RETURN(nOperaFlag > CS_BusinessPutOneItem::enumBPOI_None && nOperaFlag < CS_BusinessPutOneItem::enumBPOI_Count, false);

		BusinessCellMapVec::iterator itorFind = m_ListOtherItem.find(nBusinessPos);
		if(CS_BusinessPutOneItem::enumBPOI_Add == nOperaFlag)
		{
			ASSERT_RETURN(pItem, false);
			if(itorFind != m_ListOtherItem.end())
			{
				return false;
			}

			m_ListOtherItem[nBusinessPos] = pItem;

			KBusinessBlock* pBusinessBlock = KBusinessBlock::Alloc();
			pBusinessBlock->SetPos(enumBTID_BusinessOtherBox, nBusinessPos);
			KBlockBoxManager::Instance()->PutBlock(enumBTID_BusinessOtherBox, nBusinessPos, pBusinessBlock);
		}
		else if(CS_BusinessPutOneItem::enumBPOI_Remove == nOperaFlag)
		{
			if(itorFind == m_ListOtherItem.end())
			{
				return false;
			}
			KBlockBoxManager::Instance()->Drop(enumBTID_BusinessOtherBox, nBusinessPos);
			KItem* pBusinessItem = itorFind->second;
			if(pBusinessItem)
			{
				KItem::Free(pBusinessItem);
			}
			m_ListOtherItem.erase(itorFind);
		}
		else
		{
			return false;
		}
*/
		return true;
	}

	void KBusinessManager::BusinessCancel(const SC_BusinessCancel* pBC)
	{
		//ASSERT_RETURN_VOID(pBC);

		//UINT64 nOtherPlayerID = m_key.GetOtherPlayer(KMainPlayer::Instance()->GetMainPlayerID());
		//if(m_key.IsValid())
		//{
		//	Reset();

		//	BusinessDialogParam BDP;
		//	BDP.key1 = m_key.GetKey1();
		//	BDP.key2 = m_key.GetKey2();
		//	BDP.bIsOpen = false;
		//	g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_DIALOG, nOtherPlayerID, (unsigned long)(&BDP));
		//	KBusinessAbout::KBusinessManager::Instance()->CloseBusiness();
		//}

		//char szTmp[1024] = "";
		//switch(pBC->nReason)
		//{
		//case SC_BusinessCancel::enumBC_OtherCancel:
		//	{
		//		// %s放弃交易
		//		const char* pInfo = KStringManager::Instance()->GetStringByID(1403335);
		//		KPlayer* pOtherPlayer = g_DynamicWorld.GetOnePlayer(nOtherPlayerID);
		//		const char* pOtherName = NULL;
		//		if(pOtherPlayer)
		//		{
		//			pOtherName = pOtherPlayer->GetName();
		//		}
		//		if(pOtherName && pInfo)
		//		{
		//			sprintf_k(szTmp, sizeof(szTmp), pInfo, pOtherName);
		//		}
		//	}
		//	break;
		//case SC_BusinessCancel::enumBC_SelfCancel:
		//	{
		//		// 1403358	您放弃了交易
		//		const char* pInfo = KStringManager::Instance()->GetStringByID(1403358);
		//		if(pInfo)
		//		{
		//			sprintf_k(szTmp, sizeof(szTmp), pInfo);
		//		}
		//	}
		//	break;
		//case SC_BusinessCancel::enumBC_DistanceBeyond:
		//	{
		//		// 1405212	距离过远，交易失败
		//		const char* pInfo = KStringManager::Instance()->GetStringByID(1405212);
		//		if(pInfo)
		//		{
		//			sprintf_k(szTmp, sizeof(szTmp), pInfo);
		//		}
		//	}
		//	break;
		//default:
		//	return;
		//}
		//g_DynamicWorld.ShowSystemMsg(szTmp);
	}
	
	void KBusinessManager::BusinessLock(const SC_BusinessLock* pBL)
	{
		//ASSERT_RETURN_VOID(pBL && m_key.IsValid());

		//const char* pInfo = NULL;
		//switch(pBL->nRet)
		//{
		//case SC_BusinessLock::enumBL_suc:
		//	{
		//		if(false == m_bOtherLockState && pBL->bOtherLockState)
		//		{
		//			// 1403337	对方已点击锁定，等待交易
		//			const char* pInfo = KStringManager::Instance()->GetStringByID(1403337);
		//			g_DynamicWorld.ShowSystemMsg(pInfo);
		//		}
		//		m_bMyLockState = pBL->bMyLockState;
		//		m_bOtherLockState = pBL->bOtherLockState;

		//		BusinessLockState BLS;
		//		BLS.key1 = m_key.GetKey1();
		//		BLS.key2 = m_key.GetKey2();
		//		BLS.bMyLockState = pBL->bMyLockState;
		//		BLS.bOtherLockState = pBL->bOtherLockState;
		//		g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_LOCKSTATE, 0, (unsigned long)(&BLS));
		//	}
		//	break;
		//case SC_BusinessLock::enumBL_allEmtpy:
		//	{
		//		// 1403360	未放入物品，金钱，无法锁定
		//		pInfo = KStringManager::Instance()->GetStringByID(1403360);
		//	}
		//	break;
		//}
		//g_DynamicWorld.ShowSystemMsg(pInfo);
	}

	void KBusinessManager::BusinessStart(const SC_BusinessStart* pBS)
	{
		//ASSERT_RETURN_VOID(pBS && m_key.IsValid());
		//if(false == pBS->bMyStartState && pBS->bOtherStartState)
		//{
		//	// 1403338	对方已点击交易，等待交易
		//	const char* pInfo = KStringManager::Instance()->GetStringByID(1403338);
		//	g_DynamicWorld.ShowSystemMsg(pInfo);
		//}

		//BusinessStartState BSS;
		//BSS.key1 = m_key.GetKey1();
		//BSS.key2 = m_key.GetKey2();
		//BSS.bMyStartState = pBS->bMyStartState;
		//BSS.bOtherStartState = pBS->bOtherStartState;
		//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_STARTSTATE, 0, (unsigned long)(&BSS));
	}

	void KBusinessManager::BusinessFinish(const SC_BusinessFinish* pBF)
	{
		//ASSERT_RETURN_VOID(pBF && m_key.IsValid());

		//UINT64 nOtherPlayerID = m_key.GetOtherPlayer(KMainPlayer::Instance()->GetMainPlayerID());
		//ASSERT_RETURN_VOID(nOtherPlayerID > 0);

		//Reset();
		//if(enumIO_sucess == pBF->nRet)
		//{
		//	BusinessDialogParam BDP;
		//	BDP.key1 = m_key.GetKey1();
		//	BDP.key2 = m_key.GetKey2();
		//	BDP.bIsOpen = false;

		//	g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_DIALOG, nOtherPlayerID, (unsigned long)(&BDP));
		//	CloseBusiness();

		//	// 1403344	交易成功
		//	const char* pInfo = KStringManager::Instance()->GetStringByID(1403344);
		//	g_DynamicWorld.ShowSystemMsg(pInfo);
		//}
		//else
		//{
		//	g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BUSINESS_FAILED, 0, 0);
		//	const char* pInfo = NULL;
		//	switch(pBF->nRet)
		//	{
		//	case enumIO_bagNotEnough:
		//		// 1403512	背包物品已满，不能放入请求清理背包
		//		pInfo = KStringManager::Instance()->GetStringByID(1403512);
		//		break;
		//	case enumIO_bagNotEnoughOther:
		//		// 1403359	对方背包不足，交易失败
		//		pInfo = KStringManager::Instance()->GetStringByID(1403359);
		//		break;
		//	}

		//	g_DynamicWorld.ShowSystemMsg(pInfo);
		//}
	}

	void KBusinessManager::TryBusinessRequest( UINT64 nBusinessID )
	{
		//ASSERT_RETURN_VOID(nBusinessID > 0);
		//KPlayer* pPlayer = g_DynamicWorld.GetOnePlayer(nBusinessID);
		//ASSERT_RETURN_VOID(pPlayer);
		//
		//const char* pName = pPlayer->GetName();
		//// 1416024	已向%s发出交易申请
		//const char* pInfo = KStringManager::Instance()->GetStringByID(1416024);
		//if(pInfo && pName)
		//{
		//	char szInfo[100];
		//	sprintf_k(szInfo, sizeof(szInfo), pInfo, pName);
		//	g_DynamicWorld.ShowSystemMsg(szInfo);
		//}

		//CS_BusinessRequest BR;
		//BR.nBusinessID = nBusinessID;
		//g_DynamicWorld.SendToGatewayServer(CS_BusinessRequest::s_nCode, &BR, sizeof(BR));
	}

	void KBusinessManager::TryBusinessConfirm( const BusinessKey& key, bool bConfirm )
	{
	/*	ASSERT_RETURN_VOID(key.IsValid());

		CS_BusinessConfirm BC;
		BC.key = key;
		BC.bConfirm = bConfirm;
		g_DynamicWorld.SendToGatewayServer(CS_BusinessConfirm::s_nCode, &BC, sizeof(BC));*/
	}

	void KBusinessManager::TryBusinessPutOneItem( int nBoxID, int nBagPos, int nBusinessPos, int nOperaFlag)
	{
	/*	if(!CanBusiness()) return;
		ASSERT_RETURN_VOID(m_key.IsValid());
		ASSERT_RETURN_VOID(nOperaFlag > CS_BusinessPutOneItem::enumBPOI_None && nOperaFlag < CS_BusinessPutOneItem::enumBPOI_Count);
		
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetMainPlayer()->GetBagManagerPtr();
		ASSERT_RETURN_VOID(pBagMgr);
		int nBagID = pBagMgr->BoxID2BagID(nBoxID);
		ASSERT_RETURN_VOID(-1 != nBagID);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, nBagPos);
		if(CS_BusinessPutOneItem::enumBPOI_Add == nOperaFlag)
		{
			ASSERT_RETURN_VOID(pCell);
			const KItem& item = pCell->GetItem();
			if(!KBusinessCheck::Instance()->CheckItemByBusiness(&item))
			{
				return;
			}
		}
		else if(CS_BusinessPutOneItem::enumBPOI_Remove == nOperaFlag)
		{
			if(pCell) return;
		}
		else
		{
			return;
		}

		CS_BusinessPutOneItem BPOI;
		BPOI.key = m_key;
		BPOI.nOperaFlag = (BYTE)nOperaFlag;
		BPOI.nBagID = nBagID;
		BPOI.nBagPos = nBagPos;
		BPOI.nBusinessPos = nBusinessPos;
		g_DynamicWorld.SendToGatewayServer(CS_BusinessPutOneItem::s_nCode, &BPOI, sizeof(BPOI));*/
	}

	void KBusinessManager::TryBusinessLockStateChange( bool bLockState )
	{
	/*	ASSERT_RETURN_VOID(m_key.IsValid());

		CS_BusinessLock BL;
		BL.key = m_key;
		BL.bLockState = bLockState;
		g_DynamicWorld.SendToGatewayServer(CS_BusinessLock::s_nCode, &BL, sizeof(BL));*/
	}

	void KBusinessManager::TryBusinessStart()
	{
	/*	ASSERT_RETURN_VOID(m_key.IsValid());
		CS_BusinessStart BS;
		BS.key = m_key;
		g_DynamicWorld.SendToGatewayServer(CS_BusinessStart::s_nCode, &BS, sizeof(BS));*/
	}

	void KBusinessManager::TryBusinessCancel()
	{
	/*	ASSERT_RETURN_VOID(m_key.IsValid());
		CS_BusinessCancel BC;
		BC.key = m_key;
		g_DynamicWorld.SendToGatewayServer(CS_BusinessCancel::s_nCode, &BC, sizeof(BC));*/
	}

	void KBusinessManager::StartBusiness( const BusinessKey& key )
	{
		m_key = key;
		_ClearOtherAllCell();
	}

	const BusinessKey& KBusinessManager::GetBusinessKey() const
	{
		return m_key;
	}

	KWorldObjAbout::KPlayer* KBusinessManager::GetBusinessPlayer()
	{
		//ASSERT_RETURN(m_key.IsValid(), NULL);
		//UINT64 nOtherPlayerID = m_key.GetOtherPlayer(KMainPlayer::Instance()->GetMainPlayerID());
		//ASSERT_RETURN(nOtherPlayerID, NULL);
		//return g_DynamicWorld.GetOnePlayer(nOtherPlayerID);
		return NULL;
	}

	void KBusinessManager::_ClearOtherAllCell()
	{
		BusinessCellMapVec::iterator itorTmp = m_ListOtherItem.begin();
		for(;itorTmp!=m_ListOtherItem.end();++itorTmp)
		{
			KItemAbout::KItem* pItem = itorTmp->second;
			if(pItem)
			{
				KItemAbout::KItem::Free(pItem);
			}
		}
		m_ListOtherItem.clear();
	}

	KItemAbout::KItem* KBusinessManager::FindBusinessCell( int nPos )
	{
		BusinessCellMapVec::iterator itorTmp = m_ListOtherItem.find(nPos);
		if(itorTmp == m_ListOtherItem.end())
		{
			return NULL;
		}

		return itorTmp->second;
	}

	void KBusinessManager::CloseBusiness()
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		for(int i=0;i<BUSINESS_POS_MAX;i++)
		{
			IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(enumBTID_BusinessSelfBox, i);
			if(pIB && pBagMgr)
			{
				KSingleItemLinkBlock* pSingleItemLink = dynamic_cast<KSingleItemLinkBlock*>(pIB);
				if(pSingleItemLink)
				{
					pBagMgr->ChangeItemState(pSingleItemLink->GetLinkBoxID(), pSingleItemLink->GetLinkPos(), false);
				}
			}
			KBlockBoxManager::Instance()->Drop(enumBTID_BusinessOtherBox, i);
			KBlockBoxManager::Instance()->Drop(enumBTID_BusinessSelfBox, i);
		}

		m_key.Reset();
		_ClearOtherAllCell();
	}

	bool KBusinessManager::IsBusiness() const
	{
		return m_key.IsValid();
	}

	bool KBusinessManager::_CheckRequestBusinessState()
	{
		//KPlayer* pMainPlayer = KMainPlayer::Instance()->GetMainPlayer();
		//ASSERT_RETURN(pMainPlayer, false);

		//if(m_key.IsValid())
		//{
		//	const char* pInfo = KStringManager::Instance()->GetStringByID(1403357);
		//	g_DynamicWorld.ShowHeadInfoMsg(pInfo);
		//	return false;
		//}

		return true;
	}

	void KBusinessManager::MoveItemToBusiness( int nBoxID, int nPos )
	{
		//ASSERT_RETURN_VOID(enumBTID_ItemNormalBox == nBoxID || enumBTID_Bp_ItemMaterialBox == nBoxID);

		//IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nBoxID, nPos);
		//ASSERT_RETURN_VOID(pIB);
		//if(pIB->GetLock())
		//{
		//	return;
		//}

		//// 找个空位置放物品上去
		//for(int i=0;i<BUSINESS_POS_MAX;i++)
		//{
		//	IBlock* pIBlock = KBlockBoxManager::Instance()->GetBlock(enumBTID_BusinessSelfBox, i);
		//	if(NULL == pIBlock)
		//	{
		//		TryBusinessPutOneItem(nBoxID, nPos, i, CS_BusinessPutOneItem::enumBPOI_Add);
		//		break;
		//	}
		//}
	}

	void KBusinessManager::MoveItemToBag( int nBoxID, int nBagPos, int nFromPos )
	{
		IBlock* pIBlock = KBlockBoxManager::Instance()->GetBlock(enumBTID_BusinessSelfBox, nFromPos);
		ASSERT_RETURN_VOID(pIBlock);
		TryBusinessPutOneItem(nBoxID, nBagPos, nFromPos, CS_BusinessPutOneItem::enumBPOI_Remove);
	}

	bool KBusinessManager::CanBusiness() const
	{
		if(m_bOtherLockState && m_bMyLockState)
		{
			return false;
		}
		return true;
	}

	void KBusinessManager::TryBusinessPutMoney( const char* pMoney)
	{
		//ASSERT_RETURN_VOID(pMoney);
		//if(!CanBusiness()) return;
		//ASSERT_RETURN_VOID(m_key.IsValid());
		//KMoneyAbout::KMoney moneyPut;
		//moneyPut.m_money = pMoney;
		//ASSERT_RETURN_VOID(moneyPut.m_money >= 0 && moneyPut.m_money <= BUSINESS_PUTMONEY_MAX);
		//
		//// 未变化不更新
		//if(m_moneyMyPutMoney.m_money == moneyPut.m_money) return;
		//
		//if(!KMainPlayer::Instance()->HasMoney(moneyPut.m_money))
		//{
		//	// 1405230 背包金钱不足
		//	const char* pInfo = KStringManager::Instance()->GetStringByID(1405230);
		//	g_DynamicWorld.ShowSystemMsg(pInfo);
		//	return;
		//}

		//char buff[512];
		//KDataOutputStream os(buff, sizeof(buff));
		//ASSERT_RETURN_VOID(os.WriteByteArray(&m_key, sizeof(m_key)));
		//ASSERT_RETURN_VOID(os.WriteString(pMoney));

		//g_DynamicWorld.SendToGatewayServer(s_nCS_BusinessPutMoney, os.m_pBuf, os.m_pos);
	}

}

