/********************************************************************
created:	2011/8/4
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "IBlock.h"
#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"
//#include "Base/KLogicMsgParam.h"
#include "KBlockGroup.h"
#include "Inc/Lua/KLuaWrap.h"

namespace KBlockAbout
{
	class KBlockBox;
	class KBlockGroupManager;
	class KBlockBoxManager
		: public System::Patterns::KSingleton<KBlockBoxManager>
	{
	public:
		KBlockBoxManager();
		// 创建一个Box
		bool CreateBlockBox(int boxID, int maxSize);
// 		bool CreateBpMatBlockBox(int nMatBoxID);
		bool DeleteBlockBox(int nBoxID);
		bool ClearBlockBox(int nBoxID);
		// 执行拖拽
		bool Drag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// 执行拖拽
		bool DragOverNotDstPos(int fromBoxID, int fromPos, int toBoxID);
		// 得到Block
		const IBlock* GetBlock(int boxID, int pos) const;
		// 得到Block，非const方法
		IBlock* GetBlock(int boxID, int pos);
		// 丢弃
		bool Drop(int boxID, int pos);
		// 呼吸
		void Breathe();
		// 根据当前玩家的状态初始化BlockManager
		void Build(const void* shortcutBuffer, int bufferSize);
		// 放入一个新的block
		void PutBlock(int boxID, int pos, IBlock* pBlock);

		/**
		 * 事件处理，CD改变
		 */
		void OnMainPlayerCDChanged(int cdgroup);

	

		/**
		 * 事件处理，Item有所改变
		 */
		void OnMainPlayerItemChanged(int itemID);

		/**
		 * 事件处理，block有所改变
		 */
		void OnBlockInfoChanged(int boxID, int pos);

	
		// Block是道具0、装备1、技能2
		int GetBlockStandFor(int boxID, int pos);

		// 背包物品有位置改动时，要遍历快捷栏，把对应的itemlink类型的block的链接位置做个改变
		void OnItemBlockChanged(int fromBoxID, int toBoxID, int fromPos, int toPos);

		void OnShortcutBlockChanged(int fromBox, int fromPos, int toBox, int toPos);
		/**
		 * 事件处理，玩家等级改变
		 */
		void OnPlayerLevelChanged();
		/**
		 * 事件处理，block锁定状态发生改变
		 */
		void OnItemLockStatusChanged(int boxID, int pos);

		void Reset();
	
	
		void UseSlot(int boxID, int pos, float x, float y, bool bOnlyUse, bool bOnlyPrepare);
		void PostUseSlot(int boxID, int pos);

		// 物品拖拽
		bool ItemDrag(int fromBoxID, int fromPos, int toBoxID, int toPos, bool bIgnore = false);

	private:
		// 设置一个Block
		IBlock* SetBlock(int boxID, int pos, IBlock* pBlock);
		// 交换拖拽
		bool SwitchDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// 汇总总数拖拽
		bool ItemSumDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// 物品链接拖拽
		bool ItemLinkDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// 复制一个超链接
		IBlock* CopyLink(const IBlock* pB);
		// 塞改变消息
		void PushChangeInfo(int boxID, int pos);
		// 塞改变消息(CD)
		void PushChangeInfo_CD(int boxID, int pos);
		/**
		** 玩家等级改变消息
		**/
		void PushChangeInfo_PlayerItemUse(int boxID, int pos);
		/**
		** block lock改变消息
		**/
		void PushChangeInfo_LockStatus(int boxID, int pos);
		void _OnItemBlockChanged(int fromBoxID, int toBoxID, int checkBoxID, int fromPos, int toPos);

		friend class KBlockGroupManager;
		friend class KMainPlayerSkillBlock;
		friend class KMainPlayerSkillLinkBlock;
		friend class KItemBlock;
		friend class KItemLinkBlock;
		friend class KItemSumBlock;

	public:
		// 删除一个block
		void DestroyBlock(IBlock* pB, bool bAnnounceGroupChange = true);

	private:
		typedef System::Collections::KMapByVector<int, KBlockBox*> MAP_TYPE;
		MAP_TYPE m_boxMap;

		KBlockChangeList m_blockChangeList;
		KBlockChangeList m_blockCDChangeList;
		KBlockChangeList m_blockPlayerItemUseChangeList;
		KBlockChangeList m_blockLockStatusChangeList;

		KBlockGroupManager m_blockGroup;
		
		int m_nDragTmpBagPos;	// 拖拽宝石进行合成/雕刻时用于记录宝石在背包位置

		// 记录快捷栏上技能是否可用
		bool m_bShortCutUsable[12];

		const IBlock* m_BreathBlock;
	public:
	/*	BeginDefLuaClass(KBlockBoxManager)
			DefMemberFunc(GetBlockStandFor)
		EndDef*/
	};
}