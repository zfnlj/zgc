#include "Inc/KCommonDefine.h"
#include "KItemCreate.h"
//#include "KItemDrop.h"
#include "System/Log/log.h"
#include "CommonLogic/StringManager/KStringManager.h"
//#include "KItemPrefix.h"
#include <System/File/KTabfileLoader.h>
//#include <KServerTabfileLoader.h>

using namespace System::File;

#if !defined(max)
#	define max(a,b) (((a) < (b)) ? (b) : (a))
#endif

#define SPLITER ":, \\/;"

namespace KItemAbout
{
	KItemCreate::KItemCreate(void)
	{
	}

	KItemCreate::~KItemCreate(void)
	{
		m_mapCreateInfoItem.clear();
	}

	bool KItemCreate::LoadItemCreateInfo(int nType)
	{
		LoadCreateInfo_Item(ITEM_TAB_FILENAME);
		return true;
	}

	bool KItemCreate::LoadCreateInfo_Item(const char* pFileName)
	{
		m_mapCreateInfoItem.clear();
		return _LoadCreateInfo<MAP_CREATE_ITEM>(m_mapCreateInfoItem, pFileName);
	}

	// 根据物品ID获得物品属性
	KCreateInfo_Item* KItemCreate::GetCreateInfo_Item(DWORD dwItemID)
	{
		if(0 == dwItemID) return NULL;
		MAP_CREATE_ITEM::iterator it = m_mapCreateInfoItem.find(dwItemID);
		if(it == m_mapCreateInfoItem.end())
			return NULL;
		return &it->second;
	}


	KCreateInfo_ItemBase* KItemCreate::GetItemCreateInfo(DWORD dwItemID)
	{
		if(0 == dwItemID) return NULL;
		// TODO:在这里添加代码的同学情注意,由于判断范围的编号规则,编号越大的必须位置越靠前
		if(KCreateInfo_ItemBase::IsUsableItemStatic(dwItemID))
		{
			return GetCreateInfo_Item(dwItemID);
		}
		return NULL;
	}

	int KItemCreate::_LoadItemInfo(KTabFile2& itemTabFile, KCreateInfo_ItemBase& CIIB)
	{
		int nParam = 0;
		itemTabFile.GetInteger("ID", 0, (int*)&nParam);
		CIIB.s_nItemID = nParam;
		if(0 == CIIB.s_nItemID)
			return -1;

		itemTabFile.GetInteger("NameID", 0, (int*)&nParam);
		CIIB.s_nNameID = nParam;
		itemTabFile.GetInteger("DesID", 0, (int*)&nParam);
		CIIB.s_nDesID = nParam;
		itemTabFile.GetInteger("Level", 0, (int*)&nParam);
		CIIB.s_nLevel = nParam;
		itemTabFile.GetInteger("Quality", 0, (int*)&nParam);
		CIIB.s_byQuality = nParam;
		itemTabFile.GetInteger("DisplayID", 0, (int*)&nParam);
		CIIB.s_nDisplayID = nParam;
		itemTabFile.GetInteger("ItemType", 0, (int*)&nParam);
		CIIB.s_byItemType = nParam;
		itemTabFile.GetInteger("Price", 0, (int*)&nParam);
		CIIB.s_nPrice = nParam;
		itemTabFile.GetInteger("Bind", 0, (int*)&nParam);
		CIIB.s_byBindType = nParam;
		itemTabFile.GetInteger("AliveType", 0, (int*)&nParam);
		CIIB.s_byAliveType = nParam;
		itemTabFile.GetInteger("AliveTime", 0, (int*)&nParam);
		CIIB.s_dwAliveTime = nParam;
		itemTabFile.GetInteger("Flags", 0, (int*)&nParam);
		CIIB.s_dwFlags = nParam;

		for(int i = 0 ; i < MAX_WORLD_DROP_COUNT; i++)
		{
			char chBuf[128] = { 0 };
			sprintf_k( chBuf, sizeof(chBuf), "WGroup%d", i + 1);
			itemTabFile.GetInteger(chBuf, 0, (int*)&nParam);
			CIIB.s_nWGroup[i] = nParam;
			sprintf_k(chBuf, sizeof(chBuf), "WProb%d", i + 1);
			itemTabFile.GetInteger(chBuf, 0, (int*)&nParam);
			CIIB.s_nWProb[i] = nParam;
		}

		itemTabFile.GetInteger("Stacks", 0, (int*)&nParam);
		CIIB.s_nMaxStacks = max(nParam, 1);

		itemTabFile.GetInteger("VanishType", 0, (int*)&nParam);
		CIIB.s_byVanishType = nParam;
		itemTabFile.GetInteger("CostType", 0, (int*)&nParam);
		CIIB.s_nCostType = nParam;

		itemTabFile.GetString("Icon", "", CIIB.s_icon, sizeof(CIIB.s_icon));

		itemTabFile.GetInteger("MaxKeep", 0, (int*)&nParam);
		CIIB.s_nMaxKeep = nParam;

		return CIIB.s_nItemID;
	}

	int KItemCreate::_LoadItemInfo(KTabFile2& itemTabFile, KCreateInfo_Item& CII)
	{
		int nBaseReturn = _LoadItemInfo(itemTabFile, *((KCreateInfo_ItemBase*)(&CII)));
		if(nBaseReturn > 0)
		{
			int nParam = 0;
			itemTabFile.GetInteger("SkillID", 0, (int*)&nParam);
			CII.s_nSkillID = nParam;
			for( int i = 0 ; i < MAX_EFFECT_PARAM_COUNT; i ++ )
			{
				char chBuf[128] = { 0 };
				sprintf_k( chBuf, sizeof(chBuf), "Effect1Param%d", i + 1);
				itemTabFile.GetInteger(chBuf, 0, (int*)&nParam);

			}
			itemTabFile.GetInteger("ConditionID", 0, (int*)&nParam);
			CII.s_nConditionID = nParam;
			itemTabFile.GetInteger("CParam1", 0, (int*)&nParam);
			CII.s_nCParam = nParam;

			itemTabFile.GetInteger("MaxDura", 0, (int*)&nParam);
			CII.s_wMaxDura = nParam;
			itemTabFile.GetInteger("InitDura", 0, (int*)&nParam);
			CII.s_wInitDura = nParam;

			itemTabFile.GetInteger("MaxUseTime", 0, (int*)&nParam);
			CII.s_byMaxUseTime = nParam;
			itemTabFile.GetInteger("InitFrequency", 0, (int*)&nParam);
			CII.s_byInitUseTime = nParam;

			itemTabFile.GetInteger("ItemWeight", 0, (int*)&nParam);
			CII.s_nItemWeight = nParam;

			itemTabFile.GetInteger("ItemMaterial",	0, (int*)&nParam);
			CII.ItemMaterial = nParam;
		}
		return nBaseReturn;
	}

	
	template<class T_MAP>
	bool KItemCreate::_LoadCreateInfo(T_MAP& mapCreateInfo, const char* pFileName)
	{
		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KTabFile2* pTF2 = loader.GetFileReader(pFileName);
		if(NULL == pTF2)
		{
			AssertFile(pFileName);
			return false;
		}
		KTabFile2& ItemTabFile = *pTF2;

		INT i = 0;
		while (true)
		{
			i++;
			int nRet = ItemTabFile.ReadLine();
			if(nRet == -1)
			{
				loader.CloseFileReader(pTF2);
				AssertFile(pFileName);
				return false;
			}
			if(nRet == 0) break;

			typename T_MAP::value_type item;
			//T_CreateInfo item;
			int nID = _LoadItemInfo(ItemTabFile, item);
			if(nID < 0)
			{
				Log(LOG_ERROR, "错误的物品ID:%s第%d行错误.", pFileName, i);
				continue;
			}

			if(mapCreateInfo.end() == mapCreateInfo.insert(nID, item))
			{
				Log(LOG_ERROR, "Item initilaze error, item id[%d] duplicate insert", nID);
				continue;
			}
		}

		loader.CloseFileReader(pTF2);
		printf("文件[%s]读取记录[%d]条。\n", pFileName, mapCreateInfo.size());
		return true;
	}

}
