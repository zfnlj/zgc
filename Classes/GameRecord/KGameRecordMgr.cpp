#include "KGameRecordMgr.h"
#include "Inc/KTypeDef.h"

IMPLEMENT_SINGLETON(KGameRecordMgr)

void KGameRecordMgr::StartRecord()
{
	m_task.StartRecrod();
	m_recState = REC_RECORD;
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
	if(m_recState!=REC_RECORD) return;
	m_task.RecordPlayOp(src,des,slot);
}

void KGameRecordMgr::RecordMouseEvt(KRecordUIMouseData::Mouse_evt evt)
{
	if(m_recState!=REC_RECORD) return;
	m_task.RecordMouseEvt(evt);
}

void KGameRecordMgr::InitTest()
{
	StartRecord();
	RecordPlayOp(1,2,3);
	RecordPlayOp(3,2,1);
	SaveToFile("abc");

	LoadFile("abc");
	m_task.StartPlay();
}