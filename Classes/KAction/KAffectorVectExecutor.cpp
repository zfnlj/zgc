#include "KAffectorVectExecutor.h"
#include "KActionStaticMgr.h"
#include "KAffectorVectExecutor.h"
//#include "KAffectorAlphaExecutor.h"
#include "KAffectorStatic.h"
#include "KAction.h"

KAffectorVectExecutor::KAffectorVectExecutor()
{
}


void KAffectorVectExecutor::Init(KAction* pAction)
{
	m_pAction = pAction;
	const CCArray& arr = m_pAction->GetStatic()->m_AffectorVect;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&arr, pObj){
		KAffectorStatic* st = (KAffectorStatic*)pObj;
		KAffectorExecutor* pExec = CreateExecFactory(st);
		if (NULL == pExec)	continue;
		pExec->Init(m_pAction,st->GetCreateTime());
		AddChild(pExec);
	}
}


void KAffectorVectExecutor::OnPlayExecutor(AdvExecutor* exec)
{
	KAffectorExecutor* objExc = (KAffectorExecutor*)(exec);
	if(objExc)
		objExc->OnPlay(m_pAction->GetParam());//参数变量传给生成的3DNode管理，由其负责删除
}

KAffectorExecutor* KAffectorVectExecutor::GetExecutor(AffectorType tp)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		KAffectorExecutor* exec = (KAffectorExecutor*)pObj;
		if(exec->Type()==tp) return exec;
	}
	return NULL;
}

KAffectorExecutor* KAffectorVectExecutor::CreateExecFactory(KAffectorStatic* st)
{
	KAffectorExecutor* exec = NULL;
	switch(st->Type())
	{
	case Affector_action:
		exec = new KAffectorActionExecutor;
			break;
	case Affector_missile:
		exec = new KAffectorMissileExecutor;
		break;
	case Affector_indicate:
		exec = new KAffectorIndicateExecutor;
		break;
	case Affector_timerBar:
		exec = new KAffectorTimerBarExecutor;
		break;
	default :
		exec = new KAffectorExecutor;
		break;
	}
	exec->init(st);
	exec->autorelease();
	return exec;
}

void KAffectorVectExecutor::OnStopExecutor(AdvExecutor* exec)
{
	if(exec)
	{
		KAffectorExecutor* objExec = (KAffectorExecutor*)(exec);
		objExec->OnStop();
	}
}



