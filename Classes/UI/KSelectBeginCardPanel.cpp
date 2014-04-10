#include "KSelectBeginCardPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"

using namespace cocos2d::extension;

KSelectBeginCardPanel::KSelectBeginCardPanel():m_selectPanel(NULL)
{
}

KSelectBeginCardPanel::~KSelectBeginCardPanel()
{
	CC_SAFE_RELEASE_NULL(m_selectPanel);
}
void KSelectBeginCardPanel::init(cocos2d::extension::UILayer* layer)
{
	m_selectPanel = GUIReader::shareReader()->widgetFromJsonFile("GUI/select.json");
	CC_SAFE_RETAIN(m_selectPanel);

	m_layer = layer;
	m_layer->addWidget(m_selectPanel);
	
	UIImageView* base = (UIImageView*)m_layer->getWidgetByName("my_slot_base");

	UIWidget* pBut = m_layer->getWidgetByName("but_ok");
	KSelectBeginCardPanel* me = this;
    
	pBut->addPushDownEvent(me, coco_pushselector(KSelectBeginCardPanel::DoClickOK));
	

	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		actor->GetUI()->setPosition(base->getWorldPosition());
		actor->GetUI()->setScale(base->getScale());
	}
	KUIAssist::_moveCardSet(lst,"go_select");
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
		KUIAssist::_showCardSet("select_base",lst);

		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
			KCardInst* card = *it;
			KCardActor* actor = KCardActor::create(card);
			actor->GetUI()->setScale(base->getScale());
			actor->GetActionMgr().PlayAction("select_to_hand");
		}
		m_layer->removeWidget(m_selectPanel);
	}
}
