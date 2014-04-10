/********************************************************************
created:	2011/9/13
manager:	HAN Yuxin
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include	"Inc/KCommonStruct.h"
#include	"CommonLogic/Item/KStruct_Item.h"
#include	"System/File/KTabFile.h"
#include	"System/Collections/KMapByVector.h"
#include	"CommonLogic/Item/KItemDef.h"
#include	<System/File/KTabfileLoader.h>

namespace KItemAbout
{
	enum ENUM_ITEM_FILE_TYPE
	{
		E_ITEM_FILE_SERVER = 0,	//服务器端的配置文件
		E_ITEM_FILE_CLIENT,		//客户端的配置文件
	};
	struct KVipItemInfoSimple 
	{
		INT nItemID;
		INT nItemNum;
	};
	struct KVipItemInfoData
	{
		KVipItemInfoSimple m_Item[CFG_MAX_VIP_ITEM_BY_LINE];
	};

	typedef System::Collections::KMapByVector<DWORD, KVipItemInfoData> MAP_CREATE_VIP_ITEM;
	typedef System::Collections::KMapByVector<DWORD, KCreateInfo_Item> MAP_CREATE_ITEM;					// 普通物品

	class KItemCreate : public System::Patterns::KSingleton<KItemCreate>
	{
		friend class KItemDrop;
		friend class KWorldDrop;
	public:
		KItemCreate(void);
		~KItemCreate(void);
		/**
		** 加载全部Item配置文件信息
		**/
		bool LoadItemCreateInfo(int nType = E_ITEM_FILE_SERVER);						// 读取物品数据
		//bool	LoadCreateInfo_Item(MAP_CREATE_ITEM & mapCreateInfo, const char* filename);
		/**
		** 加载物品Item配置文件信息
		**/
		bool LoadCreateInfo_Item(const char* pFileName = ITEM_TAB_FILENAME);
	
		/**
		** 获取物品(普通物品、装备等等)信息
		**/
		KCreateInfo_ItemBase* GetItemCreateInfo(DWORD dwItemID);
		/**
		** 获取普通物品信息
		**/
		KCreateInfo_Item* GetCreateInfo_Item(DWORD dwItemID);

	protected:
		int _LoadItemInfo(System::File::KTabFile2& itemTabFile, KCreateInfo_ItemBase& CIIB);
		// 普通物品
		int _LoadItemInfo(System::File::KTabFile2& itemTabFile, KCreateInfo_Item& CII);
	
	
		template<class T_MAP>
		bool _LoadCreateInfo(T_MAP& mapCreateInfo, const char* pFileName);

		int  m_nItemCreateInfoCount;
		MAP_CREATE_ITEM		m_mapCreateInfoItem;		// 普通物品

	public:
		BeginDefLuaClass(KItemCreate)
			DefMemberFunc(LoadItemCreateInfo);
		EndDef
	};
};
