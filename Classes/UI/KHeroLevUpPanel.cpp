#include "KHeroLevUpPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "assist/KUIAssist.h"
#include "../VirtualService.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "../Quest/KQuestManager.h"
#include "assist/KQuestFacade.h"
#include "../Item/KItemCreate.h"
#include "../WorldObject/KPlayer.h"

using namespace cocos2d::extension;
using namespace KItemAbout;

KHeroLevUpPanel::KHeroLevUpPanel():m_Panel(NULL),m_pHeroDef(NULL)
{
}

KHeroLevUpPanel::~KHeroLevUpPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KHeroLevUpPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/HeroLevUp.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "close_but");
		pBut->addPushDownEvent(this, coco_pushselector(KHeroLevUpPanel::DoClickClose));

	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	updatePanel();
}

void KHeroLevUpPanel::ShowPanel(const KHeroDef* hero)
{
	m_pHeroDef = hero;
	m_layer->addWidget(m_Panel);
}

void KHeroLevUpPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();
	
	//KUIAssist::_switch2MainMenu();

	/*   CCPoint pt(m_pt.x,m_pt.y+600);
	switch(m_panelType){
	case enum_panel_task:
	{

	KUIAssist::_moveOutWidget(m_Panel,m_pt,pt,0.8f);
	}
	break;
	case enum_panel_game:
	{
	m_Panel->setVisible(false);
	KUIAssist::_switch2MainMenu();
	}
	break;
	default:
	break;
	}
	*/
}

void KHeroLevUpPanel::updatePanel()
{
	
}


void KHeroLevUpPanel::update(float dt)
{
}