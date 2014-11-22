#include "StageSelectScene.h"
#include "ccMacros.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KUIAssist.h"
#include "StageWaitScene.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "../PlayerCard/KTowerAssist.h"

#define MAX_PAGE_STAGE_NUM 20

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* StageSelectScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StageSelectScene *layer = StageSelectScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void StageSelectScene::update(float dt)
{
}

// on "init" you need to initialize your instance
bool StageSelectScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

    // 1. super init first
	if ( !KSceneLayerBase::init() )
	{
		return false;
	}
	CreateCloseBut();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	if(!m_ui){
		m_ui = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/stage_scene.json");
		addWidget(m_ui);
		InitTower();
		InitStageBut();
	}
    return true;
}

void StageSelectScene::InitTower()
{
	UIWidget* pBut = this->getWidgetByName("but_tower");
	pBut->addPushDownEvent(this, coco_pushselector(StageSelectScene::DoClickTower));
	bool bShowTower = KMainPlayer::RealPlayer()->IsShowTower();

	KUIAssist::_setButVisible(pBut, bShowTower);
	UILabelAtlas* pLayer = (UILabelAtlas*)this->getWidgetByName("tower_layer_txt");
	char sz[64];
	sprintf(sz,"%d",  KTowerAssist::_getLayer());
	pLayer->setStringValue(sz);
	pLayer->setVisible(bShowTower);
}

void StageSelectScene::InitStageBut()
{
	int dailyStageLev = KMainPlayer::RealPlayer()->GetDailyStageLev();
	int towerPos = KMainPlayer::RealPlayer()->GetPlayerRecord()->_gameData._tower.GetPos();
	bool bTowerShow =KMainPlayer::RealPlayer()->GetPlayerRecord()->_gameData._tower._bShow;
	for(int i=0;i<MAX_PAGE_STAGE_NUM;i++){
		char sz[64];
		sprintf(sz,"but_stage_%d",i);
		UIWidget* pBut = this->getWidgetByName(sz);
		pBut->setTouchEnabled(true);
		pBut->setTag(i+1);
		pBut->addPushDownEvent(this, coco_pushselector(StageSelectScene::DoClickStage));
		KUIAssist::_setButVisible(pBut, i<dailyStageLev);
		sprintf(sz,"stage_txt_%d",i);
		UILabelAtlas* stageNumLabel = (UILabelAtlas*)this->getWidgetByName(sz);
		if(!bTowerShow && towerPos==(i+1)){
			stageNumLabel->setColor(ccc3(240, 55, 11));
		}else{
			stageNumLabel->setColor(ccc3(255, 255, 255));
		}
		sprintf(sz,"%d",i+1);
		stageNumLabel->setStringValue(sz);
		stageNumLabel->setVisible(i<dailyStageLev);
	}
}

void StageSelectScene::DoClickStage(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	StageWaitScene::SetSceneVal(scene_daily,pBut->getTag());
	KUIAssist::_switch2StageWaitScene();
}

void StageSelectScene::DoClickTower(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	StageWaitScene::SetSceneVal(scene_tower, 911);
	KUIAssist::_switch2StageWaitScene();
}

void StageSelectScene::onCloseCallback(CCObject* sender)
{
	UIWidget* pBut = this->getWidgetByName("bk");
	pBut->setTouchEnable(false);
	KUIAssist::PlayClickButSound();
	KUIAssist::_switch2MainMenu();
}
