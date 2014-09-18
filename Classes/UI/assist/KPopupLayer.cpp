#include "KPopupLayer.h"
#include "KJsonDictMgr.h"
#include "KUIAssist.h"
#include "../../StaticTable/KGameStatic.h"
#include "../../common/KUIMsgDef.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define MODAL_DIALOGUE_ZORDER 9999


KPopupLayer* KPopupLayer::s_pInstance = NULL;
int KPopupLayer::m_val=0;
CCObject*	KPopupLayer::m_pPushListener=NULL;
KPopupLayer::KPopupLayer(void)
{
}

KPopupLayer::~KPopupLayer(void)
{

}

bool KPopupLayer::init()
{
	if (!UILayer::init())
	{
		return false;
	}

	m_panel =  KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/popMsg.json");
	m_panel->setAnchorPoint(CCPoint(0.5f,0.5f));
	addWidget( m_panel );

	m_pLabCaption = (UILabel*)getWidgetByName("title");
	m_pLabContent = (UILabel*)getWidgetByName("msg");

	m_pBtn_Ok = (UIButton*)getWidgetByName("but_ok");
	m_pBtn_Ok->addReleaseEvent(this,coco_releaseselector(KPopupLayer::btn_Click));
	m_pBtn_Ok->setWidgetTag(RT_Ok);

	m_pBtn_Yes = (UIButton*)getWidgetByName("but_yes");
	m_pBtn_Yes->addReleaseEvent(this,coco_releaseselector(KPopupLayer::btn_Click));
	m_pBtn_Yes->setWidgetTag(RT_YES);

	m_pBtn_No = (UIButton*)getWidgetByName("but_no");
	m_pBtn_No->addReleaseEvent(this,coco_releaseselector(KPopupLayer::btn_Click));
	m_pBtn_No->setWidgetTag(RT_NO);
	m_pPushListener = NULL;
	setTouchEnabled(true);  //开启触摸响应
	return true;
}


void KPopupLayer::registerWithTouchDispatcher(){
	// 这里的触摸优先级设置为-128，与CCMenu同级，保证了屏蔽下方的触摸
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);

}
void KPopupLayer::btn_Click( CCObject* pSender )
{
	HideModal();
	if (m_pFunc) (m_pPushListener->*m_pFunc)(pSender);
}

void KPopupLayer::updateLayer(const char* szTitle,const char* szContent,DialogueType type,SEL_PushEvent p)
{
	s_pInstance->m_pLabCaption->setText(szTitle);
	s_pInstance->m_pLabContent->setText(szContent);
	s_pInstance->m_Type = type;
	s_pInstance->m_pFunc = p;

	if (DT_Normal==s_pInstance->m_Type)
	{
		m_pBtn_Ok->setVisible(false);
		m_pBtn_Ok->setTouchEnable(false);
		m_pBtn_Yes->setVisible(false);
		m_pBtn_Yes->setTouchEnable(false);
		m_pBtn_No->setVisible(false);
		m_pBtn_No->setTouchEnable(false);
	}
	else if (DT_Ok==m_Type)
	{
		m_pBtn_Ok->setVisible(true);
		m_pBtn_Ok->setTouchEnable(true);
		m_pBtn_Yes->setVisible(false);
		m_pBtn_Yes->setTouchEnable(false);
		m_pBtn_No->setVisible(false);
		m_pBtn_No->setTouchEnable(false);
	}
	else if (DT_Yes_No==m_Type)
	{
		m_pBtn_Ok->setVisible(false);
		m_pBtn_Ok->setTouchEnable(false);
		m_pBtn_Yes->setVisible(true);
		m_pBtn_Yes->setTouchEnable(true);
		m_pBtn_No->setVisible(true);
		m_pBtn_No->setTouchEnable(true);
	}
	else
	{
		CCLOG("KPopupLayer::error m_Type");
	}
}

bool KPopupLayer::ccTouchBegan(CCTouch * touch,CCEvent * pevent)
{
	if (m_pInputManager) m_pInputManager->onTouchBegan(touch);
	return true;
}

void KPopupLayer::DoNotifyDlg(int contentId)
{
	KHelpStringStatic* helpString = KGameStaticMgr::getSingleton().GetHelpString(UI_NOTIFY_STR);
	if(!helpString) return;

	KHelpStringStatic* contentString = KGameStaticMgr::getSingleton().GetHelpString(contentId);
	if(!contentString) return;
	DoModal(helpString->GetString(),contentString->GetString(),DT_Ok,NULL,NULL);
}

void KPopupLayer::DoModal(const char* szTitle,const char* szContent,DialogueType type,SEL_PushEvent p,CCObject* pListener)
{
	if (!s_pInstance)
	{
		s_pInstance = KPopupLayer::create();
		s_pInstance->retain(); //let it leak,that's allright
	}
	CC_ASSERT(s_pInstance);
	s_pInstance->HideModal();

	s_pInstance->updateLayer(szTitle,szContent,type,p);
	s_pInstance->setPosition( KUIAssist::_getScreenCenter());
	
	CCScene* pCurrentScene = CCDirector::sharedDirector()->getRunningScene();
	CC_ASSERT(pCurrentScene);
	pCurrentScene->addChild(s_pInstance,MODAL_DIALOGUE_ZORDER);
	m_pPushListener = pListener;
	s_pInstance->m_panel->setScale(0.01f);
	CCSequence *popupActions = CCSequence::create(
		CCScaleTo::create(0.0f, 0.0f), 
		CCScaleTo::create(0.26f, 1.00f),NULL);
	s_pInstance->m_panel->runAction(popupActions);

	CCLOG("KPopupLayer::ShowModal()");

}

void KPopupLayer::HideModal()
{
	if (s_pInstance&&s_pInstance->getParent())
	{
		s_pInstance->removeFromParent();
		CCLOG("KPopupLayer::HideModal()");
	}
}

void KPopupLayer::DoModal(int titleId,int contentId,DialogueType type,SEL_PushEvent p,CCObject* pListener)
{
	KHelpStringStatic* helpString = KGameStaticMgr::getSingleton().GetHelpString(titleId);
	if(!helpString) return;
	const char* titleStr = helpString->GetString();
	helpString = KGameStaticMgr::getSingleton().GetHelpString(contentId);
	if(!helpString) return;
	const char* contentStr = helpString->GetString();
	DoModal(titleStr,contentStr,type,p,pListener);
}

void KPopupLayer::DoModal(int titleId,int contentId,int val,DialogueType type,SEL_PushEvent p,CCObject* pListener)
{
	KHelpStringStatic* helpString = KGameStaticMgr::getSingleton().GetHelpString(titleId);
	if(!helpString) return;
	const char* titleStr = helpString->GetString();
	helpString = KGameStaticMgr::getSingleton().GetHelpString(contentId);
	if(!helpString) return;
	const char* contentStr = helpString->GetString();
	char sz[128];
	sprintf(sz,contentStr,val);
	DoModal(titleStr,sz,type,p,pListener);
}