/********************************************************************
created:	2012/9/19
manager:	Zhang Liuxi
decr:		
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once

#include "Inc/KCommonStruct.h"
#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/DynArray.h"
#include "System/Collections/KMapByVector.h"
#include "System/Collections/KVector.h"
#include "CommonLogic/StaticTable/IStaticTabFile.h"
#include "Inc/Lua/KLuaWrap.h"
#include "CommonLogic/Store/KStoreProduct.h"

#define DEF_STORE_PRODUCT_TAB_FILENAME "item/StateStores.txt"
#define DEF_STORE_PRODUCT	(KStoreAbout::KStoreCreateInfoManager::Instance())

namespace KStoreAbout
{
	typedef System::Collections::KMapByVector< int, KStoreProduct* > MAP_STORE_PRODUCT;	// 商品索引 -> 商品

	/**
	* 商店类
	*/
	class KStoreCreateInfo : public System::Memory::KPortableStepPool<KStoreCreateInfo, 36>
	{
	public:
		KStoreCreateInfo();
		virtual ~KStoreCreateInfo();
		bool Initialize();

		KStoreProduct* GetStoreProduct(int nProductIndex);
		const KStoreProduct* GetStoreProduct(int nProductIndex) const;
		bool Insert(int nProductIndex, KStoreProduct* pStoreProduct);
		int m_nStoreID;		// 商店ID
		MAP_STORE_PRODUCT m_mapStoreProduct;
	public:
		BeginDefLuaClass(KStoreCreateInfo)
			DefMemberVar(m_nStoreID)
		EndDef
	};
	typedef System::Collections::KMapByVector< int, KStoreCreateInfo* > MAP_STORE_CREATE;	// 商店id->商店列表

	/**
	* 商店管理类
	*/
	class KStoreCreateInfoManager : public StaticTabFileAbout::IStaticTabFile, public System::Patterns::KSingleton<KStoreCreateInfoManager>
	{
	public:
		KStoreCreateInfoManager();
		virtual ~KStoreCreateInfoManager();
		bool Initialize();
		/**
		* 加载
		*/
		virtual bool Reload(const char* szFilePath);
		/**
		* 根据商店ID获取商店信息
		*/
		KStoreCreateInfo* GetStoreCreateInfo(int nStore);
		const KStoreCreateInfo* GetStoreCreateInfo(int nStore) const;

	public:
		MAP_STORE_CREATE mMapStoreCreateInfo;

	public:
	/*	BeginDefLuaClass(KStoreCreateInfoManager)
			DefMemberFunc(Initialize);
		EndDef*/
	};
};