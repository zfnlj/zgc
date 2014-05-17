#ifndef _KGAMERECORDTASK_H
#define _KGAMERECORDTASK_H

#include <System/Misc/StreamInterface.h>
#include "KRecordData.h"
#include "KRecordFile.h"
#include "System/Misc/KStream.h"
#include "cocos2d.h"
// һ��¼��/�طŵ��������ݣ����߳�д/��ʱֱ�Ӱ���ṹ����ȥ
#define CUR_TASK_VER  1

class KGameRecordTask
{
public:
	struct Task_Head{
		int _ver;
		char _mode; // need operator;
		Task_Head(){
			_ver = 1;
			_mode = 1;
		}
	};
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
	void StartPlay();
	void Stop();
	bool Play(float);

	bool IsClickCardValidate(KCardInst* card);
	bool IsClickFightAreaValidate(int slot);
	bool IsClickButValidate(cocos2d::CCObject* obj);
private:

	// Ϊ���ڴ������ĸ��ֳ�
	Task_Head m_head;
	KRecordDataList m_FrameData;
	KMemoryStream m_deckStream;
	char* m_dataBuf;
	KRecordDataBase* m_pCurOpera;
};


#endif // __HELLOWORLD_SCENE_H__
