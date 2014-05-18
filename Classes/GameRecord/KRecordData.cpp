#include "KRecordData.h"
#include "../GameRoot.h"
#include "../UI/BattleFieldScene.h"
#include "../UI/KUIAssist.h"
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
	KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(src);
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

	KClientBattleCtrl& ctrl = GameRoot::getSingleton().BattleCtrl();
	if(ctrl.GetCurState()!=KBattleCtrlBase::battle_play) return false;
	if(ctrl.IsWaitDrama()) return false;
	KBattleCtrlBase::BattleOp& op = ctrl.GetCurOp();
	KBattleCtrlBase::BattleOp tar;
	tar.Set(m_data._src,m_data._des,m_data._slot);

	if(!ctrl.IsMyTurn()|| mode==0){
		
		op.Set(m_data._src,m_data._des,m_data._slot);
		return true;
	}
	if(ctrl.GetCardOwner(m_data._src)!=ctrl.GetMainPlayer()) return false;// cur op isn't my

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
	
	return false;
}

void KRecordPlayOpData::ActiveCard(int realId)
{
 	KCardInst* pCard= GameRoot::getSingleton().BattleCtrl().GetCard(realId);
	KUIAssist::_playClickCardAction(pCard);
}

bool KRecordPlayOpData::IsClickFightAreaValidate(int slot)
{
	KClientBattleCtrl& ctrl = GameRoot::getSingleton().BattleCtrl();
	KBattleCtrlBase::BattleOp& op = ctrl.GetCurOp();
	Cur_OP_Step step = _GetCurOpStep(op,m_data._src,m_data._des,m_data._slot);
	if(step==step_slot){
		return (m_data._slot==slot);
	}else if(step==step_area){
		return true;
	}
	return false;
}

bool KRecordPlayOpData::IsClickCardValidate(KCardInst* card)
{
	bool ret =false;
	KClientBattleCtrl& ctrl = GameRoot::getSingleton().BattleCtrl();
	KBattleCtrlBase::BattleOp& op = ctrl.GetCurOp();
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

	KClientBattleCtrl& ctrl = GameRoot::getSingleton().BattleCtrl();
	if(ctrl.GetCurState()!=KBattleCtrlBase::battle_play) return false;
	if(ctrl.IsWaitDrama()) return false;
	if(!ctrl.IsMyTurn()|| mode==0){
		switch(m_evt){
		case evt_turn_end:
			GameRoot::getSingleton().getBattleScene()->onTurnEnd();
			break;
		}
		return true;
	}
	switch(m_evt){
	case evt_turn_end:
		KUIAssist::_playClickTurnEnd();
		break;
	}
	return false;
}

bool KRecordUIMouseData::IsClickButValidate(cocos2d::CCObject* obj)
{
	UIWidget* pBut = (UIWidget*)obj;
	switch(m_evt){
	case evt_turn_end:
		if(strcmp(pBut->getName(),"turn_end")==0) return true;
		break;
	}
	return false;
}