#include "KGameTipPanel.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "assist/KJsonDictMgr.h"
#include "../StaticTable/StaticData.h"


using namespace cocos2d::extension;

#define SELECT_ITEM_TAG_BASE 10000
KGametTipPanel::KGametTipPanel():m_Panel(NULL)
{
}

KGametTipPanel::~KGametTipPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGametTipPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/tipPanel.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "but_ok");
		pBut->addPushDownEvent(this, coco_pushselector(KGametTipPanel::DoClickClose));
		
	}
	m_layer = layer;
	m_Panel->setZOrder(999);
	UpdatePanel();
}

void KGametTipPanel::UpdatePanel()
{
}

void KGametTipPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1f);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3f, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());
}


void KGametTipPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();
}

