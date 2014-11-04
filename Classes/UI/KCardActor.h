#ifndef _KCARDACTOR_H
#define _KCARDACTOR_H

#include "cocos2d.h"
#include "../KAction/KActionMgr.h"
#include "../BaseClasses/UIWidget.h"
#include "KActor.h"
#include "PlayerGuide.h"

USING_NS_CC;
class KCardInst;
struct K3DActionParam;

class KCardActor :public KActor
{
public:
	KCardActor(){}
	~KCardActor();
	static KCardActor* create(KCardInst*,bool bBig=false);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void update(float dt);
	cocos2d::extension::UIWidget* GetUI();
	virtual void UpdateCardAttr();
	virtual void UpdateCardAttr(cocos2d::extension::UIWidget*,bool bBigCard,bool bInit=false,K3DActionParam* hitParam=NULL);
	virtual void MoveOnHit(K3DActionParam*,float speed,CCActionDef& actionDef);
	virtual cocos2d::extension::UIWidget* GetWidget(const char* obj);
	virtual void MoveBack(float speed);//“∆∂Øªÿ≤€Œª
	virtual CCNode* ShowHit(const char* slot,K3DActionParam* param,float scale,bool bGood);
	virtual CCPoint GetDestPosition(K3DActionParam* param,const char* obj,int index);
	void DoSelect(CCObject* sender);
	void onMoveEvent(CCObject* sender);
	CC_SYNTHESIZE(bool,m_bBack,Back);
	void UpdateUI(bool bFlip=false);
	cocos2d::extension::UIWidget* GetBigCard();
	void initTombAction(K3DActionParam* param);
	virtual void MoveReached(CCObject* sender);
	virtual void DrawCard();
	bool m_bSelected;
	void ActiveGreen();
	void ActiveRed();
	void Deactive();
    virtual KCardInst* GetCard(){ return m_card;}
	virtual void SummonSelf();
	virtual void UpdateCardBuf();

	virtual void addWidget(const char* obj,int zOrder);
	virtual void delWidget(const char* obj);
	virtual void updateSecret();
	virtual CCNode* GetCNode(const char* obj);
	void Clear();
	CCSprite* GetActiveRedSprite();
	CCSprite* GetActiveGreenSprite();

	virtual void resortZOrder(int val);
	void RemoveSelectImg();
private:
	void init(KCardInst*,bool bBig=false,bool bFlip=false);
	bool DoSelectBeginCard(CCObject* sender);
	KCardInst* m_card;
	cocos2d::extension::UIWidget* m_bigPhoto;
	K3DActionParam m_tombAction;
	CCSprite* m_activeRedSprite;
	CCSprite* m_activeGreenSprite;
	int m_curSlot;
	
};

#endif // __KCardActor_H__
