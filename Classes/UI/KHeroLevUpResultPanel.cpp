#include "KHeroLevUpResultPanel.h"
#include "assist/KJsonDictMgr.h"

using namespace cocos2d::extension;


KHeroLevUpResultPanel::KHeroLevUpResultPanel():m_Panel(NULL)
{
}

KHeroLevUpResultPanel::~KHeroLevUpResultPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KHeroLevUpResultPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/GameResult.json");
		CC_SAFE_RETAIN(m_Panel);
		//pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		//pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));
	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	updatePanel();
	// old_strong_val,strong_to,new_strong_val
	// old_lucky_val,lucky_to,new_lucky_val
	// skill_name,
}

void KHeroLevUpResultPanel::update(float dt)
{
}