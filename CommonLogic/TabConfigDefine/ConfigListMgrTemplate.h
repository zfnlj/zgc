#pragma once

#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/KString.h"
#include "System/Collections/KVector.h"
#include "System/File/KTabFile.h"

using namespace System::File;
using namespace System::Collections;
using namespace System::Patterns;

template <class TManager, class TConfig, int NCapacity=1024, int NStepSize=1024>
class CCfgListMgrTemplate : public KSingleton<TManager>
{
public:
	typedef KVector<TConfig*> TDataList;

	CCfgListMgrTemplate() : m_DataList(NCapacity, NStepSize) {}
	~CCfgListMgrTemplate() { this->Clear(); }

public:
	BOOL LoadTable(const char* pFilePath);

	const TDataList* GetAllData() const { return &m_DataList; }

	INT Size() const { return m_DataList.size(); }

	const TConfig* GetConfig(INT index)
	{
		if (index >= 0 && index < m_DataList.size())
		{
			return m_DataList[index];
		}
		return NULL;
	}

protected:
	void Clear()
	{
		for (int i = 0; i < m_DataList.size(); ++i)
		{
			TConfig::Free(m_DataList[i]);
		}
		m_DataList.clear();
	}

protected:
	TDataList m_DataList;
};

template <class TManager, class TConfig, int NCapacity, int NStepSize>
BOOL CCfgListMgrTemplate<TManager, TConfig, NCapacity, NStepSize>::LoadTable(const char* pFilePath)
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
			Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
			tabFile.Clear();
			return FALSE;
		}

		pConfig = TConfig::Alloc();
		if (pConfig->ReadItem(tabFile))
		{
			m_DataList.push_back(pConfig);
			continue;
		}

		TConfig::Free(pConfig);
		this->Clear();
		Log(LOG_ERROR, "[ERROR] LoadTable from %s failed, line:%d", pFilePath, nLine);
		return FALSE;
	}
	return TRUE;
}
