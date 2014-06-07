#ifndef _KRECORDMGR_H
#define _KRECORDMGR_H

#include "System/Singleton.h"
#include "KGameRecordTask.h"
#include "cocos2d.h"

class KCardInst;

class KGameRecordMgr : public Singleton<KGameRecordMgr>
{
public:
	enum RecState{
		REC_NULL,
		REC_RECORD,
		REC_PLAY,
	};
	KGameRecordMgr(){}
	~KGameRecordMgr(){}
	static KGameRecordMgr& getSingleton(void);
	void init(){m_recState = REC_NULL; m_task.init();}
	
	void update(float elapsed);
	bool SaveToFile( const char* szFileName);
	bool LoadFile(const char* szFileName);
	
	void onPlayOp(int src,int des,int slot);
	void onMouseEvt(KRecordUIMouseData::Mouse_evt);
	void onClickWidget(cocos2d::CCObject* obj);
	void InitTest();
	void StartRecord();
	bool StartPlay(const char*);
	void onPlayEnd();
	void Stop();
	bool IsClickCardValidate(KCardInst* card);
	bool IsClickFightAreaValidate(int slot);
	bool IsClickButValidate(cocos2d::CCObject* obj);
	bool IsClickWidgetValidate(cocos2d::CCObject* layer,cocos2d::CCPoint& pt);
	bool IsPlaying(){ return m_recState==REC_PLAY;}
	void onPlayStepOn();
private:
	void RecordPlayOp(int src,int des,int slot);
	void RecordMouseEvt(KRecordUIMouseData::Mouse_evt);
	

	KGameRecordTask m_task;
	RecState m_recState;

	static int g_dlgId_inc;

};

#endif // __HELLOWORLD_SCENE_H__
