#include "KSocketFacade.h"
#include "KClientGateListener.h"
#include "../GameRoot.h"

void KSocketFacade::DoSelectBeginCard(KCardInstList* lst)
{
	KBufferOutputStream<1024> so;
	for(KCardInstList::iterator it = lst->begin();it!= lst->end();it++){
		so.WriteInt((*it)->GetRealId());
	}
	KClientGateListener::getSingleton().SendToGameServer(s_nCS_BT_SelectCardOK,so.data(),so.size());
}

void KSocketFacade::DoBattleOpDone(KBattleCtrlBase::BattleOp* p)
{
	CS_BattleOpDone op;
	op._src = p->_src;
	op._des = p->_des;
	GameRoot::getSingleton().BattleCtrl().GetCurOp().Empty();
	KClientGateListener::getSingleton().SendToGameServer( op.s_nCode,&op,sizeof(op));
}

void KSocketFacade::DoEndTurn()
{
	KClientGateListener::getSingleton().SendToGameServer(s_nCS_BT_EndTurn,NULL,0);
}

void KSocketFacade::DoAcceptQuest(int id)
{
	CS_AcceptQuest op;
	op.qid = id;
	KClientGateListener::getSingleton().SendToGameServer( op.s_nCode,&op,sizeof(op));
}

void KSocketFacade::DoSubmitQuest(int id)
{
}