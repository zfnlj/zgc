#include "KClickCardMgr.h"
#include "KCardActor.h"
#include "../GameLogic/KCardInst.h"
#include "../GameRoot.h"
#include "BattleFieldScene.h"

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
	m_cacheActor = NULL;
	m_CardElapsed = 0.0f;
}

void KClickCardMgr::onClickCard(KCardActor* actor)
{
	KActor& mainActor = GameRoot::getSingleton().getBattleScene()->GetActor();
	if(mainActor.GetActionMgr().FindAction("bigCard_switch")){
		m_cacheActor = actor;
		return;
	}
	m_CardElapsed = 0;
	m_bTouchEnd = false;
	if(m_curActor==actor) return;
	if(m_curActor && actor){
		K3DActionParam param1;
		param1.init("bigCard_switch");
		param1._srcID = m_curActor->GetCard()->GetRealId();
		param1.SetDestVal(actor->GetCard()->GetRealId(),0);
		mainActor.GetActionMgr().PlayAction(&param1);
	}else{
		actor->GetActionMgr().PlayAction("bigCard_show");
	}
	m_curActor = actor;
	
}

void KClickCardMgr::onTouchEnd()
{
	m_bTouchEnd = true;
	/*if(m_curActor){
		m_curActor->GetActionMgr().PlayAction("bigCard_hide");
		m_curActor = NULL;
	}*/
}

void KClickCardMgr::update(float dt)
{
	KActor& mainActor = GameRoot::getSingleton().getBattleScene()->GetActor();
	if(m_cacheActor && !mainActor.GetActionMgr().FindAction("bigCard_switch")){
		onClickCard(m_cacheActor);
		m_cacheActor = NULL;
	}
	if(m_bTouchEnd) m_CardElapsed+= dt;
	if(m_CardElapsed>3 && m_curActor){
		m_curActor->GetActionMgr().PlayAction("bigCard_hide");
		m_curActor = NULL;
	}
}