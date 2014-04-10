#include "KActionMgr.h"
#include "KActionStaticMgr.h"
#include <System/Misc/StrUtil.h>
#include "../UI/KCardActor.h"
#include "../GameLogic/KCardInst.h"
#include "../UI/KUIAssist.h"

using  namespace Ogre;

KActionMgr::KActionMgr()
{
}

KActionMgr::~KActionMgr()
{
	m_Actor = NULL;
}



void KActionMgr::Init(KActor* actor)
{
	m_slotAction.init();
	m_Actor = actor;
	m_cacheHead = m_cacheTail = 0;
}

bool KActionMgr::pickCacheActionParam(K3DActionParam& param)
{
	if(m_cacheHead==m_cacheTail) return false;
	param.clone(&m_cacheAction[m_cacheHead]);
	m_cacheHead++;
	if(m_cacheHead==MAX_CACHE_ACTION) m_cacheHead=0;
	return true;
}
void KActionMgr::PlayCacheAction()
{
	if(m_cacheHead==m_cacheTail) return;
	if(FoundClassAction()) return;

	K3DActionParam& param = m_cacheAction[m_cacheHead];
	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param._name);
	createAction(&m_cacheAction[m_cacheHead],0);
	m_cacheHead++;
	if(m_cacheHead==MAX_CACHE_ACTION) m_cacheHead=0;
}

void KActionMgr::breathe(float deltaTime)
{
	CCObject* pObj = NULL;
	CCArray delArr;
	CCARRAY_FOREACH(&m_ActionArr, pObj){ //同时只能执行一个有class的Action
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
	for(int i=0;i<MAX_CACHE_ACTION;i++){
		if(strcmp(m_cacheAction[i]._name,name)==0) return &m_cacheAction[i];
	}
	return NULL;
}																														 

void KActionMgr::PlaySlotAction()
{
	if(m_slotAction.IsEmpty()) return;
	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(m_slotAction._name);
	if(!pST) return;
	KCardInst* card = m_Actor->GetCard();
	if(!card) return;
	if((int)card->GetSlot()!= pST->GetSlot()) return;
	cocos2d::CCPoint pt = KUIAssist::_queryCardPos(NULL,card);
	if(m_Actor->GetUI()->getPosition().getDistance(pt) >2) return;
	createAction(&m_slotAction,0);
	m_slotAction.init();
}

KAction* KActionMgr::PlayAction(K3DActionParam* param,int key,bool bCached)//生成动作									 
{
	m_debugInfo +=param->_name;
	m_debugInfo +="#";

	KActionStatic* pST = KActionStaticMgr::getSingleton().GetAction(param->_name);
	if(!pST){
		//CCAssert(false , "Error to Create Action!");
		return NULL;
	}
	KCardInst* card = m_Actor->GetCard();
	if(pST->GetSlot()>0){
		m_slotAction.clone(param);
		return NULL;
	}
	if(MergeCastAction(pST,param)) return NULL;
	//LimitClassAction(pST);

	if( (pST->GetClass()!=KActionStatic::class_null&& FoundClassAction())||
		bCached){
		CacheAction(*param);
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
		if(pAction->GetStatic()==pST){
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
	int slot = m_cacheHead;
	slot--;
	if(slot <0) slot = MAX_CACHE_ACTION-1;
	CCAssert( m_cacheTail!=slot,"Action Cache is full!");
	K3DActionParam* pSlot = &m_cacheAction[slot];
	pSlot->clone(&param);
	m_cacheHead = slot;
}

void KActionMgr::CacheAction(K3DActionParam& param)
{
	int slot = m_cacheTail;
	m_cacheTail++;
	if(m_cacheTail==MAX_CACHE_ACTION) m_cacheTail=0;
	CCAssert( m_cacheTail!=m_cacheHead,"Action Cache is full!");
	if(m_cacheTail==m_cacheHead) return;

	K3DActionParam* pSlot = &m_cacheAction[slot];
	pSlot->clone(&param);

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

KAction* KActionMgr::FindAction(const char* name)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(strcmp(name,pAction->GetName())==0) return pAction;
	}
	return NULL;
}

void KActionMgr::LimitSameClassAction(KActionStatic* pST) //class 等级高的action会停止低级的action
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&m_ActionArr, pObj){
		KAction* pAction = (KAction*)pObj;
		if(pAction->GetClass()==KActionStatic::class_null) continue;
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
