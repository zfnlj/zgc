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

#include "KStaticTabFileManager.h"
#include <System/File/KFile.h>
#include "System/File/KTabfileLoader.h"
#include <time.h>

using namespace System::Collections;
using namespace System::File;

namespace StaticTabFileAbout
{
	BOOL KStaticTabFileManager::KStaticTabFileInfo_Less::operator () (const value_type& a, const value_type& b) const
	{
		return (_stricmp(a->mFilePath.c_str(), b->mFilePath.c_str()) > 0);
	}

	KStaticTabFileManager::	KStaticTabFileManager()
	{
		UnregisterAll();
	}

	KStaticTabFileManager::~KStaticTabFileManager()
	{
		UnregisterAll();
	}

	void KStaticTabFileManager::Breathe(INT nInterval)
	{
		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KString<MAX_PATH> lFullPath = ("");
		TIME_T lNow = (TIME_T)time(NULL);
		TIME_T tmload = 0;
		KStaticTabFileInfo * lpTabFileInfo = NULL;
		for(int i = 0; i < mStaticTabFileMap.size(); i++)
		{
			lpTabFileInfo = mStaticTabFileMap.at(i);
			if( NULL == lpTabFileInfo ) continue;
			if( 0 == lpTabFileInfo->mTryToReLoadCyc ) continue;
			if( lpTabFileInfo->mLastTryTime + lpTabFileInfo->mTryToReLoadCyc > lNow ) continue;

			lpTabFileInfo->mLastTryTime = lNow;
			
			lFullPath = loader.GetBaseFilepath( lpTabFileInfo->mFilePath );
			tmload = KFileUtil::GetLastModified( lFullPath.c_str() );
			if( 0x7fffffff == tmload ) continue;
			if( tmload == lpTabFileInfo->mLastModifiedTime ) continue;
			lpTabFileInfo->mLastModifiedTime = tmload;

			_ReloadFile(lpTabFileInfo);
		}
	}

	bool KStaticTabFileManager::Register(const char * szFilePath, IStaticTabFile * pStaticTabFilePtr, bool bLoadOnRegister /* = true */, TIME_T nReloadCyc /* = DEF_DEFAULT_RELOAD_INTERVAL */)
	{
		if( NULL == szFilePath ) return false;
		if( NULL == pStaticTabFilePtr ) return false;
		if( 0 == nReloadCyc ) return false;

		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KString<MAX_PATH> lFullPath = (KString<MAX_PATH>)loader.GetBaseFilepath(szFilePath);
		TIME_T tmload = KFileUtil::GetLastModified( lFullPath.c_str() );
// 		if(0x7fffffff == tmload) return false;

		KStaticTabFileInfo * lpTabFileInfo = NULL;
		KStaticTabFileInfo lTabFileInfoForFind;
		lTabFileInfoForFind.mFilePath = szFilePath;
		INT lnIndex = mStaticTabFileMap.find( &lTabFileInfoForFind );
		if( -1 == lnIndex )
		{
			lpTabFileInfo = KStaticTabFileInfo::Alloc();
		}
		else
		{
			lpTabFileInfo = mStaticTabFileMap.at( lnIndex );
		}

		if( NULL == lpTabFileInfo ) return false;

		lpTabFileInfo->mFilePath = szFilePath;
		lpTabFileInfo->mpStaticTabFilePtr = pStaticTabFilePtr;

		lpTabFileInfo->mLastTryTime = (TIME_T)time(NULL);
		
		lpTabFileInfo->mLastModifiedTime = tmload;
		lpTabFileInfo->mTryToReLoadCyc = nReloadCyc;

		if( -1 == lnIndex )
		{
			mStaticTabFileMap.insert_unique( lpTabFileInfo );

			if( bLoadOnRegister )
			{
				_ReloadFile( lpTabFileInfo );
			}
		}
		return true;
	}

	bool KStaticTabFileManager::Unregister(const char * szFilePath)
	{
		KStaticTabFileInfo lTabFileInfoForFind;
		lTabFileInfoForFind.mFilePath = szFilePath;
		INT lnIndex = mStaticTabFileMap.find( &lTabFileInfoForFind );
		if( -1 == lnIndex ) return true;
		KStaticTabFileInfo::Free( mStaticTabFileMap.at( lnIndex ) );
		mStaticTabFileMap.erase( lnIndex );
		return true;
	}

	bool KStaticTabFileManager::UnregisterAll()
	{
		for(int i = 0; i < mStaticTabFileMap.size(); i++)
		{
			if( NULL == mStaticTabFileMap.at(i) ) continue;
			KStaticTabFileInfo::Free( mStaticTabFileMap.at(i) );
		}
		mStaticTabFileMap.clear();
		return true;
	}

	bool KStaticTabFileManager::ForceReload(const char * szFilePath)
	{
		KStaticTabFileInfo lTabFileInfoForFind;
		lTabFileInfoForFind.mFilePath = szFilePath;
		INT lnIndex = mStaticTabFileMap.find( &lTabFileInfoForFind );
		if( -1 == lnIndex ) return false;
		KStaticTabFileInfo * lpTabFileInfo = mStaticTabFileMap.at( lnIndex );
		return _ReloadFile(lpTabFileInfo);
	}

	bool KStaticTabFileManager::_ReloadFile(KStaticTabFileInfo * pStaticTabFile)
	{
		if( NULL == pStaticTabFile ) return false;
		if( NULL == pStaticTabFile->mpStaticTabFilePtr ) return false;
		return pStaticTabFile->mpStaticTabFilePtr->Reload( pStaticTabFile->mFilePath.c_str() );
	}

	void KStaticTabFileManager::CheckAndReloadOnce()
	{
		KTabfileLoader& loader = KTabfileLoader::GetInstance();
		KString<MAX_PATH> lFullPath = ("");
		TIME_T lNow = (TIME_T)time(NULL);
		TIME_T tmload = 0;
		KStaticTabFileInfo * lpTabFileInfo = NULL;
		for(int i = 0; i < mStaticTabFileMap.size(); i++)
		{
			lpTabFileInfo = mStaticTabFileMap.at(i);
			if( NULL == lpTabFileInfo ) continue;

			lpTabFileInfo->mLastTryTime = lNow;

			lFullPath = loader.GetBaseFilepath( lpTabFileInfo->mFilePath );
			tmload = KFileUtil::GetLastModified( lFullPath.c_str() );
			if( 0x7fffffff == tmload ) continue;
			if( tmload == lpTabFileInfo->mLastModifiedTime ) continue;
			lpTabFileInfo->mLastModifiedTime = tmload;

			_ReloadFile(lpTabFileInfo);
		}
	}
};
