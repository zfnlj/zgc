/********************************************************************
created:	2012/02/27
manager:	
copyright:	Aurora Interactive, 2011

*********************************************************************/


#pragma once

#include "System/Memory/KStepObjectPool.h"
#include "System/Collections/KMapByVector.h"
#include "System/Collections/KVector.h"
#include "System/Misc/KStream.h"
#include "KItem.h"
#include "Inc/KStruct_ClientItem.h"
#include "Inc/Lua/KLuaWrap.h"


namespace KItemAbout
{
	class KBagManager;
	struct KBagItemPosition
	{
		bool bFreePos;		// 是否是完全空的背包格：true表示是不够叠加数量总数量，false表示是完全空
		int nPosIndex;		// 空格位置
		WORD nNumFind;		// 添加物品时时剩余的可叠加数量，消耗物品时是背包格内的物品数量
		KBagItemPosition()
			: bFreePos(false)
			, nPosIndex(0)
			, nNumFind(0)
		{
		}
	};
	typedef ::System::Collections::KVector<KBagItemPosition> VEC_BAG_FREE_POS;

	// 背包格子类
	class KCellBag : public System::Memory::KPortableMemoryPool<KCellBag,1024>
	{
	public:
		KCellBag();
		virtual ~KCellBag();
		KCellBag(const KCellBag& cell);
		KCellBag& operator=(const KCellBag& cell);

		void Reset();
		virtual bool Release();
		
		KItem& GetItem();
		const KItem& GetItem() const;
		KItem* GetItemPtr();
		int GetPos() const;
		DWORD GetItemID() const {return m_Item.GetID();}
		DWORD GetItemStackNumber() const {return m_Item.GetStackNumber();}
		DWORD GetFrequency() const {return m_Item.GetFrequency();}

		bool IsFrequency() const;
		void UpdateStackNumber(int nStackNumber);
		void UpdateFrequency(int nFrequency);
		void UpdateDurable(int nDurable);

		// 反序列化
		bool Unserilize(KMsgInputStream& si);

		bool IsLock() const;
		void SetStallLockType(bool bLock);
		bool IsStallLock() const;
		void SetLock(int nLockType);

	public:
		int m_nPos;
		KItem m_Item;
		int m_nLockType;		// 锁状态

	public:
		BeginDefLuaClass(KCellBag)
			DefMemberFunc(GetPos)
		EndDef
	};

	// 背包基类
	class KBag
	{
	public:
		KBag();
		virtual ~KBag();
		KBag(const KBag& Bag);
		KBag& operator=(const KBag& Bag);

		virtual void Reset();
		void SetCellLimite(int CellMaxCount, int CellUnLockCount);
		virtual bool Release() {return false;}
		/**
		** 初始化反序列化
		**/
		virtual bool UnserilizeInitInfo(KMsgInputStream& si);

		int GetBagID() const;
		UINT64 GetMasterID() const;
		int GetMaxCount() const;
		int GetUnLockCount() const;
		
		KCellBag* FindCell(int nPos);
		const KCellBag* FindCell(int nPos) const;
		bool IsPosValid(int nPos) const;
		/**
		** 格子是否是锁定状态(即格子未打开，不能使用)
		**/
		bool IsCellUnLock(int nPos) const;

		/**
		** 新添加一个格子，并放上物品
		**/
		bool AddOneCell(int nPos, const KItem& item);
		bool Add(UINT itemID,int count);
		/**
		** 删除一个格子上的物品
		**/
		bool DelOneCell(int nPos);
		/**
		** 整理包内物品
		**/
		bool Sortout(KBagManager* pBagMgr);

		int GetItemSize(DWORD dwItemID) const;
		int GetNotEmptyCellSize() const;
		/**
		** 包中物品数量发生变化
		**/
		int GetEmptyCellSize() const;

		int StatBagDisplayItem(VEC_ITEM_DISPLAY& vecItemDisplay) const;
		
		int GetFirstPos(DWORD dwItemID) const;
		/**
		** 反序列化
		**/
		bool Unserilize(KMsgInputStream& si);
		int FindFirstEmptyCell() const;

		int FindBagPosByAdd(DWORD dwItemID, int nItemNum, const KItemAbout::KCreateInfo_ItemBase* pCIIB, VEC_BAG_FREE_POS& vecBagPos) const;
		// 背包内是否有物品
		bool IsEmpty() const;
		/**
		** 包中物品数量发生变化
		**/
		virtual void NumChange(DWORD dwItemID, int nNumChange) {}
		
		bool IsFull() const;

		bool TryToUse(int nPos,UINT64);
	private:
		/**
		** 合并包内物品
		**/
		static bool _MergeAllCell(KBagManager* pBagMgr, KBag& bag);
		static bool _SyncDataMergeItem(KBag& bag, int nFromPos, int nToPos, int nStackLimit);
		static bool _ExchangeItem(KBagManager* pBagMgr, KBag& bag);
		static bool _SyncDataExchangeItem(KBag& bag, int nFromPos, int nToPos);
		static bool _GetMinItemID(KBagManager* pBagMgr, int nBoxID, KBag& bag, int nStartPos, int& nPos);

	private:
		void _DelAllCell();

	public:
		int m_nBagID;				// 背包ID(代替背包类型)
		int m_nCellMaxCount;		// 格子最大数目(加锁未加锁都算)
		int m_nCellUnLockCount;		// 未加锁格子数目
		UINT64 m_nMasterID;

	public:
		// 格子列表
		typedef System::Collections::KMapByVector<int, KCellBag*> CellMapVec;
		CellMapVec m_ListItem;

	public:
	/*	BeginDefLuaClass(KBag)
			DefMemberFunc(IsPosValid)
		EndDef*/
	};


	// 普通背包
	class KBagNormal : public KBag, public System::Memory::KPortableMemoryPool<KBagNormal,1024>
	{
	public:
		KBagNormal();
		virtual ~KBagNormal();
		KBagNormal(const KBagNormal& Bag);
		KBagNormal& operator=(const KBagNormal& Bag);

		virtual void Reset();
		virtual bool Release();
	public:
	/*	BeginDefLuaClass(KBagNormal)
			InheritFrom(KBag)
		EndDef*/
	};

	// 任务背包
	class KBagQuest : public KBag, public System::Memory::KPortableMemoryPool<KBagQuest,1024>
	{
	public:
		KBagQuest();
		virtual ~KBagQuest();
		KBagQuest(const KBagQuest& Bag);
		KBagQuest& operator=(const KBagQuest& Bag);

		virtual void Reset();
		virtual bool Release();

	public:
		BeginDefLuaClass(KBagQuest)
			InheritFrom(KBag)
		EndDef
	};

	// 收件箱附件
	class KBagReaderAttach : public KBag, public System::Memory::KPortableMemoryPool<KBagReaderAttach,1024>
	{
	public:
		KBagReaderAttach();
		virtual ~KBagReaderAttach();
		KBagReaderAttach(const KBagReaderAttach& Bag);
		KBagReaderAttach& operator=(const KBagReaderAttach& Bag);

		virtual void Reset();
		virtual bool Release();

	public:
		//BeginDefLuaClass(KBagReaderAttach)
		//	InheritFrom(KBag)
		//EndDef
	};

}