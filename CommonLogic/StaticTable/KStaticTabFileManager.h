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

			// �ļ�����·��
			System::Collections::KString<MAX_PATH> mFilePath;
			// �ļ���ȡ�ӿ�ָ��
			IStaticTabFile * mpStaticTabFilePtr;
			// �ϴ��ļ��޸�ʱ��
			TIME_T mLastModifiedTime;
			// �ϴγ��Զ�ȡʱ���
			TIME_T mLastTryTime;
			// �����ض�����
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

		// ע��һ����Ҫ�������ص��ļ�
		// szFilePath			��·���ļ���
		// pStaticTabFilePtr	�ļ���ȡ�ӿ�
		// nReloadCyc			�ظ����Զ�ȡ���� Ĭ��300��
		bool Register(const char * szFilePath, IStaticTabFile * pStaticTabFilePtr, bool bLoadOnRegister = true, TIME_T nReloadCyc = DEF_DEFAULT_RELOAD_INTERVAL);

		// ɾ��һ���Ѿ���ע��������ض��ļ�
		bool Unregister(const char * szFilePath);

		// �������
		bool UnregisterAll();

		// ��ʱǿ�ƶ�ȡ
		bool ForceReload(const char * szFilePath);

		// ��顢��ȡһ��
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