#include "KBlockGroup.h"
#include "KBlockBoxManager.h"
#include "KBlock.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "../WorldObject/KPlayer.h"

using namespace KItemAbout;
using namespace KAttributeAbout;

namespace KBlockAbout
{
	KBlockGroupManager::KBlockGroupManager()
	{
	}

	void KBlockGroupManager::OnAddBlock( IBlock* pBlock )
	{
		if (!pBlock) return;
		// 根据pBlock决定将其放入哪些组
		switch(pBlock->GetType())
		{
		case enumBT_ItemBlock:
			{
				KItemBlock* pIB = (KItemBlock*)pBlock;
				const KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
			
					{
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}
				}
			}
			break;
		case enumBT_ItemLinkBlock:
			{
				KItemLinkBlock* pIB = (KItemLinkBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
					
					{
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}
				}
			}
			break;
		case enumBT_ItemSumBlock:
			{
				KItemSumBlock* pIB = (KItemSumBlock*)pBlock;
				_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);		
				{
					_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				}
			}
			break;
		case enumBT_ItemReaderAttachBlock:
			{
				KAttachBlock* pIB = (KAttachBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				
					{
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}

				}
			}
			break;
		case enumBT_ItemBusinessBlock:
			{
				KBusinessBlock* pIB = (KBusinessBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				
					{
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}
				}
			}
			break;
		case enumBT_ItemStoreBlock:
			{
				KStoreBlock* pIB = (KStoreBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
                    {
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}
				}
			}
			break;
		case enumBT_ItemStoreBuyBackBlock:
			{
				KStoreBuyBackBlock* pIB = (KStoreBuyBackBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
					{
						_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
					}
				}
			}
			break;
		case enumBT_BagBlock:
			{
				KBagBlock* pIB = (KBagBlock*)pBlock;
				const KItemAbout::KCreateInfo_ItemBase* pCIIB = pIB->GetItemCreateInfo();
				if(pCIIB)
				{
					_Insert(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
					_Insert(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				}
			}
			break;
		default:
			ASSERT(false);
		}

		OnBlockChanged(pBlock);
	}

	void KBlockGroupManager::OnDelBlock( IBlock* pBlock, bool bAnnounceGroupChange)
	{
		if (!pBlock) return;
		// 根据pBlock决定将删除哪些组的内容，需要比照OnAddBlock处理，保持增删一致
		switch(pBlock->GetType())
		{
		case enumBT_ItemBlock:
			{
				KItemBlock* pIB = (KItemBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_ItemLinkBlock:
			{
				KItemLinkBlock* pIB = (KItemLinkBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_ItemSumBlock:
			{
				KItemSumBlock* pIB = (KItemSumBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_ItemBusinessBlock:
			{
				KBusinessBlock* pIB = (KBusinessBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_ItemStoreBlock:
			{
				KStoreBlock* pIB = (KStoreBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_ItemStoreBuyBackBlock:
			{
				KStoreBuyBackBlock* pIB = (KStoreBuyBackBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_SignleItemLinkBlock:
			{
				KSingleItemLinkBlock* pIB = (KSingleItemLinkBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if (pIB->GetSkillGroup())
				{
					_Remove(enumGMI_MainPlayerCDGroup, pIB->GetSkillGroup(), pBlock);
				}
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		case enumBT_BagBlock:
			{
				KBagBlock* pIB = (KBagBlock*)pBlock;
				_Remove(enumGMI_MainPlayerItemGroup, pIB->GetItemID(), pBlock);
				_Remove(enumGMI_MainPlayerItemUseGroup, 0, pBlock);
				if(bAnnounceGroupChange)
					_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
			}
			break;
		default:
			ASSERT(false);
		}

		// DELETE时这里就不能这样搞了
		// OnBlockChanged(pBlock);
	}

	void KBlockGroupManager::OnMainPlayerCDChanged( int cdgroup )
	{
		_AnnounceGroupChange(enumGMI_MainPlayerCDGroup, cdgroup);
	}

	void KBlockGroupManager::OnCarrierCDChanged(int cdgroup)
	{
		_AnnounceGroupChange(enumGMI_CarrierCDGroup, cdgroup);
	}

	void KBlockGroupManager::OnCarrierSkillChanged( int skillID )
	{
		_AnnounceGroupChange(enumGMI_CarrierSkillGroup, skillID);
	}

	void KBlockGroupManager::OnMainPlayerItemChanged( int itemID )
	{
		_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, itemID);
	}

	void KBlockGroupManager::OnMainPlayerItemUseChanged()
	{
		_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
	}

	void KBlockGroupManager::OnBlockChanged( IBlock* pBlock )
	{
		if (!pBlock) return;
		// 根据pBlock决定需要更新哪些组
		switch(pBlock->GetType())
		{
		case enumBT_ItemBlock:
			{
				KItemBlock* pIB = (KItemBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_ItemLinkBlock:
			{
				KItemLinkBlock* pIB = (KItemLinkBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_ItemSumBlock:
			{
				KItemSumBlock* pIB = (KItemSumBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_ItemBusinessBlock:
			{
				KBusinessBlock* pIB = (KBusinessBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
			// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_ItemStoreBlock:
			{
				KStoreBlock* pIB = (KStoreBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_ItemStoreBuyBackBlock:
			{
				KStoreBuyBackBlock* pIB = (KStoreBuyBackBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_SignleItemLinkBlock:
			{
				KSingleItemLinkBlock* pIB = (KSingleItemLinkBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
				// 不会因为某物品改变而影响CD
			}
			break;
		case enumBT_BagBlock:
			{
				KBagBlock* pIB = (KBagBlock*)pBlock;
				_AnnounceGroupChange(enumGMI_MainPlayerItemGroup, pIB->GetItemID());
				_AnnounceGroupChange(enumGMI_MainPlayerItemUseGroup, 0);
			}
			break;
		default:
			ASSERT(false);
		}
	}

	void KBlockGroupManager::_Insert( int type, int group, IBlock* pToInsert )
	{
		m_groupMap[type][group].insert_unique(pToInsert);
	}

	void KBlockGroupManager::_Remove( int type, int group, IBlock* pToRemove )
	{
		ALL_GROUP::iterator allGroupIt = m_groupMap.find(type);
		if (allGroupIt != m_groupMap.end())
		{
			BLOCK_GROUP_MAP::iterator bgmIt = allGroupIt->second.find(group);
			if (bgmIt != allGroupIt->second.end())
			{
				bgmIt->second.erase(pToRemove);
			}
		}
	}

	void KBlockGroupManager::_AnnounceGroupChange( int type, int group )
	{
		if (type == enumGMI_MainPlayerCDGroup || type == enumGMI_CarrierCDGroup)
		{
			BLOCK_GROUP_MAP::iterator it = m_groupMap[type].find(group);
			if (it != m_groupMap[type].end())
			{
				BLOCK_GROUP& bg = it->second;
				for (int i = 0; i < bg.size(); i++)
				{
					KBlockBoxManager::Instance()->PushChangeInfo_CD(bg[i]->GetBoxID(), bg[i]->GetPosInfo());
				}
			}
		}
		else if(enumGMI_MainPlayerItemUseGroup == type)
		{
			BLOCK_GROUP_MAP::iterator it = m_groupMap[type].find(group);
			if (it != m_groupMap[type].end())
			{
				BLOCK_GROUP& bg = it->second;
				for (int i = 0; i < bg.size(); i++)
				{
					KBlockBoxManager::Instance()->PushChangeInfo_PlayerItemUse(bg[i]->GetBoxID(), bg[i]->GetPosInfo());
				}
			}
		}
		else
		{
			BLOCK_GROUP_MAP::iterator it = m_groupMap[type].find(group);
			if (it != m_groupMap[type].end())
			{
				BLOCK_GROUP& bg = it->second;
				for (int i = 0; i < bg.size(); i++)
				{
					KBlockBoxManager::Instance()->PushChangeInfo(bg[i]->GetBoxID(), bg[i]->GetPosInfo());
				}
			}
		}
	}
}