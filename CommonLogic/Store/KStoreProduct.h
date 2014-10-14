/********************************************************************
created:	2012/9/19
manager:	Zhang Liuxi
decr:		
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once

#include "Inc/Lua/KLuaWrap.h"
#include <System/Memory/KStepObjectPool.h>
#include "System/Misc/KStream.h"
#include <System/KMacro.h>
#include "../ObjectFactory/KObjectFactory.h"
#include "KStoreDef.h"
#include "System/Collections/KString.h"

namespace KWorldObjAbout
{
	class KCharacter;
}

namespace KStoreAbout
{
	/**
	** 货币类型
	**/
	enum PRICE_TYPE
	{
		enumPT_none = -1,
		enumPT_money,			// 金钱 消耗T1Param1
		enumPT_cash,            //现金
		enumPT_bluePower,		// 深蓝之力 消耗T1Param1
		enumPT_item,			// 道具 消耗ID为T1Param2的道具T1Param1个

		enumPT_count,
	};

	struct KStoreProductPrice
	{
		int nMoneyType;		// 货币类型
		int nTParam1;		// 参数1
		int nTParam2;		// 参数2

		BeginDefLuaClass(KStoreProductPrice)
			DefMemberVar(nMoneyType);
			DefMemberVar(nTParam1);
			DefMemberVar(nTParam2);
		EndDef
	};

	class KStoreProduct : public System::Memory::KPortableStepPool<KStoreProduct, 36>
	{
	public:
		KStoreProduct();
		void Initialize();
		bool AddProductPrice(int nMoneyType, int nTParam1, int nTParam2);

		bool CanSee(const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;
		bool CanBuy(const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;

		bool Serilize(KDataOutputStream& os, const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;

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
		KStoreProductPrice m_Prices[PRICE_TYPE_MAX]; // 价格列表
		int m_nLimitSaleCount;						// 一次最多买的个数
		System::Collections::KString<64> m_strCanBuyScript;				// 能不能买脚本
		System::Collections::KString<64> m_strCanSeeScript;				// 物品是否看见脚本

	public:
	/*	BeginDefLuaClass(KStoreProduct)
			DefMemberVar(m_nID);
			DefMemberVar(m_nStoreID);
			DefMemberVar(m_nLabelID);
			DefMemberVar(m_dwItemID);
			DefMemberVar(m_nPileCount);
			DefMemberVar(m_nLimitSaleCount);

			DefMemberFunc(AddProductPrice);
			DefMemberFunc(CanSee);
			DefMemberFunc(CanBuy);
			DefMemberFunc(GetMoneyType);
		EndDef*/
	};

};



