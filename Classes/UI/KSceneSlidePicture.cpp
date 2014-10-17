#include "KSceneSlidePicture.h"
#include "ccMacros.h"
#include "KSceneSlideMgr.h"
#include "assist/KUIAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;

void KSceneSlidePicture::update(float dt)
{
}

KSceneSlidePicture::~KSceneSlidePicture()
{
}

bool KSceneSlidePicture::init()
{
	m_ui=NULL;
	UILayer::init();
	registerWithTouchDispatcher();

	return true;
}


void KSceneSlidePicture::CreateCloseBut()
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseNormal.png",
                                        this,
                                        menu_selector(KSceneSlidePicture::menuCloseCallback));
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);
}

void KSceneSlidePicture::menuCloseCallback(CCObject* pSender)
{
	onCloseCallback();
}

void KSceneSlidePicture::onClickNext()
{
	if(KSceneSlideMgr::getSingleton().NextSlide()){
	}else{
		KSceneSlideMgr::getSingleton().Clear();
		KUIAssist::_switch2MainMenu();
	}
}

void KSceneSlidePicture::CreateSelectRadio()
{
}