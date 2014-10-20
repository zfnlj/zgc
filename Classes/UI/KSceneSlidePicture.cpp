#include "KSceneSlidePicture.h"
#include "ccMacros.h"
#include "assist/KUIAssist.h"
#include "assist/KJsonDictMgr.h"

USING_NS_CC;
using namespace cocos2d::extension;

int KSceneSlidePicture::m_firstId = 0;
int KSceneSlidePicture::m_lastId = 0;
int KSceneSlidePicture::m_curId = 0;
KSceneSlidePicture::Goto KSceneSlidePicture::m_goto = KSceneSlidePicture::goto_mainMenu;
CCScene* KSceneSlidePicture::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    KSceneSlidePicture *layer = KSceneSlidePicture::create();

	// add layer as a child to scene
    scene->addChild(layer,0,1982);
	
    // return the scene
    return scene;
}

void KSceneSlidePicture::update(float dt)
{
}

KSceneSlidePicture::~KSceneSlidePicture()
{
}

bool KSceneSlidePicture::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }

	m_ui = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/Slider.json"); 
	UIWidget* pBut = UIHelper::seekWidgetByName(m_ui,"bk");
	pBut->addPushDownEvent(this, coco_pushselector(KSceneSlidePicture::onClickNext));

	CreateCheckSlideRadio();
	cocos2d::extension::UILayer* layer = cocos2d::extension::UILayer::create();
	layer->addWidget(m_ui);
	addChild(layer, 1);
	CreateCloseBut();
	UpdatePanel();
	return true;
}

void KSceneSlidePicture::CreateCheckSlideRadio()
{
	int slideNum = m_lastId - m_firstId +1;
	m_radioSelect.AddGroupBut("check_slide",8,
								m_ui,this,coco_pushselector(KSceneSlidePicture::onClickSelectSlide),0);

	for(int i=slideNum;i<8;i++){
		m_radioSelect.SetVisible(i,false);
	}

	int realWidth = 56;
	int totalWidth;
	int radioOffset = 20;
	int offset;
	
	totalWidth  =slideNum *56  + radioOffset*(slideNum-1);
	offset = realWidth + radioOffset;

	UIWidget* pSlotBase = UIHelper::seekWidgetByName(m_ui,"radio_slide_base");
	int cur_x = pSlotBase->getWorldPosition().x -(totalWidth-offset)*0.5;
	for(int i=0;i<slideNum;i++){
		UIWidget* pCheckSlide = KUIAssist::GetIndexWidget(m_ui,"check_slide",i);
		cocos2d::CCPoint pt = pSlotBase->getWorldPosition();
		pt.x = cur_x;
		pCheckSlide->setPosition(pt);
		cur_x += offset;
	}
}

void KSceneSlidePicture::CreateCloseBut()
{
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "but_exit0.png",
                                        "but_exit1.png",
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
	KUIAssist::_switch2MainMenu();
}

void KSceneSlidePicture::onClickNext(CCObject* sender)
{
	if( m_curId>=m_lastId){
		KUIAssist::_switch2MainMenu();
		return;
	}
	m_curId++;
	m_radioSelect.SetSelected(m_curId-m_firstId,true);
	UpdatePanel();
}

void KSceneSlidePicture::UpdatePanel()
{
	UIImageView* slotWidget = (UIImageView*)UIHelper::seekWidgetByName(m_ui, "slide_slot");
	char sz[64];
	sprintf(sz,"Slider/slider_%d.jpg",m_curId);
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(sz);
	slotWidget->loadTexture(fullPath.c_str());
}

void KSceneSlidePicture::onClickSelectSlide(CCObject* sender)
{
	m_curId  = m_firstId + m_radioSelect.GetSelectVal();
	UpdatePanel();
}

void KSceneSlidePicture::OpenSceneSlide(int first,int last)
{
	m_firstId = first;
	m_lastId = last;
	m_curId = first;
}

