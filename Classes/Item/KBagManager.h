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
	// 背包管理类
		struct KItemListInfo
	{
		struct KOpItemInfo
		{
			DWORD dwItemID;		// 物品ID
			int nNum;			// 物品个数
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
		** 增加一个空包，包中物品一个一个添加，不在此地生成
		**/
		void InitOneBag(const void* pData, int nLen);
		/**
		** 增加一个装备包，包中物品一个一个添加，不在此地生成
		**/
		/**
		** 增加一个普通包，包中物品一个一个添加，不在此地生成
		**/
		void InitOneBagNormal(KMsgInputStream& si);
		/**
		** 增加一个收件箱附件包，包中物品一个一个添加，不在此地生成
		**/
		void InitOneReaderAttach();
	public:
		/**
		** 通过包ID查找包
		**/
		KBag* FindBag(int nBagID);
		const KBag* FindBag(int nBagID) const;

		/**
		** 查找普通背包
		**/
		KBagNormal* FindNormalBag();
		const KBagNormal* FindNormalBag() const;
		/**
		** 查找任务背包
		**/
		KBagQuest* FindQuestBag();
		const KBagQuest* FindQuestBag() const;
		KBag* FindBaglua(int nBagID);
	public:
		/**
		** 删除一个包
		**/
		void DelOneBag(int nBagID);

		bool AddOneCell( const SC_BagAddResponse* pBAR );
		/**
		** 包操作函数入口
		**/
		int ItemOperator(const SC_BagOperation* pBO);

		/**
		** 包操作 —— 更新格子中物品相关信息
		**/
		bool UpdateOneCell(const SC_BagUpdateResponse* pBUR);

		/**
		** 调整包ID
		**/
		void ExchangeBagID(int nNewBagID, int nDelBagID);
		/**
		** 整理包
		**/
		bool TrySortoutBag(int nBagID);

		bool CanCostItemInAllBag(DWORD dwItemID, int nItemNumber) const;
		bool CanCostOneBag(int nBag, DWORD dwItemID, int nItemNumber) const;
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo(DWORD dwItemID) const;
		const KItemAbout::KCreateInfo_ItemBase* GetOneItemCreateInfoBase(int nBagID, DWORD dwItemID);
		/**
		** 得到某个格子物品的数量
		**/
		int GetCount(int nBagID, int nPos) const;
		KCellBag* FindCell(int nBagID, int nPos);
		const KCellBag* FindCell(int nBagID, int nPos) const;
		bool IsEmpty(int nBagID, int nPos) const;


	private:
// 		bool _ChangLockStateAllGemItemInBag(bool bLock);

	public:
		/**
		** 包内指定位置是否已解锁
		**/
		bool IsCellUnLock(int nBoxId, int nPos) const;
		/**
		** 包内指定位置是否有物品
		**/
		bool HaveItem(int nBoxId, int nPos) const;
		/**
		** 返回位置，没有则返回-1
		**/
		int GetItemPosition(int nBoxId, DWORD dwItemID) const;
		/**
		** 得到某个BoxID和位置的物品是否是按使用次数
		**/
		bool IsFrequency(int nBoxId, int nPos) const;
		/**
		** 得到某个BoxID和位置的物品数量
		**/
		int GetItemNum(int nBoxId, int nPos) const;
		/**
		** 得到某个BoxID和位置的使用次数
		**/
		int GetFrequency(int nBoxId, int nPos) const;
		/**
		** 得到某个BoxID和位置的物品ICON
		**/
		bool GetIcon(int nBoxId, int nPos, void* buffer, int size) const;
		bool GetIcon(int nPos, void* buffer, int size) const;
		/**
		** BoxID上的指定位置是否合法
		**/
		bool IsValidCell(int nBoxId, int nPos) const;

		/**
		** 得到某个物品ID在玩家背包(不包括工坊)中的总数
		**/
		int GetItemCountByID(DWORD dwItemID) const;
		/**
		** 得到某个物品ID在指定背包中的总数
		**/
		int GetItemCountByID(int nBagID, DWORD dwItemID) const;
		int StatBagDisplay(int nBagID, VEC_ITEM_DISPLAY& vecItemDisplay) const;
		int StatBagDisplayEquip(VEC_ITEM_DISPLAY& vecItemDisplay) const;
		/**
		** 获得包ID对应的BoxID
		**/
		int BagId2BoxID(int nBagID) const;
		/**
		** 获得BoxID对应的包ID
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
		** 是否可拆分
		** return: 0 能拆 1 一般错误 2 物品不允许拆 3 背包已满
		**/
		int IsSeparable(int nBoxId, int nPos);
		/**
		** 锁定/解锁未个格子
		**/
		void ChangeItemState(int nBoxId, int nPos, bool bLock);

		/**
		** 背包内是否有复活物品
		**/
		bool IsInSitu() const;


		/**
		** 背包内是否有物品
		**/
		bool BagIsEmpty(int nBagID) const;
	
		/**
	

		** 通过物品的类型得到它的背包类型
		**/
		int GetBagTypeByItemType(BYTE nItemType) const;
	

		bool BagIsFull( int nBagID ) const;
		/**
		** 查找背包中第一个为空的格子编号
		**/
		int FindFirstEmptyCell(int nBagID) const;

	private:
		/**
		** 添加一个背包到包列表
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