#ifndef _HEROBROWSESCENE_H
#define _HEROBROWSESCENE_H

#include "cocos2d.h"
#include "KSceneLayerBase.h"
#include "cocos-ext.h"
#include "assist/KRadioWidget.h"
#include "KHeroLevUpPanel.h"
#include "assist/KCardGroupAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;
class KCardInst;
class KCardInst;
class KPlayerCardDepot;

#define  PAGE_CARD_NUM 8




class HeroBrowseScene : public KSceneLayerBase
{
public:
		enum MainType{
		type_cardgroup,
		type_card,
	};
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(HeroBrowseScene);
	virtual void onCloseCallback(){}
	virtual cocos2d::extension::UIWidget* GetPanel();
	void DoBreakHero(CCObject* sender);
protected:

	void onClickBack(CCObject* sender);
	void onClickCost(CCObject* sender);
	void onClickPageDown(CCObject* sender);
	void onClickPageUp(CCObject* sender);
	void onClickRace(CCObject* sender);
	void onClickHeroLevUp(CCObject* sender);
	void onClickBreakHero(CCObject* sender);
	void UpdateUI();
	void ShowHeroCards();
	void UpdateHeroLevUpBut();
	void UpdatePageInfo(int moreNum);

	KPlayerCardDepot* m_depot;
	
	UILabel* m_pPageInfo;
	KRadioWidget m_radioRace;

	KHeroLevUpPanel m_heroLevUpPanel;
	int m_curCardGroup;
	int m_curPage;
	KCardGroupSlotElem m_slotElem[PAGE_CARD_NUM];

};

#endif // __HeroBrowseScene_SCENE_H__
