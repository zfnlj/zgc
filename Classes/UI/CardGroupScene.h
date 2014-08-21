#ifndef _CARDGROUPSCENE_H
#define _CARDGROUPSCENE_H

#include "cocos2d.h"
#include "KSceneLayerBase.h"
#include "assist/KCardGroupAssist.h"
#include "cocos-ext.h"
#include "assist/KRadioWidget.h"
#include "KHeroLevUpPanel.h"

USING_NS_CC;
using namespace cocos2d::extension;
class KCardInst;
class KCardInst;
class KPlayerCardDepot;

#define  PAGE_CARD_NUM 8




class CardGroupScene : public KSceneLayerBase
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
    CREATE_FUNC(CardGroupScene);

	virtual cocos2d::extension::UIWidget* GetPanel();
	virtual bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
protected:

	void CreateMiniCardList(int index);
	void ShowMiniCardList();
	void FreshMiniCardList();
	void UpdateDeckCardCount();
	void onClickBack(CCObject* sender);
	void onClickCardGroup(int index);
	void onClickSlot(CCObject* sender);
	void onClickNewCardGroup(CCObject* sender);
	void onClickMiniCard(CCObject* obj);
	void onClickMiniHero(CCObject* obj);
	void onClickSlotAdd(CCObject* sender);
	void onClickSlotSub(CCObject* sender);
	void onClickCost(CCObject* sender);
	void onClickMainType(CCObject* sender);
	void onClickPageDown(CCObject* sender);
	void onClickPageUp(CCObject* sender);
	void onClickRace(CCObject* sender);
	void onClickSelectHero(CCObject* sender);
	void onClickCard(CCObject* sender);
	void DoClearDeck(CCObject* sender);
	void onClickSaveDeck(CCObject* sender);
	void onClickClearDeck(CCObject* sender);
	void onClickSmartCardGroup(CCObject* sender);
	void onClickHeroLevUp(CCObject* sender);

	void UpdateUI();
	void ShowCardGroup();
	void ShowCardBrowse();
	void ShowAllHero();
	void HideNewGroupIcon();
	void UpdateAddSubBut();
	void UpdateSelectHeroBut();
	void UpdateHeroLevUpBut();
	void UpdatePageInfo(int moreNum);
	void onMiniCardChanged();
	void SetMiniHeroWidget(UIWidget* newHero);
	void ShowMiniHero();
	void UpdateMiniCardNumInfo();
	void UpdateSmartCardGroupBut();

	void UpdateCurDeckRadio();
	void ShowHelpDrama();
	KPlayerCardDepot* m_depot;
	KMiniCardList m_miniCardList;
	UIWidget* m_pMiniHeroWidget;
	
	UILabel* m_pPageInfo;
	KRadioWidget m_radioCost;
	KRadioWidget m_radioRace;
	KRadioWidget m_radioMain;
	KRadioWidget m_radioSelectHero;
	KHeroDef m_miniHero;
	MainType m_mainType;
	UIWidget* m_pSaveWidget;
	UIWidget* m_pClearWidget;
	UIWidget* m_pSmartCardGroup;
	KHeroLevUpPanel m_heroLevUpPanel;
	int m_curCardGroup;
	int m_curPage;
	KCardGroupSlotElem m_slotElem[PAGE_CARD_NUM];

};

#endif // __CardGroupScene_SCENE_H__
