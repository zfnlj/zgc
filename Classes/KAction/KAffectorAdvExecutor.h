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

class KAffectorExecutor :public AdvExecutor
{
protected:
	KAffectorStatic* m_AffectorStatic;
	K3DActionParam* m_param;
	CCParticleSystem*    m_emitter;
	CCSprite* m_sprite;
	cocos2d::extension::CCArmature* m_armature;
public:
	KAffectorExecutor();
	virtual void init(KAffectorStatic* st);
	~KAffectorExecutor();
	virtual void OnPlay(K3DActionParam*);
	virtual void OnStop(void);
	virtual void Stop();
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
	float m_totalTime;
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
	float m_totalTime;
};
