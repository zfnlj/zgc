#ifndef _KRECORDMGR_H
#define _KRECORDMGR_H

#include "System/Singleton.h"
#include "KGameRecordTask.h"
class KGameRecordMgr : public Singleton<KGameRecordMgr>
{
public:
	KGameRecordMgr(){}
	~KGameRecordMgr(){}
	static KGameRecordMgr& getSingleton(void);

	void StartRecord();
	bool SaveToFile( const char* szFileName);
	bool LoadFile(const char* szFileName);
	void RecordPlayOp(int src,int des,int slot);
	void InitTest();
private:
	KGameRecordTask m_task;
};

#endif // __HELLOWORLD_SCENE_H__
