#ifndef _KGAMERECORDTASK_H
#define _KGAMERECORDTASK_H

#include <System/Misc/StreamInterface.h>
#include "KRecordData.h"
// һ��¼��/�طŵ��������ݣ����߳�д/��ʱֱ�Ӱ���ṹ����ȥ
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

	// Ϊ���ڴ������ĸ��ֳ�
	KRecordDataList m_FrameData;
};


#endif // __HELLOWORLD_SCENE_H__
