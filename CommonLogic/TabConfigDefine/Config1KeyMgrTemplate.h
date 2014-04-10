#pragma once

#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/KString.h"
#include "System/Collections/KMapByVector.h"
#include "System/File/KTabFile.h"

using namespace System::File;
using namespace System::Collections;
using namespace System::Patterns;

template <class TManager, class TKeyType, class TConfig, int NCapacity=1024, int NStepSize=1024>
class CCfg1KeyMgrTemplate : public KSingleton<TManager>
{
public:
	typedef KMapByVector<TKeyType,TConfig*> TDataMap;

	CCfg1KeyMgrTemplate() : m_DataMap(NCapacity, NStepSize) {}
	~CCfg1KeyMgrTemplate() { this->Clear(); }

public:
	BOOL LoadTable(const char* pFilePath);

	const TConfig* GetConfig(const TKeyType& keyValue) const
	{
		typename TDataMap::const_iterator iter = m_DataMap.find(keyValue);
		if (iter != m_DataMap.end())
		{
			return iter->second;
		}
		return NULL;
	}

	const TDataMap* GetAllData() const { return &m_DataMap; }

	BOOL Has(const TKeyType& keyValue) const
	{
		return GetConfig(keyValue) != NULL;
	}

protected:
	void Clear()
	{
		for (typename TDataMap::iterator iter = m_DataMap.begin(); iter != m_DataMap.end(); ++iter)
		{
			TConfig::Free(iter->second);
		}
		m_DataMap.clear();
	}

protected:
	TDataMap m_DataMap;
};

template <class TManager, class TKeyType, class TConfig, int NCapacity, int NStepSize>
BOOL CCfg1KeyMgrTemplate<TManager, TKeyType, TConfig, NCapacity, NStepSize>::LoadTable(const char* pFilePath)
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
	for (INT nLine = 2; nRet = tabFile.ReadLine(); ++nLine)
	{
		if (nRet == -1)
		{
			tabFile.Clear();
			Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
			return FALSE;
		}

		pConfig = TConfig::Alloc();
		if (pConfig->ReadItem(tabFile))
		{
			if (this->GetConfig(pConfig->GetKey1()) == NULL)
			{
				m_DataMap.insert_unique(pConfig->GetKey1(), pConfig);
				continue;
			}
			else
			{
				Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d, multi KeyValue:%d", pFilePath, nLine, pConfig->GetKey1());
			}
		}

		TConfig::Free(pConfig);
		this->Clear();
		Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
		return FALSE;
	}
	return TRUE;
}
