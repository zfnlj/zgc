#include "KClickCardMgr.h"
#include "KCardActor.h"
#include "../GameLogic/KCardInst.h"

template<> KClickCardMgr* Singleton<KClickCardMgr>::mSingleton = 0;

KClickCardMgr& KClickCardMgr::getSingleton(void)
{  
	if(!mSingleton){
		mSingleton = new KClickCardMgr;
		mSingleton->init();
	}
	return ( *mSingleton );  
}

KClickCardMgr::KClickCardMgr()
{
}

KClickCardMgr::~KClickCardMgr()
{
}

void KClickCardMgr::init()
{
	m_curActor = NULL;
}

void KClickCardMgr::onClickCard(KCardActor* actor)
{
	if(m_curActor==actor) return;
	if(m_curActor && actor){
		K3DActionParam param1;
		param1.init("bigCard_switch");
		param1._srcID = m_curActor->GetCard()->GetRealId();
		param1.SetDestVal(actor->GetCard()->GetRealId(),0);
		actor->GetActionMgr().PlayAction(&param1);
	}else{
		actor->GetActionMgr().PlayAction("bigCard_show");
	}
	m_curActor = actor;
}

void KClickCardMgr::onTouchEnd()
{
	/*if(m_curActor){
		m_curActor->GetActionMgr().PlayAction("bigCard_hide");
		m_curActor = NULL;
	}*/
}