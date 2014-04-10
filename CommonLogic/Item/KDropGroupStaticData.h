/********************************************************************
created:	2012/3/23
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once

#include "System/Patterns/KSingleton.h"
#include "System/Memory/KStepObjectPool.h"
#include "System/Collections/DynComplexArray.h"
#include "System/Collections/KMapByVector.h"
#include "../StaticTable/IStaticTabFile.h"

#define DEF_ITEM_DROPGROUP_TAB_FILENAME "item/DropGroup.txt"
#define DEF_ITEM_DROPGROUP	(KItemAbout::KDropGroupStaticDataManager::Instance())

namespace KItemAbout
{
	class KDropGroupStaticData
		: public System::Memory::KPortableMemoryPool<KDropGroupStaticData>
	{
	public:
		KDropGroupStaticData();
		KDropGroupStaticData(const KDropGroupStaticData& dropData);
		KDropGroupStaticData& operator = (const KDropGroupStaticData& dropData);
		virtual ~KDropGroupStaticData();

		void Initialize();

	public:
		int m_nId;				// 类型id
		int m_Cnt;				// 随机次数
		/**
		 * 掉落物列表信息
		 */
		struct KDropItemInfo
		{
			DWORD m_dwItemID;
			int m_nItemWeight;
		};
		typedef System::Collections::DynComplexArray<KDropItemInfo> KDropItemInfoList;
		KDropItemInfoList m_DropItemList;

		
		/** 按加权平均的方式抽取一个掉落物
		**/
		DWORD RandOneDropItem() const;

	public:
		//BeginDefLuaClass(KDropGroupStaticData)
		//	DefMemberVar(m_nId)
		//	DefMemberVar(m_Cnt)
		//EndDef
	};

	class KDropGroupStaticDataManager
		: public System::Patterns::KSingleton<KDropGroupStaticDataManager>
		, public StaticTabFileAbout::IStaticTabFile
	{
	public:
		typedef System::Collections::KMapByVector< int, KDropGroupStaticData* > KDropGroupMap;

	public:
		KDropGroupStaticDataManager();
		~KDropGroupStaticDataManager();

		bool Initialize();
		/**
		* 加载文件
		*/
		virtual bool Reload(const char* szFilePath);

		KDropGroupStaticData* GetDropGroupStaticData(int nID);
		DWORD RandItem(int nID);

	public:
		/**
		* 保存所有掉落物组
		*/
		KDropGroupMap mDropGroupMap;

	public:
	/*	BeginDefLuaClass(KDropGroupStaticDataManager)
			DefMemberFunc(GetDropGroupStaticData)
		EndDef*/

	};
}