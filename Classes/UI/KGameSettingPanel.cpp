#include "KGameSettingPanel.h"
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
#include "assist/KJsonDictMgr.h"


using namespace cocos2d::extension;
using namespace KItemAbout;

#define SELECT_ITEM_TAG_BASE 10000
KGameSettingPanel::KGameSettingPanel():m_Panel(NULL)
{
}

KGameSettingPanel::~KGameSettingPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGameSettingPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/GameResult.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_exit");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_continue");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickContinue));

		for(int i=0; i<3; i++)
		{
			UIImageView* pSlot = (UIImageView*)KUIAssist::GetIndexWidget(m_Panel,"slot",i);
			pSlot->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickSlot));
			pSlot->setTag(i);
		}

	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	m_bSelectGift = false;
	m_expBar.init(NULL,0,0,0.0f);

	updatePanel();
}

void KGameSettingPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1f);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3f, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());
	updatePanel();
}


void KGameSettingPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();
	
	KUIAssist::_switch2MainMenu();
}
