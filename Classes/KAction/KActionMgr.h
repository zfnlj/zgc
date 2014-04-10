/********************************************************************
	created:	2011/04/13
	created:	13:4:2011   14:00
	filename: 	e:\Work\petworkshopV001\AthenaClient\EngineEx\K3DBase\KAction\KActionMgr.h
	file path:	e:\Work\petworkshopV001\AthenaClient\EngineEx\K3DBase\KAction
	file base:	KActionMgr
	file ext:	h
	author:		zhufanan
	
	purpose:	�������������ʵ��ǰ�������ں�
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
	KAction*	createAction(K3DActionParam* param,int key);//���ɶ���
	KAction*	PlayAction(const char* ActionName,float delayTime=0.0f,bool bCached=false); //���ɶ���
	KAction*	PlayAction(K3DActionParam* param,int key=0,bool bCached=false);//���ɶ���

	K3DActionParam* FindActionParam(const char* name);
	KAction*	FindAction(const char* name);
	void        LimitAlive(const char* name);//����

	virtual void breathe(float deltaTime);		// ���¶����飬��Ҫ�������ȼ���ߵĶ����鲥���������Ҳ��еĲ�����Ч
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
	K3DActionParam m_slotAction;//����slot�Ŵ�����Action
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


