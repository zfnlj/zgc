#include "KAction.h"
#include <math.h>
#include "KActionStaticMgr.h"
#include "KActionMgr.h"
//-----------------------------------------------------------------------

int KAction::g_totalClassAction = 0;

KAction::KAction()

{

}

//-----------------------------------------------------------------------
KAction::~KAction()
{
	if(m_parent){
		m_parent->AddFinishedAction(m_key);
		CC_SAFE_RELEASE(m_parent);
	}
	if(m_bPlay)
		Stop();
	if(m_pActionStatic && 
		m_pActionStatic->GetClass()!=KActionStatic::class_null && 
		m_pActionStatic->GetClass()!=KActionStatic::class_click) g_totalClassAction--;
}

KAction* KAction::create(KActionStatic* pST,KActionMgr* pMgr,K3DActionParam* pParam,int key)
{
	KAction* pAction = new KAction;
	
	pAction->Init(pST,pMgr,pParam,key);
	pAction->autorelease();
	return pAction;
}

void KAction::Init(KActionStatic* pST,KActionMgr* pMgr,K3DActionParam* pParam,int key)
{
	m_elapsed = 0.0f;
	m_key = key;
	m_parent = NULL;
	m_delayTime = 0;
	m_Mgr = pMgr;
	m_bPlay = true;
	m_pActionStatic = pST;
	m_bLoop = pST->IsLoop();
	m_Param.clone(pParam);
	m_Name = pParam->_name;
	if(GetStatic())
	{
		//m_EffectMgr.Init(this,m_Param);
		m_AffectMgr.Init(this);
		//m_SoundMgr.Init(this,NULL);
		if(m_pActionStatic->GetClass()!=KActionStatic::class_null && m_pActionStatic->GetClass()!=KActionStatic::class_click) {
			g_totalClassAction++;
		}
	}
}

//-----------------------------------------------------------------------
void KAction::LimitAlive(float val)
{
	
	m_bLoop = false;
	//m_EffectMgr.LimitAlive(0);
	m_AffectMgr.LimitAlive(val);
	//m_ObjectMgr.LimitAlive(0);
	//m_SoundMgr.LimitAlive(0.5);
}

bool KAction::IsPlay()
{
	return m_bPlay;
}

//-----------------------------------------------------------------------
bool KAction::Stop()
{
	LimitAlive(0.01f);
	breathe(2);
	m_bPlay = false;
	return true;
}

//-----------------------------------------------------------------------
void KAction::breathe(float timeElapsed)
{
	timeElapsed = BreatheDelayTime(timeElapsed);

	m_elapsed += timeElapsed;
	if(m_elapsed>6.0f  && m_AffectMgr.getSize()>4){
		int kk=0;
	}
	if(timeElapsed <=0.0)
	{
		return;
	}
	if (m_bPlay )
	{
		//m_EffectMgr.BreatheI(timeElapsed);
		m_AffectMgr.BreatheImp(timeElapsed);
		//m_ObjectMgr.BreatheI(timeElapsed); //ObjectMgr放最后，否则可能出现对象创建不成功问题
		//m_SoundMgr.BreatheI(timeElapsed);
		UpdataPlayStatus();
	}
	
}

float KAction::BreatheDelayTime(float timeElapsed)
{
	if(m_delayTime > timeElapsed)
	{
		m_delayTime -= timeElapsed;
		return 0;
	}
	else
	{
		timeElapsed -= m_delayTime;
		m_delayTime = 0;
		return timeElapsed;
	}
}

void KAction::UpdataPlayStatus()
{
	m_bPlay =  m_AffectMgr.IsPlaying();//(m_EffectMgr.IsPlaying() || m_AffectMgr.IsPlaying() || m_SoundMgr.IsPlaying()) ? true : false;
	if(!m_bPlay){
		if(m_bLoop){
			Reset();
		}else{
			Stop();
		}
	}
}

void KAction::Reset()
{
	m_bPlay = true;
	//m_EffectMgr.Reset();
	//m_ObjectMgr.Reset();
	m_AffectMgr.Reset();
	//m_SoundMgr.Reset();
	
	//m_EffectMgr.Init(this,&m_Param);
	//m_ObjectMgr.Init(this,&m_Param);
	m_AffectMgr.Init(this);

	//m_SoundMgr.Init(this,NULL);

}


void KAction::SetDelayTime(float delay)
{
	m_delayTime = delay;
}

KActor* KAction::GetActor() 
{
	if(!m_Mgr) return NULL;
	return m_Mgr->GetActor();
}

void KAction::AddFinishedAction(int id)
{
	m_finishAction.push_back(id);
}

bool KAction::IsActonFinished(int id)
{
	if(id==0) return true;
	for(KIntegerList::iterator it=m_finishAction.begin();it!=m_finishAction.end();it++){
		if(*it==id) return true;
	}
	return false;
}