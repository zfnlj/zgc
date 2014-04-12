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
	static KCardActor* create(KCardInst*);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual void update(float dt);
	cocos2d::extension::UIWidget* GetUI();
	virtual void UpdateCardAttr();
	virtual void UpdateCardAttr(cocos2d::extension::UIWidget*,bool bInit=false);
	virtual cocos2d::extension::UIWidget* GetWidget(const char* obj);
	virtual void MoveBack(float speed);//“∆∂Øªÿ≤€Œª
	virtual void ShowHit(const char* slot,K3DActionParam* param,float scale,bool bGood);
	virtual CCPoint GetDestPosition(K3DActionParam* param,const char* obj,int index);
	void DoSelect(CCObject* sender);
	CC_SYNTHESIZE(bool,m_bBack,Back);
	void UpdateUI();
	cocos2d::extension::UIWidget* GetBigCard();
	void initTombAction(K3DActionParam* param);
	virtual void MoveReached(CCObject* sender);
	virtual void DrawCard();
	bool m_bSelected;
	void ActiveGreen();
	void ActiveRed();
	void Deactive();
	bool IsActive();
    virtual KCardInst* GetCard(){ return m_card;}
	virtual void SummonSelf();
	virtual void UpdateCardBuf();
private:
	void init(KCardInst*);
	bool DoSelectBeginCard(CCObject* sender);
	KCardInst* m_card;
	cocos2d::extension::UIWidget* m_bigPhoto;
	K3DActionParam m_tombAction;
	CCSprite* m_ActiveRedSprite;
	CCSprite* m_ActiveGreenSprite;
	
};

#endif // __KCardActor_H__
