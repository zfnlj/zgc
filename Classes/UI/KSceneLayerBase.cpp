#include "KSceneLayerBase.h"
#include "ccMacros.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "assist/KUIAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;

void KSceneLayerBase::update(float dt)
{
	if(KUIAssist::_activeSceneLayer()==this){
		m_actor.update(dt);
		KGameRecordMgr::getSingleton().update(dt);
	}
}

bool KSceneLayerBase::init()
{
	m_ui=NULL;
	UILayer::init();
	registerWithTouchDispatcher();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


	//CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
	//	"CloseNormal.png",
	//	"CloseSelected.png",
	//	this,
	//	menu_selector(BattleFieldScene::menuCloseCallback));

	//pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
	//	origin.y + pCloseItem->getContentSize().height/2));

	//// create menu, it's an autorelease object
	//CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	//pMenu->setPosition(CCPointZero);
	//this->addChild(pMenu, 1);

	return true;
}

