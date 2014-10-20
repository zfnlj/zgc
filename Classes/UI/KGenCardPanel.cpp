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
	for(int i=0;i<MAX_GEN_CARD_NUM;i++){
		m_GenCard[i].init();
	}

	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/select.json");
		CC_SAFE_RETAIN(m_Panel);
		
		m_layer = layer;
		

		UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel,"panel");
		pBut->addPushDownEvent(this, coco_pushselector(KGenCardPanel::DoClickOK));
	}
	m_layer->addWidget(m_Panel);
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(KGenCardPanel::update),this,0.033f,false);

	SC_GenPlayerCard* pGen = (SC_GenPlayerCard*)p1;
	for(int i=0;i<pGen->count;i++){
		char slotName[20];
		sprintf(slotName,"select_%d",i);
		UIImageView* slotView = (UIImageView*)m_layer->getWidgetByName(slotName);
		if(!slotView) continue;

		m_GenCard[i].init(0,NULL,pGen->cardID[i]);
		KCardActor* actor = KCardActor::create(&m_GenCard[i],true);
		actor->GetUI()->setPosition(slotView->getWorldPosition());
		actor->GetUI()->setVisible(false);
		actor->GetUI()->addPushDownEvent(this, coco_pushselector(KGenCardPanel::onClickCard));

		m_Panel->addChild(actor->GetUI());

		UIImageView* pNewSlot = (UIImageView*) UIHelper::seekWidgetByName(actor->GetUI(),"buf_slot_0");
		pNewSlot->loadTexture("new_icon.png",UI_TEX_TYPE_PLIST);
		pNewSlot->setVisible(pGen->newFlag[i]>0);
		
		KAction* pAction = actor->GetActionMgr().PlayAction("gen_card");
		pAction->SetDelayTime(i*1.5);
	}
	UIImageView* pMoneyTxt = (UIImageView*)UIHelper::seekWidgetByName(m_Panel,"money_return_txt");
	UILabelBMFont* pMoneyVal = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel,"money_return_val");
	if(pGen->money>0){
		pMoneyTxt->setVisible(true);
		pMoneyVal->setVisible(true);
		char sz[64];
		sprintf(sz,"%d",pGen->money);
		pMoneyVal->setText(sz);

	}else{
		pMoneyTxt->setVisible(false);
		pMoneyVal->setVisible(false);
	}

	m_delayCloseTime = 10.0f;
}

void KGenCardPanel::onClickCard(CCObject* sender)
{

}

void KGenCardPanel::update(float dt)
{
	m_delayCloseTime -= dt;
	if(m_delayCloseTime<0) {
		DoClickOK(NULL);
		m_delayCloseTime = 9999.0f;
	}

	for(int i=0;i<MAX_GEN_CARD_NUM;i++){
		KCardActor* actor = (KCardActor*)m_GenCard[i].getActor();
		if(actor) actor->update(dt);
	}
}

void KGenCardPanel::DoClickOK(CCObject* sender)
{
	if(m_delayCloseTime>6.0f){
		return;
	}

	for(int i=0;i<MAX_GEN_CARD_NUM;i++){
		KCardActor* actor = (KCardActor*)m_GenCard[i].getActor();
		m_Panel->removeChild(actor->GetUI());
		m_GenCard[i].releaseActor();
	}
	CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
	m_layer->removeWidget(m_Panel);
}
