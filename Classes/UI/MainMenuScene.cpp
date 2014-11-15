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
#include "assist/KCardGroupAssist.h"

#include "../WorldObject/KMainPlayer.h"
#include "../StaticTable/StaticData.h"
#include "StageWaitScene.h"
#include "platform/IOSFunc_Bridge.h"
#include "../common/KPlayerRecordAssist.h"

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

void MainMenuScene::runAutoTest(float dt)
{
	GameRoot::getSingleton().RunAutoTest();
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{

	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MainMenuScene::update),this,0.1f,false);

	if(GameRoot::getSingleton().m_autoTest){
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(MainMenuScene::runAutoTest),this,10.0f,0,0,false);
	}
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
    CCMenuItemImage *pMailBut = CCMenuItemImage::create(
                                        "mail_icon.png",
                                        "mail_icon.png",
                                        this,
                                        menu_selector(MainMenuScene::menuMailCallback));
    
	pMailBut->setPosition(ccp(origin.x + visibleSize.width - pMailBut->getContentSize().width/2 ,
                                origin.y + pMailBut->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pMailBut, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
  
    // add "MainMenuScene" splash screen"

	this->addChild(GetUILayer(), 1);

	UpdateLockStatus();
	ShowCollectInfo();
	IOSFunc_Bridge::_CheckGCAvailability();
    return true;
}


void MainMenuScene::menuMailCallback(CCObject* pSender)
{
	IOSFunc_Bridge::_OpenFeedback();
/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
*/
 }


cocos2d::extension::UILayer* MainMenuScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/MainMenu.json"));

		UIWidget* pBut = m_ui->getWidgetByName("but_shop");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickShopBut));

		pBut = m_ui->getWidgetByName("but_battle");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickBattleBut));

		pBut = m_ui->getWidgetByName("but_quest");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickAdventureBut));

		pBut = m_ui->getWidgetByName("but_cardgroup");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickStoreBut));

		pBut = m_ui->getWidgetByName("but_hero");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickHeroBrowse));
		KUIAssist::ShowButton(pBut,KMainPlayer::RealPlayer()->GetCardDepot()->GetHeroNum()>0);

		UIWidget* pTiebar = m_ui->getWidgetByName("Tiebar_url");
		pTiebar->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickTiebar));

		pBut = m_ui->getWidgetByName("but_gc");
		pBut->addPushDownEvent(this, coco_pushselector(MainMenuScene::DoClickGameCenter));

	}
	return m_ui;
}

void MainMenuScene::DoClickStoreBut(CCObject* sender)
{
	KUIAssist::_switch2CardGroupScene();
}

void MainMenuScene::DoClickAdventureBut(CCObject* sender)
{
	KUIAssist::_switch2StageSelectScene();
}

void MainMenuScene::DoClickTiebar(CCObject* sender)
{
	IOSFunc_Bridge IOSFunc;
	IOSFunc.OpenUrl("http://tieba.baidu.com/f?kw=%E5%8D%A1%E7%89%8C%E6%88%98%E5%9B%BD%E7%AD%96");
}


void MainMenuScene::DoClickHeroBrowse(CCObject* sender)
{
	KUIAssist::_switch2HeroBrowseScene();
}

void MainMenuScene::DoClickBattleBut(CCObject* sender)
{
	StageWaitScene::SetSceneVal(scene_battle);
	KUIAssist::_switch2StageWaitScene();
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
	pBut = m_ui->getWidgetByName("but_hero");
	KUIAssist::ShowButton(pBut,KMainPlayer::RealPlayer()->GetCardDepot()->GetHeroNum()>0);
}

void MainMenuScene::UpdateLockStatus()
{
	UpdateLockStatus("open_adventure","but_quest","lock_quest");
	UpdateLockStatus("open_cardgroup","but_cardgroup","lock_cardgroup");
	UIWidget* pLockWidget = m_ui->getWidgetByName("lock_shop");
	pLockWidget->setVisible(false);
	//UpdateLockStatus("open_shop","but_shop","lock_shop");

	UILabelBMFont* labelName = (UILabelBMFont*)m_ui->getWidgetByName("daily_num_txt");
	int openVal = STATIC_DATA_INT("open_adventure");
	if(openVal){
		labelName->setVisible(true);

		int dailyQuestNum = KPlayerRecordAssist::RemainDailyQuestNum(&KMainPlayer::RealPlayer()->m_questRecord);
		char sz[64];
		sprintf(sz,"%d/%d",dailyQuestNum,MAX_DAILY_AWARD_SLOT);
		labelName->setText(sz);
	}else{
		labelName->setVisible(false);
	}
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
}

void MainMenuScene::DoClickGameCenter(CCObject* sender)
{
	IOSFunc_Bridge::_CheckGCAvailability();
}

void MainMenuScene::ShowCollectInfo()
{
	KItemUnitList tmpList,desList;
	KMainPlayer::RealPlayer()->GetCardDepot()->PickStoreCard(tmpList);

	SetRaceCollectInfo(tmpList,KCardStatic::race_gold);
	SetRaceCollectInfo(tmpList,KCardStatic::race_fire);
	SetRaceCollectInfo(tmpList,KCardStatic::race_water);
	SetRaceCollectInfo(tmpList,KCardStatic::race_all);
	SetRaceCollectInfo(tmpList,KCardStatic::race_null);
}

void MainMenuScene::SetRaceCollectInfo(KItemUnitList& tmpList,KCardStatic::CardRace race)
{
	KItemUnitList desList;
	KCardGroupAssist::FilterCard(tmpList,desList,KCardGroupAssist::browse_all,race,0);
	int n1 = desList.size();
	int n2 = KGameStaticMgr::getSingleton().GetCardNum(race);
	float rate = 100.0f*(float)desList.size()/(float)n2;
	char sz[64];
	sprintf(sz,"%d/%d",n1,n2);

	UILoadingBar* pBarWidget = NULL; 
	UILabel* pTxtWidget = NULL;
	switch(race){
	case KCardStatic::race_gold:
		pBarWidget = (UILoadingBar*)m_ui->getWidgetByName("bar_gold_collect");
		pTxtWidget = (UILabel*)m_ui->getWidgetByName("txt_gold_collect");
		break;
	case KCardStatic::race_fire:
		pBarWidget = (UILoadingBar*)m_ui->getWidgetByName("bar_fire_collect");
		pTxtWidget = (UILabel*)m_ui->getWidgetByName("txt_fire_collect");
		break;
	case KCardStatic::race_water:
		pBarWidget = (UILoadingBar*)m_ui->getWidgetByName("bar_water_collect");
		pTxtWidget = (UILabel*)m_ui->getWidgetByName("txt_water_collect");
		break;
	case KCardStatic::race_null:
		pBarWidget = (UILoadingBar*)m_ui->getWidgetByName("bar_nor_collect");
		pTxtWidget = (UILabel*)m_ui->getWidgetByName("txt_nor_collect");
		break;
	case KCardStatic::race_all:
		pBarWidget = (UILoadingBar*)m_ui->getWidgetByName("bar_all_collect");
		pTxtWidget = (UILabel*)m_ui->getWidgetByName("txt_all_collect");
		break;
	default:
		break;
	}
	if(pBarWidget) pBarWidget->setPercent(rate);
	if(pTxtWidget) pTxtWidget->setText(sz);
}
//