#ifndef _CARDGROUPSCENE_H
#define _CARDGROUPSCENE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "assist/KCardGroupAssist.h"
#include "cocos-ext.h"
#include "assist/KRadioWidget.h"

USING_NS_CC;
using namespace cocos2d::extension;
class KCardInst;
class KCardInst;
class KPlayerCardDepot;

#define  PAGE_CARD_NUM 8

class CardGroupScene : public cocos2d::CCLayer
{
	enum MainType{
		type_cardgroup,
		type_card,
	};
	
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void update(float dt);
    // implement the "static node()" method manually
    CREATE_FUNC(CardGroupScene);

	cocos2d::extension::UILayer* GetUILayer();

private:
	CCLabelTTF* m_InfoLabel;

	cocos2d::extension::UILayer* m_ui;
	void InitTest();

	void CreateMiniCardList(int index);
	void ShowMiniCardList();
	KPlayerCardDepot* m_depot;
	KItemUnitList m_miniCardList;
	void onClickCheckHero(CCObject* sender);
	void onClickCheckSoldier(CCObject* sender);
	void onClickCheckSkill(CCObject* sender);
	void onClickBack(CCObject* sender);
	void onClickCardGroup(int index);
	void onClickSlot(CCObject* sender);
	void onClickNewCardGroup(CCObject* sender);
	void UpdateUI();
	void ShowCardGroup();
	void ShowCardBrowse();
	void HideNewGroupIcon();
	KRadioWidget m_radioMain;
	MainType m_mainType;
	int m_curCardGroup;
	KCardGroupAssist::BrowseCard m_browseType;
	int m_curPage;
	KCardGroupSlotElem m_slotElem[PAGE_CARD_NUM];
};

#endif // __CardGroupScene_SCENE_H__