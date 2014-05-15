#ifndef _KGAMERECORDTASK_H
#define _KGAMERECORDTASK_H

#include <System/Misc/StreamInterface.h>
#include "KRecordData.h"
// 一次录制/回放的所有数据，多线程写/读时直接把这结构传过去
class KGameRecordTask
{
public:
	KGameRecordTask(){}
	~KGameRecordTask(){}
	bool Save( const char* szFileName);
	bool Load( const char* szFileName);
	bool SerializeData( StreamInterface* pStream );
	bool DeserializeData( StreamInterface* pStream );
	KRecordDataBase* AllocData( EGameRecordedDataType eType );

	void RecordPlayOp(int src,int des,int slot);
	void RecordMouseEvt(KRecordUIMouseData::Mouse_evt evt);
	void Empty();
private:

	// 为了内存分配快搞的各种池
	KRecordDataList m_FrameData;
};


#endif // __HELLOWORLD_SCENE_H__
