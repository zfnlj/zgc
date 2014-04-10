#include "KBag.h"
#include "CommonLogic/Item/KItemDef.h"
#include "CommonLogic/WorldObject/ICharacter.h"
//#include "PacketGate/KGCPacket.h"
#include "../WorldObject/KPlayer.h"
#include "System/Collections/KString.h"
#include "../Block/IBlock.h"
#include "../Block/KBlock.h"
#include "../Block/KBlockBoxManager.h"
#include "../Item/KBagManager.h"
#include "../WorldObject/KMainPlayer.h"
#include "KUseItemManager.h"

using namespace KWorldObjAbout;
using namespace KBlockAbout;

namespace KItemAbout
{
	KCellBag::KCellBag()
	{
	}

	KCellBag::~KCellBag()
	{
	
	}

	KCellBag::KCellBag(const KCellBag& cell)
	{
		m_nPos = cell.m_nPos;
		m_Item = cell.m_Item;
		m_nLockType = cell.m_nLockType;
	}

	KCellBag& KCellBag::operator=(const KCellBag& cell)
	{
		if(this == &cell)
		{
			return *this;
		}

		m_nPos = cell.m_nPos;
		m_Item = cell.m_Item;
		m_nLockType = cell.m_nLockType;

		return *this;
	}

	void KCellBag::Reset()
	{
		m_nPos = ITEM_INVALID_POS;
		m_Item.Clear();
		m_nLockType = 0;
	}

	bool KCellBag::Release()
	{
		KCellBag::Free(this);
		return true;
	}

	KItem& KCellBag::GetItem()
	{
		return m_Item;
	}
	
	const KItem& KCellBag::GetItem() const
	{
		return m_Item;
	}

	KItem* KCellBag::GetItemPtr()
	{
		return &m_Item;
	}

	int KCellBag::GetPos() const
	{
		return m_nPos;
	}

	bool KCellBag::IsFrequency() const
	{
		return m_Item.IsFrequency();
	}

	void KCellBag::UpdateStackNumber( int nStackNumber )
	{
		m_Item.SetStackNumber(nStackNumber);
	}

	void KCellBag::UpdateFrequency(int nFrequency)
	{
		m_Item.SetFrequency(nFrequency);
	}
	
	void KCellBag::UpdateDurable(int nDurable)
	{
		m_Item.SetDurable(nDurable);
	}

	bool KCellBag::Unserilize(KMsgInputStream& si)
	{
		short nPos = 0;
		BOOL bRet = si.ReadShort(nPos);
		ASSERT_RETURN(bRet, false);
		m_nPos = nPos;
		bool bSerRet = m_Item.Unserilize(si);
		ASSERT_RETURN(bSerRet, false);

		return true;
	}

	bool KCellBag::IsLock() const
	{
		return (0 != m_nLockType ? true : false);
	}

	void KCellBag::SetStallLockType( bool bLock )
	{
		if(bLock)
		{
			m_nLockType |= (1 << enumLT_stall);
		}
		else
		{
			m_nLockType &= ~(1 << enumLT_stall);
		}
	}

	bool KCellBag::IsStallLock() const
	{
		return ((m_nLockType & (1 << enumLT_stall)) ? true : false);
	}

	void KCellBag::SetLock( int nLockType )
	{
		m_nLockType = nLockType;
	}

	//////////////////////////////////////////////////////////////////////////
	// 背包类
	KBag::KBag()
	{
	}

	KBag::~KBag()
	{
		_DelAllCell();
	}

	KBag::KBag(const KBag& Bag)
	{
		m_nBagID = Bag.m_nBagID;						// 背包ID(代替背包类型)
		m_nCellMaxCount = Bag.m_nCellMaxCount;			// 格子最大数目(加锁未加锁都算)
		m_nCellUnLockCount = Bag.m_nCellUnLockCount;	// 未加锁格子数目
		m_nMasterID = Bag.m_nMasterID;

		CellMapVec::const_iterator itorTmp = Bag.m_ListItem.begin();
		for(;itorTmp!=Bag.m_ListItem.end();itorTmp++)
		{
			KCellBag* pNewCell = KCellBag::Alloc();
			ASSERT_I(pNewCell);
			pNewCell->Reset();
			const KCellBag* pCell = (const KCellBag*)(itorTmp->second);
			*pNewCell = *pCell;
			m_ListItem[pNewCell->m_nPos] = pNewCell;
		}
	}

	KBag& KBag::operator=(const KBag& Bag)
	{
		if(this == &Bag)
		{
			return *this;
		}

		_DelAllCell();
		m_nBagID = Bag.m_nBagID;						// 背包ID(代替背包类型)
		m_nCellMaxCount = Bag.m_nCellMaxCount;			// 格子最大数目(加锁未加锁都算)
		m_nCellUnLockCount = Bag.m_nCellUnLockCount;	// 未加锁格子数目
		m_nMasterID = Bag.m_nMasterID;

		CellMapVec::const_iterator itorTmp = Bag.m_ListItem.begin();
		for(;itorTmp!=Bag.m_ListItem.end();itorTmp++)
		{
			KCellBag* pNewCell = KCellBag::Alloc();
			ASSERT_I(pNewCell);
			pNewCell->Reset();
			const KCellBag* pCell = (const KCellBag*)(itorTmp->second);
			*pNewCell = *pCell;
			m_ListItem[pNewCell->m_nPos] = pNewCell;
		}

		return *this;
	}

	void KBag::_DelAllCell()
	{
		CellMapVec::iterator itorTmp = m_ListItem.begin();
		for(;itorTmp!=m_ListItem.end();itorTmp++)
		{
			KCellBag* pCell = itorTmp->second;
			if(pCell)
			{
				pCell->Release();
			}
		}

		m_ListItem.clear();
	}

	void KBag::SetCellLimite(int CellMaxCount, int CellUnLockCount)
	{
// 		m_nBagID = enum_item_BagReaderAttach;
		m_nMasterID = KMainPlayer::GetInstancePtr()->GetMainPlayerID();
		m_nCellMaxCount = CellMaxCount;
		m_nCellUnLockCount = CellUnLockCount;
	}

	void KBag::Reset()
	{
		m_nBagID = -1;
		m_nMasterID = 0;
		m_nCellMaxCount = 0;
		m_nCellUnLockCount = 0;
		_DelAllCell();
	}

	bool KBag::UnserilizeInitInfo(KMsgInputStream& si)
	{
		// m_nBagID在外面解析，这里不要加

		ASSERT_RETURN(si.ReadUint64(m_nMasterID), false);
		short nValue = 0;
		ASSERT_RETURN(si.ReadShort(nValue), false);
		m_nCellMaxCount = nValue;
		ASSERT_RETURN(si.ReadShort(nValue), false);
		m_nCellUnLockCount = nValue;
		return true;
	}

	int KBag::GetBagID() const
	{
		return m_nBagID;
	}

	UINT64 KBag::GetMasterID() const
	{
		return m_nMasterID;
	}

	int KBag::GetMaxCount() const
	{
		return m_nCellMaxCount;
	}

	int KBag::GetUnLockCount() const
	{
		return m_nCellUnLockCount;
	}

	KCellBag* KBag::FindCell( int nPos )
	{
		ASSERT_RETURN(nPos >= 0, NULL);
		CellMapVec::iterator itorFind = m_ListItem.find(nPos);
		if(itorFind != m_ListItem.end())
		{
			return (KCellBag*)(itorFind->second);
		}

		return NULL;
	}

	const KCellBag* KBag::FindCell( int nPos ) const
	{
		ASSERT_RETURN(nPos >= 0, NULL);
		CellMapVec::const_iterator itorFind = m_ListItem.find(nPos);
		if(itorFind != m_ListItem.end())
		{
			return (const KCellBag*)(itorFind->second);
		}

		return NULL;
	}

	int KBag::GetItemSize( DWORD dwItemID ) const
	{
		int nCount = 0;
		CellMapVec::const_iterator itorFind = m_ListItem.begin();
		for(;itorFind!=m_ListItem.end();itorFind++)
		{
			const KCellBag* pCell = itorFind->second;
			if(NULL == pCell)
			{
				continue;
			}

			const KItem& item = pCell->GetItem();
			if(item.GetID() != dwItemID)
			{
				continue;
			}

			nCount += item.GetStackNumber();
		}

		return nCount;
	}

	int KBag::GetNotEmptyCellSize() const
	{
		return m_ListItem.size();
	}

	int KBag::GetEmptyCellSize() const
	{
		return (m_nCellUnLockCount - m_ListItem.size());
	}

	int KBag::StatBagDisplayItem( VEC_ITEM_DISPLAY& vecItemDisplay ) const
	{
		CellMapVec::const_iterator itorTmp = m_ListItem.begin();
		for(;itorTmp!=m_ListItem.end();itorTmp++)
		{
			const KCellBag* pCell = itorTmp->second;
			if(NULL == pCell) continue;
			const KItem& item = pCell->GetItem();

			KItemDisplayData displayData;
			displayData.s_nItemID = item.GetID();
			displayData.s_nPos = pCell->m_nPos;
			const KCreateInfo_ItemBase* pCIIB = KItemCreate::GetInstancePtr()->GetItemCreateInfo(item.GetID());
			if(pCIIB)
			{
				displayData.s_nDisplayID = pCIIB->s_nDisplayID;
				vecItemDisplay.push_back(displayData);
			}
		}

		return vecItemDisplay.size();
	}

	int KBag::GetFirstPos( DWORD dwItemID ) const
	{
		ASSERT_RETURN(dwItemID > 0, -1);

		CellMapVec::const_iterator itorTmp = m_ListItem.begin();
		for(;itorTmp!=m_ListItem.end();itorTmp++)
		{
			const KCellBag* pCell = itorTmp->second;
			if(NULL == pCell) continue;

			if(pCell->GetItemID() == dwItemID)
			{
				return pCell->m_nPos;
			}
		}

		return -1;
	}

	bool KBag::TryToUse(int nPos,UINT64 playerId)
	{
		

		CellMapVec::iterator itorFind = m_ListItem.find(nPos);
		if(itorFind == m_ListItem.end()) return false;
		KCellBag* pCell = (KCellBag*)(itorFind->second);
		if(pCell->GetItemStackNumber()<=0) return false;
		KUseItemManager::GetInstance()->UseItem(pCell->GetItemID(),playerId);
		return true;
	}

	bool KBag::Unserilize(KMsgInputStream& si)
	{
		short nCellMaxCount = 0;
		BOOL bRet = si.ReadShort(nCellMaxCount);
		ASSERT_RETURN(bRet, false);
		m_nCellMaxCount = nCellMaxCount;

		short nCellUnLockCount = 0;
		bRet = si.ReadShort(nCellUnLockCount);
		ASSERT_RETURN(bRet, false);
		m_nCellUnLockCount = nCellUnLockCount;

		short nCellSize = 0;
		bRet = si.ReadShort(nCellSize);
		ASSERT_RETURN(bRet, false);
		ASSERT_RETURN(nCellSize >= 0, false);
		for(int nLoop=0;nLoop<nCellSize;nLoop++)
		{
			KCellBag* pCellItem = KCellBag::Alloc();
			ASSERT_RETURN(pCellItem, false);
			pCellItem->Reset();
			bool bSerRet = pCellItem->Unserilize(si);
			ASSERT_RETURN(bSerRet, false);
			if(!FindCell(pCellItem->m_nPos))
			{
				m_ListItem[pCellItem->m_nPos] = pCellItem;
			}
			else
			{
				ASSERT_I(false);
				pCellItem->Release();
			}
		}

		return true;
	}

	int KBag::FindFirstEmptyCell() const
	{
		int nUnLockCount = m_nCellUnLockCount;
		for(int nLoop=0;nLoop<nUnLockCount;nLoop++)
		{
			if(NULL == FindCell(nLoop))
			{
				return nLoop;
			}
		}

		return -1;
	}

	bool KBag::IsEmpty() const
	{
		return (0 == m_ListItem.size());
	}

	bool KBag::IsPosValid(int nPos) const
	{
		if(nPos < 0 || nPos > m_nCellMaxCount)
		{
			return false;
		}

		return true;
	}

	bool KBag::IsCellUnLock( int nPos ) const
	{
		if(nPos < 0 || nPos > m_nCellUnLockCount)
		{
			return false;
		}

		return true;
	}

	bool KBag::Add(UINT itemID,int count)
	{
		int pos = GetFirstPos(itemID);
		if(pos<0){
			pos = FindFirstEmptyCell();
			if(pos<0) return false;
			KItem item;
			item.Clear();
			item.m_dwID = itemID;
			item.SetStackNumber(count);
			AddOneCell(pos,item);
		}else{
			KCellBag* cell = FindCell(pos);
			cell->UpdateStackNumber(cell->GetItemStackNumber()+count);
		}
		KCellBag* cell = FindCell(pos);
		if(cell->GetItemStackNumber()==0){
			DelOneCell(pos);
		}
		return true;
	}

	bool KBag::AddOneCell( int nPos, const KItem& item )
	{
		if(!IsPosValid(nPos)) return false;
		if(!IsCellUnLock(nPos)) return false;

		KCellBag* pCell = FindCell(nPos);
		if(pCell) return false;
		
		pCell = KCellBag::Alloc();
		ASSERT_RETURN(pCell, false);
		pCell->Reset();
		pCell->m_nPos = nPos;
		pCell->m_Item = item;
		m_ListItem[nPos] = pCell;

		return true;
	}

	bool KBag::DelOneCell( int nPos )
	{
		ASSERT_RETURN(nPos >= 0, false);
		CellMapVec::iterator itorFind = m_ListItem.find(nPos);
		if(itorFind != m_ListItem.end())
		{
			KCellBag* pDelCell = (KCellBag*)(itorFind->second);
			if(pDelCell)
			{
				pDelCell->Release();
			}
			m_ListItem.erase(itorFind);
			return true;
		}

		// TODO: 下装备时是先删除cell，还有下装备的通知，这里会删除两次，优化一下
// 		ASSERT_I(false);
		return false;
	}

	bool KBag::Sortout(KBagManager* pBagMgr)
	{
		ASSERT_RETURN(pBagMgr, false);
		KBag bag = *this;
		KBag::_MergeAllCell(pBagMgr, bag);
		KBag::_ExchangeItem(pBagMgr, bag);

		return true;
	}

	bool KBag::_MergeAllCell(KBagManager* pBagMgr, KBag& bag)
	{
		ASSERT_RETURN(pBagMgr, false);
		int nBoxID = pBagMgr->BagId2BoxID(bag.m_nBagID);
		ASSERT_RETURN(nBoxID >= 0, false);
		CellMapVec::iterator itorTmp = bag.m_ListItem.begin();
		for(;itorTmp!=bag.m_ListItem.end();itorTmp++)
		{
			KCellBag* pCell = (KCellBag*)(itorTmp->second);
			if(pCell)
			{
				IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nBoxID, pCell->GetPos());
				if(pIB && pIB->GetLock()) continue;


				const KItem& item = pCell->GetItem();
				const KCreateInfo_ItemBase* pCIIB = KItemCreate::GetInstancePtr()->GetItemCreateInfo(item.GetID());
				ASSERT_RETURN(pCIIB, false);
				for(int nLoop=pCell->m_nPos + 1;nLoop<bag.m_nCellUnLockCount;nLoop++)
				{
					IBlock* pIBNext = KBlockBoxManager::Instance()->GetBlock(nBoxID, nLoop);
					if(NULL == pIBNext || (pIBNext && pIBNext->GetLock())) continue;
					KItemBlock* pItemBlockNext = dynamic_cast<KItemBlock*>(pIBNext);
					if(NULL == pItemBlockNext || (pItemBlockNext ))
					{
						continue;
					}

					if(_SyncDataMergeItem(bag, nLoop, pCell->m_nPos, pCIIB->s_nMaxStacks))
					{
						break;
					}
				}
			}
		}
		return true;
	}

	bool KBag::_SyncDataMergeItem( KBag& bag, int nFromPos, int nToPos, int nStackLimit )
	{
		if(nFromPos > bag.m_nCellUnLockCount || nToPos > bag.m_nCellUnLockCount)
		{
			return false;
		}
		KCellBag* pFromCell = bag.FindCell(nFromPos);
		KCellBag* pToCell = bag.FindCell(nToPos);
		if(NULL == pFromCell || NULL == pToCell)
		{
			return false;
		}
		KItem& itemFrom = pFromCell->GetItem();
		KItem& itemTo = pToCell->GetItem();
		if((itemFrom.GetID() != itemTo.GetID()) || (itemFrom.GetID() == ITEM_INVALID_ID) || (itemTo.GetID() == ITEM_INVALID_ID))
		{
			return false;
		}
		if(!itemFrom.IsStack() || !itemTo.IsStack())
		{
			return false;
		}

		int nItemNumSum = itemFrom.GetStackNumber() + itemTo.GetStackNumber();
		int nToNewNum = (nItemNumSum >= nStackLimit ? nStackLimit : nItemNumSum);
		int nFromNewNum = nItemNumSum - nToNewNum;
		if(itemFrom.GetStackNumber() == nFromNewNum || itemTo.GetStackNumber() == nToNewNum)
		{
			// 什么也不用做
			return true;
		}

		itemTo.SetStackNumber(nToNewNum);
		if(nFromNewNum <= 0)
		{
			CellMapVec::iterator itorFind = bag.m_ListItem.find(nFromPos);
			if(itorFind == bag.m_ListItem.end())
			{
				return false;
			}

			KCellBag* pDelCell = (KCellBag*)(itorFind->second);
			if(pDelCell)
			{
				pDelCell->Release();
			}

			bag.m_ListItem.erase(itorFind);
		}
		else
		{
			itemFrom.SetStackNumber(nFromNewNum);
		}

		//KMainPlayer::Instance()->TryMoveItem(bag.GetBagID(), nFromPos, bag.GetBagID(), nToPos);
		if(itemTo.GetStackNumber() >= nStackLimit)
		{
			return true;
		}
		return false; // 继续合并
	}

	bool KBag::_ExchangeItem(KBagManager* pBagMgr, KBag& bag)
	{
		ASSERT_RETURN(pBagMgr, false);
		int nBoxID = pBagMgr->BagId2BoxID(bag.m_nBagID);
		ASSERT_RETURN(nBoxID >= 0, false);

		for(int nLoop=0;nLoop<bag.m_nCellUnLockCount;nLoop++)
		{
			IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nBoxID, nLoop);
			if(pIB && pIB->GetLock()) continue;
			KItemBlock* pItemBlock = dynamic_cast<KItemBlock*>(pIB);
			if(pItemBlock )
			{
				continue;
			}

			int nPosOfMin = -1;
			_GetMinItemID(pBagMgr, nBoxID, bag, nLoop, nPosOfMin);
			if(nPosOfMin < 0)
			{
				return true;
			}

			if(nLoop < nPosOfMin)
			{
				_SyncDataExchangeItem(bag, nPosOfMin, nLoop);
			}
		}

		return true;
	}

	bool KBag::_GetMinItemID(KBagManager* pBagMgr, int nBoxID, KBag& bag, int nStartPos, int& nPos)
	{
		ASSERT_RETURN(pBagMgr, false);

		ASSERT_RETURN(nStartPos >= 0 && nStartPos < bag.m_nCellUnLockCount, false);
		DWORD dwItemTmp = ITEM_INVALID_ID;
		int nPosTmp = -1;
		for(int nLoop=nStartPos;nLoop<bag.m_nCellUnLockCount;nLoop++)
		{
			const KCellBag* pCell = bag.FindCell(nLoop);
			if(pCell)
			{
				IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nBoxID, pCell->GetPos());
				if(pIB && pIB->GetLock()) continue;
				KItemBlock* pItemBlock = dynamic_cast<KItemBlock*>(pIB);
				if(pItemBlock)
				{
					continue;
				}

				const KItem& item = pCell->GetItem();
				if(ITEM_INVALID_ID == dwItemTmp || dwItemTmp > item.GetID())
				{
					dwItemTmp = item.GetID();
					nPosTmp = nLoop;
				}
			}
		}

		nPos = nPosTmp;
		return true;
	}
	
	bool KBag::_SyncDataExchangeItem(KBag& bag, int nFromPos, int nToPos)
	{
		if(nFromPos > bag.m_nCellUnLockCount || nToPos > bag.m_nCellUnLockCount)
		{
			return false;
		}

		CellMapVec::iterator itorFrom = bag.m_ListItem.find(nFromPos);
		if(itorFrom == bag.m_ListItem.end())
		{
			return false;
		}
		KCellBag* pFromCell = (KCellBag*)(itorFrom->second);
		ASSERT_I(pFromCell);

		KCellBag* pToCell = bag.FindCell(nToPos);
		bool bNew = false;
		if(NULL == pToCell)
		{
			bNew = true;
			pToCell = KCellBag::Alloc();
			ASSERT_RETURN(pToCell, false);
			pToCell->Reset();
			pToCell->m_nPos = nToPos;
		}
		KItem& itemFrom = pFromCell->GetItem();
		KItem& itemTo = pToCell->GetItem();
		KItem itemTmp = itemFrom;
		itemFrom = itemTo;
		itemTo = itemTmp;
		
		if(bNew)
		{
			pFromCell->Release();
			bag.m_ListItem.erase(itorFrom);
			bag.m_ListItem[pToCell->m_nPos] = pToCell;
		}

		//KMainPlayer::Instance()->TryMoveItem(bag.GetBagID(), nFromPos, bag.GetBagID(), nToPos);
		return true;
	}

	int KBag::FindBagPosByAdd(DWORD dwItemID, int nItemNum, const KCreateInfo_ItemBase* pCIIB, VEC_BAG_FREE_POS& vecBagPos) const
	{
		ASSERT_RETURN(pCIIB, nItemNum);
		int nStackLimit = pCIIB->GetMaxStacks();
		// 返回值是不能放入背包的数量
		vecBagPos.clear();
		int nCellUnLockCount = GetUnLockCount();

		// 优先查找可叠加的位置
		for(int nLoop = 0;nLoop < nCellUnLockCount && nItemNum > 0;nLoop++)
		{
			const KCellBag* pCell = FindCell(nLoop);
			if(NULL == pCell) continue;
			const KItem& item = pCell->GetItem();
			if(item.GetID() == dwItemID)
			{
				int nFreeCount = 0;
				bool bCanStack =  true;
				if(pCIIB->IsUsableItem())
				{
					const KCreateInfo_Item* pCII = (const KCreateInfo_Item*)pCIIB;
					if(pCII->GetMaxUseTime() > 0) // ItemInfo.txt中MaxUseTime大于0一定不能叠加
					{
						bCanStack = false;
					}
				}
				if(item.IsStack() && bCanStack)
				{
					nFreeCount = nStackLimit - item.GetStackNumber();
				}
				if(nFreeCount > 0)
				{
					KBagItemPosition freePos;
					freePos.bFreePos = false;
					freePos.nPosIndex = nLoop;
					freePos.nNumFind = nFreeCount < nItemNum ? nFreeCount : nItemNum;
					vecBagPos.push_back(freePos);
					nItemNum -= freePos.nNumFind;
				}
			}
		}

		// 还有未添加的，加到空的格子里
		if(nItemNum > 0)
		{
			for(int nLoop=0;nLoop<nCellUnLockCount && nItemNum>0;nLoop++)
			{
				const KCellBag* pCell = FindCell(nLoop);
				if(pCell) continue;

				KBagItemPosition freePos;
				freePos.bFreePos = true;
				freePos.nPosIndex = nLoop;
				freePos.nNumFind = nStackLimit < nItemNum ? nStackLimit : nItemNum;
				vecBagPos.push_back(freePos);
				nItemNum -= freePos.nNumFind;
			}
		}

		return nItemNum;
	}

	bool KBag::IsFull() const
	{
		return (m_nCellUnLockCount == m_ListItem.size());
	}

	//////////////////////////////////////////////////////////////////////////
	// 普通背包
	KBagNormal::KBagNormal() : KBag()
	{
	}

	KBagNormal::~KBagNormal()
	{

	}

	KBagNormal::KBagNormal( const KBagNormal& Bag ) : KBag(Bag)
	{

	}

	KBagNormal& KBagNormal::operator=( const KBagNormal& Bag )
	{
		if(this == &Bag)
		{
			return *this;
		}
		(*((KBag*)this)) = Bag;

		return *this;
	}

	void KBagNormal::Reset()
	{
		KBag::Reset();
		m_nBagID = enum_item_BagNormal;
	}

	bool KBagNormal::Release()
	{
		KBagNormal::Free(this);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// 收件箱附件
	KBagReaderAttach::KBagReaderAttach() : KBag()
	{
	}

	KBagReaderAttach::~KBagReaderAttach()
	{

	}

	KBagReaderAttach::KBagReaderAttach( const KBagReaderAttach& Bag ) : KBag(Bag)
	{

	}

	KBagReaderAttach& KBagReaderAttach::operator=( const KBagReaderAttach& Bag )
	{
		if(this == &Bag)
		{
			return *this;
		}
		(*((KBag*)this)) = Bag;

		return *this;
	}

	void KBagReaderAttach::Reset()
	{
		KBag::Reset();
		m_nBagID = enum_item_BagReaderAttach;
	}

	bool KBagReaderAttach::Release()
	{
		KBagNormal::Free((KBagNormal*)this);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// 任务背包
	KBagQuest::KBagQuest() : KBag()
	{
	}

	KBagQuest::~KBagQuest()
	{

	}

	KBagQuest::KBagQuest( const KBagQuest& Bag ) : KBag(Bag)
	{

	}

	KBagQuest& KBagQuest::operator=( const KBagQuest& Bag )
	{
		if(this == &Bag)
		{
			return *this;
		}
		(*((KBag*)this)) = Bag;

		return *this;
	}

	void KBagQuest::Reset()
	{
		KBag::Reset();
		m_nBagID = enum_item_BagQuest;
	}

	bool KBagQuest::Release()
	{
		KBagQuest::Free(this);
		return true;
	}


}