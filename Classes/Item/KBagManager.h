/********************************************************************
created:	2012/02/27
manager:	
copyright:	Aurora Interactive, 2011

*********************************************************************/


#pragma once

//#include "System/Patterns/KSingleton.h"
#include "System/Memory/KStepObjectPool.h"
#include "KBag.h"
#include "Inc/Lua/KLuaWrap.h"
#include "Inc/KStruct_ClientItem.h"
#include "Inc/PacketGate/c_game_packet.h"
namespace KItemAbout
{
	// ����������
		struct KItemListInfo
	{
		struct KOpItemInfo
		{
			DWORD dwItemID;		// ��ƷID
			int nNum;			// ��Ʒ����
		};

		KItemListInfo() { Clear(); }
		~KItemListInfo() { Clear(); }

		inline void Clear()
		{
			m_ItemListInfo.clear();
		}

		inline bool IsEmpty() const { return m_ItemListInfo.size() > 0 ? false : true; }

		inline void Add(DWORD dwItemID, int nNum)
		{
			ASSERT_RETURN_VOID(dwItemID > 0 && nNum > 0);
			KOpItemInfo opItem;
			opItem.dwItemID = dwItemID;
			opItem.nNum = nNum;

			m_ItemListInfo.push_back(opItem);
		}
		System::Collections::DynArray<KOpItemInfo> m_ItemListInfo;
	};

	class KBagManager : public System::Memory::KPortableMemoryPool<KBagManager,1024>
	{
	public:

		class GemLevel_Cmp
		{
		public:
			bool operator () (int n1, int n2) const
			{
				if(n1 > n2)
					return false;
				else
					return true;
			}
		};
		typedef System::Collections::DynSortedArray<int, GemLevel_Cmp, 20, 20> KGemLevelSortArray;

	public:
		KBagManager();
		~KBagManager();

		void Reset();
		/**
		** ����һ���հ���������Ʒһ��һ����ӣ����ڴ˵�����
		**/
		void InitOneBag(const void* pData, int nLen);
		/**
		** ����һ��װ������������Ʒһ��һ����ӣ����ڴ˵�����
		**/
		/**
		** ����һ����ͨ����������Ʒһ��һ����ӣ����ڴ˵�����
		**/
		void InitOneBagNormal(KMsgInputStream& si);
		/**
		** ����һ���ռ��丽������������Ʒһ��һ����ӣ����ڴ˵�����
		**/
		void InitOneReaderAttach();
	public:
		/**
		** ͨ����ID���Ұ�
		**/
		KBag* FindBag(int nBagID);
		const KBag* FindBag(int nBagID) const;

		/**
		** ������ͨ����
		**/
		KBagNormal* FindNormalBag();
		const KBagNormal* FindNormalBag() const;
		/**
		** �������񱳰�
		**/
		KBagQuest* FindQuestBag();
		const KBagQuest* FindQuestBag() const;
		KBag* FindBaglua(int nBagID);
	public:
		/**
		** ɾ��һ����
		**/
		void DelOneBag(int nBagID);

		bool AddOneCell( const SC_BagAddResponse* pBAR );
		/**
		** �������������
		**/
		int ItemOperator(const SC_BagOperation* pBO);

		/**
		** ������ ���� ���¸�������Ʒ�����Ϣ
		**/
		bool UpdateOneCell(const SC_BagUpdateResponse* pBUR);

		/**
		** ������ID
		**/
		void ExchangeBagID(int nNewBagID, int nDelBagID);
		/**
		** �����
		**/
		bool TrySortoutBag(int nBagID);

		bool CanCostItemInAllBag(DWORD dwItemID, int nItemNumber) const;
		bool CanCostOneBag(int nBag, DWORD dwItemID, int nItemNumber) const;
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo(DWORD dwItemID) const;
		const KItemAbout::KCreateInfo_ItemBase* GetOneItemCreateInfoBase(int nBagID, DWORD dwItemID);
		/**
		** �õ�ĳ��������Ʒ������
		**/
		int GetCount(int nBagID, int nPos) const;
		KCellBag* FindCell(int nBagID, int nPos);
		const KCellBag* FindCell(int nBagID, int nPos) const;
		bool IsEmpty(int nBagID, int nPos) const;


	private:
// 		bool _ChangLockStateAllGemItemInBag(bool bLock);

	public:
		/**
		** ����ָ��λ���Ƿ��ѽ���
		**/
		bool IsCellUnLock(int nBoxId, int nPos) const;
		/**
		** ����ָ��λ���Ƿ�����Ʒ
		**/
		bool HaveItem(int nBoxId, int nPos) const;
		/**
		** ����λ�ã�û���򷵻�-1
		**/
		int GetItemPosition(int nBoxId, DWORD dwItemID) const;
		/**
		** �õ�ĳ��BoxID��λ�õ���Ʒ�Ƿ��ǰ�ʹ�ô���
		**/
		bool IsFrequency(int nBoxId, int nPos) const;
		/**
		** �õ�ĳ��BoxID��λ�õ���Ʒ����
		**/
		int GetItemNum(int nBoxId, int nPos) const;
		/**
		** �õ�ĳ��BoxID��λ�õ�ʹ�ô���
		**/
		int GetFrequency(int nBoxId, int nPos) const;
		/**
		** �õ�ĳ��BoxID��λ�õ���ƷICON
		**/
		bool GetIcon(int nBoxId, int nPos, void* buffer, int size) const;
		bool GetIcon(int nPos, void* buffer, int size) const;
		/**
		** BoxID�ϵ�ָ��λ���Ƿ�Ϸ�
		**/
		bool IsValidCell(int nBoxId, int nPos) const;

		/**
		** �õ�ĳ����ƷID����ұ���(����������)�е�����
		**/
		int GetItemCountByID(DWORD dwItemID) const;
		/**
		** �õ�ĳ����ƷID��ָ�������е�����
		**/
		int GetItemCountByID(int nBagID, DWORD dwItemID) const;
		int StatBagDisplay(int nBagID, VEC_ITEM_DISPLAY& vecItemDisplay) const;
		int StatBagDisplayEquip(VEC_ITEM_DISPLAY& vecItemDisplay) const;
		/**
		** ��ð�ID��Ӧ��BoxID
		**/
		int BagId2BoxID(int nBagID) const;
		/**
		** ���BoxID��Ӧ�İ�ID
		**/
		int BoxID2BagID(int nBoxID) const;

		bool UpdateCellLock(const SC_BagCellUpdateLock* pBCUL);
		
		int GetFirstPos(int nBagID, DWORD dwItemID) const;

		KItem* GetItem(int nBagID, int nPos);
		const KItem* GetItem(int nBagID, int nPos) const;
		DWORD GetItemID(int nBagID, int nPos) const;

		int GetNotEmptyCellSize(int nBagID) const;
		void SetMasterID(UINT64 nMasterID);
		int ItemOperatorErr(const SC_BagOperationErr* pBOE);

		void TryToUse(KBag* bag,int nPos,UINT64 playerId);
		/**
		** �Ƿ�ɲ��
		** return: 0 �ܲ� 1 һ����� 2 ��Ʒ������� 3 ��������
		**/
		int IsSeparable(int nBoxId, int nPos);
		/**
		** ����/����δ������
		**/
		void ChangeItemState(int nBoxId, int nPos, bool bLock);

		/**
		** �������Ƿ��и�����Ʒ
		**/
		bool IsInSitu() const;


		/**
		** �������Ƿ�����Ʒ
		**/
		bool BagIsEmpty(int nBagID) const;
	
		/**
	

		** ͨ����Ʒ�����͵õ����ı�������
		**/
		int GetBagTypeByItemType(BYTE nItemType) const;
	

		bool BagIsFull( int nBagID ) const;
		/**
		** ���ұ����е�һ��Ϊ�յĸ��ӱ��
		**/
		int FindFirstEmptyCell(int nBagID) const;

	private:
		/**
		** ���һ�����������б�
		**/
		bool _AddBagList(KBag* pAddBag);
		void _UseItemErrResponse(const SC_BagUseErrResponse* pBUER);

		void _DelAllBag();
		bool _FindLevel(int nLevel, KGemLevelSortArray& SortLevel) const;

	private:
		typedef System::Collections::KMapByVector<int, KBag*> BagList;
		BagList m_listBag;
		
		UINT64 m_nMasterID;
	public:
		static int GenSyncDataUpdateCell(char* szTmp,int nBagID, int nPos, enumItemDbStruct enumFlag, BYTE* pBuffer, int nBufferLen, int eOptType);
	public:
		//BeginDefLuaClassNoCon(KBagManager)
		//	DefMemberFunc(TrySortoutBag)
		//EndDef
	};
}