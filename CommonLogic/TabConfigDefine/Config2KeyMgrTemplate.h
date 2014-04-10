#pragma once

#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/KString.h"
#include "System/Collections/KMapByVector.h"
#include "System/File/KTabFile.h"

using namespace System::File;
using namespace System::Collections;
using namespace System::Patterns;

template <class TManager, class TKeyType1, class TKeyType2, class TConfig, int NGroupNum=128, int NListNum=128>
class CCfg2KeyMgrTemplate : public KSingleton<TManager>
{
public:
	static const int STEP_DIV = 4;
	typedef KMapByVector<TKeyType2, TConfig*> TDataList;
	typedef struct SDataListNode : public System::Memory::KPortableStepPool<SDataListNode, NGroupNum>
	{
		SDataListNode() : DataList(NListNum, NListNum/STEP_DIV) {}
		TDataList DataList;
	};
	typedef KMapByVector<TKeyType1,SDataListNode*> TDataMap;

public:
	CCfg2KeyMgrTemplate() : m_DataMap(NGroupNum, NGroupNum/STEP_DIV) {}
	~CCfg2KeyMgrTemplate() { this->Clear(); }

public:
	BOOL LoadTable(const char* pFilePath);

	const TDataList* GetGroup(const TKeyType1& keyValue) const
	{
		typename TDataMap::const_iterator iter = m_DataMap.find(keyValue);
		if (iter != m_DataMap.end())
		{
			return &(iter->second->DataList);
		}
		return NULL;
	}

	const TConfig* GetConfig(const TKeyType1& key1, const TKeyType2 key2) const
	{
		const TDataList* pList = GetGroup(key1);
		if (pList)
		{
			typename TDataList::const_iterator iter = pList->find(key2);
			if (iter != pList->end())
			{
				return iter->second;
			}
		}
		return NULL;
	}

	const TDataMap* GetAllData() const { return &m_DataMap; }

	BOOL HasGroup(const TKeyType1& keyValue) const
	{
		return m_DataMap.find(keyValue) != m_DataMap.end();
	}

	BOOL Has(const TKeyType1& key1, const TKeyType2& key2)
	{
		return GetConfig(key1, key2) != NULL;
	}

protected:
	void Clear()
	{
		for (typename TDataMap::iterator iter = m_DataMap.begin(); iter != m_DataMap.end(); ++iter)
		{
			TDataList& dataList = iter->second->DataList;
			for(typename TDataList::iterator it = dataList.begin(); it != dataList.end(); ++it)
			{
				TConfig::Free(it->second);
			}
			SDataListNode::Free(iter->second);
		}
		m_DataMap.clear();
	}

protected:
	TDataMap m_DataMap;
};

template <class TManager, class TKeyType1, class TKeyType2, class TConfig, int NGroupNum, int NListNum>
BOOL CCfg2KeyMgrTemplate<TManager, TKeyType1, TKeyType2, TConfig, NGroupNum, NListNum>::LoadTable(const char* pFilePath)
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
	TKeyType1 key1;
	TKeyType2 key2;
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
			key1 = pConfig->GetKey1();
			key2 = pConfig->GetKey2();
			if (GetConfig(key1, key2) == NULL)
			{
				if (GetGroup(key1) == NULL)
				{
					iter = m_DataMap.insert_unique(key1, SDataListNode::Alloc());
				}
				iter->second->DataList.insert_unique(key2, pConfig);
				continue;
			}
			else
			{
				Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d, multi key, key1:%d, key2:%d", pFilePath, nLine, key1, key2);
			}
		}

		TConfig::Free(pConfig);
		this->Clear();
		Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
		return FALSE;
	}
	return TRUE;
}
