#include "KSelectBeginCardPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"

using namespace cocos2d::extension;

KSelectBeginCardPanel::KSelectBeginCardPanel():m_ui(NULL)
{
}

KSelectBeginCardPanel::~KSelectBeginCardPanel()
{
	CC_SAFE_RELEASE_NULL(m_ui);
}
void KSelectBeginCardPanel::init(cocos2d::extension::UILayer* layer)
{
	m_ui = GUIReader::shareReader()->widgetFromJsonFile("GUI/select_card.json");
	CC_SAFE_RETAIN(m_ui);

	m_layer = layer;
	m_layer->addWidget(m_ui);
	
	UIImageView* base = (UIImageView*)UIHelper::seekWidgetByName(m_ui,"my_slot_base");

	UIWidget* pPanel = UIHelper::seekWidgetByName(m_ui,"main_panel");
	pPanel->addPushDownEvent(this, coco_pushselector(KSelectBeginCardPanel::DoClickOK));
	

	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		actor->GetUI()->setPosition(base->getWorldPosition());
		actor->GetUI()->setScale(base->getScale());
	}
	KUIAssist::_moveCardSet(lst,"go_select");
}


void KSelectBeginCardPanel::onClickCard(CCObject* sender)
{

}

void KSelectBeginCardPanel::DoClickOK(CCObject* sender)
{
	KCardInstList skiplst;
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		if(actor->m_bSelected){

			skiplst.push_back(*it);
			(*it)->releaseActor();
		}
	}
	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		GameRoot::getSingleton().BattleCtrl().DoSelectBeginCard(pMainPlayer->GetImp(),&skiplst);
	}else{
		KSocketFacade::DoSelectBeginCard(&skiplst);
	}
}

void KSelectBeginCardPanel::onSelectCardOK(FBattleGuy* guy)
{
	UIImageView* base = (UIImageView*)m_layer->getWidgetByName("select_base");
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	if(guy==GameRoot::getSingleton().BattleCtrl().GetMainPlayer()){
		//KUIAssist::_showCardSet(lst);

		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
			KCardInst* card = *it;
			KCardActor* actor = KCardActor::create(card);
			actor->GetUI()->setScale(base->getScale());
			actor->GetActionMgr().PlayAction("select_to_hand");
		}
		m_layer->removeWidget(m_ui);
	}
}
