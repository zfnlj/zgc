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
	virtual void UpdateCardBuf(){}
	void Move(const char* obj,const char* slot,float speed);
	virtual void MoveBack(float speed){}//“∆∂Øªÿ≤€Œª
	virtual CCAction* FadeIn(const char* obj,float val);
	virtual CCAction* FadeOut(const char* obj,float val);
	virtual CCAction* Scale(const char* obj,float val,float elapse);
	CCSprite* CreateSprite(const char* obj,const char* slot,float scale,int order);
	void Shake(const char* obj,float valX,float valY,float elapse);
	void Filp(const char* obj,float elapse);
	void ParticleFire(float elapse);
	void SetVisible(const char* obj,bool flag);
	void ShowEff(CCParticleSystem* emitter,CCPoint* pt);
	void RemoveEff(CCParticleSystem* emitter,const char* obj);
	KActionMgr& GetActionMgr(){ return m_ActionMgr;}
	void setZOrder(const char* obj,int val);
	CCNodeRGBA* GetRenderer(const char* obj);
	void RemoveCCAction(const char* obj);	
	virtual void ShowHit(const char* obj,K3DActionParam*,float scale,bool bGood){}
	void AddWidget(const char* obj,const char* slot);
	void DelWidget(const char* obj);
	virtual cocos2d::extension::UIWidget* GetWidget(const char* obj);
	CCNode* GetCNode(const char* obj);
	virtual void MoveReached(CCObject* sender){}
	virtual void SummonCard(int);
	virtual void SummonSelf(){}
	virtual void ShowCard(int realId);
	virtual void DrawCard(){}
	virtual void AtkMove(int des,float val);
	void RemoveSprite(CCSprite* sprite,const char* obj);
	CCSprite* CreateAnim(const char* obj,const char* slot,int zOrder);
	cocos2d::extension::CCArmature* CreateArmature(const char* obj,const char* slot,float scale,int zOrder);
	CCParticleSystem* CreateEff(const char* obj,const char* slot,int zOrder,float scale);
	virtual CCPoint GetDestPosition(K3DActionParam* param,const char* obj,int index);
	virtual KCardInst* GetCard(){ return NULL;}
protected:
	cocos2d::extension::UIWidget* m_ui;
	KActionMgr m_ActionMgr;
	CCDictionary m_spriteDict;
};

#endif // __KCardActor_H__
