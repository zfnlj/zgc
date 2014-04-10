#include "KStoreCreate.h"
#include "Inc/KCommonDefine.h"
#include "System/Log/log.h"
#include "System/File/KTabfileLoader.h"
#include "CommonLogic/StaticTable/KStaticTabFileManager.h"
#include "CommonLogic/KGlobalFunction.h"
#include "CommonLogic/WorldObject/ICharacter.h"
#include "CommonLogic/Item/KItemDef.h"
#include <string>

#ifdef _USE_COCOS2DX
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#endif

using namespace KWorldObjAbout;
using namespace System::Collections;
using namespace System::File;

namespace KStoreAbout
{
	KStoreCreateInfo::KStoreCreateInfo()
	{
	}

	KStoreCreateInfo::~KStoreCreateInfo()
	{
		MAP_STORE_PRODUCT::iterator itorTmp = m_mapStoreProduct.begin();
		for(;itorTmp!=m_mapStoreProduct.end();++itorTmp)
		{
			KStoreProduct* pStoreProduct = itorTmp->second;
			if(pStoreProduct)
			{
				KStoreProduct::Free(pStoreProduct);
			}
		}
		m_mapStoreProduct.clear();
	}

	bool KStoreCreateInfo::Initialize()
	{
		m_nStoreID = 0;

		return true;
	}

	KStoreProduct* KStoreCreateInfo::GetStoreProduct( int nProductIndex )
	{
		MAP_STORE_PRODUCT::iterator itorFind = m_mapStoreProduct.find(nProductIndex);
		return m_mapStoreProduct.end() == itorFind ? NULL : itorFind->second;
	}

	const KStoreProduct* KStoreCreateInfo::GetStoreProduct( int nProductIndex ) const
	{
		MAP_STORE_PRODUCT::const_iterator itorFind = m_mapStoreProduct.find(nProductIndex);
		return m_mapStoreProduct.end() == itorFind ? NULL : itorFind->second;
	}

	bool KStoreCreateInfo::Insert( int nProductIndex, KStoreProduct* pStoreProduct )
	{
		ASSERT_RETURN(nProductIndex > 0 && pStoreProduct, false);
		MAP_STORE_PRODUCT::iterator itorFind = m_mapStoreProduct.find(nProductIndex);
		if(m_mapStoreProduct.end() == itorFind)
		{
			m_mapStoreProduct[nProductIndex] = pStoreProduct;
			return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	KStoreCreateInfoManager::KStoreCreateInfoManager(void)
	{
		mMapStoreCreateInfo.clear();
	}

	KStoreCreateInfoManager::~KStoreCreateInfoManager(void)
	{
		mMapStoreCreateInfo.clear();
	}

	bool KStoreCreateInfoManager::Initialize()
	{
		std::string pathKey = STORE_TAB_FILENAME;

		std::string fullPath;
#ifdef _USE_COCOS2DX
		fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
		fullPath = pathKey;
#endif

		return DEF_REGISTER_STATIC_TABFILE_RELOAD(fullPath.c_str(), this);
	}
	
	bool KStoreCreateInfoManager::Reload(const char* szFilePath)
	{
		ASSERT_RETURN(szFilePath, false);
		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KTabFile2* tabFile = loader.GetFileReader(szFilePath);
		if( NULL == tabFile )
		{
			AssertFile(szFilePath);
			return false;
		}

		int nRet = -1;
		int lnValue = 0;
		KStoreCreateInfo* pStoreCreateInfo = NULL;
		char szValue[200] = "";
		while(true)
		{
			nRet = tabFile->ReadLine();
			if(nRet == -1)
			{
				loader.CloseFileReader(tabFile);
				AssertFile(szFilePath);
				return false;
			}
			if(nRet == 0) break;

			tabFile->GetInteger("StoreID", 0, &lnValue);
			pStoreCreateInfo = GetStoreCreateInfo(lnValue);
			if(NULL == pStoreCreateInfo)
			{
				pStoreCreateInfo = KStoreCreateInfo::Alloc();
				pStoreCreateInfo->Initialize();
				pStoreCreateInfo->m_nStoreID = lnValue;
				mMapStoreCreateInfo.insert_unique(lnValue, pStoreCreateInfo);
				pStoreCreateInfo = GetStoreCreateInfo(lnValue);
				ASSERT_I(pStoreCreateInfo);
			}

			tabFile->GetInteger("ID", 0, &lnValue);
			KStoreProduct* pStoreProduct = pStoreCreateInfo->GetStoreProduct(lnValue);
			if(NULL == pStoreProduct)
			{
				pStoreProduct = KStoreProduct::Alloc();
				pStoreProduct->Initialize();
				pStoreProduct->m_nID = lnValue;
				pStoreProduct->m_nStoreID = pStoreCreateInfo->m_nStoreID;
				pStoreCreateInfo->Insert(lnValue, pStoreProduct);
				pStoreProduct = pStoreCreateInfo->GetStoreProduct(lnValue);
				ASSERT_I(pStoreProduct);
			}

			tabFile->GetInteger("LabelID", 0, &lnValue);
			pStoreProduct->m_nLabelID = lnValue;

			tabFile->GetInteger("ItemID", 0, &lnValue);
			pStoreProduct->m_dwItemID = lnValue;

			tabFile->GetInteger("PileCount", 0, &lnValue);
			pStoreProduct->m_nPileCount = lnValue;

			int nPriceType, TParam1, TParam2;
			tabFile->GetInteger( "PriceType1", 0, &nPriceType);
			tabFile->GetInteger("T1Param1", 0, &TParam1);
			tabFile->GetInteger("T1Param2", 0, &TParam2);
			pStoreProduct->AddProductPrice(nPriceType, TParam1, TParam2);

			tabFile->GetInteger( "PriceType2", 0, &nPriceType);
			tabFile->GetInteger("T2Param1", 0, &TParam1);
			tabFile->GetInteger("T2Param2", 0, &TParam2);
			pStoreProduct->AddProductPrice(nPriceType, TParam1, TParam2);

			tabFile->GetInteger("LimitSaleCount", 0, &lnValue);
			pStoreProduct->m_nLimitSaleCount = lnValue;

			tabFile->GetString("CanBuyScript", "", szValue, sizeof(szValue));
			pStoreProduct->m_strCanBuyScript.clear();
			if(strcmp(szValue, "0"))
			{
				pStoreProduct->m_strCanBuyScript.append(szValue);
			}

			tabFile->GetString("CanSeeScript", "", szValue, sizeof(szValue));
			pStoreProduct->m_strCanSeeScript.clear();
			if(strcmp(szValue, "0"))
			{
				pStoreProduct->m_strCanSeeScript.append(szValue);
			}
		}
		loader.CloseFileReader(tabFile);
		return true;
	}

	KStoreCreateInfo* KStoreCreateInfoManager::GetStoreCreateInfo(int nStore)
	{
		MAP_STORE_CREATE::iterator itorFind = mMapStoreCreateInfo.find(nStore);
		return mMapStoreCreateInfo.end() == itorFind ? NULL : itorFind->second;
	}

	const KStoreCreateInfo* KStoreCreateInfoManager::GetStoreCreateInfo( int nStore ) const
	{
		MAP_STORE_CREATE::const_iterator itorFind = mMapStoreCreateInfo.find(nStore);
		return mMapStoreCreateInfo.end() == itorFind ? NULL : itorFind->second;
	}

};