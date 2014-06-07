#include "RecCardGroupScene.h"
#include "ccMacros.h"
#include "../GameRecord/KGameRecordMgr.h"

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* RecCardGroupScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    RecCardGroupScene *layer = RecCardGroupScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void RecCardGroupScene::update(float dt)
{
}

// on "init" you need to initialize your instance
bool RecCardGroupScene::init()
{
	CardGroupScene::init();
	m_gameRecPanel.init(this);
    return true;
}



void RecCardGroupScene::onClickMainType(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickMainType(sender);
}

void RecCardGroupScene::onClickRace(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickRace(sender);
}

void RecCardGroupScene::onClickCost(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickCost(sender);
}

void RecCardGroupScene::onClickBack(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickBack(sender);
}

void RecCardGroupScene::onClickMiniCard(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickMiniCard(sender);
}

void RecCardGroupScene::onClickMiniHero(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickMiniHero(sender);
}

void RecCardGroupScene::onClickCard(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickCard( sender);
}

void RecCardGroupScene::onClickSlot(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickSlot(sender);
}

void RecCardGroupScene::onClickNewCardGroup(CCObject* sender)
{
	CardGroupScene::onClickNewCardGroup(sender);
}

void RecCardGroupScene::onClickSlotAdd(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickSlotAdd(sender);
}

void RecCardGroupScene::onClickSlotSub(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickSlotSub(sender);
}

void RecCardGroupScene::onClickPageDown(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickPageDown(sender);
}

void RecCardGroupScene::onClickPageUp(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickPageUp(sender);
}

void RecCardGroupScene::onClickSelectHero(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickSelectHero(sender);
}

void RecCardGroupScene::onClickSaveDeck(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickSaveDeck(sender);
}

void RecCardGroupScene::onClickClearDeck(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	CardGroupScene::onClickClearDeck(sender);
}

