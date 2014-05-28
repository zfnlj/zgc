#include "KActionMgr.h"
#include "KActionStaticMgr.h"
#include <System/Misc/StrUtil.h>
#include "../UI/KCardActor.h"
#include "../GameLogic/KCardInst.h"
#include "../UI/KUIAssist.h"

using  namespace Ogre;

K3DActionParam* KActionParamQueue::firstParam()
{
	if(_head==_tail) return NULL;
	return &_param[_head];
}

void KActionParamQueue::consumeParam()
{
	if(_head==_tail) return;
	_head++;
	if(_head==MAX_CACHE_ACTION) _head=0;
}

K3DActionParam* KActionParamQueue::findParam(const char* name)
{
	int cur = _head;
	while(cur!=_tail){
		if(strcmp(_param[cur]._name,name)==0) return &_param[cur];
		cur++;
		if(cur==MAX_CACHE_ACTION) cur=0;
	}
	return NULL;
}

void KActionParamQueue::cache(K3DActionParam& param)
{
	int slot = _tail;
	_tail++;
	if(_tail==MAX_CACHE_ACTION) _tail=0;
	CCAssert( _tail!=_head,"Action Cache is full!");
	if(_tail==_head) return;

	K3DActionParam* pSlot = &_param[slot];
	pSlot->clone(&param);

}

void KActionParamQueue::cacheImediate(K3DActionParam& param)
{
	int slot = _head;
	slot--;
	if(slot <0) slot = MAX_CACHE_ACTION-1;
	CCAssert( _tail!=slot,"Action Cache is full!");
	K3DActionParam* pSlot = &_param[slot];
	pSlot->clone(&param);
	_head = slot;
}


KActionMgr::KActionMgr()
{
}

KActionMgr::~KActionMgr()
{
	m_Actor = NULL;
}



void KActionMgr::Init(KActor* actor)
{
	m_Actor = actor;
}

void KActionMgr::PlayCacheAction()
{
	if(FoundClassAction()) return;

	K3DActionParam* param = m_cacheAction.firstParam();
	if(!param) return;

	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param->_name);
	if(pST) createAction(param,0);
	m_cacheAction.consumeParam();
}

void KActionMgr::breathe(float deltaTime)
{
	CCObject* pObj = NULL;
	CCArray delArr;
	CCArray breatheArr;
	breatheArr.initWithArray(&m_ActionArr);
	CCARRAY_FOREACH(&breatheArr, pObj){ //同时只能执行一个有class的Action
		KAction* pAction = (KAction*)pObj;
		pAction->breathe(deltaTime);
		if (!pAction->IsPlay()) delArr.addObject(pAction);
	}

	CCARRAY_FOREACH(&delArr, pObj){
		m_ActionArr.removeObject(pObj);
	}
	PlayCacheAction();
	PlaySlotAction();
}


KAction* KActionMgr::createAction(K3DActionParam* param,int key)
{
	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param->_name);
	if(!pST){
		CCAssert(false , "Error to Create Action!");
		return NULL;
	}
	return CreateOnActionStaticMgr(pST,param,key);
}

KAction* KActionMgr::PlayAction(const char* ActionName,float delayTime,bool bCached) //生成动作
{
	K3DActionParam param;
	param.init(ActionName);
	return PlayAction(&param,bCached);
}

K3DActionParam* KActionMgr::FindActionParam(const char* name)
{
	KAction* pAction = FindAction(name);
	if(pAction) return pAction->GetParam();
	K3DActionParam* param = m_slotAction.findParam(name);
	if(param) return param;
	return m_cacheAction.findParam(name);
}																														 

void KActionMgr::PlaySlotAction()
{
	if(FoundClassAction()) return;
	if(m_slotAction.IsEmpty()) return;

	K3DActionParam* param = m_slotAction.firstParam();
	if(!param) return;

	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param->_name);
	if(!pST){
		m_slotAction.consumeParam();
		return;
	}

	KCardActor* actor = KUIAssist::_getCardActor(param->_srcID);
	if(!actor) actor = (KCardActor*)m_Actor;

	KCardInst* card = actor->GetCard();
	if(!card) return;
	if((int)card->GetSlot()!= pST->GetSlot()) return;
	cocos2d::CCPoint pt = KUIAssist::_queryCardPos(NULL,card);
	if(actor->GetUI()->getPosition().getDistance(pt) >2) return;
	createAction(param,0);
	m_slotAction.consumeParam();
}

KAction* KActionMgr::PlayAction(K3DActionParam* param,int key,bool bCached)//生成动作									 
{
	m_debugInfo +=param->_name;
	m_debugInfo +="#";

	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param->_name);
	if(!pST){
		CCAssert(false , "Error to Create Action!");
		return NULL;
	}
	KCardInst* card = m_Actor->GetCard();
	if(pST->GetSlot()>0){
		m_slotAction.cache(*param);
		return NULL;
	}
	//if(MergeCastAction(pST,param)) return NULL;
	LimitSameClassAction(pST);

	if( (pST->GetClass()!=KActionStatic::class_null&& FoundClassAction())||
		bCached){
		m_cacheAction.cache(*param);
		return NULL;
	}else{
		return createAction(param,key);
	}
}

bool KActionMgr::MergeCastAction(KActionStatic* pST,K3DActionParam* p)
{
	if(pST->GetClass()!=KActionStatic::class_cast) return false;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(pAction->GetParam()->SrcID()== p->SrcID() && pAction->GetStatic()==pST){
			for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
				if(p->_desArr[i]==0) break;
				pAction->GetParam()->SetDestVal(p->_desArr[i],p->_desValArr[i]);
			}
			return true;
		}
	}
	return false;
}

void KActionMgr::CacheImediateAction(K3DActionParam& param)
{
	m_cacheAction.cacheImediate(param);
}

KAction* KActionMgr::CreateOnActionStaticMgr(KActionStatic* pST,K3DActionParam* p,int key)
{
	KAction* pAction = KAction::create(pST,this,p,key);
	m_ActionArr.addObject(pAction);
	return pAction;
}

void KActionMgr::LimitAlive(const char* name)
{
	KAction* pAction = FindAction(name);
	if(pAction)
		pAction->LimitAlive();
	K3DActionParam* param = m_cacheAction.findParam(name);
	if(param) param->SetAction("");
	param = m_slotAction.findParam(name);
	if(param) param->SetAction("");
}

void KActionMgr::onDestory(void)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		pAction->Stop();
	}
	m_ActionArr.removeAllObjects();
}

K3DActionParam* KActionMgr::ExistAction(const char* name)
{
	KAction* pAction = FindAction(name);
	if(pAction) return pAction->GetParam();
	return FindActionParam(name);
}

KAction* KActionMgr::FindAction(const char* name)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(strcmp(name,pAction->GetName())==0) return pAction;
	}
	return NULL;
}

void KActionMgr::LimitClassAction(KActionStatic::classEnum cls)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(pAction->GetClass()==cls) pAction->LimitAlive();
	}
}

void KActionMgr::LimitSameClassAction(KActionStatic* pST) //class 等级高的action会停止低级的action
{
	if(pST->GetClass()!=KActionStatic::class_click) return;

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(pAction->GetClass()==pST->GetClass()) pAction->LimitAlive();
	}
}

bool KActionMgr::FoundClassAction()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(pAction->GetClass()!=KActionStatic::class_null) return true;
	}
	return false;
}
