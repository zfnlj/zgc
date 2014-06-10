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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object

	this->addChild(GetUILayer(), 1);

	KQuestManager::GetInstance()->syncAvailQuests();
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.QueryNormalQuest();
	if(pQuest && pQuest->GetQuestStatus()!=KQ_PreStepOver){
		UILabelBMFont* labelName = (UILabelBMFont*)m_ui->getWidgetByName("stage_txt");
		char stageName[64]={0};
		pQuest->GetName(KMainPlayer::RealPlayer(),stageName,63);
		if(strlen(stageName)>0){
			labelName->setVisible(true);
			labelName->setText(stageName);
		}else{
			labelName->setVisible(false);
		}
	}

	KHelpStringStatic* tip = KGameStaticMgr::getSingleton().GetRndStory();
	SetTip(tip);
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
		
		UIWidget* pBut = m_ui->getWidgetByName("bk");
		pBut->addPushDownEvent(this, coco_pushselector(StageWaitScene::DoClickClose));

	}
	return m_ui;
}


void StageWaitScene::DoClickClose(CCObject* sender)
{
	KUIAssist::_switch2BattleScene();
	KQuestFacade::_startMainQuestBattle();
}
