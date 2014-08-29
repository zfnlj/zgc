#include "KRecordData.h"
#include "../GameRoot.h"
#include "../UI/BattleFieldScene.h"
#include "../UI/assist/KUIAssist.h"
#include "KGameRecordMgr.h"
#include "../GameLogic/assist/KBattleCtrlAssist.h"

enum Cur_OP_Step{
	step_src,
	step_des,
	step_slot,
	step_area,
	step_ok,
};

Cur_OP_Step _GetCurOpStep(KBattleCtrlBase::BattleOp& op,int src,int des,int slot)
{
	if(op._src != src) return step_src;
	KCardInst* card = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),src);
	if(card->IsKindOf(KCardStatic::card_skill)){
		if(des>0){
			return step_des;
		}else{
			return step_area;
		}
	}else if(card->IsKindOf(KCardStatic::card_soldier)){
		if(slot>=0){
			return step_slot;
		}else if(des>0){
			return step_des;
		}
	}
	return step_ok;
}

bool KRecordDataBase::Deserialize( StreamInterface* pDataStream )
{
	pDataStream->ReadData(&m_dlgId,4);
	pDataStream->ReadData(&m_elapsed,sizeof(m_elapsed));
	return true;
}
bool KRecordDataBase::Serialize( StreamInterface* pDataStream )
{
	pDataStream->WriteData(&m_dlgId,4);
	pDataStream->WriteData(&m_elapsed,sizeof(m_elapsed));
	return true;
}

void KRecordPlayOpData::RecordPlayOp(int src,int des,int slot,DWORD initTime)
{
	m_elapsed = GetTickCount()-initTime;
	m_data.set(src,des,slot);
}

bool KRecordPlayOpData::Deserialize(StreamInterface* pDataStream)
{
	KRecordDataBase::Deserialize(pDataStream);
	pDataStream->ReadData(&m_data,sizeof(m_data));
	return true;
}

bool KRecordPlayOpData::Serialize( StreamInterface* pDataStream )
{
	KRecordDataBase::Serialize(pDataStream);
	pDataStream->WriteData(&m_data,sizeof(m_data));
	return true;
}

bool KRecordPlayOpData::Replay(DWORD timeline, int mode)
{
	if(timeline<m_elapsed) return false;

	KClientBattleCtrl* ctrl = (KClientBattleCtrl*)KClientBattleCtrl::getInstance();
	if(ctrl->GetCurState()!=KBattleCtrlBase::battle_play) return false;
	if(ctrl->IsWaitDrama()) return false;
	KBattleCtrlBase::BattleOp& op = ctrl->GetCurOp();
	KBattleCtrlBase::BattleOp tar;
	tar.Set(m_data._src,m_data._des,m_data._slot);

	if(!ctrl->IsMyTurn()|| mode==0){
		
		op.Set(m_data._src,m_data._des,m_data._slot);
		return true;
	}
	if(ctrl->GetCardOwner(m_data._src)!=ctrl->GetMainPlayer()) return false;// cur op isn't my

	Cur_OP_Step step = _GetCurOpStep(op,m_data._src,m_data._des,m_data._slot);
	switch(step){
	case step_src:
		ActiveCard(m_data._src);
		break;
	case step_des:
		ActiveCard(m_data._des);
		break;
	case step_slot:
		{
			KUIAssist::_playClickSlotAction(m_data._slot);
		}
		break;
	case step_area:
		{
			KUIAssist::_playClickFightArea();
		}
		break;
	case step_ok:
		return true;
	default:
		break;
	}
	KUIAssist::_playLessonMsg(m_dlgId*10+step);
	return false;
}

void KRecordPlayOpData::ActiveCard(int realId)
{
 	KCardInst* pCard= KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),realId);
	KUIAssist::_playClickCardAction(pCard);
}

bool KRecordPlayOpData::IsClickFightAreaValidate(DWORD timeline,int slot)
{
	if(timeline<m_elapsed) return false;

	KBattleCtrlBase::BattleOp& op = KClientBattleCtrl::getInstance()->GetCurOp();
	Cur_OP_Step step = _GetCurOpStep(op,m_data._src,m_data._des,m_data._slot);
	if(step==step_slot){
		return (m_data._slot==slot);
	}else if(step==step_area){
		return true;
	}
	return false;
}

bool KRecordPlayOpData::IsClickCardValidate(DWORD timeline,KCardInst* card)
{
	if(timeline<m_elapsed) return false;

	bool ret =false;
	KBattleCtrlBase::BattleOp& op = KClientBattleCtrl::getInstance()->GetCurOp();
	Cur_OP_Step step = _GetCurOpStep(op,m_data._src,m_data._des,m_data._slot);
	switch(step){
	case step_src:
		ret  = (m_data._src ==card->GetRealId());
		break;
	case step_des:
		ret  = (m_data._des ==card->GetRealId());
		break;
	default:
		break;
	}
	return ret;
}

bool KRecordUIMouseData::Deserialize(StreamInterface* pDataStream)
{
	KRecordDataBase::Deserialize(pDataStream);
	char evt;
	pDataStream->ReadData(&evt,sizeof(evt));
	m_evt = (Mouse_evt)evt;
	return true;
}



bool KRecordUIMouseData::Serialize( StreamInterface* pDataStream )
{
	KRecordDataBase::Serialize(pDataStream);
	char evt = (char)m_evt;
	pDataStream->WriteData(&evt,sizeof(evt));
	return true;
}

void KRecordUIMouseData::RecordMouseEvt(Mouse_evt evt,DWORD initTime)
{
	m_elapsed = GetTickCount()-initTime;
	m_evt = evt;
}

bool KRecordUIMouseData::Replay(DWORD timeline,int mode)
{
	if(timeline<m_elapsed) return false;

	KClientBattleCtrl* ctrl = KClientBattleCtrl::getInstance();
	if(ctrl->GetCurState()!=KBattleCtrlBase::battle_play) return false;
	if(ctrl->IsWaitDrama()) return false;
	if(!ctrl->IsMyTurn()|| mode==0){
		switch(m_evt){
		case evt_turn_end:
			KGameRecordMgr::getSingleton().onPlayStepOn();
			ctrl->DoEndTurn();
			break;
		}
		return true;
	}
	switch(m_evt){
	case evt_turn_end:
		KUIAssist::_playClickTurnEnd();
		break;
	}
	KUIAssist::_playLessonMsg(m_dlgId*10);
	return false;
}

bool KRecordUIMouseData::IsClickButValidate(DWORD timeline,cocos2d::CCObject* obj)
{
	if(timeline<m_elapsed) return false;

	UIWidget* pBut = (UIWidget*)obj;
	switch(m_evt){
	case evt_turn_end:
		if(strcmp(pBut->getName(),"turn_end")==0) return true;
		break;
	}
	return false;
}






bool KRecordClickWidget::Deserialize(StreamInterface* pDataStream)
{
	KRecordDataBase::Deserialize(pDataStream);
	pDataStream->ReadData(&m_widgetName,sizeof(m_widgetName));
	pDataStream->ReadData(&m_index,sizeof(m_index));

	return true;
}



bool KRecordClickWidget::Serialize( StreamInterface* pDataStream )
{
	KRecordDataBase::Serialize(pDataStream);
	pDataStream->WriteData(&m_widgetName,sizeof(m_widgetName));
	pDataStream->WriteData(&m_index,sizeof(m_index));
	return true;
}

void KRecordClickWidget::RecordMouseEvt(const char* widgetName,int index,DWORD initTime)
{
	static DWORD lastClickWidget =0;
	DWORD curtime = GetTickCount();
	if(curtime -lastClickWidget <500){
		int kk=0;
	}
	lastClickWidget = curtime;
	m_elapsed = GetTickCount()-initTime;
	strcpy(m_widgetName,widgetName);
	m_index = index;

}

bool KRecordClickWidget::Replay(DWORD timeline,int mode)
{
	if(timeline<m_elapsed) return false;

	KUIAssist::_playClickWidget(m_widgetName);
	//TBD
	KUIAssist::_playLessonMsg(m_dlgId*10);
	return false;
}

bool KRecordClickWidget::IsClickWidgetValidate(cocos2d::CCObject* layer,cocos2d::CCPoint& pt,DWORD timeline)
{
	if(timeline<m_elapsed) return false;
	UIWidget* panel = (UIWidget*)layer;
	UIWidget* root = NULL;
	if(m_index<0){
		root = UIHelper::seekWidgetByName(panel,m_widgetName);
	}else{
		root = KUIAssist::GetIndexWidget(panel,m_widgetName,m_index);
	}

	if ( root && 
		root->isEnabled() && 
		root->isTouchEnabled() && 
		root->hitTest(pt) && 
		root->clippingParentAreaContainPoint(pt))
	{
		return true;
	}

	return false;
}