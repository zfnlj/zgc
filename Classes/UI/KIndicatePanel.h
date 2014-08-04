#ifndef _KINDICATEPANEL_H
#define _KINDICATEPANEL_H

#include "cocos2d.h"
#include "UILayer.h"
#include "../GameLogic/KCardInst.h"
USING_NS_CC;
class FBattleGuy;
class KCardActor;
class KIndicatePanel :public CCObject
{
public:
	KIndicatePanel();
	~KIndicatePanel(){}

	void init(cocos2d::extension::UILayer* layer);
	void Update(float );
	void OnSelectSrcCard(KCardActor*);
	void OnSelectCardOK();
	void UpdateSleepAnim();
	bool IsActiveCard(KCardInst*);
	void onBattleRefresh();
private:
	void ActiveArr(KCardInstList* arr,KCardInstList* oldLst,bool bGreen);
	void DeactiveActor(KCardInstList* actived);
	void DoCardSleep(KCardInst* card,bool flag);
	void UpdateTimeOutEff();
	cocos2d::extension::UILayer* m_layer;
	KCardInstList m_SleepArr;
	KCardInstList m_ActiveGreenArr;
	KCardInstList m_ActiveRedArr;
	KCardActor* m_selActor;
	CCParticleSystem* m_timeoutEff;
};

#endif // __BattleFieldScene_SCENE_H__
