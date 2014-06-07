#include "KGameRecordMgr.h"
#include "../Inc/KTypeDef.h"
#include "../GameRoot.h"
#include "../ui/KGameRecordPanel.h"
#include "../UI/BattleFieldScene.h"
#include "../StaticTable/StaticData.h"

IMPLEMENT_SINGLETON(KGameRecordMgr)

void KGameRecordMgr::StartRecord()
{
	int inc = STATIC_DATA_INT("recInc");
	STATIC_DATA_SET("recInc",inc+1);

	m_task.StartRecrod(inc*100);
	m_recState = REC_RECORD;
}

void KGameRecordMgr::Stop()
{
	if(m_recState==REC_RECORD){
		SaveToFile("abc");
	}else if(m_recState==REC_PLAY){
		m_task.Stop();
	}
	m_recState = REC_NULL;
}

bool KGameRecordMgr::StartPlay(const char* fileName)
{
	if(!LoadFile(fileName)) return false;
	m_recState = REC_PLAY;
	m_task.StartPlay();
	return true;
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

void KGameRecordMgr::onPlayOp(int src,int des,int slot)
{
	switch(m_recState){
	case REC_RECORD:
		RecordPlayOp(src,des,slot);
		break;
	case REC_PLAY:
		onPlayStepOn();
		break;
	default:
		break;
	}
}
void KGameRecordMgr::RecordPlayOp(int src,int des,int slot)
{
	if(m_recState!=REC_RECORD) return;
	m_task.RecordPlayOp(src,des,slot);
}

void KGameRecordMgr::onClickWidget(cocos2d::CCObject* obj)
{
	UIWidget* pWidget = (UIWidget*)obj;
	
	switch(m_recState){
	case REC_RECORD:
		m_task.RecordClickWidgetEvt(pWidget->getName());
		break;
	case REC_PLAY:
		//onPlayStepOn();
		break;
	default:
		break;
	}

}

void::KGameRecordMgr::onMouseEvt(KRecordUIMouseData::Mouse_evt evt)
{
	switch(m_recState){
	case REC_RECORD:
		m_task.RecordMouseEvt(evt);
		break;
	case REC_PLAY:
		//onPlayStepOn();
		break;
	default:
		break;
	}

}


void::KGameRecordMgr::RecordMouseEvt(KRecordUIMouseData::Mouse_evt evt)
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

void KGameRecordMgr::update(float elapsed)
{
	if(m_recState==REC_PLAY){
		if(!m_task.Play(elapsed)) onPlayEnd();
	}
}

void KGameRecordMgr::onPlayEnd()
{
	m_recState = REC_NULL;
	GameRoot::getSingleton().getBattleScene()->RecordPanel().DoClickStop(NULL);
}

bool KGameRecordMgr::IsClickCardValidate(KCardInst* card)
{
	if(m_recState!= REC_PLAY) return true;
	return m_task.IsClickCardValidate(card);
}

bool KGameRecordMgr::IsClickFightAreaValidate(int slot)
{
	if(m_recState!= REC_PLAY) return true;
	return m_task.IsClickFightAreaValidate(slot);
}

bool KGameRecordMgr::IsClickButValidate(cocos2d::CCObject* obj)
{
	if(m_recState!= REC_PLAY) return true;
	return m_task.IsClickButValidate(obj);
}

bool KGameRecordMgr::IsClickWidgetValidate(cocos2d::CCObject* layer,CCPoint& pt)
{
	if(m_recState!= REC_PLAY) return true;
	return m_task.IsClickWidgetValidate(layer,pt);
}


void KGameRecordMgr::onPlayStepOn()
{
	if(m_recState!= REC_PLAY) return;
	m_task.onPlayStepOn();
}