#include "KRecordData.h"
#include "../GameRoot.h"
#include "../UI/BattleFieldScene.h"

bool KRecordDataBase::Deserialize( StreamInterface* pDataStream )
{
	pDataStream->ReadData(&m_dlgId,4);
	return true;
}
bool KRecordDataBase::Serialize( StreamInterface* pDataStream )
{
	pDataStream->WriteData(&m_dlgId,4);
	return true;
}

void KRecordPlayOpData::RecordPlayOp(int src,int des,int slot)
{
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

bool KRecordPlayOpData::Replay()
{
	KBattleCtrlBase::BattleOp& op = GameRoot::getSingleton().BattleCtrl().GetCurOp();
	if(!op.IsEmpty()) return false;
	if(GameRoot::getSingleton().BattleCtrl().GetCurState()!=KBattleCtrlBase::battle_play) return false;
	op.Set(m_data._src,m_data._des,m_data._slot);
	return true;
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

void KRecordUIMouseData::RecordMouseEvt(Mouse_evt evt)
{
	m_evt = evt;
}

bool KRecordUIMouseData::Replay()
{
	switch(m_evt){
	case evt_turn_end:
		GameRoot::getSingleton().getBattleScene()->onTurnEnd();
		break;
	}
	return true;
}