#include "CardGroupScene.h"
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

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* CardGroupScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardGroupScene *layer = CardGroupScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void CardGroupScene::update(float dt)
{
}

// on "init" you need to initialize your instance
bool CardGroupScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CardGroupScene::update),this,0.1f,false);
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


	this->addChild(GetUILayer(), 1);

	m_depot = KMainPlayer::RealPlayer()->GetCardDepot();
	CreateMiniCardList(m_depot->GetCurDeck());
	ShowMiniCardList();
    return true;
}


cocos2d::extension::UILayer* CardGroupScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/cardGroup.json"));
		//m_ui->addWidget(GUIReader::shareReader()->widgetFromJsonFile("GUI/MainMenu.json"));
	}
	return m_ui;
}

void CardGroupScene::CreateMiniCardList(int index)
{
	if(index<0) return;
	KIntegerList cardList;
	KHeroDef heroDef;
	m_depot->GetCardDeck(index,cardList,heroDef);
	m_miniCardList.clear();
	KCardGroupAssist::SortCardGroup(cardList,m_miniCardList);
}

void CardGroupScene::ShowMiniCardList()
{
	UIWidget* slot = m_ui->getWidgetByName("mini_card_pos");
	CCPoint pt = slot->getPosition();
	for(CardGroupElemList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KCardGroupElem& elem = *it;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(elem._cardId);
		if(!pST) continue;
		UIWidget* pWidget = KUIAssist::_createMiniCard(pST,elem._num);
		pWidget->setPosition(pt);
		m_ui->addWidget(pWidget);
		pt.y -= 32;
	}
}