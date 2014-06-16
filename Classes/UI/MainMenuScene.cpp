#include "MainMenuScene.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "KCardActor.h"
#include "UILayer.h"
#include "cocos-ext.h"
#include "../Facade/FBattleGuy.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../GameLogic/KGameDef.h"
#include "assist/KUIAssist.h"
#include "GameRoot.h"
#include "MainMenuScene.h"
#include "../KNet/KUserData.h"
#include "../KNet/KSocketFacade.h"
#include "BattleFieldScene.h"
#include "VirtualService.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KQuestFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../StaticTable/StaticData.h"

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* MainMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuScene *layer = MainMenuScene::create();
	GameRoot::getSingleton().SetMainMenuScene(layer);
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void MainMenuScene::update(float dt)
{
}


// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MainMenuScene::update),this,0.1f,false);
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(MainMenuScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
  
    // add "MainMenuScene" splash screen"

	this->addChild(GetUILayer(), 1);

	UpdateLockStatus();
    return true;
}


void MainMenuScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}


cocos2d::extension::UILayer* MainMenuScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/MainMenu.json"));
		//m_ui->addWidget(GUIReader::shareReader()->widgetFromJsonFile("GUI/MainMenu.json"));

		UIWidget* pBut = m_ui->getWidgetByName("but_shop");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickShopBut));

		pBut = m_ui->getWidgetByName("but_battle");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickBattleBut));

		pBut = m_ui->getWidgetByName("but_quest");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickQuestBut));

		pBut = m_ui->getWidgetByName("but_cardgroup");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickStoreBut));

		m_gameResultPanel.init(m_ui);

	}
	return m_ui;
}

void MainMenuScene::DoClickStoreBut(CCObject* sender)
{
	KUIAssist::_switch2CardGroupScene();
}

void MainMenuScene::DoClickQuestBut(CCObject* sender)
{
}

void MainMenuScene::DoClickBattleBut(CCObject* sender)
{
	KUIAssist::_switch2StageWaitScene();
	/*if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		if(!KQuestFacade::_startMainQuestBattle())
			GameRoot::getSingleton().BattleCtrl().PlayWithAI();
	}else{

	}*/
	

}

void MainMenuScene::UpdateLockStatus(const char* key,const char* butName,const char* lockImage)
{
	UIWidget* pBut = m_ui->getWidgetByName(butName);
	int openVal = STATIC_DATA_INT(key);
	UIWidget* pLockWidget = m_ui->getWidgetByName(lockImage);
	if(openVal>0){
		pBut->setTouchEnable(true);
		pLockWidget->setVisible(false);
	}else{
		pBut->setTouchEnable(false);
		pLockWidget->setVisible(true);
	}
}

void MainMenuScene::UpdateLockStatus()
{
	UpdateLockStatus("open_quest","but_quest","lock_quest");
	UpdateLockStatus("open_cardgroup","but_cardgroup","lock_cardgroup");
	UpdateLockStatus("open_shop","but_shop","lock_shop");

}

void MainMenuScene::DoClickShopBut(CCObject* sender)
{
	KUIAssist::_switch2ShopScene();
}

void MainMenuScene::InitTest()
{
}

void MainMenuScene::onUpdateMoney()
{
}

void MainMenuScene::onUpdateBag()
{
}

void MainMenuScene::onQuestUpdate()
{
}

void MainMenuScene::onQuestFinished(int qId)
{
	//m_gameResultPanel.onQuestFinished(qId);
}