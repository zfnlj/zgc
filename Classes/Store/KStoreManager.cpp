#include "KStoreManager.h"
#include "KStoreCreate.h"
#include "../WorldObject/KCharacter.h"
#include "../WorldObject/KPlayer.h"
#include "System/Misc/KStream.h"
#include "CommonLogic/Store/KStoreDef.h"
#include "../Item/KBagManager.h"
#include "../Item/KItem.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Block/IBlock.h"
#include "../Block/KBlock.h"
#include "../Block/KBlockBoxManager.h"
#include "CommonLogic/Item/KStruct_Item.h"
//#include "Tooltip/KTooltipManager.h"
//#include "Attribute/KAttrDefine.h"
#include "CommonLogic/KMoney.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameRoot.h"
#include "../KNet/KNetMsgFacade.h"
#include "VirtualService.h"

using namespace KWorldObjAbout;
using namespace KItemAbout;
using namespace KBlockAbout;
using namespace KMoneyAbout;

namespace KStoreAbout
{
	KClientStoreProduct::KClientStoreProduct()
	{
	}

	KClientStoreProduct::~KClientStoreProduct()
	{
	}

	void KClientStoreProduct::Initialize()
	{
		m_nID = 0;						// 商品索引
		m_nStoreID = INVALID_STORE_ID;	// 商店ID
		m_nLabelID = 0;					// 标签ID
		m_dwItemID = 0;					// 物品ID
		m_nPileCount = 0;				// 一次买多少个
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			m_Prices[i].nMoneyType = enumPT_none;
			m_Prices[i].nTParam1 = -1;
			m_Prices[i].nTParam2 = -1;
		}
		m_nLimitSaleCount = 0;			// 一次最多买的个数
		m_bCanBuy = true;				// 能不能买
		m_bCanSee = true;				// 物品是否看见
	}

	bool KClientStoreProduct::Unserilize( const BYTE* pBuffer, int nBuffLen )
	{
		ASSERT_RETURN(pBuffer && nBuffLen > 0, false);
		KMsgInputStream is(pBuffer, nBuffLen);
		ASSERT_RETURN(is.ReadInt(m_nID), false);
		ASSERT_RETURN(is.ReadInt(m_nStoreID), false);
		ASSERT_RETURN(is.ReadInt(m_nLabelID), false);
		ASSERT_RETURN(is.ReadDword(m_dwItemID), false);
		ASSERT_RETURN(is.ReadInt(m_nPileCount), false);
		for(int i=0;i<PRICE_TYPE_MAX;i++)
		{
			BYTE* pp = (BYTE*)(&m_Prices[i]);
			ASSERT_RETURN(is.ReadByteArray(pp, sizeof(m_Prices[i])), false);
		}
		ASSERT_RETURN(is.ReadInt(m_nLimitSaleCount), false);
		BYTE nCanBuy = 0;
		ASSERT_RETURN(is.ReadByte(nCanBuy), false);
		m_bCanBuy = (0 != nCanBuy ? true : false);

		BYTE nCanSee = 0;
		ASSERT_RETURN(is.ReadByte(nCanSee), false);
		m_bCanSee = (0 != nCanSee ? true : false);

		return true;
	}
	
	int KClientStoreProduct::GetNumOfMoney() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_money == m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nTParam1;	// 金钱数
			}
		}
		return 0;
	}

	int KClientStoreProduct::GetNumOfBluePower() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_bluePower == m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nTParam1;	// 深蓝之力数
			}
		}
		return 0;
	}

	DWORD KClientStoreProduct::GetNumOfItem( int& nNum ) const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_item == m_Prices[i].nMoneyType)
			{
				nNum = m_Prices[i].nTParam2;	// 物品个数
				return m_Prices[i].nTParam1;	// 物品ID
			}
		}
		return 0;
	}
	
	int KClientStoreProduct::GetMoneyType() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_none != m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nMoneyType;	// 消耗类型
			}
		}
		return enumPT_none;
	}
	
	//////////////////////////////////////////////////////////////////////////
	KStoreManager::KStoreManager()
	{
	}

	KStoreManager::~KStoreManager()
	{
		_ClearClientStoreMap();
	}

	void KStoreManager::_ClearClientStoreMap()
	{
		MAP_CLIENTSTORE_PRODUCT::iterator itorProductTmp = m_mapClientStoreProduct.begin();
		for(;itorProductTmp!=m_mapClientStoreProduct.end();++itorProductTmp)
		{
			KClientStoreProduct* pClientStoreProduct = itorProductTmp->second;
			if(pClientStoreProduct)
			{
				KClientStoreProduct::Free(pClientStoreProduct);
			}
		}
		m_mapClientStoreProduct.clear();
		for(int i=0;i<STORE_STOREPOS_MAX;i++)
		{
			IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(enumBTID_StoreBox, i);
			if(pIB)
			{
				KBlockBoxManager::Instance()->Drop(enumBTID_StoreBox, i);
			}
		}

		// 回购物品列表
		int nHistorySize = m_vecItem.size();
		for(int i=0;i<nHistorySize;i++)
		{
			KItem* pItem = m_vecItem[i];
			if(pItem)
			{
				KItem::Free(pItem);
			}
		}
		m_vecItem.clear();

		for(int i=0;i<STORE_BUYBACKPOS_MAX;i++)
		{
			IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(enumBTID_StoreBuyBackBox, i);
			if(pIB)
			{
				KBlockBoxManager::Instance()->Drop(enumBTID_StoreBuyBackBox, i);
			}
		}
	}

	void KStoreManager::Reset()
	{
		m_nStoreID = INVALID_STORE_ID;
		m_pSeller = NULL;
		m_nLabelID = -1;
	}
	
	bool KStoreManager::InitStore()
	{
		_ClearClientStoreMap();
		return true;
	}

// CPP.cppStore.EnterStore(player, npc, 100)
	bool KStoreManager::EnterStoreRes( int nStoreID, UINT64 nSellerID )
	{
		//ASSERT_RETURN(INVALID_STORE_ID != nStoreID, false);
		//Reset();

		//m_nStoreID = nStoreID;
		//if(nSellerID > 0)
		//{
		//	m_pSeller = g_DynamicWorld.GetOneChar(nSellerID);
		//}

		//::System::Collections::KVector<int> vecLabel;
		//MAP_CLIENTSTORE_PRODUCT::iterator itorTmp = m_mapClientStoreProduct.begin();
		//for(;itorTmp!=m_mapClientStoreProduct.end();++itorTmp)
		//{
		//	KClientStoreProduct* pProduct = itorTmp->second;
		//	if(NULL == pProduct) continue;
		//	int nLabelSize = vecLabel.size();
		//	int i = 0;
		//	for(i=0;i<nLabelSize;i++)
		//	{
		//		if(pProduct->m_nLabelID == vecLabel[i])
		//		{
		//			break;
		//		}
		//	}
		//	if(i >= nLabelSize)
		//	{
		//		vecLabel.push_back(pProduct->m_nLabelID);
		//	}
		//}
		//StoreUIInfo SUII;
		//const char* pStoreTitle = KStringManager::Instance()->GetStringByID(nStoreID);
		//if(pStoreTitle)
		//{
		//	strcpy_k(SUII.szTitle, sizeof(SUII.szTitle), pStoreTitle);
		//}
		//int nLabelSize = vecLabel.size();
		//ASSERT_RETURN(nLabelSize <= STORE_LABEL_MAXNUM, false);
		//SUII.nLabelSize = nLabelSize;
		//for(int i=0;i<nLabelSize;i++)
		//{
		//	SUII.m_LabelInfo[i].nLabelID = vecLabel[i];
		//	const char* pLabelString = KStringManager::Instance()->GetStringByID(vecLabel[i]);
		//	if(pLabelString)
		//	{
		//		strcpy_k(SUII.m_LabelInfo[i].szLabel, sizeof(SUII.m_LabelInfo[i].szLabel), pLabelString);
		//	}
		//}
		//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_STORE_ENTER, 0, (unsigned long)&SUII);
		return true;
	}

	bool KStoreManager::LeaveStoreRes()
	{
	/*	Reset();
		_ClearClientStoreMap();
		g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_STORE_LEAVE, 0, 0);*/
		return true;
	}

	bool KStoreManager::LeaveStore()
	{
		//Reset();
		//_ClearClientStoreMap();

		//CS_StoreLeave SL;
		//g_DynamicWorld.SendToGatewayServer(CS_StoreLeave::s_nCode, &SL, sizeof(CS_StoreLeave));
		return true;
	}

	bool KStoreManager::AddOneProduct(const BYTE* pBuffer, int nBufferLen)
	{
		ASSERT_RETURN(pBuffer && nBufferLen > 0, false);
		KClientStoreProduct* pClientStoreProduct = KClientStoreProduct::Alloc();
		pClientStoreProduct->Initialize();
		if(!pClientStoreProduct->Unserilize(pBuffer, nBufferLen))
		{
			KClientStoreProduct::Free(pClientStoreProduct);
			return false;
		}
		if(!KItemAbout::KItemCreate::Instance()->GetItemCreateInfo(pClientStoreProduct->m_dwItemID))
		{
			ASSERT_I(false);
			KClientStoreProduct::Free(pClientStoreProduct);
			return false;
		}

		MAP_CLIENTSTORE_PRODUCT::iterator itorFind = m_mapClientStoreProduct.find(pClientStoreProduct->m_nID);
		if(itorFind != m_mapClientStoreProduct.end())
		{
			KClientStoreProduct::Free(pClientStoreProduct);
			return false;
		}
		
		int nSize = m_mapClientStoreProduct.size();
		m_mapClientStoreProduct[pClientStoreProduct->m_nID] = pClientStoreProduct;
		
		KStoreBlock* pStoreBlock = KStoreBlock::Alloc();
		int nNumOfMoney = pClientStoreProduct->GetNumOfMoney();
		pStoreBlock->SetProductItem(pClientStoreProduct->m_nLabelID, pClientStoreProduct->m_nID, nNumOfMoney, pClientStoreProduct->m_nLimitSaleCount);
		
		pStoreBlock->SetPos(enumBTID_StoreBox, nSize);
		KBlockBoxManager::Instance()->PutBlock(enumBTID_StoreBox, nSize, pStoreBlock);
		return true;
	}

	bool KStoreManager::AddBuyBackOneItem(const BYTE* pBuffer, int nBufferLen)
	{
		ASSERT_RETURN(pBuffer && nBufferLen > 0, false);
		KMsgInputStream is(pBuffer, nBufferLen);
		KItem* pItem = KItem::Alloc();
		pItem->Clear();
		if(!pItem->Unserilize(is))
		{
			KItem::Free(pItem);
			return false;
		}

		int nHistorySize = m_vecItem.size();
		if(nHistorySize >= STORE_BUYBACKPOS_MAX)
		{
			KItem* pItem0 = m_vecItem[0];
			if(pItem0)
			{
				KItem::Free(pItem0);
			}
			m_vecItem.erase(0);
			m_vecItem.push_back(pItem);

			// 删除最后一个block
			for(int i=0;i<STORE_BUYBACKPOS_MAX;i++)
			{
				KBlockBoxManager::Instance()->Drop(enumBTID_StoreBuyBackBox, i);
			}

			nHistorySize = m_vecItem.size();
			for(int j=0;j<nHistorySize;j++)
			{
				KStoreBuyBackBlock* pStoreBuyBackBlock = KStoreBuyBackBlock::Alloc();
				pStoreBuyBackBlock->SetPos(enumBTID_StoreBuyBackBox, j);
				KBlockBoxManager::Instance()->PutBlock(enumBTID_StoreBuyBackBox, j, pStoreBuyBackBlock);
			}
		}
		else
		{
			m_vecItem.push_back(pItem);

			KStoreBuyBackBlock* pStoreBuyBackBlock = KStoreBuyBackBlock::Alloc();
			pStoreBuyBackBlock->SetPos(enumBTID_StoreBuyBackBox, nHistorySize);
			KBlockBoxManager::Instance()->PutBlock(enumBTID_StoreBuyBackBox, nHistorySize, pStoreBuyBackBlock);
		}

		return true;
	}

	bool KStoreManager::IsOpening() const
	{
		return INVALID_STORE_ID != m_nStoreID;
	}

	bool KStoreManager::TryBuy( int nProductIndex, int nBuyCount )
	{
		ASSERT_RETURN(nBuyCount > 0, false);
		MAP_CLIENTSTORE_PRODUCT::iterator itorFind = m_mapClientStoreProduct.find(nProductIndex);
		ASSERT_RETURN(itorFind != m_mapClientStoreProduct.end(), false);

		KClientStoreProduct* pProduct = itorFind->second;
		if(!pProduct->m_bCanBuy)
		{
			return false;
		}
		if(!pProduct->m_bCanSee)
		{
			return false;
		}
		if(nBuyCount <= 0 || nBuyCount > STORE_ONCE_MAXNUM)
		{
			return false;
		}
		
		if(nBuyCount > pProduct->m_nLimitSaleCount)
		{
			return false;
		}
		
		int nMoneyType = pProduct->GetMoneyType();
		ASSERT_RETURN(nMoneyType > enumPT_none && nMoneyType < enumPT_count, false);
		switch(nMoneyType)
		{
		case enumPT_money:
			{
				int nCost = pProduct->GetNumOfMoney() * nBuyCount;
				if(!KMainPlayer::RealPlayer()->HasMoney(nCost))
				{
					// 1403957	背包金钱不足，无法购买
					KDynamicWorld::getSingleton().ShowSystemMsg(1403957);
					return false;
				}
			}
			break;

		//case enumPT_item:
		//	{
		//		KPlayer* pPlayer = KMainPlayer::Instance()->GetMainPlayer();
		//		ASSERT_RETURN(pPlayer, false);
		//		KBagManager* pBagMgr = pPlayer->GetBagManagerPtr();
		//		ASSERT_RETURN(pBagMgr, false);
		//		int nCostItemNum = 0;
		//		DWORD dwItemID = pProduct->GetNumOfItem(nCostItemNum);
		//		ASSERT_RETURN(dwItemID > 0, false);
		//		nCostItemNum *= nBuyCount;
		//		int nItemNum = pBagMgr->GetItemCountByID(dwItemID);
		//		if(nItemNum < nCostItemNum)
		//		{
		//			// 1416033	兑换失败，%s不足。
		//			char buff[100] = "";
		//			const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(dwItemID);
		//			ASSERT_RETURN(pCIIB && pCIIB->GetName(), false);
		//			const char* pInfo = KStringManager::Instance()->GetStringByID(1416033);
		//			if(pInfo)
		//			{
		//				sprintf_k(buff, sizeof(buff), pInfo, pCIIB->GetName());
		//			}
		//			KDynamicWorld::getSingleton().ShowSystemMsg(buff);
		//			return false;
		//		}
		//	}
		//	break;
		}
		
		if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
			KMoneyAbout::KMoney curMoney = KMainPlayer::RealPlayer()->m_money;
			curMoney.Increase(-pProduct->GetNumOfMoney());
			std::string s = curMoney.m_money.toString();
			KNetMsgFacade::onChangeMoney(s.c_str(),s.size()+1);
			VirtualService::getSingleton().AddItem(pProduct->m_dwItemID,1);
		}else{
			/*CS_StoreBuy SB;
			SB.nProductIdx = nProductIndex;
			SB.nNum = nBuyCount;
			KDynamicWorld::getSingleton().SendToGatewayServer(CS_StoreBuy::s_nCode, &SB, sizeof(CS_StoreBuy));*/
		}
		return false;
	}

	bool KStoreManager::TrySell( int nBoxID, int nPos )
	{
		//KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		//ASSERT_RETURN(pBagMgr, false);
		//int nBagID = pBagMgr->BoxID2BagID(nBoxID);
		//ASSERT_RETURN(-1 != nBagID, false);
		//
		//const KCellBag* pCellBag = pBagMgr->FindCell(nBagID, nPos);
		//ASSERT_RETURN(pCellBag, false);
		//DWORD dwItemID = pCellBag->GetItemID();
		//ASSERT_RETURN(dwItemID > 0, false);
		//
		//const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(dwItemID);
		//ASSERT_RETURN(pCIIB, false);
		//// 不能出售
		//if(pCIIB->IsNotSell())
		//{
		//	return false;
		//}
		//
		//CS_StoreSell SS;
		//SS.nBagID = nBagID;
		//SS.nPos = nPos;
		//g_DynamicWorld.SendToGatewayServer(CS_StoreSell::s_nCode, &SS, sizeof(CS_StoreSell));
		return true;
	}

	bool KStoreManager::TryBuyBack( int nPos )
	{
		//ASSERT_RETURN(nPos >= 0 && nPos < STORE_BUYBACKPOS_MAX, false);
		//int nHistorySize = m_vecItem.size();
		//if(nPos < 0 || nPos >= nHistorySize) return false;
		//const KItem* pItem = m_vecItem[nPos];
		//ASSERT_RETURN(pItem, false);
		//
		//const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(pItem->GetID());
		//ASSERT_RETURN(pCIIB, false);

		//int nPriceCount = pCIIB->GetPrice() * pItem->GetStackNumber();
		//if(!KMainPlayer::Instance()->HasMoney(nPos))
		//{
		//	// 1405230 背包金钱不足
		//	const char* pInfo = KStringManager::Instance()->GetStringByID(1405230);
		//	g_DynamicWorld.ShowSystemMsg(pInfo);
		//	return false;
		//}

		//CS_StoreBuyBack SBB;
		//SBB.nPos = nPos;
		//g_DynamicWorld.SendToGatewayServer(CS_StoreBuyBack::s_nCode, &SBB, sizeof(CS_StoreBuyBack));
		return true;
	}

	bool KStoreManager::BuyBackRes( const BYTE* pBuffer, int nBufferLen )
	{
		ASSERT_RETURN(pBuffer && nBufferLen > 0, false);
		KDataOutputStream os((void*)pBuffer, nBufferLen);
		BYTE nPos = 0;
		ASSERT_RETURN(os.ReadByte(nPos), false);
		int nHistorySize = m_vecItem.size();
		ASSERT_RETURN(nPos >= 0 && nPos < nHistorySize, false);

		KItem* pItem = m_vecItem[nPos];
		if(pItem)
		{
			KItem::Free(pItem);
		}
		m_vecItem.erase(nPos);
		// 删除最后一个block
		for(int i=0;i<STORE_BUYBACKPOS_MAX;i++)
		{
			KBlockBoxManager::Instance()->Drop(enumBTID_StoreBuyBackBox, i);
		}

		nHistorySize = m_vecItem.size();
		for(int j=0;j<nHistorySize;j++)
		{
			KStoreBuyBackBlock* pStoreBuyBackBlock = KStoreBuyBackBlock::Alloc();
			pStoreBuyBackBlock->SetPos(enumBTID_StoreBuyBackBox, j);
			KBlockBoxManager::Instance()->PutBlock(enumBTID_StoreBuyBackBox, j, pStoreBuyBackBlock);
		}

		return true;
	}

	bool KStoreManager::ProductChangeState()
	{
		// TODO: 还未使用
		//StoreChangeState SCS;
		//SCS.nLabel = 0;
		//SCS.nPos = 0;
		//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_STORE_CHANGESTATE, 0, (unsigned long)&SCS);
		return true;
	}

	KItemAbout::KItem* KStoreManager::GetBuyBackItem( int nPos )
	{
		int nHistorySize = m_vecItem.size();
		if(nPos >= 0 && nPos < nHistorySize)
		{
			return m_vecItem[nPos];
		}

		return NULL;
	}

	const KItemAbout::KItem* KStoreManager::GetBuyBackItem( int nPos ) const
	{
		int nHistorySize = m_vecItem.size();
		if(nPos >= 0 && nPos < nHistorySize)
		{
			return m_vecItem[nPos];
		}

		return NULL;
	}

	const KItemAbout::KCreateInfo_ItemBase* KStoreManager::GetItemCreateInfo( int nProductID ) const
	{
		MAP_CLIENTSTORE_PRODUCT::const_iterator itorFind = m_mapClientStoreProduct.find(nProductID);
		if(itorFind == m_mapClientStoreProduct.end())
		{
			return NULL;
		}

		const KClientStoreProduct* pProduct = itorFind->second;
		if(NULL == pProduct)
		{
			return NULL;
		}
		return KItemCreate::Instance()->GetItemCreateInfo(pProduct->m_dwItemID);
	}

	void KStoreManager::PopCommBuyDialog( int nFromBoxID, int nFromPos, bool bOne )
	{
	/*	PopCommBuyDialogParam PCBDP;
		PCBDP.nBoxID = nFromBoxID;

		IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nFromBoxID, nFromPos);
		if(NULL == pIB) return;

		PCBDP.nIndex = nFromPos;

		pIB->GetIcon(PCBDP.szIcon, sizeof(PCBDP.szIcon));
		PCBDP.nCount = pIB->GetCount();
		PCBDP.nFrequency = pIB->GetFrequency();
		PCBDP.nCost = pIB->GetCost();

		if(bOne)
		{
			PCBDP.nDefaultCount = 1;
		}
		else
		{
			PCBDP.nDefaultCount = pIB->GetCount();
		}
		g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_POPCOMMBUYDIALOG, 0, (unsigned long)(&PCBDP));*/
	}

};
