#include "KSelectBeginCardPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "KSceneLayerBase.h"
#include "BattleFieldScene.h"

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
	

	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();

	KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		actor->GetUI()->setPosition(base->getWorldPosition());
		actor->GetUI()->setScale(base->getScale());
	}
	KUIAssist::_moveCardSet(lst,"go_select");
	_copyCardSet(lst,&m_initSelectLst);
}

void KSelectBeginCardPanel::DoClickOK(CCObject* sender)
{
	KCardInstList skiplst;
	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		if(actor->m_bSelected){
			actor->RemoveSelectImg();
			skiplst.push_back(*it);
			(*it)->releaseActor();
		}
	}
	if(KClientBattleCtrl::getInstance()->IsServerSide()){
		KClientBattleCtrl::getInstance()->DoSelectBeginCard(pMainPlayer->GetImp(),&skiplst);
	}else{
		KSocketFacade::DoSelectBeginCard(&skiplst);
	}
}

void KSelectBeginCardPanel::onSelectCardOK_AutoTest()
{
	if(!GameRoot::getSingleton().m_autoTest) return;
	KCardInstList* handLst = KClientBattleCtrl::getInstance()->GetMainPlayer()->QueryCardSet(KCardInst::enum_slot_hand);

	for(KCardInstList::iterator it = m_initSelectLst.begin();it!=m_initSelectLst.end();++it){
		KCardInst* card = *it;
		if(_getIndexOfCard(handLst,card)<0){
			KCardActor* actor = (KCardActor*)card->getActor();
			actor->GetUI()->removeFromParent();
			(*it)->releaseActor();
		}
	}
	m_initSelectLst.clear();
}

void KSelectBeginCardPanel::onSelectCardOK(FBattleGuy* guy)
{
	UIImageView* base = (UIImageView*)m_layer->getWidgetByName("select_base");
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	if(guy==KClientBattleCtrl::getInstance()->GetMainPlayer()){
		//KUIAssist::_showCardSet(lst);
		bool bShowRefresh = false;
		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
			KCardInst* card = *it;
			if(!card->getActor()){
				KCardActor* actor = KCardActor::create(card);
				if(!actor->GetUI()->getParent()) KUIAssist::_activeScene->addWidget(actor->GetUI());

				cocos2d::CCPoint pt = KUIAssist::_queryCardPos(lst,card,base);
				actor->GetUI()->setPosition(pt);
				actor->GetActionMgr().PlayAction("gen_card");
				bShowRefresh = true;
			}
		}
		if(bShowRefresh){
			CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(KSelectBeginCardPanel::onMeSelectEnd),this,1.6f,0,0,false);
		}else{
			onMeSelectEnd(0);
		}
		onSelectCardOK_AutoTest();
	}else{
		GameRoot::getSingleton().getBattleScene()->onDrawCard(guy->QueryCardSet(KCardInst::enum_slot_hand));
	}
}

void KSelectBeginCardPanel::onMeSelectEnd(float dt)
{
	UIImageView* base = (UIImageView*)m_layer->getWidgetByName("select_base");
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetMainPlayer();
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		KCardActor* actor = KCardActor::create(card);
		actor->GetUI()->setScale(base->getScale());
		actor->RemoveSelectImg();
		actor->GetActionMgr().PlayAction("select_to_hand");
	}
	m_layer->removeWidget(m_ui);
}