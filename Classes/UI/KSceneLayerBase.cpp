#include "KSceneLayerBase.h"
#include "ccMacros.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "assist/KUIAssist.h"
#include "../Item/KUseItemManager.h"
#include "assist/KPopupLayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

void KSceneLayerBase::update(float dt)
{
	if(KUIAssist::_activeSceneLayer()==this){
		m_actor.update(dt);
		KGameRecordMgr::getSingleton().update(dt);
	}
}

KSceneLayerBase::~KSceneLayerBase()
{
	if(KUIAssist::_activeScene == this) KUIAssist::_activeScene = NULL;
}

bool KSceneLayerBase::init()
{
	m_ui=NULL;
	UILayer::init();
	registerWithTouchDispatcher();

	KUIAssist::_activeScene = this;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	return true;
}


void KSceneLayerBase::CreateCloseBut()
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "but_exit0.png",
                                        "but_exit1.png",
                                        this,
                                        menu_selector(KSceneLayerBase::menuCloseCallback));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
}

void KSceneLayerBase::menuCloseCallback(CCObject* pSender)
{
	onCloseCallback(pSender);
}

void KSceneLayerBase::onIAPCallback(const char* productName,int count)
{
	if(strlen(productName)>0){
		for(int i=0;i<count;i++){
			KItemAbout::KUseItemManager::GetInstance()->UseItem(productName,0);
		}
	}
}

void KSceneLayerBase::onSystemMsg(int id)
{
	KPopupLayer::DoNotifyDlg(id);
}