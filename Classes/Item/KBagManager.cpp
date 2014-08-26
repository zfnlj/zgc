#include "KBagManager.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "../Block/KBlock.h"
#include "../Block/KBlockBoxManager.h"
#include "../KNet/KUserData.h"
#include "Inc/KCommonDefine.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "../KNet/KNetMsgFacade.h"

using namespace KBlockAbout;

namespace KItemAbout
{
	//////////////////////////////////////////////////////////////////////////
	KBagManager::KBagManager()
	{
		m_nMasterID = 0;
	}

	KBagManager::~KBagManager()
	{
		_DelAllBag();
	}
	
	void KBagManager::Reset()
	{
		_DelAllBag();
		m_nMasterID = 0;
	}
	
	void KBagManager::_DelAllBag()
	{
		BagList::iterator itorTmp = m_listBag.begin();
		for(;itorTmp!=m_listBag.end();itorTmp++)
		{
			KBag* pBag = itorTmp->second;
			if(pBag)
			{
				pBag->Release();
			}
		}

		m_listBag.clear();
	}

	bool KBagManager::_AddBagList(KBag* pAddBag)
	{
		ASSERT_RETURN(pAddBag, false);
		int nBagID = pAddBag->GetBagID();
		ASSERT_RETURN(nBagID >= 0, false);

		KBag* pFindBag = FindBag(nBagID);
		if(NULL == pFindBag)
		{
			m_listBag[nBagID] = pAddBag;
			return true;
		}

		return false;
	}

	void KBagManager::InitOneBag(const void* pData, int nLen)
	{
		ASSERT_RETURN_VOID(pData);
		KMsgInputStream si(pData, nLen);
		BYTE nBagID = 0;
		BOOL bRet = si.ReadByte(nBagID);
		ASSERT_RETURN_VOID(bRet);
		
		// 装备背包
		switch(nBagID)
		{
		case enum_item_BagNormal:
			InitOneBagNormal(si);			// 普通背包
			break;
		default:
			ASSERT_I(false);
			break;
		}

		KBag* pBag = FindBag(enum_item_BagReaderAttach);
		if (!pBag)
		{
			InitOneReaderAttach();
		}
	}

	void KBagManager::InitOneBagNormal(KMsgInputStream& si)
	{
		KBagNormal* pNewBag = KBagNormal::Alloc();
		ASSERT_RETURN_VOID(pNewBag);
		pNewBag->Reset();

		ASSERT_RETURN_VOID(pNewBag->UnserilizeInitInfo(si));

		if(!_AddBagList(pNewBag))
		{
			pNewBag->Release();
			return;
		}
	}

	void KBagManager::InitOneReaderAttach()
	{
		KBagReaderAttach* pNewBag = KBagReaderAttach::Alloc();
		ASSERT_RETURN_VOID(pNewBag);
		pNewBag->Reset();

		pNewBag->m_nMasterID = 0;//KUserData::GetInstancePtr()->getMainPlayerID();
		pNewBag->m_nCellMaxCount = BagReaderAttachCount;
		pNewBag->m_nCellUnLockCount = BagReaderAttachCount;

		if(!_AddBagList(pNewBag))
		{
			pNewBag->Release();
			return;
		}
	}

	void KBagManager::DelOneBag(int nBagID)
	{
		BagList::iterator itorFind = m_listBag.find(nBagID);
		if(itorFind == m_listBag.end())
		{
			ASSERT_I(false);
			return;
		}

		KBag* pBag = (KBag*)(itorFind->second);
		if(pBag)
		{
			pBag->Release();
		}
		m_listBag.erase(itorFind);
	}

	KBag* KBagManager::FindBag(int nBagID)
	{
		ASSERT_RETURN(nBagID >= enum_item_Bag_Start, NULL);
		BagList::iterator itorFind = m_listBag.find(nBagID);
		if(itorFind == m_listBag.end())
		{
			return NULL;
		}

		return (KBag*)(itorFind->second);
	}

	const KBag* KBagManager::FindBag(int nBagID) const
	{
		ASSERT_RETURN(nBagID >= enum_item_Bag_Start, NULL);
		BagList::const_iterator itorFind = m_listBag.find(nBagID);
		if(itorFind == m_listBag.end())
		{
			return NULL;
		}

		return (const KBag*)(itorFind->second);
	}
	
	KBagNormal* KBagManager::FindNormalBag()
	{
		KBag* pBag = FindBag(enum_item_BagNormal);
		ASSERT_RETURN(pBag, NULL);
		KBagNormal* pBagNormal = dynamic_cast<KBagNormal*>(pBag);
		return pBagNormal;
	}

	const KBagNormal* KBagManager::FindNormalBag() const
	{
		const KBag* pBag = FindBag(enum_item_BagNormal);
		ASSERT_RETURN(pBag, NULL);
		const KBagNormal* pBagNormal = dynamic_cast<const KBagNormal*>(pBag);
		return pBagNormal;
	}

	KBagQuest* KBagManager::FindQuestBag()
	{
		KBag* pBag = FindBag(enum_item_BagQuest);
		ASSERT_RETURN(pBag, NULL);
		KBagQuest* pBagQuest = dynamic_cast<KBagQuest*>(pBag);
		return pBagQuest;
	}

	const KBagQuest* KBagManager::FindQuestBag() const
	{
		const KBag* pBag = FindBag(enum_item_BagQuest);
		ASSERT_RETURN(pBag, NULL);
		const KBagQuest* pBagQuest = dynamic_cast<const KBagQuest*>(pBag);
		return pBagQuest;
	}
	
	KBag* KBagManager::FindBaglua(int nBagID)
	{
		KBag* pBag = FindBag(nBagID);
		return pBag;
	}

	int KBagManager::ItemOperator(const SC_BagOperation* pBO)
	{
		ASSERT_RETURN(pBO, 0);
		switch(pBO->byteSubCmd)
		{
		case enumIOR_UPDATE:
			UpdateOneCell((const SC_BagUpdateResponse*)pBO);
			break;
		case enumIOR_ADD:
			AddOneCell((const SC_BagAddResponse*)pBO);
			break;
		default:
			ASSERT_I(false);
			break;
		}

		return 0;
	}

	bool KBagManager::UpdateOneCell(const SC_BagUpdateResponse* pBUR)
	{
		ASSERT_RETURN(pBUR, false);
		KBag* pBag = FindBag(pBUR->byteBagID);
		ASSERT_RETURN(pBag, false);
		KCellBag* pCell = pBag->FindCell(pBUR->nPos);
		ASSERT_RETURN(pCell, false);

		switch(pBUR->byteUpdateFlag)
		{
		case enum_attributeList:	// 属性对
			{

			}
			break;

		case enum_durable:			// 当前耐久值
			{
				int nDurable = 0;
				memcpy(&nDurable, ((BYTE*)pBUR) + sizeof(SC_BagUpdateResponse), pBUR->nUpdateBufferSize);
				pCell->UpdateDurable(nDurable);
			}
			break;

		case enum_expirationTime:		// 到期时间
			{

			}
			break;

		case enum_frequency:			// 使用剩余次数
			{
				int nFrequence = 0;
				memcpy(&nFrequence, ((BYTE*)pBUR) + sizeof(SC_BagUpdateResponse), pBUR->nUpdateBufferSize);
				pCell->UpdateFrequency(nFrequence);
			}
			break;

		case enum_stackNumber:		// 当前叠加层数
			{
				int nOldNum = pCell->GetItemStackNumber();
				int nStackNumber = 0;
				memcpy(&nStackNumber, ((BYTE*)pBUR) + sizeof(SC_BagUpdateResponse), pBUR->nUpdateBufferSize);
				pCell->UpdateStackNumber(nStackNumber);
				
				pBag->NumChange(pCell->GetItemID(), pCell->GetItemStackNumber() - nOldNum);

				if (pBag->GetBagID() == enum_item_BagNormal)
				{
					KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_NORMAL_BAG_ITEM_CHANGE, pCell->GetItemID(), 0);
				}
			}
			break;

		case enum_author:				// 作者
			{
			
			}
			break;

		case enum_customAttribute:	// 自定义属性
			{
			
			}
			break;

		case enum_binding:			// 归属
			{
			
			}
			break;
		case enum_mail:			// 邮件转物品
			{

			}
			break;

		default:
			return true;
		}

		int nBoxID = BagId2BoxID(pBUR->byteBagID);
		ASSERT_RETURN(-1 != nBoxID, false);
		KBlockBoxManager::Instance()->OnBlockInfoChanged(nBoxID, pBUR->nPos);

		return true;
	}
	

	bool KBagManager::UpdateCellLock(const SC_BagCellUpdateLock* pBCUL)
	{
		ASSERT_RETURN(pBCUL, false);

		KCellBag* pCell = FindCell(pBCUL->byteBagID, pBCUL->nPos);
		ASSERT_RETURN(pCell, false);

		pCell->SetLock(pBCUL->nLockType);
		
		// TODO: 通知UI
		return true;
	}

	bool KBagManager::TrySortoutBag(int nBagID)
	{
		KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, false);
		pBag->Sortout(this);
		
		return true;
	}

	int KBagManager::BagId2BoxID( int nBagID ) const
	{
		switch(nBagID)
		{
		// 装备背包
		case enum_item_BagEquip:
			return enumBTID_EquipBox;

		// 普通背包
		case enum_item_BagNormal:
			return enumBTID_ItemNormalBox;

		// 任务背包
		case enum_item_BagQuest:
			return enumBTID_ItemQuestBox;

		case enum_item_BagReaderAttach:
			return enunBTTD_MailReaderAttach;
		default:
			break;
		}

		return -1;
	}

	int KBagManager::BoxID2BagID( int nBoxID ) const
	{
		switch(nBoxID)
		{
		// 装备背包
		case enumBTID_EquipBox:
			return enum_item_BagEquip;

		// 普通背包
		case enumBTID_ItemNormalBox:
			return enum_item_BagNormal;
		// 任务背包
		case enumBTID_ItemQuestBox:
			return enum_item_BagQuest;

		case enunBTTD_MailReaderAttach:
			return enum_item_BagReaderAttach;

		default:
			break;
		}

		return -1;
	}

	bool KBagManager::IsCellUnLock( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, false);
		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, false);
		return pBag->IsCellUnLock(nPos);
	}

	bool KBagManager::HaveItem( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, false);
		return !IsEmpty(nBagId, nPos);
	}

	int KBagManager::GetItemPosition(int nBoxId, DWORD dwItemID) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, -1);

		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, -1);
		return pBag->GetFirstPos(dwItemID);
	}

	bool KBagManager::IsFrequency( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, false);
		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, false);
		const KCellBag* pCell = pBag->FindCell(nPos);
		if(NULL == pCell)
		{
			return false;
		}
		const KItem& item = pCell->GetItem();
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(item.GetID());
		ASSERT_RETURN(pCIIB, false);
		const KCreateInfo_Item* pCII = dynamic_cast<const KCreateInfo_Item*>(pCIIB);
		if(NULL == pCII || pCII->GetMaxUseTime() > 0)
		{
			return true;
		}

		return false;
	}

	int KBagManager::GetItemNum( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, 0);
		return GetCount(nBagId, nPos);
	}
	
	int KBagManager::GetFrequency( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, 0);
		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, 0);
		const KCellBag* pCell = pBag->FindCell(nPos);
		if(NULL == pCell)
		{
			return 0;
		}
		const KItem& item = pCell->GetItem();
		return item.GetFrequency();
	}

	bool KBagManager::GetIcon( int nBoxId, int nPos, void* buffer, int size ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, false);
		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, false);
		const KCellBag* pCell = pBag->FindCell(nPos);
		if(NULL == pCell)
		{
			return 0;
		}
		const KItem& item = pCell->GetItem();

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo(item.GetID());
		if(pCIIB)
		{
			strcpy_k((char*)buffer, size, pCIIB->s_icon);
			return true;
		}

		return false;
	}

	bool KBagManager::GetIcon( int nPos, void* buffer, int size ) const
	{
		const KBag* pBag = FindBag(enum_item_BagNormal);
		ASSERT_RETURN(pBag, false);
		const KCellBag* pCell = pBag->FindCell(nPos);
		if(NULL == pCell)
		{
			return 0;
		}
		const KItem& item = pCell->GetItem();

		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo(item.GetID());
		if(pCIIB)
		{
			strcpy_k((char*)buffer, size, pCIIB->s_icon);
			return true;
		}

		return false;
	}

	bool KBagManager::IsValidCell( int nBoxId, int nPos ) const
	{
		int nBagId = BoxID2BagID(nBoxId);
		ASSERT_RETURN(nBagId >= enum_item_Bag_Start, false);
		const KBag* pBag = FindBag(nBagId);
		ASSERT_RETURN(pBag, false);
		return pBag->IsPosValid(nPos);
	}

	void KBagManager::ExchangeBagID( int nNewBagID, int nDelBagID )
	{
		BagList::iterator itorNewFind = m_listBag.find(nNewBagID);
		if(itorNewFind != m_listBag.end())
		{
			ASSERT_I(false);
			return;
		}

		BagList::iterator itorDelFind = m_listBag.find(nDelBagID);
		if(itorDelFind == m_listBag.end())
		{
			ASSERT_I(false);
			return;
		}

		KBag* pDelBag = (KBag*)(itorDelFind->second);

		m_listBag.erase(itorDelFind);
		pDelBag->m_nBagID = nNewBagID;
		m_listBag[nNewBagID] = pDelBag;
	}
	
	bool KBagManager::CanCostItemInAllBag( DWORD dwItemID, int nItemNumber ) const
	{
		int nItemTmp = 0;
		BagList::const_iterator itoTmp = m_listBag.begin();
		for(;itoTmp!=m_listBag.end();itoTmp++)
		{
			const KBag* pBag = itoTmp->second;
			if(NULL == pBag) continue;
			
			nItemTmp += pBag->GetItemSize(dwItemID);
		}

		if(nItemTmp < nItemNumber)
		{
			// 背包里的物品不够
			return false;
		}

		return true;
	}

	bool KBagManager::CanCostOneBag( int nBag, DWORD dwItemID, int nItemNumber ) const
	{
		const KBag* pBag = FindBag(nBag);
		if(NULL == pBag) return false;
		int nItemTmp = pBag->GetItemSize(dwItemID);
		if(nItemTmp < nItemNumber)
		{
			// 背包里的物品不够
			return false;
		}

		return true;
	}

	const KCreateInfo_ItemBase* KBagManager::GetItemCreateInfo( DWORD dwItemID ) const
	{
		if(ITEM_INVALID_ID == dwItemID)
			return NULL;

		const KCreateInfo_ItemBase* pCIIB = KItemCreate::GetInstancePtr()->GetItemCreateInfo(dwItemID);
		return pCIIB;
	}

	KCellBag* KBagManager::FindCell(int nBagID, int nPos)
	{
		KBag* pBag = FindBag(nBagID);
		if(NULL == pBag)
		{
			return NULL;
		}

		return pBag->FindCell(nPos);
	}
	
	const KCellBag* KBagManager::FindCell(int nBagID, int nPos) const
	{
		const KBag* pBag = FindBag(nBagID);
		if(NULL == pBag)
		{
			return NULL;
		}

		return pBag->FindCell(nPos);
	}

	bool KBagManager::IsEmpty(int nBagID, int nPos) const
	{
		const KCellBag* pCell = FindCell(nBagID, nPos);
		return pCell ? false : true;
	}

	int KBagManager::GetCount(int nBagID, int nPos) const
	{
		const KCellBag* pCell = FindCell(nBagID, nPos);
		ASSERT_RETURN(pCell, 0);

		return pCell->GetItemStackNumber();
	}

	int KBagManager::GetItemCountByID( DWORD dwItemID ) const
	{
		ASSERT_RETURN(dwItemID > 0, 0);
		const KCreateInfo_ItemBase* pCIIB = GetItemCreateInfo(dwItemID);
		ASSERT_RETURN(pCIIB, 0);
		int nBag = GetBagTypeByItemType(pCIIB->GetItemType());
		ASSERT_RETURN(nBag >= 0, 0);
		return GetItemCountByID(nBag, dwItemID);
	}
	
	int KBagManager::GetItemCountByID(int nBagID, DWORD dwItemID) const
	{
		const KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, 0);
		
		return pBag->GetItemSize(dwItemID);
	}

	int KBagManager::StatBagDisplay(int nBagID, VEC_ITEM_DISPLAY& vecItemDisplay) const
	{
		vecItemDisplay.clear();
		const KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, 0);

		return pBag->StatBagDisplayItem(vecItemDisplay);
	}

	int KBagManager::StatBagDisplayEquip( VEC_ITEM_DISPLAY& vecItemDisplay ) const
	{
		return StatBagDisplay(enum_item_BagEquip, vecItemDisplay);
	}

	int KBagManager::GetFirstPos( int nBagID, DWORD dwItemID ) const
	{
		const KBag* pBag = FindBag(nBagID);
		if(NULL == pBag)
		{
			return -1;
		}

		return pBag->GetFirstPos(dwItemID);
	}

	const KCreateInfo_ItemBase* KBagManager::GetOneItemCreateInfoBase( int nBagID, DWORD dwItemID )
	{
		const KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, NULL);

		int nPos = pBag->GetFirstPos(dwItemID);
		ASSERT_RETURN(nPos >= 0, NULL);
		
		const KCellBag* pCell = pBag->FindCell(nPos);
		ASSERT_RETURN(pCell, NULL);
		const KItem& item = pCell->GetItem();

		ASSERT_I(item.GetID() == dwItemID);
		return GetItemCreateInfo(dwItemID);
	}

	KItem* KBagManager::GetItem( int nBagID, int nPos )
	{
		KCellBag* pCell = FindCell(nBagID, nPos);
		if(NULL == pCell) return NULL;
		return &(pCell->GetItem());
	}

	const KItem* KBagManager::GetItem( int nBagID, int nPos ) const
	{
		const KCellBag* pCell = FindCell(nBagID, nPos);
		if(NULL == pCell) return NULL;
		return &(pCell->GetItem());
	}

	DWORD KBagManager::GetItemID( int nBagID, int nPos ) const
	{
		const KCellBag* pCell = FindCell(nBagID, nPos);
		if(NULL == pCell) return 0;
		return pCell->GetItemID();
	}
	int KBagManager::GetNotEmptyCellSize( int nBagID ) const
	{
		const KBag* pBag = FindBag(nBagID);
		if(NULL == pBag)
		{
			return 0;
		}

		return pBag->GetNotEmptyCellSize();
	}

	void KBagManager::SetMasterID( UINT64 nMasterID )
	{
		m_nMasterID = nMasterID;
	}

	void KBagManager::_UseItemErrResponse(const SC_BagUseErrResponse* pBUER)
	{
		ASSERT_RETURN_VOID(pBUER);
		int nErr = 0;
		switch(pBUER->result)
		{
		case SC_BagUseErrResponse::E_RESULT::FAILED_LEVEL_LIMIT:
			nErr = 1401601;	// 1401601 无法使用:人物等级不足
			break;

		default:
			ASSERT_I(false);
			return;
		}

		const char* pErr = KStringManager::Instance()->GetStringByID(nErr);
		ASSERT_RETURN_VOID(pErr);
		//g_DynamicWorld.ShowHeadInfoMsg(pErr);
	}

	int KBagManager::ItemOperatorErr( const SC_BagOperationErr* pBOE )
	{
		ASSERT_RETURN(pBOE, 0);
		switch(pBOE->byteSubCmd)
		{

		case enumIOR_USE_RESULT:
			_UseItemErrResponse((const SC_BagUseErrResponse*)pBOE);
			break;

		default:
			ASSERT_I(false);
			break;
		}

		return 0;
	}

	int KBagManager::IsSeparable( int nBoxId, int nPos )
	{
		int nBagId = BoxID2BagID(nBoxId);
		if(enum_item_BagNormal != nBagId && enum_item_BagMat != nBagId 
			&& enum_item_MillBagNormalBox != nBagId 
			&& enum_item_MillBagMaterialBox != nBagId 
			&& enum_item_MillBagEquipmentBox != nBagId)
		{
			return 1;
		}

		const KBag* pBag = FindBag(nBagId);
		if(NULL == pBag) return 1;
		const KCellBag* pCell = pBag->FindCell(nPos);
		if(NULL == pCell)
		{
			return 1;
		}
		const KItem& item = pCell->GetItem();
		//DWORD dwItemID = item.GetID();

	
		if(item.GetStackNumber() <= 1 || !item.IsStack())
		{
			// 1405238 该物品不能拆分
// 			const char* pInfo = KStringManager::Instance()->GetStringByID(1405238);
// 			g_DynamicWorld.ShowInfoMsg(pInfo);
			return 2;
		}
		
		if(pBag->GetEmptyCellSize() <= 0)
		{
			// 1405239 背包已满，请清理
// 			const char* pInfo = KStringManager::Instance()->GetStringByID(1405239);
// 			g_DynamicWorld.ShowInfoMsg(pInfo);
			return 3;
		}

		return 0;
	}

	void KBagManager::ChangeItemState( int nBoxId, int nPos, bool bLock )
	{
		IBlock* pIB = KBlockBoxManager::Instance()->GetBlock(nBoxId, nPos);
		ASSERT_RETURN_VOID(pIB);
		pIB->SetLock(bLock);

		KBlockBoxManager::Instance()->OnBlockInfoChanged(nBoxId, nPos);
	}

	bool KBagManager::IsInSitu() const
	{
		return CanCostItemInAllBag(12000, 1);
	}


	bool KBagManager::_FindLevel(int nLevel, KGemLevelSortArray& SortLevel) const
	{
		int nLevelSize = SortLevel.size();
		for(int i=0;i<nLevelSize;i++)
		{
			if(nLevel == SortLevel.at(i))
			{
				return true;
			}
		}

		return false;
	}


	bool KBagManager::BagIsEmpty( int nBagID ) const
	{
		const KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, false);
		return pBag->IsEmpty();
	}

	int KBagManager::GetBagTypeByItemType( BYTE nItemType ) const
	{
		switch(nItemType)
		{
		case enum_item_normal:
		case enum_item_gem:
		case enum_item_hammer:
		case enum_item_PetBag:		// 宠物背包
		case enum_item_PetEgg:		// 宠物蛋
		case enum_item_SkillBook:	// 宠物技能书
		case enum_item_SkillStar:	// 宠物技能星
			return enum_item_BagNormal;

		case enum_item_quest:
			return enum_item_BagQuest;

		case enum_item_Blueprint:
			return enum_item_MillBagNormalBox;

		default:
			break;
		}

		return -1;
	}

	bool KBagManager::BagIsFull( int nBagID ) const
	{
		const KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, false);
		return pBag->IsFull();
	}

	int KBagManager::FindFirstEmptyCell( int nBagID ) const
	{
		const KBag* pBag = FindBag(nBagID);
		if(NULL == pBag) return -1;
		return pBag->FindFirstEmptyCell();
	}
	
	bool KBagManager::AddOneCell( const SC_BagAddResponse* pBAR )
	{
		ASSERT_RETURN(pBAR, false);
		int nBagID = pBAR->byteBagID;
		KBag* pBag = FindBag(nBagID);
		ASSERT_RETURN(pBag, false);
		
		KItem item;
		item.Clear();
		KMsgInputStream si(pBAR->GetItemBuffer(), pBAR->nItemBufferSize);
		if(!item.Unserilize(si))
		{
			return false;
		}
		if(!pBag->AddOneCell(pBAR->nPos, item))
		{
			return false;
		}
		
		pBag->NumChange(item.GetID(), item.GetStackNumber());

		// 通知界面
		int nBoxID = BagId2BoxID(nBagID);
		ASSERT_RETURN(-1 != nBoxID, false);
		KItemBlock* pIB = KItemBlock::Alloc();
		pIB->SetPos(nBoxID, pBAR->nPos);
		KBlockBoxManager::Instance()->PutBlock(nBoxID, pBAR->nPos, pIB);

		if (nBagID == enum_item_BagNormal)
		{
			KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_NORMAL_BAG_ITEM_CHANGE, item.GetID(), 0);
		}

		return true;
	}

	int KBagManager::GenSyncDataUpdateCell(char* szTmp,int nBagID, int nPos, enumItemDbStruct enumFlag, BYTE* pBuffer, int nBufferLen, int eOptType)
	{
		SC_BagUpdateResponse* pBUR = (SC_BagUpdateResponse*)szTmp;
		pBUR->byteSubCmd = enumIOR_UPDATE;
		pBUR->nPlayerID = 0;//m_pPlayerOwn->GetID();
		pBUR->byteBagID = nBagID;
		pBUR->nPos = nPos;
		pBUR->nOperatorType = eOptType;

		pBUR->byteUpdateFlag = enumFlag;
		pBUR->nUpdateBufferSize = nBufferLen;
		memcpy(szTmp + sizeof(SC_BagUpdateResponse), pBuffer, nBufferLen);

		return sizeof(SC_BagUpdateResponse) + nBufferLen;
	}
	
	void KBagManager::TryToUse(KBag* bag,int nPos,UINT64 playerId)
	{
		char szTmp[2048] = "";
		int nSendLen = 0;
		
		if(bag->TryToUse(nPos,playerId)){
			int newNum = bag->FindCell(nPos)->GetItemStackNumber()-1;
			nSendLen = KBagManager::GenSyncDataUpdateCell(szTmp,enum_item_BagNormal, nPos, enum_stackNumber, (BYTE*)(&newNum), sizeof(newNum), 0);

			KNetMsgFacade::onBagOperation(szTmp,nSendLen);
		}
	}

	void KBagManager::AddBagItem(KBag* bag,int nPos,int num)
	{
		char szTmp[1024] = "";
		int newNum = bag->FindCell(nPos)->GetItemStackNumber()+num;
		int nSendLen = KBagManager::GenSyncDataUpdateCell(szTmp,enum_item_BagNormal, nPos, enum_stackNumber, (BYTE*)(&newNum), sizeof(newNum), 0);
		KNetMsgFacade::onBagOperation(szTmp,nSendLen);
	}
}




