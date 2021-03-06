#include "KFightAreaPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "GameRoot.h"
#include "cocos-ext.h"
#include "BattleFieldScene.h"
#include "../GameRecord/KGameRecordMgr.h"

using namespace cocos2d::extension;

KFightAreaPanel::KFightAreaPanel()
{
}

KFightAreaPanel::~KFightAreaPanel()
{
	for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
		m_ActiveSprites[i]->removeFromParent();
		CC_SAFE_RELEASE(m_ActiveSprites[i]);
	}
}

void KFightAreaPanel::init(cocos2d::extension::UILayer* layer)
{
	m_area.init(NULL);
	m_layer = layer;
	m_bShowActive = false;
	for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
		UIWidget* pBut = KUIAssist::GetIndexWidget(layer->getRootWidget(),"my_fight_slot",i);
		pBut->setTag(i);
		pBut->addPushDownEvent(this, coco_pushselector(KFightAreaPanel::onClickFightArea));
		m_ActiveSprites[i] = KUIAssist::CreateAnimationSprite("active_green");
		//m_ActiveSprites[i]->setPosition(pBut->getWorldPosition());
		m_ActiveSprites[i]->setScale(0.8f);
		m_ActiveSprites[i]->setVisible(false);
		m_ActiveSprites[i]->setAnchorPoint(ccp(0.5,0.5));
		pBut->addRenderer(m_ActiveSprites[i],1);
		CC_SAFE_RETAIN(m_ActiveSprites[i]);
	}
	UIWidget* pClickArea = UIHelper::seekWidgetByName(layer->getRootWidget(),"fight_area_click");
	if(pClickArea){
		pClickArea->setTag(-1);
		pClickArea->addPushDownEvent(this, coco_pushselector(KFightAreaPanel::onClickFightArea));
	}
}

void KFightAreaPanel::Show()
{
	m_bShowActive = true;
}

void KFightAreaPanel::Hide()
{
	m_bShowActive = false;
	for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
		ActiveSlot(i,false);
	}
	if(KClientBattleCtrl::getInstance()->IsMyTurn()){
		KBattleCtrlBase::BattleOp& op = KClientBattleCtrl::getInstance()->GetCurOp();
		if(op._src && op._slot>=0) ActiveSlot(op._slot,true);
	}
}

void KFightAreaPanel::ActiveSlot(int pos,bool flag)
{
	if(flag&&!KGameRecordMgr::getSingleton().IsPlaying()){
		m_ActiveSprites[pos]->setVisible(true);
		//if(!m_ActiveSprites[pos]->getParent()) m_layer->addChild(m_ActiveSprites[pos],1);
	}else{
		m_ActiveSprites[pos]->setVisible(false);
		//m_ActiveSprites[pos]->removeFromParentAndCleanup(false);
	}
}

void KFightAreaPanel::update(float dt)
{
	
	if(m_bShowActive){
		FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
		for(int i=0;i<MAX_FIGHT_POS_NUM;i++){
			if(pMainPlayer&& pMainPlayer->IsEmptyFightSlot(i)){
				ActiveSlot(i,true);
			}else{
				ActiveSlot(i,false);
			}
		}
	}
	//m_area.update(dt);
}

void KFightAreaPanel::onClickFightArea(CCObject* sender)
{
	
	UIWidget* pBut = (UIWidget*)sender;
	int index = pBut->getTag();
	if(!KGameRecordMgr::getSingleton().IsClickFightAreaValidate(index)) return;
	if(!KClientBattleCtrl::getInstance()->IsMyTurn()) return;
	KCardInst* pSrc = KClientBattleCtrl::getInstance()->GetCurSrcCard();
	if(!pSrc) return;

	if(pSrc->GetSlot()==KCardInst::enum_slot_hand){
		switch(pSrc->GetKind()){
		case KCardStatic::card_soldier:
		case KCardStatic::card_secret:
			KClientBattleCtrl::getInstance()->OpSetSlot(index);
			break;
		case KCardStatic::card_skill:
			if(pSrc->IsTargetLess(KAbilityStatic::when_use)) KClientBattleCtrl::getInstance()->OpDone();
			break;
        default:
            break;
		}
	}
}

