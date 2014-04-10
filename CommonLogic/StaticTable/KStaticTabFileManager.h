/********************************************************************
created:	2011/2/28
filename: 	CommonLogic\StaticTable\KStaticTabFileManager.h
file path:	CommonLogic\StaticTable
file base:	KStaticTabFileManager
file ext:	h
author:		zaki
copyright:	AuroraGame

purpose:	
*********************************************************************/
#pragma once

#include <System/Patterns/KSingleton.h>
#include <System/Collections/KMapByVector.h>
#include <System/Memory/KStepObjectPool.h>
#include "IStaticTabFile.h"
#include "Inc/Lua/KLuaWrap.h"

#define DEF_DEFAULT_RELOAD_INTERVAL	(300)
#define DEF_REGISTER_STATIC_TABFILE_RELOAD(filepath, loadPtr)	(StaticTabFileAbout::KStaticTabFileManager::Instance()->Register((filepath), (loadPtr)))
#define DEF_STATIC_TABFILE_BREATHE(nInterval)	(StaticTabFileAbout::KStaticTabFileManager::Instance()->Breathe(nInterval))

namespace StaticTabFileAbout
{
	class KStaticTabFileManager : public ::System::Patterns::KSingleton<KStaticTabFileManager>
	{
	public:
		struct KStaticTabFileInfo : public System::Memory::KPortableMemoryPool<KStaticTabFileInfo, 32>
		{
		public:
			KStaticTabFileInfo() : mFilePath(""), mpStaticTabFilePtr(NULL), mLastModifiedTime(0), mLastTryTime(0), mTryToReLoadCyc(0) {}

			// 文件名带路径
			System::Collections::KString<MAX_PATH> mFilePath;
			// 文件读取接口指针
			IStaticTabFile * mpStaticTabFilePtr;
			// 上次文件修改时间
			TIME_T mLastModifiedTime;
			// 上次尝试读取时间点
			TIME_T mLastTryTime;
			// 尝试重读周期
			TIME_T mTryToReLoadCyc;
		};

		class KStaticTabFileInfo_Less
		{
		public:
			typedef KStaticTabFileInfo* value_type;
			BOOL operator () (const value_type& a, const value_type& b) const;
		};

		typedef System::Collections::DynSortedArray< KStaticTabFileInfo *, KStaticTabFileInfo_Less, 32, 8>	KStaticTabFileMap;

	public:
		KStaticTabFileManager();
		virtual ~KStaticTabFileManager();

		void Breathe(INT nInterval);

		// 注册一个需要按周期重的文件
		// szFilePath			带路径文件名
		// pStaticTabFilePtr	文件读取接口
		// nReloadCyc			重复尝试读取周期 默认300秒
		bool Register(const char * szFilePath, IStaticTabFile * pStaticTabFilePtr, bool bLoadOnRegister = true, TIME_T nReloadCyc = DEF_DEFAULT_RELOAD_INTERVAL);

		// 删除一个已经被注册的周期重读文件
		bool Unregister(const char * szFilePath);

		// 清除所有
		bool UnregisterAll();

		// 即时强制读取
		bool ForceReload(const char * szFilePath);

		// 检查、读取一次
		void CheckAndReloadOnce();

	protected:
		bool _ReloadFile(KStaticTabFileInfo * pStaticTabFile);

	private:
		KStaticTabFileMap mStaticTabFileMap;

	public:
	/*	BeginDefLuaClassNoCon(KStaticTabFileManager)
			DefMemberFunc(CheckAndReloadOnce)
			DefMemberFunc(ForceReload)
		EndDef*/
	};
};