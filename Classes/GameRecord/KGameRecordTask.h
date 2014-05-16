#ifndef _KGAMERECORDTASK_H
#define _KGAMERECORDTASK_H

#include <System/Misc/StreamInterface.h>
#include "KRecordData.h"
#include "KRecordFile.h"
#include "System/Misc/KStream.h"
// 一次录制/回放的所有数据，多线程写/读时直接把这结构传过去
class KGameRecordTask
{
public:
	KGameRecordTask(){}
	~KGameRecordTask(){}
	void init();
	bool Save( const char* szFileName);
	bool Load( const char* szFileName);
	bool SerializeData( StreamInterface* pStream );
	bool DeserializeData( StreamInterface* pStream );
	KRecordDataBase* AllocData( EGameRecordedDataType eType );

	void RecordPlayOp(int src,int des,int slot);
	void RecordMouseEvt(KRecordUIMouseData::Mouse_evt evt);
	void Empty();
	void StartRecrod();
private:

	// 为了内存分配快搞的各种池
	KRecordDataList m_FrameData;
	KMemoryStream m_deckStream;
	char* m_dataBuf;
};


#endif // __HELLOWORLD_SCENE_H__
