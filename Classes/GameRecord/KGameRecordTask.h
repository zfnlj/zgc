#ifndef _KGAMERECORDTASK_H
#define _KGAMERECORDTASK_H

#include <System/Misc/StreamInterface.h>
#include "KRecordData.h"
#include "KRecordFile.h"
#include "System/Misc/KStream.h"
// һ��¼��/�طŵ��������ݣ����߳�д/��ʱֱ�Ӱ���ṹ����ȥ
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

	// Ϊ���ڴ������ĸ��ֳ�
	KRecordDataList m_FrameData;
	KMemoryStream m_deckStream;
	char* m_dataBuf;
};


#endif // __HELLOWORLD_SCENE_H__
