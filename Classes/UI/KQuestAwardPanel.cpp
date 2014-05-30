#include "KQuestAwardPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../Item/KItemCreate.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"

using namespace cocos2d::extension;
using namespace KItemAbout;


KQuestAwardPanel::KQuestAwardPanel():m_Panel(NULL)
{
}

KQuestAwardPanel::~KQuestAwardPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KQuestAwardPanel::init(cocos2d::extension::UILayer* layer)
{
	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/MyStore.json");
		CC_SAFE_RETAIN(m_Panel);
	}
	m_layer = layer;
	m_layer->addWidget(m_Panel);
	
	
	UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel, "but_close");
    pBut->addPushDownEvent(this, coco_pushselector(KQuestAwardPanel::DoClickClose));
}

void KQuestAwardPanel::DoClickClose(CCObject* sender)
{
	m_layer->removeWidget(m_Panel);
}

