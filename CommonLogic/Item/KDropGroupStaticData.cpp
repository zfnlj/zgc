#include "KDropGroupStaticData.h"
#include "System/Misc/KStream.h"
//#include "StaticTable/KStaticTabFileManager.h"
#include "System/File/KTabFile.h"
#include "System/File/KTabfileLoader.h"
#include "../KGlobalFunction.h"
//#include "../Attribute/KAttrInitializer.h"

using namespace System::File;

#define SPLITER ":,. \\/;"
#define DROPGROUP_COUNT 10	// 掉落物组数

namespace KItemAbout
{

	KDropGroupStaticData::KDropGroupStaticData()
	{
		Initialize();
	}

	KDropGroupStaticData::KDropGroupStaticData( const KDropGroupStaticData& dropData )
	{
		if (&dropData == this)
		{
			return;
		}
		m_nId = dropData.m_nId;
		m_DropItemList = dropData.m_DropItemList;
		m_Cnt = dropData.m_Cnt;
	}
	KDropGroupStaticData& KDropGroupStaticData::operator = (const KDropGroupStaticData& dropData)
	{
		if (&dropData == this)
		{
			return *this;
		}
		m_nId = dropData.m_nId;
		m_DropItemList = dropData.m_DropItemList;
		m_Cnt = dropData.m_Cnt;
		return *this;
	}

	KDropGroupStaticData::~KDropGroupStaticData()
	{
	}

	void KDropGroupStaticData::Initialize()
	{
		m_nId = 0;
		m_Cnt = 0;
		m_DropItemList.clear();
	}

	DWORD KDropGroupStaticData::RandOneDropItem() const
	{
		System::Collections::DynArray<int> RightList;
		int nRigthCount = 0;
		int nDropItemSize = m_DropItemList.size();
		int nLoop = 0;
		for(nLoop=0;nLoop<nDropItemSize;nLoop++)
		{
			nRigthCount += m_DropItemList[nLoop].m_nItemWeight;
			RightList.push_back(nRigthCount);
		}

		int nRand = randrange(1, nRigthCount);
		for(nLoop=0;nLoop<nDropItemSize;nLoop++)
		{
			if(nRand < RightList[nLoop])
			{
				return m_DropItemList[nLoop].m_dwItemID;
			}
		}

		return 0;
	}

	/////////////////////////////////////////////////////////////
	KDropGroupStaticDataManager::KDropGroupStaticDataManager()
	{
		//
	}

	KDropGroupStaticDataManager::~KDropGroupStaticDataManager()
	{
		for(KDropGroupMap::iterator litItem = mDropGroupMap.begin(); mDropGroupMap.end() != litItem; ++litItem)
		{
			KDropGroupStaticData::Free(litItem->second);
		}
		mDropGroupMap.clear();
	}

	bool KDropGroupStaticDataManager::Initialize()
	{
		//return DEF_REGISTER_STATIC_TABFILE_RELOAD(DEF_ITEM_DROPGROUP_TAB_FILENAME, this);
		return false;
	}

	bool KDropGroupStaticDataManager::Reload( const char* szFilePath )
	{
		ASSERT_RETURN(szFilePath, false);
		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KTabFile2* tabFile = loader.GetFileReader(szFilePath);
		if( NULL == tabFile )
		{
			AssertFile(szFilePath);
			return false;
		}

		INT nRet = -1;
		INT lnValue = 0;
		KDropGroupStaticData* lpInfo = NULL;
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

			tabFile->GetInteger("ID", 0, &lnValue);
			lpInfo = GetDropGroupStaticData(lnValue);
			if( NULL == lpInfo )
			{
				lpInfo = KDropGroupStaticData::Alloc();
				lpInfo->Initialize();
				lpInfo->m_nId = lnValue;
				mDropGroupMap.insert_unique(lnValue, lpInfo);
				lpInfo = GetDropGroupStaticData(lnValue);
			}
			// 塞入随机次数
			int cnt = 0;
			tabFile->GetInteger("Cnt", 0, &cnt);
			lpInfo->m_Cnt = cnt;
			
			for (int i = 1; i <= DROPGROUP_COUNT; i++)
			{
				char colName[20];
				sprintf_k(colName, sizeof(colName), "ItemID%d", i);
				int nItemID = 0;
				tabFile->GetInteger(colName, 0, &nItemID);
				sprintf_k(colName, sizeof(colName), "ItemWeight%d", i);
				int nItemWeight = 0;
				tabFile->GetInteger(colName, 0, &nItemWeight);
				if(nItemID > 0 && nItemWeight > 0)
				{
					KDropGroupStaticData::KDropItemInfo dropItemInfo = {(DWORD)nItemID, nItemWeight};
					lpInfo->m_DropItemList.push_back(dropItemInfo);
				}
			}
			// 塞入miss权重
			int missright = 0;
			tabFile->GetInteger("MissRight", 0, &missright);
			if (missright > 0)
			{
				KDropGroupStaticData::KDropItemInfo missItemInfo = {0, missright};
				lpInfo->m_DropItemList.push_back(missItemInfo);
			}
		}
		loader.CloseFileReader(tabFile);
		return true;
	}

	KDropGroupStaticData* KDropGroupStaticDataManager::GetDropGroupStaticData( int nID )
	{
		KDropGroupMap::iterator litItem = mDropGroupMap.find(nID);
		return mDropGroupMap.end() == litItem ? NULL : litItem->second;
	}

	DWORD KDropGroupStaticDataManager::RandItem( int nID )
	{
		KDropGroupStaticData* pDropGroupStaticData = GetDropGroupStaticData(nID);
		if(NULL == pDropGroupStaticData)
		{
			return 0;
		}
		return pDropGroupStaticData->RandOneDropItem();
	}

}
