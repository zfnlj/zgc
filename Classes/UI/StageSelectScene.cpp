#include "StageSelectScene.h"
#include "ccMacros.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KUIAssist.h"
#include "StageWaitScene.h"

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
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

	this->addChild(GetUILayer(), 1);

    return true;
}

cocos2d::extension::UILayer* StageSelectScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/stage_scene.json"));

		InitStageBut();
	}
	return m_ui;
}

void StageSelectScene::InitStageBut()
{
	for(int i=0;i<MAX_PAGE_STAGE_NUM;i++){
		char sz[64];
		sprintf(sz,"but_stage_%d",i);
		UIWidget* pBut = m_ui->getWidgetByName(sz);
		pBut->setTag(i);
		pBut->addPushDownEvent(this, coco_pushselector(StageSelectScene::DoClickStage));

		sprintf(sz,"stage_txt_%d",i);
		UILabelAtlas* stageNumLabel = (UILabelAtlas*)m_ui->getWidgetByName(sz);
		sprintf(sz,"%d",i+1);
		stageNumLabel->setStringValue(sz);
	}
}

void StageSelectScene::DoClickClose(CCObject* sender)
{
	UIWidget* pBut = m_ui->getWidgetByName("bk");
	pBut->setTouchEnable(false);
	KUIAssist::_switch2MainMenu();
}


void StageSelectScene::DoClickStage(CCObject* sender)
{
	StageWaitScene::SetSceneType(StageWaitScene::scene_adventure);
	KUIAssist::_switch2StageWaitScene();
}