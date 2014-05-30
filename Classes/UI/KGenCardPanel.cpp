#include "KGenCardPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../../Inc/PacketGate/c_game_packet.h"

using namespace cocos2d::extension;

KGenCardPanel::KGenCardPanel():m_layer(NULL),m_Panel(NULL)
{
}

KGenCardPanel::~KGenCardPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGenCardPanel::init(cocos2d::extension::UILayer* layer,unsigned long long p1)
{
	m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/genCard.json");
	CC_SAFE_RETAIN(m_Panel);

	m_layer = layer;
	m_layer->addWidget(m_Panel);

	UIWidget* pBut = m_layer->getWidgetByName("Button_ok");
	KGenCardPanel* me = this;
    
	pBut->addPushDownEvent(me, coco_pushselector(KGenCardPanel::DoClickOK));
	
	SC_GenPlayerCard* pGen = (SC_GenPlayerCard*)p1;
	for(int i=0;i<pGen->count;i++){
		char slotName[20];
		sprintf(slotName,"select_%d",i+1);
		UIImageView* slotView = (UIImageView*)m_layer->getWidgetByName(slotName);
		if(!slotView) continue;

		m_GenCard[i].init(0,NULL,KGameStaticMgr::getSingleton().GetCard(pGen->cardID[i]));
		KCardActor* actor = KCardActor::create(&m_GenCard[i]);
		actor->GetUI()->setPosition(slotView->getWorldPosition());
		m_Panel->addChild(actor->GetUI());
	}
}

void KGenCardPanel::DoClickOK(CCObject* sender)
{
	for(int i=0;i<MAX_GEN_CARD_NUM;i++){
		KCardActor* actor = (KCardActor*)m_GenCard[i].getActor();
		m_Panel->removeChild(actor->GetUI());
		m_GenCard[i].releaseActor();
	}
	m_layer->removeWidget(m_Panel);
}
