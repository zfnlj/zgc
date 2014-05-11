/********************************************************************
	created:	
	created:	
	filename: 	
	file path:	
	file base:	
	file ext:	
	author:		
	
	purpose:	
*********************************************************************/
#pragma once

#include "AdvExecutor.h"
#include "KAffectorStatic.h"
#include "K3DActionParam.h"
#include "cocos-ext.h"
class KCardActor;

USING_NS_CC;
struct CCActionDef{
	CCAction* _action;
	CCNode* _node;
	CCActionDef():_action(NULL),_node(NULL){}
	~CCActionDef();
	void init(CCAction* action,CCNode* node){
		_action = action;
		_node = node;
		_action->retain();
	}
	void empty();
};
class KAffectorExecutor :public AdvExecutor
{
protected:
	KAffectorStatic* m_AffectorStatic;
	K3DActionParam* m_param;
	CCParticleSystem*    m_emitter;
	CCSprite* m_sprite;
	cocos2d::extension::CCArmature* m_armature;
	CCAction* m_ccAction;
	CCActionDef m_ccActionDef;
public:
	KAffectorExecutor();
	virtual void init(KAffectorStatic* st);
	~KAffectorExecutor();
	virtual void OnPlay(K3DActionParam*);
	virtual void OnStop(void);
	virtual void Stop();
	virtual float Breathe(float frameTime);
	void StopEmitter();

	AffectorType Type(void){ return m_AffectorStatic->Type();}
	virtual void LimitAlive(void);
	virtual void LimitAlive(float val);
};

class KAffectorMissileExecutor: public KAffectorExecutor
{
public:
	KAffectorMissileExecutor();
	~KAffectorMissileExecutor();
	virtual void OnPlay(K3DActionParam*);
	virtual void OnStop(void);
	void callbackMissileReach();
private:
	cocos2d::CCPoint m_srcPos;
};

class KAffectorActionExecutor: public KAffectorExecutor
{
public:
	KAffectorActionExecutor(){}
	~KAffectorActionExecutor(){}
	virtual float Breathe(float frameTime);
    bool IsTargetFlagMatch(int id);

};


class KAffectorIndicateExecutor: public KAffectorExecutor
{
public:
	KAffectorIndicateExecutor();
	~KAffectorIndicateExecutor();
	virtual void OnPlay(K3DActionParam*);
	virtual void OnStop(void);
	void callbackMissileReach();
private:
	CCProgressTimer* m_bar;
};

class KAffectorTimerBarExecutor: public KAffectorExecutor
{
public:
	KAffectorTimerBarExecutor();
	~KAffectorTimerBarExecutor();
	virtual void OnPlay(K3DActionParam*);
	virtual void OnStop(void);
private:
	CCProgressTimer* m_bar;
};