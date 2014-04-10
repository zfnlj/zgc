#include "KMyQuestPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "KUIAssist.h"
#include "../VirtualService.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "KUIAssist.h"

using namespace cocos2d::extension;


KMyQuestPanel::KMyQuestPanel():m_Panel(NULL)
{
}

KMyQuestPanel::~KMyQuestPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KMyQuestPanel::init(cocos2d::extension::UILayer* layer)
{
	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/quest_panel.json");
		CC_SAFE_RETAIN(m_Panel);
		UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel, "but_close");
		KMyQuestPanel* me = this;
		pBut->addPushDownEvent(me, coco_pushselector(KMyQuestPanel::DoClickClose));
	}
	m_layer = layer;
	m_layer->addWidget(m_Panel);
	
	

	updatePanel();
}

void KMyQuestPanel::DoClickClose(CCObject* sender)
{
	m_layer->removeWidget(m_Panel);
}

void KMyQuestPanel::DoClickSumbit(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	int questId= pWidget->getTag();
	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		VirtualService::getSingleton().SubmitQuest(questId);
	}else{
		KSocketFacade::DoSubmitQuest(questId);
	}
}

void KMyQuestPanel::updatePanel()
{
	for(int i=0;i<MAX_QUEST_SLOT_NUM;i++){
		UIWidget* slotWidget = KUIAssist::_setVisible(m_Panel,"slot",i,false);
		UIWidget* pSubmitBut =  UIHelper::seekWidgetByName(slotWidget,"but_submit");
		UIWidget* pCancleBut =  UIHelper::seekWidgetByName(slotWidget,"but_cancle");
		pSubmitBut->setVisible(false);
		pCancleBut->setVisible(false);
		pSubmitBut->setTouchEnable(false);
		pCancleBut->setTouchEnable(false);
	}

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	int num = playerQuestManager.m_quests.size();
	int pos=0;
	for(int i=0; i<num; i++)
	{
		KDString<512> giftStr;
		KQuestNew* pQuest = playerQuestManager.m_quests[i];
		KQuestStatusCode status = pQuest->GetQuestStatus();
		UIWidget* pWidget =  KUIAssist::GetIndexWidget(m_Panel,"slot",pos++);
		pWidget->setVisible(true);
		
		if(status== KQ_PreStepOver){ //quest over
			UIWidget* pSubmitBut =  UIHelper::seekWidgetByName(pWidget,"but_submit");
			if(pSubmitBut){
				pSubmitBut->setVisible(true);
				pSubmitBut->setTouchEnable(true);
				pSubmitBut->setTag(pQuest->m_qid);
				pSubmitBut->addPushDownEvent(this, coco_pushselector(KMyQuestPanel::DoClickSumbit));
			}
			//VirtualService::getSingleton().SubmitQuest(pQuest->m_qid);
		}else{
			UIWidget* pCancleBut =  UIHelper::seekWidgetByName(pWidget,"but_cancle");
			if(pCancleBut){
				pCancleBut->setVisible(true);
				pCancleBut->setTouchEnable(true);
				pCancleBut->setTag(pQuest->m_qid);
				pCancleBut->addPushDownEvent(this, coco_pushselector(KMyQuestPanel::DoCancleQuest));
			}
		}
		
		if(!pWidget) break;
		pQuest->GetGiftDesc(NULL,giftStr);
		pWidget->setTag(pQuest->m_qid);
	}

	num = playerQuestManager.m_availQuests.size();
	for(int i=0;i<num;i++)
	{
		KQuestNew* pQuest = playerQuestManager.m_availQuests[i];
		UIWidget* pWidget = KUIAssist::GetIndexWidget(m_Panel,"slot",pos++);
		if(!pWidget) break;
		pWidget->setTag(pQuest->m_qid);
		pWidget->setVisible(true);
		DoAcceptQuest(pWidget);
	}
}


void KMyQuestPanel::DoAcceptQuest(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	int questId= pWidget->getTag();
	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		VirtualService::getSingleton().AcceptQuest(questId);
	}else{
		KSocketFacade::DoAcceptQuest(questId);
	}
}

void KMyQuestPanel::DoCancleQuest(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	int questId= pWidget->getTag();
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.GetQuest(questId);
	
	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		VirtualService::getSingleton().CancelQuest(pQuest);
	}else{
		KSocketFacade::DoCancelQuest(questId);
	}
	updatePanel();
}


