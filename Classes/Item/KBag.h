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
		bool bFreePos;		// �Ƿ�����ȫ�յı�����true��ʾ�ǲ�������������������false��ʾ����ȫ��
		int nPosIndex;		// �ո�λ��
		WORD nNumFind;		// �����Ʒʱʱʣ��Ŀɵ���������������Ʒʱ�Ǳ������ڵ���Ʒ����
		KBagItemPosition()
			: bFreePos(false)
			, nPosIndex(0)
			, nNumFind(0)
		{
		}
	};
	typedef ::System::Collections::KVector<KBagItemPosition> VEC_BAG_FREE_POS;

	// ����������
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

		// �����л�
		bool Unserilize(KMsgInputStream& si);

		bool IsLock() const;
		void SetStallLockType(bool bLock);
		bool IsStallLock() const;
		void SetLock(int nLockType);

	public:
		int m_nPos;
		KItem m_Item;
		int m_nLockType;		// ��״̬

	public:
		BeginDefLuaClass(KCellBag)
			DefMemberFunc(GetPos)
		EndDef
	};

	// ��������
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
		** ��ʼ�������л�
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
		** �����Ƿ�������״̬(������δ�򿪣�����ʹ��)
		**/
		bool IsCellUnLock(int nPos) const;

		/**
		** �����һ�����ӣ���������Ʒ
		**/
		bool AddOneCell(int nPos, const KItem& item);
		bool Add(UINT itemID,int count);
		/**
		** ɾ��һ�������ϵ���Ʒ
		**/
		bool DelOneCell(int nPos);
		/**
		** ���������Ʒ
		**/
		bool Sortout(KBagManager* pBagMgr);

		int GetItemSize(DWORD dwItemID) const;
		int GetNotEmptyCellSize() const;
		/**
		** ������Ʒ���������仯
		**/
		int GetEmptyCellSize() const;

		int StatBagDisplayItem(VEC_ITEM_DISPLAY& vecItemDisplay) const;
		
		int GetFirstPos(DWORD dwItemID) const;
		/**
		** �����л�
		**/
		bool Unserilize(KMsgInputStream& si);
		int FindFirstEmptyCell() const;

		int FindBagPosByAdd(DWORD dwItemID, int nItemNum, const KItemAbout::KCreateInfo_ItemBase* pCIIB, VEC_BAG_FREE_POS& vecBagPos) const;
		// �������Ƿ�����Ʒ
		bool IsEmpty() const;
		/**
		** ������Ʒ���������仯
		**/
		virtual void NumChange(DWORD dwItemID, int nNumChange) {}
		
		bool IsFull() const;

		bool TryToUse(int nPos,UINT64);
	private:
		/**
		** �ϲ�������Ʒ
		**/
		static bool _MergeAllCell(KBagManager* pBagMgr, KBag& bag);
		static bool _SyncDataMergeItem(KBag& bag, int nFromPos, int nToPos, int nStackLimit);
		static bool _ExchangeItem(KBagManager* pBagMgr, KBag& bag);
		static bool _SyncDataExchangeItem(KBag& bag, int nFromPos, int nToPos);
		static bool _GetMinItemID(KBagManager* pBagMgr, int nBoxID, KBag& bag, int nStartPos, int& nPos);

	private:
		void _DelAllCell();

	public:
		int m_nBagID;				// ����ID(���汳������)
		int m_nCellMaxCount;		// ���������Ŀ(����δ��������)
		int m_nCellUnLockCount;		// δ����������Ŀ
		UINT64 m_nMasterID;

	public:
		// �����б�
		typedef System::Collections::KMapByVector<int, KCellBag*> CellMapVec;
		CellMapVec m_ListItem;

	public:
	/*	BeginDefLuaClass(KBag)
			DefMemberFunc(IsPosValid)
		EndDef*/
	};


	// ��ͨ����
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

	// ���񱳰�
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

	// �ռ��丽��
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