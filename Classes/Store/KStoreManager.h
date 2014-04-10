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
		int m_nID;									// 商品索引
		int m_nStoreID;								// 商店ID
		int m_nLabelID;								// 标签ID
		DWORD m_dwItemID;							// 物品ID
		int m_nPileCount;							// 一次买多少个
		KStoreProductPrice m_Prices[PRICE_TYPE_MAX];// 价格列表
		int m_nLimitSaleCount;						// 一次最多买的个数
		bool m_bCanBuy;								// 能不能买
		bool m_bCanSee;								// 物品是否看见
	};
	typedef System::Collections::KMapByVector< int, KClientStoreProduct* > MAP_CLIENTSTORE_PRODUCT;	// 商品index -> 商品
	typedef System::Collections::KVector< KItemAbout::KItem* > VEC_CLIENTSTORE_ITEM;


	class KStoreManager : public System::Patterns::KSingleton<KStoreManager>
	{
	public:
		KStoreManager();
		~KStoreManager();

		void Reset();

		bool InitStore();
		/**
		** 打开商店
		**/
		bool EnterStoreRes(int nStoreID, UINT64 nSellerID);
		/**
		** 离开商店
		**/
		bool LeaveStoreRes();
		/**
		** 请求离开商店
		**/
		bool LeaveStore();
		/**
		** 商店中添加一个物品
		**/
		bool AddOneProduct(const BYTE* pBuffer, int nBufferLen);
		/**
		** 商店中添加一个回购物品
		**/
		bool AddBuyBackOneItem(const BYTE* pBuffer, int nBufferLen);
		/**
		** 请求购买一个物品
		**/
		bool TryBuy(int nProductIndex, int nBuyCount);
		/**
		** 请求卖一个物品
		**/
		bool TrySell(int nBoxID, int nPos);
		bool TryBuyBack(int nPos);
		bool BuyBackRes(const BYTE* pBuffer, int nBufferLen);
		// TODO: 还未使用
		bool ProductChangeState();

		KItemAbout::KItem* GetBuyBackItem(int nIdx);
		const KItemAbout::KItem* GetBuyBackItem(int nIdx) const;
		bool IsOpening() const;

		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo(int nProductID) const;
		/**
		 * 弹出批量购买设置框
		 */
		void PopCommBuyDialog(int nFromBoxID, int nFromPos, bool bOne);

	private:
		void _ClearClientStoreMap();
		
	private:
		int m_nStoreID;										// 当前打开的商店
		int m_nLabelID;										// 当前正在显示的label
		MAP_CLIENTSTORE_PRODUCT m_mapClientStoreProduct;	// 商店商品列表
		VEC_CLIENTSTORE_ITEM m_vecItem;						// 回购物品列表
		KWorldObjAbout::KCharacter* m_pSeller;				// 卖方指针

	public:
		BeginDefLuaClass(KStoreManager)
		EndDef
	};

}

