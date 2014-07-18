#ifndef _KACTOR_H
#define _KACTOR_H

#include "cocos2d.h"
#include "../KAction/KActionMgr.h"
#include "../BaseClasses/UIWidget.h"

USING_NS_CC;
struct K3DActionParam;

class KActor :public CCObject
{
public:
	KActor();
	~KActor();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void update(float dt);
	virtual void init(cocos2d::extension::UIWidget* ui);
	cocos2d::extension::UIWidget* GetUI();
	void Remove(const char* obj);
	virtual void UpdateCardAttr(){}
	virtual void UpdateCardAttr(cocos2d::extension::UIWidget*,bool bBig,bool bInit,K3DActionParam* hitParam){}
	virtual void MoveOnHit(K3DActionParam*,float speed,CCActionDef& actionDef){}
	virtual void UpdateCardBuf(){}
	void Move(const char* obj,const char* slot,float speed,CCActionDef& actionDef);
	void Move(const char* obj,CCPoint& destPt,float fSpeed,CCActionDef& actionDef);
	virtual void MoveBack(float speed){}//“∆∂Øªÿ≤€Œª
	virtual CCAction* FadeIn(const char* obj,float val,CCActionDef& actionDef);
	virtual CCAction* FadeOut(const char* obj,float val,CCActionDef& actionDef);
	virtual CCAction* Scale(const char* obj,float val,float elapse,CCActionDef& actionDef);
	virtual CCAction* ScaleX(const char* obj,float val,float elapse,CCActionDef& actionDef);
	CCSprite* CreateSprite(const char* obj,const char* slot,float scale,int order);
	void Shake(const char* obj,float valX,float valY,float elapse,CCActionDef& actionDef);
	void Filp(const char* obj,float elapse);
	void ParticleFire(float elapse);
	void SetVisible(const char* obj,bool flag);
	void ShowEff(CCParticleSystem* emitter,CCPoint* pt);
	void RemoveEff(CCParticleSystem* emitter,const char* obj);
	KActionMgr& GetActionMgr(){ return m_ActionMgr;}
	void setZOrder(const char* obj,int val);
	CCNode* GetDictObj(const char* obj);
	void DictObjSetOpacity(const char* obj,int val);
	void RemoveCCAction(const char* obj);	
	virtual CCNode* ShowHit(const char* obj,K3DActionParam*,float scale,bool bGood){return NULL;}
	virtual cocos2d::extension::UIWidget* GetWidget(const char* obj);
	virtual CCNode* GetCNode(const char* obj);
	virtual void MoveReached(CCObject* sender){}
	virtual void SummonCard(int);
	virtual void SummonSelf(){}
	virtual void ShowCard(int realId);
	virtual void DrawCard(){}
	virtual void AtkMove(int des,float val,CCActionDef& actionDef);
	void RemoveDictObj(CCNode* node,const char* obj);
	CCSprite* CreateAnim(const char* obj,const char* slot,float, int zOrder,float offsetY,bool);
	cocos2d::extension::CCArmature* CreateArmature(K3DActionParam*,const char* obj,const char* slot,float scale,float yOffset,int zOrder);
	CCParticleSystem* CreateEff(const char* obj,const char* slot,int zOrder,float scale);
	virtual cocos2d::extension::UIWidget* CreateTalk(const char* obj,const char* slot,int z, int msgId);
	virtual CCPoint GetDestPosition(K3DActionParam* param,const char* obj,int index);
	virtual KCardInst* GetCard(){ return NULL;}
	virtual void addWidget(const char* obj,int zOrder){}
	virtual void delWidget(const char* obj){}
	virtual void updateSecret(){}
	virtual CCPoint GetDestPosition();
	void updateRes();
	void StopBreathe(const char* obj);
	void StartBreathe(const char* obj,float val,float elapse,CCActionDef& actionDef);
protected:
	cocos2d::extension::UIWidget* m_ui;
	KActionMgr m_ActionMgr;
	CCDictionary m_objectDict;
	float m_backup;
};

#endif // __KCardActor_H__
