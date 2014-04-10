/********************************************************************
created:	2012/09/03
manager:	
copyright:	Aurora Interactive, 2011

*********************************************************************/


#pragma once

#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"
#include "System/Memory/KStepObjectPool.h"
//#include "PacketGate/KGCPacket.h"
#include "Inc/KBusinessDefine.h"
#include "Inc/PacketGate/c_game_packet.h"

#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/KMoney.h"

namespace KItemAbout
{
	class KItem;
	class KCellBag;
}

namespace KWorldObjAbout
{
	class KPlayer;
}

namespace KBusinessAbout
{
	class KBusinessManager : public System::Patterns::KSingleton< KBusinessManager >
	{
	public:
		typedef System::Collections::KMapByVector<int, KItemAbout::KItem*> BusinessCellMapVec;

	public:
		KBusinessManager();
		~KBusinessManager();
		
		void Reset();

		void BusinessRequest(const SC_BusinessRequest* pBR);
		void BusinessConfirm(const SC_BusinessConfirm* pBC);
		void BusinessPutOneItem(const SC_BusinessPutOneItem* pBPOI);
		void BusinessPutMoney(UINT64 nID, const char* pMoney);
		bool BusinessShowOneItem(int nBusinessPos, int nOperaFlag, KItemAbout::KItem* pItem);
		void BusinessCancel(const SC_BusinessCancel* pBC);
		void BusinessLock(const SC_BusinessLock* pBL);
		void BusinessStart(const SC_BusinessStart* pBS);
		void BusinessFinish(const SC_BusinessFinish* pBF);
		/**
		** 请求交易
		**/
		void TryBusinessRequest(UINT64 nBusinessID);
		/**
		** 同意或者拒绝交易
		**/
		void TryBusinessConfirm(const BusinessKey& key, bool bConfirm);
		/**
		** 移动物品到交易框或者删除交易框内物品
		**/
		void TryBusinessPutOneItem(int nBoxID, int nBagPos, int nBusinessPos, int nOperaFlag);
		/**
		** 锁定交易状态或者解除锁定交易状态
		**/
		void TryBusinessLockStateChange(bool bLockState);
		/**
		** 交易开始
		**/
		void TryBusinessStart();
		/**
		** 交易取消
		**/
		void TryBusinessCancel();

		void StartBusiness(const BusinessKey& key);
		const BusinessKey& GetBusinessKey() const;
		KWorldObjAbout::KPlayer* GetBusinessPlayer();

		KItemAbout::KItem* FindBusinessCell(int nPos);

		void CloseBusiness();

		bool IsBusiness() const;
		void MoveItemToBusiness(int nBoxID, int nPos);
		void MoveItemToBag(int nBoxID, int nBagPos, int nFromPos);

		bool CanBusiness() const;
		/**
		** 设置交易金钱数量
		**/
		void TryBusinessPutMoney(const char* pMoney);

	private:
		void _ClearOtherAllCell();
		bool _CheckRequestBusinessState();

	private:
		bool m_bMyLockState;
		bool m_bOtherLockState;
		BusinessKey m_key;
		BusinessCellMapVec m_ListOtherItem;
		KMoneyAbout::KMoney m_moneyMyPutMoney;
		
	public:
	/*	BeginDefLuaClass(KBusinessManager)
			DefMemberFunc(TryBusinessPutMoney)
		EndDef*/
	};


}

