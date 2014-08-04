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

USING_NS_CC;
using namespace cocos2d::extension;

StageWaitScene::Scene_type StageWaitScene::m_sceneType = StageWaitScene::scene_battle;
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
	}else if(m_sceneType==scene_adventure){
		pQuest = KMainPlayer::RealPlayer()->RndQueryAdventureQuest();
	}
	m_qId = (pQuest)?pQuest->GetID():0;
	UILabelBMFont* labelName = (UILabelBMFont*)m_ui->getWidgetByName("stage_txt");
	if(pQuest && pQuest->GetQuestStatus()!=KQ_PreStepOver){
		char stageName[64]={0};
		pQuest->GetName(KMainPlayer::RealPlayer(),stageName,63);
		if(strlen(stageName)>0){
			labelName->setVisible(true);
			labelName->setText(stageName);
		}else{
			labelName->setVisible(false);
		}
	}else{
		labelName->setVisible(false);
	}

	UIWidget* pBut = m_ui->getWidgetByName("bk");
	pBut->addPushDownEvent(this, coco_pushselector(StageWaitScene::DoClickClose));
	pBut->setTouchEnable(true);

	KHelpStringStatic* tip = KGameStaticMgr::getSingleton().GetRndStory();
	SetTip(tip);
	m_delayCloseTime = 4;
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
		//m_ui->addWidget(GUIReader::shareReader()->widgetFromJsonFile("GUI/MainMenu.json"));
		
		

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
		KQuestFacade::_startMainQuestBattle();
	}else if(m_sceneType==scene_adventure){
		KQuestFacade::_startAdventureBattle(m_qId);
	}
}
