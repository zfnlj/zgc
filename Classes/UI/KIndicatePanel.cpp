#include "KIndicatePanel.h"

#include "UIHelper.h"
#include "cocos-ext.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "KUIAssist.h"
#include "GameRoot.h"
#include "BattleFieldScene.h"

using namespace cocos2d::extension;

KIndicatePanel::KIndicatePanel()
{
}

void KIndicatePanel::init(cocos2d::extension::UILayer* layer)
{
	m_selActor = NULL;
	m_layer = layer;
}

void KIndicatePanel::Update(float dt)
{
	KBattleCtrlBase::BattleState state = GameRoot::getSingleton().BattleCtrl().GetBattleState();
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	FBattleGuy* pOtherPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();

	KCardInstList curActiveGreen;
	KCardInstList curActiveRed;

	bool bShowMyFightArea = false;
	switch(state){
	case KBattleCtrlBase::battle_select_handcard:
		if(pMainPlayer->IsSelectHandCard()){
			KCardInstList* lst = pMainPlayer->QueryCardSet(KCardInst::enum_slot_hand);
			_copyCardSet(lst,&curActiveGreen);
		}
		break;
	case KBattleCtrlBase::battle_play:
		{
			if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()|| KAction::GetTotalClassActionNum()>0)
				break;
			pMainPlayer->QueryActiveHandCards(&curActiveGreen);
			pMainPlayer->QueryActiveFightCards(&curActiveGreen);
			KCardInst* pSrc = GameRoot::getSingleton().BattleCtrl().GetCurSrcCard();
			if(!pSrc) break;
			if(GameRoot::getSingleton().BattleCtrl().QueryEnterFightTarget(pSrc,&curActiveGreen,&curActiveRed)) break;
			GameRoot::getSingleton().BattleCtrl().QuerySkillTarget(pSrc,&curActiveGreen,&curActiveRed);
			switch(pSrc->GetType()){
			case KCardStatic::card_soldier:
				if(pSrc->GetSlot()==KCardInst::enum_slot_hand){
					bShowMyFightArea = true;
				}else if(pSrc->GetSlot()==KCardInst::enum_slot_fight){
					pOtherPlayer->QueryActiveDefendCards(&curActiveRed);
				}
				break;
             default:
                break;
			}
		}
		break;
    default:
        break;
	}
	DeactiveActor(&m_ActiveGreenArr);
	DeactiveActor(&m_ActiveRedArr);
	ActiveArr(&curActiveGreen,true);
	ActiveArr(&curActiveRed,false);

	m_ActiveGreenArr.clear();
	m_ActiveRedArr.clear();

	_copyCardSet(&curActiveGreen,&m_ActiveGreenArr);
	_copyCardSet(&curActiveRed,&m_ActiveRedArr);

	if(bShowMyFightArea){
		GameRoot::getSingleton().getBattleScene()->ActiveMyFightArea();
	}else{
		GameRoot::getSingleton().getBattleScene()->DeactiveMyFightArea();
	}
}

void KIndicatePanel::DeactiveActor(KCardInstList* actived)
{
	for(KCardInstList::iterator it = actived->begin();it!=actived->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		actor->Deactive();
	}
}

void KIndicatePanel::ActiveArr(KCardInstList* lst,bool bGreen)
{
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		if(bGreen){
			actor->ActiveGreen();
		}else{
			actor->ActiveRed();
		}
	}
}

void KIndicatePanel::OnSelectCard(KCardActor* actor)
{
	if(m_selActor==actor) return;
	
	UIImageView* base = (UIImageView*)m_layer->getWidgetByName("big_photo");
	if(m_selActor){
		m_selActor->GetActionMgr().PlayAction("photo_fadeout");
		m_selActor->GetActionMgr().LimitAlive("fire");
	}
	if(actor){
		actor->GetActionMgr().PlayAction("photo_fadein");
	}
	m_selActor = actor;
}