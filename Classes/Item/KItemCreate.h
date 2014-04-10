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
		E_ITEM_FILE_SERVER = 0,	//�������˵������ļ�
		E_ITEM_FILE_CLIENT,		//�ͻ��˵������ļ�
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
	typedef System::Collections::KMapByVector<DWORD, KCreateInfo_Item> MAP_CREATE_ITEM;					// ��ͨ��Ʒ

	class KItemCreate : public System::Patterns::KSingleton<KItemCreate>
	{
		friend class KItemDrop;
		friend class KWorldDrop;
	public:
		KItemCreate(void);
		~KItemCreate(void);
		/**
		** ����ȫ��Item�����ļ���Ϣ
		**/
		bool LoadItemCreateInfo(int nType = E_ITEM_FILE_SERVER);						// ��ȡ��Ʒ����
		//bool	LoadCreateInfo_Item(MAP_CREATE_ITEM & mapCreateInfo, const char* filename);
		/**
		** ������ƷItem�����ļ���Ϣ
		**/
		bool LoadCreateInfo_Item(const char* pFileName = ITEM_TAB_FILENAME);
	
		/**
		** ��ȡ��Ʒ(��ͨ��Ʒ��װ���ȵ�)��Ϣ
		**/
		KCreateInfo_ItemBase* GetItemCreateInfo(DWORD dwItemID);
		/**
		** ��ȡ��ͨ��Ʒ��Ϣ
		**/
		KCreateInfo_Item* GetCreateInfo_Item(DWORD dwItemID);

	protected:
		int _LoadItemInfo(System::File::KTabFile2& itemTabFile, KCreateInfo_ItemBase& CIIB);
		// ��ͨ��Ʒ
		int _LoadItemInfo(System::File::KTabFile2& itemTabFile, KCreateInfo_Item& CII);
	
	
		template<class T_MAP>
		bool _LoadCreateInfo(T_MAP& mapCreateInfo, const char* pFileName);

		int  m_nItemCreateInfoCount;
		MAP_CREATE_ITEM		m_mapCreateInfoItem;		// ��ͨ��Ʒ

	public:
		BeginDefLuaClass(KItemCreate)
			DefMemberFunc(LoadItemCreateInfo);
		EndDef
	};
};
