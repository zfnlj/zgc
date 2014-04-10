/********************************************************************
	created:	2011/04/13
	created:	13:4:2011   14:00
	filename: 	e:\Work\petworkshopV001\AthenaClient\EngineEx\K3DBase\KAction\KActionMgr.h
	file path:	e:\Work\petworkshopV001\AthenaClient\EngineEx\K3DBase\KAction
	file base:	KActionMgr
	file ext:	h
	author:		zhufanan
	
	purpose:	动作组管理器，实现前后动作的融合
*********************************************************************/
#pragma once

#include "KAction.h"
#include <list>

#define  MAX_CACHE_ACTION 10
struct K3DActionParam;
class KActor;
class KActionMgr
{
public:
	KActionMgr();
	virtual ~KActionMgr();
	KAction*	CreateOnActionStaticMgr(KActionStatic* pST,K3DActionParam* p,int key);
	KAction*	createAction(K3DActionParam* param,int key);//生成动作
	KAction*	PlayAction(const char* ActionName,float delayTime=0.0f,bool bCached=false); //生成动作
	KAction*	PlayAction(K3DActionParam* param,int key=0,bool bCached=false);//生成动作

	K3DActionParam* FindActionParam(const char* name);
	KAction*	FindAction(const char* name);
	void        LimitAlive(const char* name);//促死

	virtual void breathe(float deltaTime);		// 更新动作组，需要查找优先级最高的动作组播动作，并且并行的播放特效
	/// dwExclusiveType valued 0 will not exclude any action

	void		Init(KActor* actor);
	void        onDestory(void);
	CCArray m_ActionArr;
	void	CacheAction(K3DActionParam&);
	void	CacheImediateAction(K3DActionParam& param);
	KActor* GetActor(){ return m_Actor;}
protected:
	
	KActor* m_Actor;
	K3DActionParam m_cacheAction[MAX_CACHE_ACTION];
	K3DActionParam m_slotAction;//到达slot才触发的Action
	int m_cacheHead;
	int m_cacheTail;
	std::string m_debugInfo;
private:
	bool MergeCastAction(KActionStatic* pST,K3DActionParam* p);
	bool pickCacheActionParam(K3DActionParam& param);
	void PlayCacheAction();
	void PlaySlotAction();
	bool FoundClassAction();
	void LimitSameClassAction(KActionStatic* pST);
};


