#include "KCardActor.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "../StaticTable/KGameStatic.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "BattleFieldScene.h"
#include "assist/KUIAssist.h"
#include "assist/KJsonDictMgr.h"
#include "KClickCardMgr.h"
#include "../GameRecord/KGameRecordMgr.h"

USING_NS_CC;
using namespace cocos2d::extension;

cocos2d::extension::UIWidget* KCardActor::GetUI()
{
	//CCAssert( m_ui , "m_ui needs init!");
	return m_ui;
}

KCardActor::~KCardActor()
{
	Clear();
}

void KCardActor::update(float dt)
{
	m_ActionMgr.breathe(dt);
}

void KCardActor::Clear()
{
	KUIAssist::_hideBufIcon(m_ui);
	KJsonDictMgr::getSingleton().OnCardWidgetDestory(m_ui);
	m_ActionMgr.onDestory();
	if(m_ui){
		m_ui->removeFromParent();
	}
	if(m_bigPhoto){
		m_bigPhoto->removeFromParent();
	}
	Deactive();
	CC_SAFE_RELEASE(m_activeRedSprite);
	CC_SAFE_RELEASE(m_activeGreenSprite);
	CC_SAFE_RELEASE(m_ui);
	CC_SAFE_RELEASE(m_bigPhoto);
}

void KCardActor::ActiveGreen()
{
	if(!GetActiveGreenSprite()->getParent()){
		m_ui->addRenderer(GetActiveGreenSprite(),100);
	}
	CCPoint pt(m_ui->getContentSize().width*0.5,m_ui->getContentSize().height*0.5);
	GetActiveGreenSprite()->setPosition(pt);
	GetActiveGreenSprite()->setScale(1.02f);
}

void KCardActor::ActiveRed()
{
	if(!GetActiveRedSprite()->getParent()) m_ui->addRenderer(GetActiveRedSprite(),100);
	CCPoint pt(m_ui->getContentSize().width*0.5,m_ui->getContentSize().height*0.5);
	GetActiveRedSprite()->setPosition(pt);
	GetActiveRedSprite()->setScale(1.02f);
}

void KCardActor::Deactive()
{
	m_ui->removeRenderer(GetActiveRedSprite(),false);
	m_ui->removeRenderer(GetActiveGreenSprite(),false);
}

KCardActor* KCardActor::create(KCardInst* pCardInst,bool bBig)
{
	if(!pCardInst->GetST()) return NULL;
	if(pCardInst->getActor()){
		return (KCardActor*)pCardInst->getActor();
	}
	KCardActor* actor = new KCardActor;
	actor->autorelease();
	actor->init(pCardInst,bBig);
	return actor;
}

void KCardActor::UpdateUI()
{
	if(m_bigPhoto){
		m_bigPhoto->removeFromParent();
		CC_SAFE_RELEASE(m_bigPhoto);
	}
	cocos2d::extension::UIWidget* oldUI = m_ui;
	init(m_card);
	m_ui->setPosition(oldUI->getPosition());
	if(oldUI->getParent()) oldUI->removeFromParent();
	CC_SAFE_RELEASE(oldUI);
	
}

cocos2d::extension::UIWidget* KCardActor::GetBigCard()
{
	if(m_bigPhoto){
		UpdateCardAttr(m_bigPhoto,true);
		return m_bigPhoto;
	}

	char name[64]={0};
	sprintf(name,"card_%d",m_card->GetRealId());

	m_bigPhoto =  KUIAssist::_createCardLayout(m_card->GetST(),true);

	UpdateCardAttr(m_bigPhoto,true);
	CC_SAFE_RETAIN(m_bigPhoto);
	return m_bigPhoto;

}

void KCardActor::init(KCardInst* pInst,bool bBig)
{
	m_ui = NULL;
	m_bigPhoto = NULL;
	m_card = pInst;
	m_bSelected = false;
	char name[64]={0};
	sprintf(name,"card_%d",pInst->GetRealId());

	pInst->retainActor(this);

	if(GameRoot::getSingleton().BattleCtrl().IsShowBack(pInst)){ //对方手牌，不显示牌信息
		m_ui = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/cardBack.json");
		m_ui->setScale(0.8f);
		m_bBack = true;
	}else{
		m_ui = KUIAssist::_createCardLayout(pInst->GetST(),bBig);
		UpdateCardAttr(m_ui,true);
		m_bBack = false;
		RemoveSelectImg();
		m_ui->addPushDownEvent(this, coco_pushselector(KCardActor::DoSelect));
		m_ui->addMoveEvent(this, coco_moveselector(KCardActor::onMoveEvent));
	}
	m_activeRedSprite = NULL;
	m_activeGreenSprite = NULL;
	CC_SAFE_RETAIN(m_ui);
	m_ui->setPosition(ccp(-500.0f,-500.0f));
	KActor::init(m_ui);
}

CCSprite* KCardActor::GetActiveRedSprite()
{
	if(!m_activeRedSprite){
		m_activeRedSprite = KUIAssist::CreateAnimationSprite("active_red");
		CC_SAFE_RETAIN(m_activeRedSprite);
	}
	return m_activeRedSprite;
}

CCSprite* KCardActor::GetActiveGreenSprite()
{
	if(!m_activeGreenSprite){
		m_activeGreenSprite = KUIAssist::CreateAnimationSprite("active_green");
		CC_SAFE_RETAIN(m_activeGreenSprite);
	}
	return m_activeGreenSprite;
}

void KCardActor::RemoveSelectImg()
{
	UIImageView* pRemoveImage = (UIImageView*)UIHelper::seekWidgetByName((UIWidget*)m_ui,"remove_x");
	if(pRemoveImage) pRemoveImage->removeFromParent();
}

bool KCardActor::DoSelectBeginCard(CCObject* sender)
{
	if(GameRoot::getSingleton().BattleCtrl().GetBattleState()!=KBattleCtrlBase::battle_select_handcard) return false;
	m_bSelected = !m_bSelected;

	UIImageView* pRemoveImage = (UIImageView*)UIHelper::seekWidgetByName((UIWidget*)m_ui,"remove_x");
	if(!pRemoveImage){
		pRemoveImage = UIImageView::create();
		pRemoveImage->loadTexture("card_x.png",UI_TEX_TYPE_PLIST);
		pRemoveImage->setAnchorPoint(CCPoint(0.0f,0.0f));
		pRemoveImage->setName("remove_x");
		pRemoveImage->setWidgetZOrder(100);
		m_ui->addChild(pRemoveImage);
	}
	pRemoveImage->setVisible(m_bSelected);
	return true;
}

void KCardActor::onMoveEvent(CCObject* sender)
{
	int kk=0;
}
void KCardActor::DoSelect(CCObject* sender)
{
	if(!KGameRecordMgr::getSingleton().IsClickCardValidate(m_card)) return;
	KClickCardMgr::getSingleton().onClickCard(this);
	if(DoSelectBeginCard(sender)) return;
	if(!KUIAssist::_IsPlayCardAble()) return;

	if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()) return;
	BattleFieldScene* layer = GameRoot::getSingleton().getBattleScene();

	if(GameRoot::getSingleton().BattleCtrl().GetCurOp().IsEmpty()){
		if(KUIAssist::_IsValidateSrcCard(m_card)) GameRoot::getSingleton().getBattleScene()->DoSelectSrcCard(this);
	}else{
		int srcID = GameRoot::getSingleton().BattleCtrl().GetCurOp()._src;
		if(srcID==m_card->GetRealId()){
			GameRoot::getSingleton().getBattleScene()->DoSelectSrcCard(NULL);

			return;
		}
		if(KUIAssist::_IsValidateDesCard(m_card)){
			GameRoot::getSingleton().BattleCtrl().DoSelectCard(m_card);
		}else{
			if(KUIAssist::_IsValidateSrcCard(m_card)){
				GameRoot::getSingleton().getBattleScene()->DoSelectSrcCard(this);
			}
		}
	}
}

void KCardActor::UpdateCardAttr()
{
	if(m_bigPhoto) UpdateCardAttr(m_bigPhoto,true);
	if(m_ui) UpdateCardAttr(m_ui,false);
}
void KCardActor::UpdateCardBuf()
{
	if(m_ui){
		KUIAssist::_updateBufIcon(m_ui,m_card);
	}
}

void KCardActor::UpdateCardAttr(cocos2d::extension::UIWidget* ui,bool bBigCard,bool bInit,K3DActionParam* hitParam)
{
	if(!ui) return;
	UILabelAtlas* labelHp = (UILabelAtlas*)ui->getChildByName("hp");
	UILabelAtlas* labelAtk = (UILabelAtlas*)ui->getChildByName("atk");
	if(m_card->IsKindOf(KCardStatic::card_hero)||
		m_card->IsKindOf(KCardStatic::card_soldier)){
		
		char info[64]={0};
		if(hitParam){
			int val = hitParam->GetVal(m_card->GetRealId());
			int oldHp = atoi(labelHp->getStringValue());
			int newVal = oldHp + val;
			if(val <0){
				if(newVal < m_card->GetHp()) newVal = m_card->GetHp();
			}else{
				if(newVal > m_card->GetHp()) newVal = m_card->GetHp();
			}
			sprintf(info,"%d",newVal);
		}else{
			if(bInit){
				sprintf(info,"%d",m_card->GetST()->GetHp());
			}else{
				sprintf(info,"%d",m_card->GetHp());
			}
		}
		if(strcmp(info,labelHp->getStringValue())!=0){
			if(!bInit && !bBigCard){
				float oldScale = labelHp->getScale();
				CCActionInterval*  actionBy = CCScaleBy::create(0.2f, 1.2/oldScale, 1.2/oldScale);
				labelHp->runAction( CCSequence::create(actionBy, actionBy->reverse(), NULL));
			}
			labelHp->setStringValue(info);
		}
		if(bInit){
			sprintf(info,"%d",m_card->GetST()->GetAtk());
		}else{
			sprintf(info,"%d",m_card->GetAtk());
		}
		if(strcmp(info,labelAtk->getStringValue())!=0){
			if(!bInit && !bBigCard){
				float oldScale = labelAtk->getScale();
				CCActionInterval*  actionBy = CCScaleBy::create(0.2f, 1.2/oldScale, 1.2/oldScale);
				labelAtk->runAction( CCSequence::create(actionBy, actionBy->reverse(), NULL));
			}
			labelAtk->setStringValue(info);
		}
	}else{
		labelHp->setVisible(false);
		labelAtk->setVisible(false);
	}

}

void KCardActor::MoveBack(float speed)
{
	K3DActionParam param;
	switch(m_card->GetSlot()){
	case KCardInst::enum_slot_tomb:
		{
			if(m_card->IsKindOf(KCardStatic::card_hero)){
				param.init("hero_dead");	
			}else{
				param.init("go_tomb");	
			}
			/*if(m_card->IsKindOf(KCardStatic::card_skill)){
				FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(m_card);
				KUIAssist::_resortHandCardSet(guy);
			}*/
		}
		break;
	case KCardInst::enum_slot_hand:
		{
			GameRoot::getSingleton().getBattleScene()->onFighterBackHand(m_card);
			return;
		}
		break;
	default:
		{
			param.init("go_back");
		}
		break;
	}
	m_ActionMgr.CacheImediateAction(param);
	
}

void KCardActor::DrawCard()
{
	UpdateUI();
	KCardInstList lst;
	lst.push_back(m_card);

	GameRoot::getSingleton().getBattleScene()->onDrawCard(&lst,true);
}
void KCardActor::MoveReached(CCObject* sender)
{
}

cocos2d::extension::UIWidget* KCardActor::GetWidget(const char* obj)
{

	if(strcmp(obj,"card")==0){
		return m_ui;
	}else if(strcmp(obj,"bigcard")==0){
		return GetBigCard();
	}else{
		return KActor::GetWidget(obj);
	}
}

void KCardActor::initTombAction(K3DActionParam* param)
{
	m_tombAction.clone(param);
}

CCNode* KCardActor::ShowHit(const char* slot,K3DActionParam* param,float scale,bool bGood)
{
	int val = param->GetVal(m_card->GetRealId());

	UIWidget* widget = GetWidget(slot);
	char msg[64];
	if(val >0){
		sprintf(msg,"+%d",val);
	}else{
		if(bGood){
			sprintf(msg,"+%d",val);
		}else{
			if(val==0)
				sprintf(msg,"-%d",val);
			else
				sprintf(msg,"%d",val);
		}
	}
	CCLabelBMFont* bmFont = CCLabelBMFont::create(msg,"GUI/num_1.fnt");
	bmFont->setAnchorPoint(ccp(0.50f,0.50f));
	bmFont->setPosition(widget->getWorldPosition());
	KUIAssist::_activeSceneLayer()->addChild(bmFont,100);

	bmFont->setScale(scale);
	m_objectDict.setObject(bmFont, "hit_text");
	return bmFont;
}

CCPoint KCardActor::GetDestPosition(K3DActionParam* param,const char* slot,int index)
{
	if(strcmp(slot,"card_pos")==0){
		return KUIAssist::_queryCardPos(NULL,m_card);
	}else if(strcmp(slot,"secret_pos")==0){
		return KUIAssist::_querySecretPos(m_card);
	}else if(strcmp(slot,"secret_show")==0){
		return KUIAssist::_querySecretShowPos(m_card);
	}else if(strcmp(slot,"res")==0){
		bool bMy = GameRoot::getSingleton().BattleCtrl().IsMyCard(m_card);
		UIWidget* widget = (bMy)?KUIAssist::_activeScene->getWidgetByName("my_res"):KUIAssist::_activeScene->getWidgetByName("your_res");
		if(widget) return widget->getWorldPosition();
	}
	return KActor::GetDestPosition(param,slot,index);
}

void KCardActor::SummonSelf()
{
	//secret card use other method
	//int slot = m_card->GetSlot();
	//m_card->m_attr.setSlot(KCardInst::enum_slot_fight);
	KUIAssist::_showCard(m_card);
	//m_card->m_attr.setSlot(slot);
}

void KCardActor::addWidget(const char* obj,int z)
{
	UIWidget* widget = NULL;
	
	if(strcmp(obj,"bigcard")==0){
		widget = GetBigCard();	
	}
	else if(strcmp(obj,"card")==0){
		UpdateUI();
		widget = m_ui;
	}
	if(widget) {
		widget->setZOrder(z);
		KUIAssist::_activeSceneLayer()->addWidget(widget);
	}
}

CCNode* KCardActor::GetCNode(const char* obj)
{
	if(strcmp(obj,"bigcard")==0){
		return GetBigCard()->getRenderer();
	}
	else if(strcmp(obj,"card")==0){
		return m_ui->getRenderer();
	}else{
		return  KActor::GetCNode(obj);
	}
}

void KCardActor::delWidget(const char* obj)
{
	UIWidget* widget = NULL;
	if(strcmp(obj,"bigcard")==0){
		widget = GetBigCard();	
	}
	else if(strcmp(obj,"card")==0){
		widget = m_ui;
	}
	if(widget) KUIAssist::_activeSceneLayer()->removeWidget(widget);
}

void KCardActor::updateSecret()
{
	KCardInstList* cardLst = m_card->GetOwner()->QueryCardSet(KCardInst::enum_slot_secret);
	bool bMy  = GameRoot::getSingleton().BattleCtrl().GetMainPlayer()== m_card->GetOwner()->GetFacade();
	KUIAssist::_updateSecretIcon(bMy,cardLst);
}

void KCardActor::MoveOnHit(K3DActionParam* param,float speed,CCActionDef& actionDef)
{
	KCardInst::CardSlot slot = (KCardInst::CardSlot)param->GetSlot(m_card->GetRealId());
	if(slot == KCardInst::enum_slot_fight){
		CCPoint pt = KUIAssist::_queryFighterPos(m_card);
		Move("",pt,speed,actionDef);

	}else{
		MoveBack(speed);
	}
}