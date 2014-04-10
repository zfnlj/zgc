/********************************************************************
created:	2011/8/11
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include "IBlock.h"
#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"
//#include "Base/KLogicMsgParam.h"

namespace KBlockAbout
{
	class KBlockGroupManager
	{
	public:
		KBlockGroupManager();
	public:
		/**
		 * 增加一个IBlock
		 */
		void OnAddBlock(IBlock* pBlock);

		/**
		 * 删除一个IBlock
		 */
		void OnDelBlock(IBlock* pBlock, bool bAnnounceGroupChange = true);

		/**
		 * 事件处理，CD改变
		 */
		void OnMainPlayerCDChanged(int cdgroup);

		/**
		 * 事件处理，Skill有所改变
		 */
		
		/**
		 * 事件处理，CD改变
		 */
		void OnCarrierCDChanged(int cdgroup);

		/**
		 * 事件处理，Skill有所改变
		 */
		void OnCarrierSkillChanged(int skillID);

		/**
		 * 事件处理，Item有所改变
		 */
		void OnMainPlayerItemChanged(int itemID);

		/**
		 * 事件处理，某单个Block变更
		 */
		void OnBlockChanged(IBlock* pBlock);

		/**
		 * 事件处理，主角发生技能替换
		 */
		/**
		 * 事件处理，玩家等级有所改变
		 */
 		void OnMainPlayerItemUseChanged();

		void Reset()
		{
			m_groupMap.clear();
		}

	private:
		// 每组中block的内容
		typedef System::Collections::DynSortedArray<IBlock*> BLOCK_GROUP;
		// 以id为索引的组列表
		typedef System::Collections::KMapByVector<int, BLOCK_GROUP> BLOCK_GROUP_MAP;
		// 所有的组map
		typedef System::Collections::KMapByVector<int, BLOCK_GROUP_MAP> ALL_GROUP;
		ALL_GROUP m_groupMap;

		// 所有的组的类型列表
		enum GROUP_MAP_INDEX
		{
			enumGMI_MainPlayerCDGroup = 0,
			enumGMI_MainPlayerSkillGroup,
			enumGMI_MainPlayerItemGroup,

			enumGMI_CarrierCDGroup,
			enumGMI_CarrierSkillGroup,
			enumGMI_MainPlayerItemUseGroup,		// 物品是否可用(判断等级与职业是否相符)
		};
		// 插入组
		void _Insert(int type, int group, IBlock* pToInsert);
		// 从组中删除
		void _Remove(int type, int group, IBlock* pToRemove);

		// 通知组中所有block变更
		void _AnnounceGroupChange(int type, int group);
	};
}