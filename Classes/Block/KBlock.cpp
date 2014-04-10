#include "KBlock.h"
#include "KBlockBoxManager.h"
#include "../WorldObject/KPlayer.h"
#include "../Item/KItemCreate.h"
#include "../Item/KItem.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "System/Algorithm/AngleComputer.h"
#include "../WorldObject/KMainPlayer.h"
#include "CommonLogic/WorldObject/kAttrDefines.h"
#include "../Business/KBusinessManager.h"
#include "../Store/KStoreManager.h"

using namespace KWorldObjAbout;
using namespace KItemAbout;
using namespace KAttributeAbout;
using namespace KBusinessAbout;

namespace KBlockAbout
{

	//******************************************************************************************/
	//背包物品block
	// UI中KItemBlock关联格子中的数据
	void KItemBlock::Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const
	{
		int nBagID = GetBagID();
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return;
		}

	
		if (nBagID == enum_item_BagNormal)
		{
			// 装备物品
		}
	}

	bool KItemBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	bool KItemBlock::GetCDTime( int& passedTime, int& totalTime ) const
	{
		return true;
	}

	int KItemBlock::GetStatus() const
	{
		// TODO:
		return 0;
	}

	int KItemBlock::GetCount() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, 0);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, 0);

		return pCell->GetItemStackNumber();
	}
	
	bool KItemBlock::IsFrequency() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, false);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, false);

		return pCell->IsFrequency();
	}

	int KItemBlock::GetFrequency() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, 0);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, 0);

		return pCell->GetFrequency();
	}

	bool KItemBlock::GetTooltip( void* buffer, int size ) const
	{

		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, false);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, false);

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		/*const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, &item, pCIIB, enumITT_myself, enum_item_BagEquip == nBagID);
		strcpy_k((char*)buffer, size, p);*/
		return true;
	}

	void KItemBlock::OnDestroy()
	{
		// 不应该放在这里，因为必须是在物品删除成功下才能删除block并更新界面
		//KMainPlayer::Instance()->TryDropItem(GetBagType(), m_posInfo);
	}

	const KCreateInfo_ItemBase* KItemBlock::GetItemCreateInfo() const
	{
		int nBagID = GetBagID();

		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, NULL);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, NULL);
		const KItem& item = pCell->GetItem();

		return pBagMgr->GetItemCreateInfo(item.GetID());
	}


	int KItemBlock::GetBagID() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, -1);
		return pBagMgr->BoxID2BagID(m_boxID);
	}

	bool KItemBlock::Release()
	{
		KItemBlock::Free(this);
		return true;
	}

	bool KItemBlock::IsUseStatus() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return false;
		
		int nLevel = KMainPlayer::RealPlayer()->GetAttrValue(ca_level);
		if(nLevel >= pCIIB->GetLevel()) return false;
		return true;
	}

	bool KItemBlock::GetDragable() const
	{
		return IBlock::GetDragable();
	}
	
	const char* KItemBlock::GetName() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return NULL;
		return pCIIB->GetName();
	}

	int KItemBlock::GetQuality() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return 0;
		return pCIIB->GetQuality();
	}

	// -------------------------------------- KItemLinkBlock -------------------------------------------
	KItemLinkBlock::KItemLinkBlock()
		:m_LinkBoxID(0), m_LinkPos(0), m_ItemID(0), m_SkillGroup(0)
	{

	}

	void KItemLinkBlock::SetItemBlock(int BoxID, int Pos)
	{
		m_LinkBoxID = BoxID;
		m_LinkPos = Pos;

		m_ItemID = GetItemCreateInfo()->GetItemID();
		
	}
	
	bool KItemLinkBlock::GetIcon(void* buffer, int size) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetIcon(buffer, size);
	}

	bool KItemLinkBlock::GetCDTime(int& passedTime, int& totalTime) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
// 		if(NULL == Link) return false; // TODO: 先这么判断一下
		return Link->GetCDTime(passedTime, totalTime);
	}

	int KItemLinkBlock::GetStatus() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetStatus();
	}

	int KItemLinkBlock::GetCount() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetCount();
	}

	bool KItemLinkBlock::GetTooltip(void* buffer, int size) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetTooltip(buffer, size);
	}

	void KItemLinkBlock::Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const
	{
		//int nBoxID = GetBoxID();
		//// 如果当前交易框打开，向交易框放物品
		//if(enumBTID_BusinessSelfBox == nBoxID)
		//{
		//	KBusinessAbout::KBusinessManager::Instance()->MoveItemToBag(m_LinkBoxID, m_LinkPos, m_posInfo);
		//	return;
		//}

		//const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		//Link->Use(x, y, bOnlyUse, bOnlyPrepare);
	}

	const KCreateInfo_ItemBase* KItemLinkBlock::GetItemCreateInfo() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetItemCreateInfo();
	}

	int KItemLinkBlock::GetLinkBoxID() const
	{
		return m_LinkBoxID;
	}

	int KItemLinkBlock::GetLinkPos() const
	{
		return m_LinkPos;
	}

	void KItemLinkBlock::SetLinkPos(int Pos)
	{
		SetItemBlock(m_LinkBoxID, Pos);
	}

	bool KItemLinkBlock::Exist()const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link != NULL;
	}
	
	// 是否显示使用次数，而不是堆叠数
	bool KItemLinkBlock::IsFrequency() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, false);
		return Link->IsFrequency();
	}

	// 得到使用次数
	int KItemLinkBlock::GetFrequency() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, 0);
		return Link->GetFrequency();
	}

	bool KItemLinkBlock::Release()
	{
		KItemLinkBlock::Free(this);
		return true;
	}

	bool KItemLinkBlock::IsUseStatus() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, false);
		return Link->IsUseStatus();
	}

	const char* KItemLinkBlock::GetName() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return NULL;
		return pCIIB->GetName();
	}

	// --------------------------------------- KItemSumBlock ----------------------------------------
	bool KItemSumBlock::GetIcon(void* buffer, int size) const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if (pCIIB)
		{
			strcpy_k((char*)buffer, size, pCIIB->s_icon);
			return true;
		}

		return false;
	}

	bool KItemSumBlock::GetCDTime(int& passedTime, int& totalTime) const
	{
		return true;
	}

namespace{
	class BagItemVisitor
	{
	public:
		BagItemVisitor(const KCreateInfo_ItemBase* ItemBase)
			:m_ItemBase(ItemBase), m_Count(0)
		{}
		void operator()(int type, int pos)
		{
			KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
			ASSERT_RETURN_VOID(pBagMgr);
			KCellBag* pCell = pBagMgr->FindCell(type, pos);
			if(pCell)
			{
				KItem& item = pCell->GetItem();
				if(item.GetID() == m_ItemBase->s_nItemID && m_ItemBase->s_nMaxStacks > 1)
				{
					m_Count += item.GetStackNumber();
				}
			}
		}

		int GetCount()
		{
			return m_Count;
		}

	protected:
		const KCreateInfo_ItemBase* m_ItemBase;
		int m_Count;
	};

}
	int KItemSumBlock::GetCount() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, 0);
		return pBagMgr->GetItemCountByID(m_BagType, m_ItemID);
	}

	bool KItemSumBlock::GetTooltip(void* buffer, int size) const
	{
	/*	KWorldObjAbout::KCharacter* pChar = KMainPlayer::Instance()->GetMainPlayer();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, false);

		int nBagID = GetBagID();
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, GetLinkPos());
		ASSERT_RETURN(pCell, false);
		const KItem& item = pCell->GetItem();

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, &item, pCIIB, enumITT_myself, false);
		strcpy_k((char*)buffer, size, p);*/
		return true;
	}

	void KItemSumBlock::Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const
	{
		KMainPlayer::Instance()->TryUseItem(m_BagType, GetLinkPos());
	}

	void KItemSumBlock::SetBagType(int nBoxID)
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN_VOID(pBagMgr);
		m_BagType = (ENUM_ITEM_BAG_TYPE)(pBagMgr->BoxID2BagID(nBoxID));
	}

	const KCreateInfo_ItemBase* KItemSumBlock::GetItemCreateInfo() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, NULL);
		return pBagMgr->GetOneItemCreateInfoBase(m_BagType, m_ItemID);
	}

// 	const NS_KDBItem_Base* KItemSumBlock::GetItemBase() const
// 	{
// 		return KMainPlayer::Instance()->m_itemMain.GetOneDB_ItemBase(m_BagType, m_ItemID);
// 	}

	void KItemSumBlock::SetItemID(int ItemID)
	{
		m_ItemID = ItemID;
	}

	int KItemSumBlock::GetLinkBoxID() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, -1);
		return pBagMgr->BagId2BoxID(m_BagType);
	}

	int KItemSumBlock::GetLinkPos() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, -1);
		return pBagMgr->GetFirstPos(m_BagType, m_ItemID);
	}

	bool KItemSumBlock::Exist()const
	{
		return GetLinkPos() > -1;
	}

	bool KItemSumBlock::Release()
	{
		KItemSumBlock::Free(this);
		return true;
	}

	const char* KItemSumBlock::GetName() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return NULL;
		return pCIIB->GetName();
	}

	/*************************************附件物品栏*****************************************/
	bool KAttachBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	int KAttachBlock::GetStatus() const
	{
		// TODO:
		return 0;
	}

	int KAttachBlock::GetCount() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, 0);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, 0);

		return pCell->GetItemStackNumber();
	}

	bool KAttachBlock::IsFrequency() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, false);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, false);

		return pCell->IsFrequency();
	}

	int KAttachBlock::GetFrequency() const
	{
		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, 0);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, 0);

		return pCell->GetFrequency();
	}

	bool KAttachBlock::GetTooltip( void* buffer, int size ) const
	{
	/*	KWorldObjAbout::KCharacter* pChar = KMainPlayer::Instance()->GetMainPlayer();

		int nBagID = GetBagID();
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, false);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, false);
		const KItem& item = pCell->GetItem();

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, &item, pCIIB, enumITT_myself, false);
		strcpy_k((char*)buffer, size, p);*/
		return true;
	}

	void KAttachBlock::OnDestroy()
	{
		// 不应该放在这里，因为必须是在物品删除成功下才能删除block并更新界面
		//KMainPlayer::Instance()->TryDropItem(GetBagType(), m_posInfo);
	}

	const KCreateInfo_ItemBase* KAttachBlock::GetItemCreateInfo() const
	{
		int nBagID = GetBagID();

		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, NULL);
		const KCellBag* pCell = pBagMgr->FindCell(nBagID, m_posInfo);
		ASSERT_RETURN(pCell, NULL);
		const KItem& item = pCell->GetItem();

		return pBagMgr->GetItemCreateInfo(item.GetID());
	}

	int KAttachBlock::GetBagID() const
	{
		KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
		ASSERT_RETURN(pBagMgr, -1);
		return pBagMgr->BoxID2BagID(m_boxID);
	}

	bool KAttachBlock::Release()
	{
		KAttachBlock::Free(this);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool KBusinessBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	bool KBusinessBlock::GetCDTime( int& passedTime, int& totalTime ) const
	{
		return true;
	}

	int KBusinessBlock::GetStatus() const
	{
		// TODO:
		return 0;
	}

	int KBusinessBlock::GetCount() const
	{
		const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		ASSERT_RETURN(pItem, 0);

		return pItem->GetStackNumber();
	}

	bool KBusinessBlock::IsFrequency() const
	{
		const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		ASSERT_RETURN(pItem, false);

		return pItem->IsFrequency();
	}

	int KBusinessBlock::GetFrequency() const
	{
		const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		ASSERT_RETURN(pItem, 0);

		return pItem->GetFrequency();
	}

	bool KBusinessBlock::GetTooltip( void* buffer, int size ) const
	{
		//KWorldObjAbout::KCharacter* pChar = KBusinessManager::Instance()->GetBusinessPlayer();
		//ASSERT_RETURN(pChar, false);

		//const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		//ASSERT_RETURN(pItem, false);

		//const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		//ASSERT_RETURN(pCIIB, false);

		//const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, pItem, pCIIB, enumITT_otherPlayer, false);
		//strcpy_k((char*)buffer, size, p);
		return true;
	}

	void KBusinessBlock::OnDestroy()
	{
		// 不应该放在这里，因为必须是在物品删除成功下才能删除block并更新界面
		//KMainPlayer::Instance()->TryDropItem(GetBagType(), m_posInfo);
	}

	const KCreateInfo_ItemBase* KBusinessBlock::GetItemCreateInfo() const
	{
		const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		ASSERT_RETURN(pItem, NULL);

		const KCreateInfo_ItemBase* pCIIB = KItemCreate::GetInstancePtr()->GetItemCreateInfo(pItem->GetID());
		return pCIIB;
	}
	
	bool KBusinessBlock::Release()
	{
		KBusinessBlock::Free(this);
		return true;
	}

	bool KBusinessBlock::IsUseStatus() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return false;
        int nLevel = KMainPlayer::RealPlayer()->GetAttrValue(ca_level);
		if(nLevel >= pCIIB->GetLevel()) return false;
		return true;
	}

	int KBusinessBlock::GetQuality() const
	{
		const KItem* pItem = KBusinessManager::Instance()->FindBusinessCell(m_posInfo);
		ASSERT_RETURN(pItem, 0);
		return pItem->GetQuality();
	}

	//////////////////////////////////////////////////////////////////////////
	bool KStoreBuyBackBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	bool KStoreBuyBackBlock::GetCDTime( int& passedTime, int& totalTime ) const
	{
		return true;
	}

	int KStoreBuyBackBlock::GetStatus() const
	{
		// TODO:
		return 0;
	}

	int KStoreBuyBackBlock::GetCount() const
	{
		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, 0);

		return pItem->GetStackNumber();
	}

	bool KStoreBuyBackBlock::IsFrequency() const
	{
		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, false);

		return pItem->IsFrequency();
	}

	int KStoreBuyBackBlock::GetFrequency() const
	{
		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, 0);

		return pItem->GetFrequency();
	}

	bool KStoreBuyBackBlock::GetTooltip( void* buffer, int size ) const
	{
	/*	KWorldObjAbout::KCharacter* pChar = KMainPlayer::Instance()->GetMainPlayer();
		ASSERT_RETURN(pChar, false);

		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, false);

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, pItem, pCIIB, enumITT_store, false);
		strcpy_k((char*)buffer, size, p);*/
		return true;
	}

	void KStoreBuyBackBlock::OnDestroy()
	{
		// 不应该放在这里，因为必须是在物品删除成功下才能删除block并更新界面
		//KMainPlayer::Instance()->TryDropItem(GetBagType(), m_posInfo);
	}

	const KCreateInfo_ItemBase* KStoreBuyBackBlock::GetItemCreateInfo() const
	{
		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, NULL);

		const KCreateInfo_ItemBase* pCIIB = KItemCreate::GetInstancePtr()->GetItemCreateInfo(pItem->GetID());
		return pCIIB;
	}

	bool KStoreBuyBackBlock::Release()
	{
		KStoreBuyBackBlock::Free(this);
		return true;
	}

	bool KStoreBuyBackBlock::IsUseStatus() const
	{
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_ItemID);
		if(NULL == pCIIB) return false;
        int nLevel = KMainPlayer::RealPlayer()->GetAttrValue(ca_level);
		if(nLevel >= pCIIB->GetLevel()) return false;
		return true;
	}

	int KStoreBuyBackBlock::GetQuality() const
	{
		const KItem* pItem = KStoreAbout::KStoreManager::Instance()->GetBuyBackItem(m_posInfo);
		ASSERT_RETURN(pItem, 0);
		return pItem->GetQuality();
	}

	//////////////////////////////////////////////////////////////////////////
	bool KStoreBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	bool KStoreBlock::GetCDTime( int& passedTime, int& totalTime ) const
	{
		return true;
	}

	int KStoreBlock::GetStatus() const
	{
		// TODO:
		return 0;
	}

	int KStoreBlock::GetCount() const
	{
		// UI规定请用0
		return 0;
	}

	bool KStoreBlock::IsFrequency() const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		return (enumCT_Frequency == pCIIB->s_nCostType);
	}

	int KStoreBlock::GetFrequency() const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, 0);

		return pCIIB->GetMaxStacks();
	}

	bool KStoreBlock::GetTooltip( void* buffer, int size ) const
	{
		//const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		//ASSERT_RETURN(pCIIB, false);
		//KWorldObjAbout::KCharacter* pMainPlayer = KMainPlayer::Instance()->GetMainPlayer();
		//ASSERT_RETURN(pMainPlayer, false);
		//const char* p = KTooltipManager::Instance()->GetStoreItemTooltip(pMainPlayer, NULL, pCIIB);
		//strcpy_k((char*)buffer, size, p);
		return true;
	}

	void KStoreBlock::OnDestroy()
	{
		// 不应该放在这里，因为必须是在物品删除成功下才能删除block并更新界面
		//KMainPlayer::Instance()->TryDropItem(GetBagType(), m_posInfo);
	}

	const KCreateInfo_ItemBase* KStoreBlock::GetItemCreateInfo() const
	{
		const KCreateInfo_ItemBase* pCIIB = KStoreAbout::KStoreManager::Instance()->GetItemCreateInfo(m_nProductID);
		return pCIIB;
	}

	bool KStoreBlock::Release()
	{
		KStoreBlock::Free(this);
		return true;
	}

	bool KStoreBlock::IsUseStatus() const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, 0);
		

		int nLevel = KMainPlayer::RealPlayer()->GetAttrValue(ca_level);
		if(nLevel >= pCIIB->GetLevel()) return false;
		return true;
	}

	int KStoreBlock::GetCost() const
	{
// 		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
// 		if(pCIIB)
// 		{
// 			return pCIIB->GetPrice();
// 		}
		
		return (int)(m_moneyPrice.m_money);
	}

	void KStoreBlock::SetProductItem(int nLableID, int nProductID, int nPrice, int nLimitSaleCount)
	{
		m_nLableID = nLableID;
		m_nProductID = nProductID;
		m_moneyPrice.m_money = nPrice;
		m_nLimitSaleCount = nLimitSaleCount;
	}
	
	int KStoreBlock::GetLableID() const
	{
		return m_nLableID;
	}
	
	int KStoreBlock::GetProductID() const
	{
		return m_nProductID;
	}

	const char* KStoreBlock::GetName() const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, "");
		return pCIIB->GetName();
	}

	int KStoreBlock::GetLimitSaleCount() const
	{
		return m_nLimitSaleCount;
	}

	int KStoreBlock::GetQuality() const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, 0);
		return pCIIB->GetQuality();
	}

	//////////////////////////////////////////////////////////////////////////
	// -------------------------------------- KSingleItemLinkBlock -------------------------------------------
	KSingleItemLinkBlock::KSingleItemLinkBlock()
		:m_LinkBoxID(0), m_LinkPos(0), m_ItemID(0), m_SkillGroup(0)
	{

	}

	void KSingleItemLinkBlock::SetItemBlock(int BoxID, int Pos)
	{
		m_LinkBoxID = BoxID;
		m_LinkPos = Pos;

		// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
		m_ItemID = GetItemCreateInfo()->GetItemID();
	}

	bool KSingleItemLinkBlock::GetIcon(void* buffer, int size) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetIcon(buffer, size);
	}

	bool KSingleItemLinkBlock::GetCDTime(int& passedTime, int& totalTime) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		// 		if(NULL == Link) return false; // TODO: 先这么判断一下
		return Link->GetCDTime(passedTime, totalTime);
	}

	int KSingleItemLinkBlock::GetStatus() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetStatus();
	}

	bool KSingleItemLinkBlock::GetTooltip(void* buffer, int size) const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetTooltip(buffer, size);
	}

	void KSingleItemLinkBlock::Use(float x, float y, bool prepare, bool bOnlyPrepare) const
	{
		int nBoxID = GetBoxID();
		// 如果当前交易框打开，向交易框放物品
		if(enumBTID_BusinessSelfBox == nBoxID)
		{
			KBusinessAbout::KBusinessManager::Instance()->MoveItemToBag(m_LinkBoxID, m_LinkPos, m_posInfo);
			return;
		}

		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		Link->Use(x, y, prepare, bOnlyPrepare);
	}

	const KCreateInfo_ItemBase* KSingleItemLinkBlock::GetItemCreateInfo() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetItemCreateInfo();
	}

	int KSingleItemLinkBlock::GetLinkBoxID() const
	{
		return m_LinkBoxID;
	}

	int KSingleItemLinkBlock::GetLinkPos() const
	{
		return m_LinkPos;
	}

	void KSingleItemLinkBlock::SetLinkPos(int Pos)
	{
		SetItemBlock(m_LinkBoxID, Pos);
	}

	bool KSingleItemLinkBlock::Exist()const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link != NULL;
	}

	int KSingleItemLinkBlock::GetCount() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		return Link->GetCount();
	}

	// 是否显示使用次数，而不是堆叠数
	bool KSingleItemLinkBlock::IsFrequency() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, false);
		return Link->IsFrequency();
	}

	// 得到使用次数
	int KSingleItemLinkBlock::GetFrequency() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, 0);
		return Link->GetFrequency();
	}

	bool KSingleItemLinkBlock::Release()
	{
		KSingleItemLinkBlock::Free(this);
		return true;
	}

	bool KSingleItemLinkBlock::IsUseStatus() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, false);
		return Link->IsUseStatus();
	}

	int KSingleItemLinkBlock::GetQuality() const
	{
		const KItemBlock* Link = (const KItemBlock*)KBlockBoxManager::GetInstancePtr()->GetBlock(m_LinkBoxID, m_LinkPos);
		ASSERT_RETURN(Link, 0);
		return Link->GetQuality();
	}

	//////////////////////////////////////////////////////////////////////////

	


	//////////////////////////////////////////////////////////////////////////
	bool KBagBlock::GetIcon( void* buffer, int size ) const
	{
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		if(NULL == pCIIB)
		{
			return false;
		}

		strcpy_k((char*)buffer, size, pCIIB->s_icon);
		return true;
	}

	bool KBagBlock::GetTooltip( void* buffer, int size ) const
	{
	/*	KWorldObjAbout::KCharacter* pChar = KMainPlayer::Instance()->GetMainPlayer();
		ASSERT_RETURN(pChar, false);

		KItem Item;
		Item.m_dwID = m_ItemID;
		int nItemCount = 1;
		Item.SetStackNumber(nItemCount);

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo();
		ASSERT_RETURN(pCIIB, false);

		const char* p = KTooltipManager::Instance()->GetItemTooltip(pChar, &Item, pCIIB, enumITT_myself, false);
		strcpy_k((char*)buffer, size, p);*/
		return true;
	}

	void KBagBlock::OnDestroy()
	{
	}

	const KItemAbout::KCreateInfo_ItemBase* KBagBlock::GetItemCreateInfo() const
	{
		return NULL;
	}

	bool KBagBlock::Release()
	{
		KBagBlock::Free(this);
		return true;
	}

	void KBagBlock::SetPos( int boxID, int pos )
	{
		IBlock::SetPos(boxID, pos);

		// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
		m_ItemID = GetItemCreateInfo()->GetItemID();
		m_dragable = false;
	}
}
