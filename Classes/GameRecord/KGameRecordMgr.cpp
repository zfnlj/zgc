#include "KGameRecordMgr.h"

IMPLEMENT_SINGLETON(KGameRecordMgr)

void KGameRecordMgr::StartRecord()
{
	m_task.Empty();
}

bool KGameRecordMgr::SaveToFile( const char* szFileName)
{
	return m_task.Save(szFileName);
}

bool KGameRecordMgr::LoadFile(const char* szFileName)
{
	m_task.Empty();
	return m_task.Load(szFileName);
}

void KGameRecordMgr::RecordPlayOp(int src,int des,int slot)
{
	m_task.RecordPlayOp(src,des,slot);
}


void KGameRecordMgr::InitTest()
{
	StartRecord();
	RecordPlayOp(1,2,3);
	RecordPlayOp(3,2,1);
	SaveToFile("abc");

	LoadFile("abc");
}