/********************************************************************
created:	2012/9/20
manager:	Zhang Liuxi
decr:		
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once


#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/ObjectFactory/KObjectFactory.h"
#include <System/Memory/KStepObjectPool.h>
#include "System/Patterns/KSingleton.h"
#include "CommonLogic/Store/KStoreDef.h"
#include "CommonLogic/Store/KStoreProduct.h"
#include "System/Collections/KVector.h"

namespace KWorldObjAbout
{
	class KCharacter;
}

namespace KItemAbout
{
	class KItem;
	struct KCreateInfo_ItemBase;
}

namespace KStoreAbout
{
	class KClientStoreProduct : public System::Memory::KPortableStepPool<KClientStoreProduct, 36>
	{
	public:
		KClientStoreProduct();
		~KClientStoreProduct();
		void Initialize();
		bool Unserilize(const BYTE* pBuffer, int nBuffLen);

		int GetNumOfMoney() const;
		int GetNumOfBluePower() const;
		DWORD GetNumOfItem(int& nNum) const;
		int GetMoneyType() const;

	public:
		int m_nID;									// ��Ʒ����
		int m_nStoreID;								// �̵�ID
		int m_nLabelID;								// ��ǩID
		DWORD m_dwItemID;							// ��ƷID
		int m_nPileCount;							// һ������ٸ�
		KStoreProductPrice m_Prices[PRICE_TYPE_MAX];// �۸��б�
		int m_nLimitSaleCount;						// һ�������ĸ���
		bool m_bCanBuy;								// �ܲ�����
		bool m_bCanSee;								// ��Ʒ�Ƿ񿴼�
	};
	typedef System::Collections::KMapByVector< int, KClientStoreProduct* > MAP_CLIENTSTORE_PRODUCT;	// ��Ʒindex -> ��Ʒ
	typedef System::Collections::KVector< KItemAbout::KItem* > VEC_CLIENTSTORE_ITEM;


	class KStoreManager : public System::Patterns::KSingleton<KStoreManager>
	{
	public:
		KStoreManager();
		~KStoreManager();

		void Reset();

		bool InitStore();
		/**
		** ���̵�
		**/
		bool EnterStoreRes(int nStoreID, UINT64 nSellerID);
		/**
		** �뿪�̵�
		**/
		bool LeaveStoreRes();
		/**
		** �����뿪�̵�
		**/
		bool LeaveStore();
		/**
		** �̵������һ����Ʒ
		**/
		bool AddOneProduct(const BYTE* pBuffer, int nBufferLen);
		/**
		** �̵������һ���ع���Ʒ
		**/
		bool AddBuyBackOneItem(const BYTE* pBuffer, int nBufferLen);
		/**
		** ������һ����Ʒ
		**/
		bool TryBuy(int nProductIndex, int nBuyCount);
		/**
		** ������һ����Ʒ
		**/
		bool TrySell(int nBoxID, int nPos);
		bool TryBuyBack(int nPos);
		bool BuyBackRes(const BYTE* pBuffer, int nBufferLen);
		// TODO: ��δʹ��
		bool ProductChangeState();

		KItemAbout::KItem* GetBuyBackItem(int nIdx);
		const KItemAbout::KItem* GetBuyBackItem(int nIdx) const;
		bool IsOpening() const;

		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo(int nProductID) const;
		/**
		 * ���������������ÿ�
		 */
		void PopCommBuyDialog(int nFromBoxID, int nFromPos, bool bOne);

	private:
		void _ClearClientStoreMap();
		
	private:
		int m_nStoreID;										// ��ǰ�򿪵��̵�
		int m_nLabelID;										// ��ǰ������ʾ��label
		MAP_CLIENTSTORE_PRODUCT m_mapClientStoreProduct;	// �̵���Ʒ�б�
		VEC_CLIENTSTORE_ITEM m_vecItem;						// �ع���Ʒ�б�
		KWorldObjAbout::KCharacter* m_pSeller;				// ����ָ��

	public:
		BeginDefLuaClass(KStoreManager)
		EndDef
	};

}

