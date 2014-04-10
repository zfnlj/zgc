#pragma once

#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/KString.h"
#include "System/Collections/KMapByVector.h"
#include "System/File/KTabFile.h"

using namespace System::File;
using namespace System::Collections;
using namespace System::Patterns;

template <class TManager, class TKeyType, class TConfig, int NGroupNum=128, int NListNum=128>
class CCfg1GroupMgrTemplate : public KSingleton<TManager>
{
public:
	static const int STEP_DIV = 4;
	typedef DynArray<TConfig*, NListNum, NListNum/STEP_DIV> TDataList;
	typedef struct SDataListNode : public System::Memory::KPortableStepPool<SDataListNode, NGroupNum>
	{
		 TDataList DataList;
	};
	typedef KMapByVector<TKeyType,SDataListNode*> TDataMap;

public:
	CCfg1GroupMgrTemplate() : m_DataMap(NGroupNum, NGroupNum/STEP_DIV) {}
	~CCfg1GroupMgrTemplate() { this->Clear(); }

public:
	BOOL LoadTable(const char* pFilePath);

	const TDataList* GetGroup(const TKeyType& keyValue) const
	{
		typename TDataMap::const_iterator iter = m_DataMap.find(keyValue);
		if (iter != m_DataMap.end())
		{
			return &(iter->second->DataList);
		}
		return NULL;
	}

	const TDataMap* GetAllData() const { return &m_DataMap; }

	BOOL HasGroup(const TKeyType& keyValue) const
	{
		return m_DataMap.find(keyValue) != m_DataMap.end();
	}

protected:
	void Clear()
	{
		for (typename TDataMap::iterator iter = m_DataMap.begin(); iter != m_DataMap.end(); ++iter)
		{
			for (int i = 0; i < iter->second->DataList.size(); ++i)
			{
				TConfig::Free(iter->second->DataList[i]);
			}
			SDataListNode::Free(iter->second);
		}
		m_DataMap.clear();
	}

protected:
	TDataMap m_DataMap;
};

template <class TManager, class TKeyType, class TConfig, int NGroupNum, int NListNum>
BOOL CCfg1GroupMgrTemplate<TManager, TKeyType, TConfig, NGroupNum, NListNum>::LoadTable(const char* pFilePath)
{
	ASSERT_RETURN(pFilePath, FALSE);
	
	KString<512> fullFilePath = "./";
	fullFilePath.append(pFilePath);
	KTabFile2 tabFile;
	if(!tabFile.Open(fullFilePath.c_str()))
	{
		Log(LOG_ERROR, "[ERROR] LoadTable from %s failed", pFilePath);
		return FALSE;
	}

	INT nRet = 0;
	TConfig* pConfig = NULL;
	typename TDataMap::iterator iter;
	for (INT nLine = 2; nRet = tabFile.ReadLine(); ++nLine)
	{
		if (nRet == -1)
		{
			Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
			tabFile.Clear();
			return FALSE;
		}

		pConfig = TConfig::Alloc();
		if (pConfig->ReadItem(tabFile))
		{
			iter = m_DataMap.find(pConfig->GetKey1());
			if (iter == m_DataMap.end())
			{
				iter = m_DataMap.insert_unique(pConfig->GetKey1(), SDataListNode::Alloc());
			}
			iter->second->DataList.push_back(pConfig);
			continue;
		}

		TConfig::Free(pConfig);
		this->Clear();
		Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
		return FALSE;
	}
	return TRUE;
}
