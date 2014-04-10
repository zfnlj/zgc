#include "KBlockBoxManager.h"
#include "KBlockBox.h"
#include "KBlock.h"
#include "../Item/KItemCreate.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "../Item/KBagManager.h"
#include "../Business/KBusinessManager.h"
#include "../Store/KStoreManager.h"
#include "../WorldObject/KMainPlayer.h"

using namespace KItemAbout;

namespace KBlockAbout
{
	bool KBlockBoxManager::CreateBlockBox( int boxID, int maxSize )
	{
		ASSERT_RETURN(maxSize > 0, false);
		MAP_TYPE::iterator it = m_boxMap.find(boxID);
		if (it != m_boxMap.end())
		{
			// �Ѿ������ˣ����ܴ���
			return false;
		}
		KBlockBox* pBB = KBlockBox::Alloc();
		pBB->SetMaxBlockSize(maxSize);
		pBB->SetBoxID(boxID);
		m_boxMap[boxID] = pBB;
		return true;
	}

// 	bool KBlockBoxManager::CreateBpMatBlockBox(int nMatBoxID)
// 	{
// 		return CreateBlockBox(nMatBoxID, BOXSIZE_BpMatBox);
// 	}

	bool KBlockBoxManager::DeleteBlockBox(int nBoxID)
	{
		MAP_TYPE::iterator it = m_boxMap.find(nBoxID);
		if (it == m_boxMap.end())
		{
			// ������
			return true;
		}
		
		KBlockBox* pBB = it->second;
		if(pBB)
		{
			pBB->Clear();
			KBlockBox::Free(pBB);
			m_boxMap.erase(it);
		}

		return true;
	}
	
	bool KBlockBoxManager::ClearBlockBox(int nBoxID)
	{
		MAP_TYPE::iterator it = m_boxMap.find(nBoxID);
		if (it == m_boxMap.end())
		{
			// ������
			return true;
		}

		KBlockBox* pBB = it->second;
		if(pBB)
		{
			pBB->Clear();
		}

		return true;
	}

	bool KBlockBoxManager::Drag( int fromBoxID, int fromPos, int toBoxID, int toPos )
	{
		// �϶���pos����
		if(enumBTID_StoreBox == toBoxID
		|| enumBTID_StoreBuyBackBox == toBoxID
		|| (enumBTID_StoreBox == fromBoxID && enumBTID_StoreBox != toBoxID))
		{
			return DragOverNotDstPos(fromBoxID, fromPos, toBoxID);
		}

		// ��̯��Ʒ�ϼ�
		

		if(enumBTID_OtherStallBox == fromBoxID && enumBTID_OtherStallBox != toBoxID)
		{
			return DragOverNotDstPos(fromBoxID, fromPos, toBoxID);
		}

		if(enumBTID_BusinessSelfBox == toBoxID)
		{
			if(enumBTID_ItemNormalBox != fromBoxID)
			{
				return false;
			}
		}

		// Դ��Ŀ��box�Ƿ���
		if (m_boxMap.find(fromBoxID) == m_boxMap.end() || m_boxMap.find(toBoxID) == m_boxMap.end())
		{
			return false;
		}
		KBlockBox* fromBox = m_boxMap.find(fromBoxID)->second;
		KBlockBox* toBox = m_boxMap.find(toBoxID)->second;

		// Դblock�Ƿ���
		IBlock* fromBlock = fromBox->GetBlock(fromPos);
		if (!fromBlock)
		{
			return false;
		}
		
		IBlock* toBlock = toBox->GetBlock(toPos);

		if(toBlock && ((toBlock->GetLock() || !toBlock->GetDragable())))
		{
			return false;
		}
		
		// ȡ����������
		if (fromBoxID == enumBTTD_MailAttachment && toBoxID == enumBTID_ItemNormalBox)
		{
			//return KMailManager::Instance()->DelItemFromSend(fromPos);
		}

		// ��������Ϊ���û�
		if (fromBoxID == toBoxID)
		{
			if (fromBoxID == enumBTID_ItemNormalBox) // �����ͨ����
			{
				// ��������Ʒ��λ��
				//KMainPlayer::Instance()->TryMoveItem(enum_item_BagNormal, fromPos, enum_item_BagNormal, toPos);
				return true;
			}
			else if (fromBoxID == enumBTID_ItemQuestBox) // ������񱳰�
			{
				// ��������Ʒ��λ��
				//KMainPlayer::Instance()->TryMoveItem(enum_item_BagQuest, fromPos, enum_item_BagQuest, toPos);
				return true;
			}
			else if (enumBTID_DirectUse == toBoxID)
			{
				return false;
			}
			// �����ǰ��ڲ����ƶ���
			else if((enumBTID_BusinessSelfBox == toBoxID && enumBTID_BusinessSelfBox == fromBoxID)
			|| (enumBTID_BusinessOtherBox == toBoxID && enumBTID_BusinessOtherBox == fromBoxID)
			|| (enumBTID_StoreBox == toBoxID && enumBTID_StoreBox == fromBoxID)
			|| (enumBTID_StoreBuyBackBox == toBoxID && enumBTID_StoreBuyBackBox == fromBoxID)
			|| (enumBTID_BusinessOtherBox == toBoxID && enumBTID_BusinessOtherBox == fromBoxID)
			|| (enumBTID_MyselfStallBox == toBoxID && enumBTID_MyselfStallBox == fromBoxID)
			)
			{
				return false;
			}

			toBox->Switch(fromPos, toPos);
			// �����б仯������Ϣ
			PushChangeInfo(fromBoxID, fromPos);
			PushChangeInfo(toBoxID, toPos);
			return true;
		}
		else // ��֮�����
		{
			// TODO: ���ݺ����ж��������
			switch(toBoxID)
			{
			case enumBTID_Shoutcut:
				{
					OnShortcutBlockChanged(-1, -1, toBoxID, toPos);

					switch(fromBoxID)
					{
					case enumBTID_ItemNormalBox:	// ����ͨ�������Ϲ���
					case enumBTID_ItemQuestBox:		// ����ͨ�������Ϲ���
						{
							// ��������Ҽ�����λ��ֻ�ܷż���
							if (toPos == 10 || toPos == 11)
							{
								return false;
							}
							return ItemDrag(fromBoxID, fromPos, toBoxID, toPos, true);
						}

					case enumBTID_EquipBox:			// ��װ�����Ϲ���
						// ��Ӧ�������װ������ҷ��
						//{
						//	// ��������Ҽ�����λ��ֻ�ܷż���
						//	if (toPos == 10 || toPos == 11)
						//	{
						//		return false;
						//	}
						//	return ItemDrag(fromBoxID, fromPos, toBoxID, toPos);
						//}
						break;
					default:
						break;
					}
				}
				break;

			case enumBTID_DirectUse:
				{
					switch(fromBoxID)
					{
					case enumBTID_ItemQuestBox:
					case enumBTID_ItemNormalBox:
						return ItemDrag(fromBoxID, fromPos, toBoxID, toPos, true);
					}
				}
				break;
			case enumBTTD_MailAttachment:		// �ʼ�����
				{
					//KMailManager::Instance()->AddItemToSend(fromBoxID, fromPos, toPos);
				}
				break;
			case enumBTID_ItemNormalBox:	// Ŀ������ͨ����
				{
					switch(fromBoxID)
					{
					case enumBTID_EquipBox:			// ��װ�����Ϲ���
						//KMainPlayer::Instance()->TryMoveItem(enum_item_BagEquip, fromPos, enum_item_BagNormal, toPos);
						break;
					case enumBTID_BusinessSelfBox:
						{
							KItemLinkBlock* pFromItemLink = (KItemLinkBlock*)fromBlock;
							const KCreateInfo_ItemBase* pCIIB = pFromItemLink->GetItemCreateInfo();
							ASSERT_RETURN(pCIIB, false);
							if(pCIIB->GetItemType() != enum_item_quest )
							{
								KBusinessAbout::KBusinessManager::Instance()->TryBusinessPutOneItem(enumBTID_ItemNormalBox, toPos, fromPos, CS_BusinessPutOneItem::enumBPOI_Remove);
							}
							return true;
						}
						break;
					default:
						// ����������ط��Ͻ���
						;
					}
				}
				break;
			case enumBTID_BusinessSelfBox:	// �����׿����ƶ���Ʒ
				{
					if(enumBTID_ItemNormalBox == fromBoxID )
					{
						KItemBlock* pItemFrom = (KItemBlock*)GetBlock(fromBoxID, fromPos);
						if(NULL == pItemFrom) return false;
						KItemLinkBlock* pItemLinkTo = (KItemLinkBlock*)GetBlock(toBoxID, toPos);
						if(pItemLinkTo) return false;

						KBusinessAbout::KBusinessManager::Instance()->TryBusinessPutOneItem(fromBoxID, fromPos, toPos, CS_BusinessPutOneItem::enumBPOI_Add);
						return true;
					}
				}
				break;
			}
		}
		return false;
	}

	bool KBlockBoxManager::DragOverNotDstPos( int fromBoxID, int fromPos, int toBoxID )
	{
		// Դblock��
		KBlockBox* fromBox = m_boxMap.find(fromBoxID)->second;
		ASSERT_RETURN(fromBox, false);
		IBlock* fromBlock = fromBox->GetBlock(fromPos);
		if(!fromBlock)
		{
			return false;
		}

		// ��������Ϊ���û�
		if (fromBoxID == toBoxID)
		{
			return false;
		}
		if(enumBTID_StoreBox == fromBoxID)	// �̵깺����קĬ������Ϊ1
		{
			if(enumBTID_ItemNormalBox == toBoxID)
			{
				KStoreAbout::KStoreManager::Instance()->PopCommBuyDialog(fromBoxID, fromPos, true);
				return true;
			}
			else
			{
				return false;
			}
		}
		
		if(enumBTID_OtherStallBox == fromBoxID && enumBTID_OtherStallBox != toBoxID)
		{
			if(enumBTID_ItemNormalBox == toBoxID )
			{
				KStoreAbout::KStoreManager::Instance()->PopCommBuyDialog(fromBoxID, fromPos, false);
				return true;
			}
			else
			{
				return false;
			}
		}
		
		if(
			(enumBTID_ItemNormalBox == fromBoxID && enumBTID_MillBagNormalBox == toBoxID)
			|| (enumBTID_MillBagNormalBox == fromBoxID && enumBTID_ItemNormalBox == toBoxID)
		  )
		{
			KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
			ASSERT_RETURN(pBagMgr, false);
			int nFromBagID = pBagMgr->BoxID2BagID(fromBoxID);
			int nToBagID = pBagMgr->BoxID2BagID(toBoxID);
			if(pBagMgr->BagIsFull(nToBagID))
			{
				const char* pInfo = NULL;
				switch(toBoxID)
				{
				case enumBTID_MillBagNormalBox:
					// 1404998	 ��ͨ�ֿ��������޷����룡
					pInfo = KStringManager::Instance()->GetStringByID(1404998);
					break;
				case enumBTID_ItemNormalBox:
					// 1404999	 �����������޷�ȡ����
					pInfo = KStringManager::Instance()->GetStringByID(1404999);
					break;
				}
				//g_DynamicWorld.ShowHeadInfoMsg(pInfo);
				return false;
			}
			int toPos = pBagMgr->FindFirstEmptyCell(nToBagID);
			if(toPos >= 0)
			{
				//KMainPlayer::Instance()->TryMoveItem(nFromBagID, fromPos, nToBagID, toPos);
			}
			return true;
		}

		// TODO: ���ݺ����ж��������
		switch(toBoxID)
		{
		case enumBTID_StoreBox:
		case enumBTID_StoreBuyBackBox:
			{
				if(enumBTID_ItemNormalBox == fromBoxID)
				{
					const KItemBlock* pItemFrom = (const KItemBlock*)GetBlock(fromBoxID, fromPos);
					if(NULL == pItemFrom) return false;
					KStoreAbout::KStoreManager::Instance()->TrySell(fromBoxID, fromPos);
					return true;
				}
			}
			break;
		}
		return false;
	}

	IBlock* KBlockBoxManager::SetBlock( int boxID, int pos, IBlock* pBlock )
	{
		MAP_TYPE::iterator it = m_boxMap.find(boxID);
		if (it == m_boxMap.end())
		{
			return NULL;
		}

		IBlock* pB = it->second->SetBlock(pos, pBlock);
		// �����б仯������Ϣ
		PushChangeInfo(boxID, pos);
		// ��pBlock���룬��pBȡ����֪ͨblockGroup
		m_blockGroup.OnAddBlock(pBlock);

		// ������ﲻ��Ҫ����������
		//m_blockGroup.OnDelBlock(pB);

		return pB;
	}

	IBlock* KBlockBoxManager::GetBlock( int boxID, int pos )
	{
		MAP_TYPE::iterator it = m_boxMap.find(boxID);
		if (it == m_boxMap.end())
		{
			return NULL;
		}
		return it->second->GetBlock(pos);
	}

	const IBlock* KBlockBoxManager::GetBlock( int boxID, int pos ) const
	{
		MAP_TYPE::const_iterator it = m_boxMap.find(boxID);
		if (it == m_boxMap.end())
		{
			return NULL;
		}
		return it->second->GetBlock(pos);
	}

	bool KBlockBoxManager::SwitchDrag( int fromBoxID, int fromPos, int toBoxID, int toPos )
	{
		ASSERT_RETURN(m_boxMap.find(fromBoxID) != m_boxMap.end(), false);
		ASSERT_RETURN(m_boxMap.find(toBoxID) != m_boxMap.end(), false);
		IBlock* pB1 = GetBlock(fromBoxID, fromPos);
		IBlock* pB2 = GetBlock(toBoxID, toPos);
		SetBlock(toBoxID, toPos, pB1);
		SetBlock(fromBoxID, fromPos, pB2);
		return true;
	}

	bool KBlockBoxManager::ItemDrag(int fromBoxID, int fromPos, int toBoxID, int toPos, bool bIgnore)
	{
		// ֻ�ܴ���ͨ������λ��ҷ����
		//if (fromBoxID == enumBTID_ItemNormalBox)
		{
			KItemBlock* IB = (KItemBlock*)GetBlock(fromBoxID, fromPos);
			int ItemID = IB->GetItemCreateInfo()->s_nItemID;
			const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(ItemID);

			if (pCIIB && pCIIB->s_nMaxStacks > 1 && bIgnore)
			{
				return ItemSumDrag(fromBoxID, fromPos, toBoxID, toPos);
			}
			else
			{
				return ItemLinkDrag(fromBoxID, fromPos, toBoxID, toPos);
			}
		}

		return false;
	}

	bool KBlockBoxManager::ItemSumDrag( int fromBoxID, int fromPos, int toBoxID, int toPos )
	{
		if (fromBoxID != enumBTID_ItemNormalBox &&
			fromBoxID != enumBTID_ItemQuestBox)
			return false; 

		KItemBlock* IB = (KItemBlock*)GetBlock(fromBoxID, fromPos);
		KItemSumBlock* KISB= KItemSumBlock::Alloc();
		KISB->SetBagType(fromBoxID);	// Ҫ����SetItemID֮ǰ
		KISB->SetItemID(IB->GetItemCreateInfo()->s_nItemID);
		KISB->SetPos(toBoxID, toPos);
		SetBlock(toBoxID, toPos, KISB);

		if (toBoxID == enumBTID_DirectUse)
		{
			KISB->SetDragable(false);
		}

		return true;
	}

	bool KBlockBoxManager::ItemLinkDrag( int fromBoxID, int fromPos, int toBoxID, int toPos )
	{
		KItemLinkBlock* KILB= KItemLinkBlock::Alloc();
		KILB->SetItemBlock(fromBoxID, fromPos);
		KILB->SetPos(toBoxID, toPos);
		SetBlock(toBoxID, toPos, KILB);

		if (toBoxID == enumBTID_DirectUse)
		{
			KILB->SetDragable(false);
		}


		return true;
	}

	bool KBlockBoxManager::Drop( int boxID, int pos )
	{
		// �����������ܲ��ܸı�
		if (boxID == enumBTID_Shoutcut)
		{
			if (pos == 10)
			{
				return false;
			}
			
			OnShortcutBlockChanged(pos, boxID, -1, -1);
		}

		// ��������
		IBlock* pB = SetBlock(boxID, pos, NULL);
		if (pB)
		{
			pB->OnDestroy();
			DestroyBlock(pB);
		}

		return true;
	}

	IBlock* KBlockBoxManager::CopyLink( const IBlock* pB )
	{
		ASSERT_RETURN(pB, NULL);
		switch(pB->GetType())
		{
		default:
			{
				// ��֧�ֵ�����
				ASSERT_I(false);
			}
		}
		return NULL;
	}

	void KBlockBoxManager::DestroyBlock( IBlock* pB, bool bAnnounceGroupChange )
	{
		if (!pB) return;
		m_blockGroup.OnDelBlock(pB, bAnnounceGroupChange);
// 		m_blockGroup.RemoveFromGroup(pB);
		// �ڴ��ͷ�
		pB->Release();
	}

	void KBlockBoxManager::Breathe()
	{
		if (m_blockChangeList.size() > 0)
		{
			//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BLOCK_CHANGED, 0, (unsigned long)&m_blockChangeList);
			m_blockChangeList.clear();
		}

		if (m_blockCDChangeList.size() > 0)
		{
			//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BLOCK_CHANGED_CD, 0, (unsigned long)&m_blockCDChangeList);
			m_blockCDChangeList.clear();
		}
		
		if(m_blockPlayerItemUseChangeList.size() > 0)
		{
			//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BLOCK_CHANGED_PLAYERITEMUSE, 0, (unsigned long)&m_blockPlayerItemUseChangeList);
			m_blockPlayerItemUseChangeList.clear();
		}
		
		if(m_blockLockStatusChangeList.size() > 0)
		{
			//g_DynamicWorld.SendWorldMsg(LOGIC_WORLD_BLOCK_CHANGED_LOCKSTATUS, 0, (unsigned long)&m_blockLockStatusChangeList);
			m_blockLockStatusChangeList.clear();
		}
	}

	void KBlockBoxManager::PushChangeInfo( int boxID, int pos )
	{
		KBlockPosInfo bpi;
		bpi.boxID = boxID;
		bpi.posID = pos;
		if (m_blockChangeList.find(bpi) < 0)
		{
			m_blockChangeList.insert(bpi);
		}
	}

	void KBlockBoxManager::PushChangeInfo_CD( int boxID, int pos )
	{
		KBlockPosInfo bpi;
		bpi.boxID = boxID;
		bpi.posID = pos;
		if (m_blockCDChangeList.find(bpi) < 0)
		{
			m_blockCDChangeList.insert(bpi);
		}
	}
	
	void KBlockBoxManager::PushChangeInfo_PlayerItemUse(int boxID, int pos)
	{
		KBlockPosInfo bpi;
		bpi.boxID = boxID;
		bpi.posID = pos;
		if (m_blockPlayerItemUseChangeList.find(bpi) < 0)
		{
			m_blockPlayerItemUseChangeList.insert(bpi);
		}
	}

	void KBlockBoxManager::PushChangeInfo_LockStatus(int boxID, int pos)
	{
		KBlockPosInfo bpi;
		bpi.boxID = boxID;
		bpi.posID = pos;
		if (m_blockLockStatusChangeList.find(bpi) < 0)
		{
			m_blockLockStatusChangeList.insert(bpi);
		}
	}

	void KBlockBoxManager::OnBlockInfoChanged( int boxID, int pos )
	{
		IBlock* pB = GetBlock(boxID, pos);
		if (pB)
		{
			m_blockGroup.OnBlockChanged(pB);
		}
	}

	KBlockBoxManager::KBlockBoxManager()
	{
		CreateBlockBox(enumBTID_DirectUse, BOXSIZE_DirectUseBox);
		// �����
		CreateBlockBox(enumBTID_Shoutcut, BOXSIZE_Shoutcut);

		// װ������
		CreateBlockBox(enumBTID_EquipBox, BOXSIZE_EquipBox);

		// ��������
		CreateBlockBox(enumBTID_ItemNormalBox, BOXSIZE_ItemNormalBox);
		CreateBlockBox(enumBTID_ItemQuestBox, BOXSIZE_ItemQuestBox);

		// �ؾ�
		CreateBlockBox(enumBTID_Carrier, BOXSIZE_Carrier);

		// �ʼ�����
		CreateBlockBox(enumBTTD_MailAttachment, BOXSIZE_MailAttachBox);
		CreateBlockBox(enunBTTD_MailReaderAttach, BOXSIZE_MailAttachBox);
		// ���׽���
		CreateBlockBox(enumBTID_BusinessOtherBox, BOXSIZE_BusinessBox);	// �Է�
		CreateBlockBox(enumBTID_BusinessSelfBox, BOXSIZE_BusinessBox);	// �Լ�
		// NPC�̵����
		CreateBlockBox(enumBTID_StoreBox, BOXSIZE_StoreBox);				// NPC�̵�ع����
		CreateBlockBox(enumBTID_StoreBuyBackBox, BOXSIZE_StoreBuyBackBox);	// NPC�̵�ع����

		// ������
		CreateBlockBox(enumBTID_DropGroundBox, BOXSIZE_DropGroundBox);
		// �������
		for (int i = 0; i < 12; i++)
		{
			m_bShortCutUsable[i] = true;
		}
	}

	void KBlockBoxManager::Build( const void* shortcutBuffer, int bufferSize )
	{
		// �Ӽ����б��ʼ������
		//KMainPlayer::Instance()->m_skillList.TraverseList(SkillListVisitor());
	}

	void KBlockBoxManager::PutBlock( int boxID, int pos, IBlock* pBlock )
	{
		DestroyBlock(SetBlock(boxID, pos, pBlock));
	}

	void KBlockBoxManager::OnMainPlayerCDChanged( int cdgroup )
	{
		m_blockGroup.OnMainPlayerCDChanged(cdgroup);
	}

	void KBlockBoxManager::OnMainPlayerItemChanged( int itemID )
	{
		m_blockGroup.OnMainPlayerItemChanged(itemID);
	}

	void KBlockBoxManager::OnPlayerLevelChanged()
	{
		m_blockGroup.OnMainPlayerItemUseChanged();
	}

	void KBlockBoxManager::OnItemLockStatusChanged(int boxID, int pos)
	{
		PushChangeInfo_LockStatus(boxID, pos);
	}

	int KBlockBoxManager::GetBlockStandFor(int boxID, int pos)
	{
		IBlock *ib = GetBlock(boxID, pos);
		if (!ib)
		{
			return -1;
		}
		else if (ib->GetType() == enumBT_MainPlayerSkillBlock ||
				 ib->GetType() == enumBT_MainPlayerSkillLinkBlock)
		{
			return 2;
		}
		else if (ib->GetType() == enumBT_ItemBlock)
		{
			KItemBlock* IB = (KItemBlock*)ib;
			const KCreateInfo_ItemBase* pCIIB = IB->GetItemCreateInfo();
			if (pCIIB->IsUsableItem())
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (ib->GetType() == enumBT_ItemLinkBlock)
		{
			KItemLinkBlock* ILB = (KItemLinkBlock*)ib;
			const KCreateInfo_ItemBase* pCIIB = ILB->GetItemCreateInfo();
			if (pCIIB->IsUsableItem())
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if (ib->GetType() == enumBT_ItemSumBlock)
		{
			KItemSumBlock* ISB = (KItemSumBlock*)ib;
			const KCreateInfo_ItemBase* pCIIB = ISB->GetItemCreateInfo();
			if (pCIIB->IsUsableItem())
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

		return -1;
	}

	void KBlockBoxManager::OnItemBlockChanged(int fromBoxID, int toBoxID, int fromPos, int toPos)
	{
		_OnItemBlockChanged(fromBoxID, toBoxID, enumBTID_Shoutcut, fromPos, toPos);
		_OnItemBlockChanged(fromBoxID, toBoxID, enumBTID_BusinessSelfBox, fromPos, toPos);
		_OnItemBlockChanged(fromBoxID, toBoxID, enumBTID_DirectUse, fromPos, toPos);
	}

	void KBlockBoxManager::OnShortcutBlockChanged(int fromBox, int fromPos, int toBox, int toPos)
	{
	}

	void KBlockBoxManager::_OnItemBlockChanged(int fromBoxID, int toBoxID, int checkBoxID, int fromPos, int toPos)
	{
		MAP_TYPE::iterator itorFind = m_boxMap.find(checkBoxID);
		if(itorFind == m_boxMap.end()) return;

		// ���ǰ����ƶ�������²���Ҫ�����ﴦ��
		if (fromBoxID != toBoxID && toBoxID != -1)
			return ;

		int nShutcutMax = itorFind->second->GetMaxBlockSize();
		for (int i = 0; i < nShutcutMax; i++)
		{
			IBlock* block = m_boxMap[checkBoxID]->GetBlock(i);
			if (block)
			{
				if (block->GetType() == enumBT_ItemLinkBlock)
				{
					KItemLinkBlock* b = (KItemLinkBlock*)block;
					if (b->GetLinkBoxID() != fromBoxID)
						continue;

					if (b->GetLinkPos() == fromPos)
					{
						if (toPos == -1)
						{
							Drop(checkBoxID, i);
						}
						else
						{
							b->SetLinkPos(toPos);
						}
					}
					else if (b->GetLinkPos() == toPos)
					{
						if (toPos == -1)
						{
							Drop(checkBoxID, i);
						}
						else
						{
							b->SetLinkPos(fromPos);
						}
					}
				}
				else if (block->GetType() == enumBT_ItemSumBlock)
				{
					KItemSumBlock* b = (KItemSumBlock*)block;
					if (b->GetLinkBoxID() != fromBoxID)
						continue;

					if (!b->Exist())
					{
						Drop(checkBoxID, i);
					}
				}
			}
		}
	}

	void KBlockBoxManager::Reset()
	{

		MAP_TYPE::iterator it = m_boxMap.begin();
		while(it != m_boxMap.end())
		{
			it->second->Clear(false);
			it++;
		}

		m_blockGroup.Reset();
	}

	void KBlockBoxManager::UseSlot(int boxID, int pos, float x, float y, bool bOnlyUse, bool bOnlyPrepare)
	{
		if (boxID < 0 || pos < 0)
			return;

		const IBlock* block = GetBlock(boxID, pos);
		if (block)
		{
			block->Use(x, y, bOnlyUse, bOnlyPrepare);
		}
	}

	void KBlockBoxManager::PostUseSlot(int boxID, int pos)
	{
		if (boxID < 0 || pos < 0)
			return;

		const IBlock* block = GetBlock(boxID, pos);
		if (block)
		{
			block->PostUse();
		}
	}
}
