#include "KRankStaticMgr.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>

void KRankStaticDataManager::Insert( const KRankStaticData& data )
{
	DATA_MAP_TYPE::iterator it = m_dataMap.find(data.m_level);
	KRankStaticData* p = NULL;
	if (it != m_dataMap.end())
	{
		p = it->second;
		ASSERT_RETURN_VOID(p);
	}
	else
	{
		p = KRankStaticData::Alloc();
		ASSERT_RETURN_VOID(p);
		m_dataMap.insert_unique(data.m_level, p);
	}
	*p = data;
	ELEM_DATA ed;
	ed.m_level = data.m_level;
	ed.m_exp = data.m_exp;
	m_expToLevelMap.insert_unique(ed);
}

bool KRankStaticDataManager::Init(System::File::KTabFile2* fileReader)
{
	DATA_MAP_TYPE::iterator it = m_dataMap.begin();
	for(; it != m_dataMap.end(); it++)
	{
		KRankStaticData::Free(it->second);
	}
	m_dataMap.clear();
	Reload(fileReader);

	return true;
}

const KRankStaticData* KRankStaticDataManager::Get( int idx ) const
{
	DATA_MAP_TYPE::const_iterator it = m_dataMap.find(idx);
	if (it != m_dataMap.end())
	{
		return it->second;
	}
	return NULL;
}

const KRankStaticData* KRankStaticDataManager::operator[]( int idx ) const
{
	return Get(idx);
}

bool KRankStaticDataManager::Reload(System::File::KTabFile2* fileReader )
{
	m_expToLevelMap.clear();

	if(!fileReader) return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1)
		{
			return false;
		}
		if(nRet == 0) break;

		KRankStaticData rsd;
		struct INIT_DATA
		{
			const char* szColumnName;
			const int_r defValue;
			void* writePos;
			enum
			{
				enumNumber = 0,
			}const type;
		} initData[] = {
			"Level",					0,					&rsd.m_level,									INIT_DATA::enumNumber,
			"Exp",						0,					&rsd.m_exp,										INIT_DATA::enumNumber,
		};

		for (int j = 0; j < sizeof(initData)/sizeof(INIT_DATA); j++)
		{
			switch(initData[j].type)
			{
			case INIT_DATA::enumNumber:
				fileReader->GetInteger(initData[j].szColumnName, initData[j].defValue, (int*)(initData[j].writePos));
				break;
			}
		}

		Insert(rsd);
	}
	return true;
}

float KRankStaticDataManager::GetLevRate(int exp)
{
	BOOL bFind = FALSE;
	int pos = m_expToLevelMap.find(exp, bFind);
	if(pos==0){
		return (float)exp/m_expToLevelMap[pos].m_exp;
	}else{
		exp -= m_expToLevelMap[pos-1].m_exp;
		int needExp = m_expToLevelMap[pos].m_exp -m_expToLevelMap[pos-1].m_exp;
		return (float)exp/(float)needExp;
	}
}

int KRankStaticDataManager::ExpToLevel( int exp )
{
	BOOL bFind = FALSE;
	int pos = m_expToLevelMap.find(exp, bFind);
	if (pos >= 0)
	{
		return m_expToLevelMap[pos].m_level;
	}
	return 0;
}

const char* KRankStaticDataManager::GetLevelDesp(int level)
{
	ASSERT_RETURN(Get(level), NULL);
	int stringID = 3000001 + (level-1);
	return "";//KStringManager::Instance()->GetStringByID(stringID);
}