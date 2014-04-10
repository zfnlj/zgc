#include "KCoordinateTable.h"
#include <System/File/KTabFile.h>
#include <System/Log/log.h>
#include "Inc/KCommonDefine.h"
#include "../KGlobalFunction.h"
#include "System/File/KTabfileLoader.h"

using namespace JG_C;
using namespace JG_S;
using namespace JG_M;
using namespace JG_F;

KCoordinateTable::KCoordinateTable()
{

}

KCoordinateTable::~KCoordinateTable()
{

}

BOOL KCoordinateTable::Add(const KCoordinatePoint& point)
{
	Iterator it = m_map.find(point.m_Id);
	if(it != m_map.end())
	{
		Log(LOG_ERROR, "warn: CoordinateStatus.txt duplicated ID:%d", point.m_Id);
	}
	m_map[point.m_Id] = point;
	return TRUE;
}

BOOL KCoordinateTable::Remove(int pointID)
{
	Iterator it = m_map.find(pointID);
	if(it == m_map.end()) return FALSE;
	m_map.erase(it);
	return TRUE;
}

const KCoordinatePoint* KCoordinateTable::GetPoint(int pointID) const
{
	ConstIterator it = m_map.find(pointID);
	if(it == m_map.end()) return NULL;
	return &it->second;
}

BOOL KCoordinateTable::Initialize(const char* filename)
{
	return !!this->Reload(filename);
}

bool KCoordinateTable::Reload(const char * szFilePath)
{
	ASSERT_RETURN(szFilePath, false);		
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	JG_C::KString<MAX_PATH> lFullPath = loader.GetBaseFilepath(szFilePath).c_str();
	JG_F::KTabFile2 * fileReader = loader.GetFileReader(lFullPath.c_str());

	if(!fileReader )
	{
		AssertFile(szFilePath);
		return false;
	}

	m_map.clear();

	int rowNo = 2;
	KCoordinatePoint point;

	while(TRUE)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return FALSE; }
		if(nRet == 0) break;

		if(!fileReader->GetInteger("Id", 0, &point.m_Id)) break;
		if(point.m_Id == 0) return FALSE;

		if(!fileReader->GetInteger("MapId", 0, &point.m_MapID)) return FALSE;
		if(!fileReader->GetInteger("Xaxis", 0, &point.m_Xaxis)) return FALSE;
		if(!fileReader->GetInteger("Yaxis", 0, &point.m_Yaxis)) return FALSE;
		if(!fileReader->GetInteger("CDirection", 0, &point.m_CDirection)) return FALSE;

		if(!fileReader->GetInteger("CAngle", 0, &point.m_CAngle)) return FALSE;
		if(!fileReader->GetInteger("CFaces", 0, &point.m_CFaces)) return FALSE;
		if(!fileReader->GetInteger("RDirection", 0, &point.m_RDirection)) return FALSE;
		if(!fileReader->GetString("Type", "", point.m_desc, sizeof(point.m_desc))) return FALSE;

		this->Add(point);
		rowNo += 1;
	}

	loader.CloseFileReader(fileReader);
	return TRUE;
}

void KCoordinateTable::Finalize()
{
	return;
}
