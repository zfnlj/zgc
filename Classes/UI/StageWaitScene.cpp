#include "StageWaitScene.h"
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
#include "../KNet/KUserData.h"
#include "../KNet/KSocketFacade.h"
#include "BattleFieldScene.h"
#include "VirtualService.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KQuestFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "../Quest/KQuestManager.h"
#include "../StaticTable/KHelpStringStatic.h"
#include "../WorldObject/KPlayer.h"
#include "../PlayerCard/KTowerAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;

Scene_type StageWaitScene::m_sceneType = scene_battle;
int StageWaitScene::m_val = 0;
CCScene* StageWaitScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StageWaitScene *layer = StageWaitScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void StageWaitScene::update(float dt)
{
	m_delayCloseTime -= dt;
	if(m_delayCloseTime<0) {
		DoClickClose(NULL);
	}
}

// on "init" you need to initialize your instance
bool StageWaitScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(StageWaitScene::update),this,0.1f,false);
    //////////////////////////////
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

	KQuestManager::GetInstance()->syncAvailQuests();
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = NULL;
	if(m_sceneType==scene_battle){
		pQuest = playerQuestManager.QueryNormalQuest();
	}else if(m_sceneType==scene_daily){
		pQuest = KQuestManager::GetInstance()->RndDailyQuest(m_val);
	}else if(m_sceneType==scene_tower){
		pQuest = KQuestManager::GetInstance()->GetQuest(m_val);
	}
	m_qId = (pQuest)?pQuest->GetID():0;
	UILabelBMFont* labelName = (UILabelBMFont*)m_ui->getWidgetByName("stage_txt");
	if(pQuest && pQuest->GetQuestStatus()!=KQ_PreStepOver){
		char stageName[64]={0};
		if(m_sceneType==scene_tower){
			sprintf(stageName,"Myst %d",KTowerAssist::_getLayer());
		}else{
			pQuest->GetName(KMainPlayer::RealPlayer(),stageName,63);
		}
		if(strlen(stageName)>0){
			labelName->setText(stageName);
		}else{
			labelName->setText("Clear!");
		}
	}else{
		labelName->setText("Clear!");
		//labelName->setVisible(false);
	}

	UIWidget* pBut = m_ui->getWidgetByName("bk");
	pBut->addPushDownEvent(this, coco_pushselector(StageWaitScene::DoClickClose));
	pBut->setTouchEnable(true);

	KHelpStringStatic* tip = NULL;
	if(KMainPlayer::RealPlayer()->getLevel()<5){
		tip = KGameStaticMgr::getSingleton().GetRndTip();
	}else{
		tip = KGameStaticMgr::getSingleton().GetRndStory();
	}
	SetTip(tip);
	m_delayCloseTime = 4;
	CreateExitBut();
    return true;
}

void StageWaitScene::SetTip(KHelpStringStatic* tip)
{
	UILabel* pLabel = (UILabel*)m_ui->getWidgetByName("tip_txt");
	pLabel->setText(tip->GetString());
	
	pLabel = (UILabel*)m_ui->getWidgetByName("author");
	if(tip->HasAutorh()){
		pLabel->setText(tip->GetAuthor());
	}else{
		pLabel->setText("");
	}
}

cocos2d::extension::UILayer* StageWaitScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/stage_wait.json"));
	}
	return m_ui;
}


void StageWaitScene::DoClickClose(CCObject* sender)
{
	m_delayCloseTime = 99999.0f;
	UIWidget* pBut = m_ui->getWidgetByName("bk");
	pBut->setTouchEnable(false);
	if(m_qId==0){
		KUIAssist::_switch2MainMenu();
		return;
	}
	KUIAssist::_switch2BattleScene();
	

	if(m_sceneType==scene_battle){
		KQuestFacade::_startBattle();
	}else if(m_sceneType==scene_daily){
		KQuestFacade::_startDaily(m_qId);
	}else if(m_sceneType==scene_tower){
		KQuestFacade::_startTower(m_qId);
	}
}

void StageWaitScene::CreateExitBut()
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "but_exit0.png",
                                        "but_exit1.png",
                                        this,
                                        menu_selector(StageWaitScene::menuExitCallback));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
}

void StageWaitScene::menuExitCallback(CCObject* pSender)
{
	KUIAssist::_switch2MainMenu();
}
