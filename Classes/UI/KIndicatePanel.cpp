#include "KIndicatePanel.h"

#include "UIHelper.h"
#include "cocos-ext.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "GameRoot.h"
#include "BattleFieldScene.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "../GameLogic/assist/KSkillAssist.h"

using namespace cocos2d::extension;

KIndicatePanel::KIndicatePanel()
{
}

void KIndicatePanel::init(cocos2d::extension::UILayer* layer)
{
	m_selActor = NULL;
	m_layer = layer;
}

void KIndicatePanel::UpdateSleepAnim()
{
	KCardInstList tmpList;
	if(GameRoot::getSingleton().BattleCtrl().IsMyTurn()){
		FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
		pMainPlayer->QuerySleepFightCards(&tmpList);
	}
	for(KCardInstList::iterator it=m_SleepArr.begin();it!=m_SleepArr.end();++it){
		KCardInst* card = *it;
		if(_getIndexOfCard(&tmpList,card)<0) DoCardSleep(card,false);
	}
	for(KCardInstList::iterator it=tmpList.begin();it!=tmpList.end();++it){
		KCardInst* card = *it;
		DoCardSleep(card,true);
	}
	m_SleepArr.clear();
	_copyCardSet(&tmpList,&m_SleepArr);
}

void KIndicatePanel::DoCardSleep(KCardInst* card,bool flag)
{
	KCardActor* actor = (KCardActor*) card->getActor();
	if(!actor) return;
	if(flag){
		if(!actor->GetActionMgr().ExistAction("sleep_effect")){
			actor->GetActionMgr().PlayAction("sleep_effect");
		}
	}else{
		actor->GetActionMgr().LimitAlive("sleep_effect");
	}
}

void KIndicatePanel::Update(float dt)
{
	UpdateSleepAnim();
	if(KGameRecordMgr::getSingleton().IsPlaying()){
		GameRoot::getSingleton().getBattleScene()->DeactiveMyFightArea();
		return;
	}

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
			if(!KUIAssist::_IsPlayCardAble())
				break;
			KCardInst* pSrc = GameRoot::getSingleton().BattleCtrl().GetCurSrcCard();
			if(pSrc){
				if(pSrc->IsKindOf(KCardStatic::card_soldier)){
					GameRoot::getSingleton().BattleCtrl().QueryEnterFightTarget(pSrc,&curActiveGreen,&curActiveRed);
					if(pSrc->GetSlot()==KCardInst::enum_slot_hand){
						
						bShowMyFightArea = true;
						if(curActiveGreen.size()+curActiveRed.size()>0 && KSkillAssist::_needEnterFightTarget(pSrc->GetCardId())) KUIAssist::_playAdviceMsg(ADVICE_ENTER_OR_CAST);
					}else if(pSrc->GetSlot()==KCardInst::enum_slot_fight){
						pOtherPlayer->QueryActiveDefendCards(&curActiveRed);
					}
				}else if(pSrc->IsKindOf(KCardStatic::card_skill)){
					GameRoot::getSingleton().BattleCtrl().QuerySkillTarget(pSrc,&curActiveGreen,&curActiveRed);
				}
			}else{
				pMainPlayer->QueryValidateHandCards(&curActiveGreen);
				pMainPlayer->QueryValidateFightCards(&curActiveGreen);
				if(curActiveGreen.size()==0) KUIAssist::_playAdviceMsg(ADVICE_TURN_END);
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

	KBattleCtrlBase::BattleOp& op = GameRoot::getSingleton().BattleCtrl().GetCurOp();
	if(op._slot>=0){
		bShowMyFightArea= false;
	}

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

void KIndicatePanel::OnSelectCardOK()
{
	if(m_selActor) m_selActor->GetActionMgr().LimitAlive("card_breathe");
	m_selActor = NULL;
}

void KIndicatePanel::OnSelectSrcCard(KCardActor* actor)
{
	if(m_selActor==actor){
		return;
	}
	
	if(m_selActor){
		m_selActor->GetActionMgr().LimitAlive("card_breathe");
		//m_selActor->GetActionMgr().LimitAlive("fire");
	}
	if(actor){
		actor->GetActionMgr().PlayAction("card_breathe");
		//actor->GetActionMgr().PlayAction("photo_fadein");
	}
	m_selActor = actor;
}

bool KIndicatePanel::IsActiveCard(KCardInst* card)
{
	if(_getIndexOfCard(&m_ActiveGreenArr,card)>=0) return true;
	if(_getIndexOfCard(&m_ActiveRedArr,card)>=0) return true;
	return false;
}

void KIndicatePanel::onBattleRefresh()
{
	m_SleepArr.clear();
	m_ActiveGreenArr.clear();
	m_ActiveRedArr.clear();
	m_selActor = NULL;
}