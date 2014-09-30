#include "KUIAssist.h"
#include "KUICardAssist.h"
#include "cocos2d.h"
#include "../BattleFieldScene.h"
#include "../MainMenuScene.h"
#include "cocos-ext.h"
#include "../../GameLogic/KDynamicWorld.h"
#include "../../GameLogic/KCardInst.h"
#include "../../Facade/FBattleGuy.h"
#include "../KCardActor.h"
#include "GameRoot.h"
#include "KJsonDictMgr.h"
#include "../../GameRecord/KGameRecordMgr.h"
#include "../StageWaitScene.h"
#include "../CardGroupScene.h"
#include "KPopupLayer.h"
#include "../../common/KUIMsgDef.h"
#include "../KShopScene.h"
#include "../../WorldObject/KMainPlayer.h"
#include "../../WorldObject/KPlayer.h"
#include "../../GameLogic/assist/KSkillAssist.h"
#include "../../GameLogic/assist/KBattleCtrlAssist.h"
#include "../../WorldObject/KPlayer.h"
#include "../../Item/KItemCreate.h"
#include "../StageSelectScene.h"
#include "../HeroBrowseScene.h"

#define SHOW_CARD_OFFSET 10
#define MAX_BUF_SLOT_NUM 5

static char* raceIcon[]={"null","gold","tree","water","fire","mud"};
using namespace cocos2d::extension;

KSceneLayerBase* KUIAssist::_activeScene = NULL;




bool KUIAssist::_queryScreenPos(const char* name,cocos2d::CCPoint& pt)
{
	UIWidget* obj = _activeSceneLayer()->getWidgetByName(name);
	if(!obj) return false;
	pt = obj->getWorldPosition();
	return true;
}

cocos2d::CCPoint KUIAssist::_querySecretShowPos(KCardInst* card)
{
	if(KClientBattleCtrl::getInstance()->IsMyCard(card)){
		UIWidget* widget = _activeScene->getWidgetByName("my_secret_show");
		if(widget) return widget->getWorldPosition();
	}else{
		UIWidget* widget = _activeScene->getWidgetByName("your_secret_show");
		if(widget) return widget->getWorldPosition();
	}
	return CCPoint(-500,-500);
}

cocos2d::CCPoint KUIAssist::_querySecretPos(KCardInst* card)
{
	bool bMy = KClientBattleCtrl::getInstance()->IsMyCard(card);
	const char* sz = card->GetBasePosName(bMy);
	UIWidget* obj =  _activeScene->getWidgetByName(sz);
	return obj->getWorldPosition();
}

cocos2d::CCPoint KUIAssist::_queryFighterPos(KCardInst* card)
{
	bool bMy = KClientBattleCtrl::getInstance()->IsMyCard(card);
	char sz[64];
	if(bMy){
		sprintf(sz,"my_fight_slot_%d",card->m_attr.getPos());
	}else{
		sprintf(sz,"your_fight_slot_%d",card->m_attr.getPos());
	}
	UIImageView* base = (UIImageView*)_activeScene->getWidgetByName(sz);
	return base->getWorldPosition();
}

void KUIAssist::_showCardSet(KCardInstList* lst)
{
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		cocos2d::CCPoint pt = _queryCardPos(lst,*it);
		actor->GetUI()->setPosition(pt);
		if(!actor->GetUI()->getParent()) _activeScene->addWidget(actor->GetUI());
	}
}

cocos2d::CCPoint KUIAssist::_queryCardPos(KCardInstList* lst,KCardInst* card,UIWidget* base)
{
	cocos2d::CCPoint pt(-999.0f,-999.0f);
	if(!base) base = _activeScene->getWidgetByName(_getBasePosName(card));
	
	if(card->GetSlot()==KCardInst::enum_slot_tomb){
		if(!lst) lst = KBattleCtrlAssist::GetCardSet(KClientBattleCtrl::getInstance(),card);
		KCardActor* cardActor = (KCardActor*)card->getActor();
		cardActor->GetUI()->setZOrder(50+_getIndexOfCard(lst,card));
		return base->getWorldPosition();
	}else if(card->GetSlot()==KCardInst::enum_slot_fight||
		card->GetSlot()==KCardInst::enum_slot_hero){
			return base->getWorldPosition();;
	}

	if(!lst) lst = KBattleCtrlAssist::GetCardSet(KClientBattleCtrl::getInstance(),card);

	
	float realWidth = base->getSize().width*base->getScale();
	int cardNum = lst->size();
	int totalWidth;
	int offset;
	if(cardNum> 5){
		totalWidth = realWidth*5;
		offset = totalWidth/cardNum;
	}else{
		totalWidth  =cardNum *realWidth  + SHOW_CARD_OFFSET*(cardNum-1);
		offset = realWidth + SHOW_CARD_OFFSET;
	}

	int cur_x = base->getWorldPosition().x -(totalWidth-offset)*0.5;
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		cocos2d::CCPoint pt = base->getWorldPosition();
		pt.x = cur_x;
		if(*it == card) return pt;
		cur_x += offset;
	}
	return pt;
}

const char* KUIAssist::_getBasePosName(KCardInst* card)
{
	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	bool bMy = (pMainPlayer->GetCard(card->GetRealId()))?true:false;
	return card->GetBasePosName(bMy);
}

void KUIAssist::_updateCard(KCardInst* card)
{
	KCardActor* cardActor = (KCardActor*)card->getActor();
	cardActor->UpdateUI();
	if(!cardActor->GetUI()->getParent()) _activeScene->addWidget(cardActor->GetUI());
}

void KUIAssist::_showCard(KCardInst* card)
{
	cocos2d::CCPoint pt = _queryCardPos(NULL,card);
	KCardActor* actor = KCardActor::create(card);
	actor->GetUI()->setPosition(pt);
	if(!actor->GetUI()->getParent()) _activeScene->addWidget(actor->GetUI());

	
	FBattleGuy* guy = KClientBattleCtrl::getInstance()->GetCardOwner(card);
	KCardInstList* lst = guy->QueryCardSet(card->GetSlot());
	KUIAssist::_moveCardSet(lst,"card_resort");
	
}

void KUIAssist::_removeCardSet(KCardInstList* lst)
{
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		KCardActor* actor = (KCardActor*)card->getActor();
		actor->Remove("");
	}
}

void KUIAssist::_moveCardSet(KCardInstList* lst,const char* actionname)
{
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		KCardActor* actor = KCardActor::create(card);
		actor->GetActionMgr().PlayAction(actionname);
	}
}

UIWidget* KUIAssist::_setVisible(UIWidget* panel,const char* baseName,int pos,bool bVisible)
{
	UIWidget* pWidget = GetIndexWidget(panel,baseName,pos);
	if(pWidget) pWidget->setVisible(bVisible);
	return pWidget;
}

UIWidget* KUIAssist::GetIndexWidget(UIWidget* panel,const char* baseName,int pos)
{
	char sz[64];
	sprintf(sz,"%s_%d",baseName,pos);
	return  UIHelper::seekWidgetByName(panel,sz);
}

void KUIAssist::_switch2ShopScene()
{
	CCScene* scene = CCTransitionSplitRows::create(0.5f, KShopScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2StageWaitScene()
{
	CCScene* scene = CCTransitionSplitRows::create(0.5f, StageWaitScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2StageSelectScene()
{
	CCScene* scene = CCTransitionSplitRows::create(0.5f, StageSelectScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2CardGroupScene()
{
	//CCScene* scene = CCTransitionSplitRows::create(0.5f, RecCardGroupScene::scene());
	CCScene* scene = CCTransitionSplitRows::create(0.5f, CardGroupScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2HeroBrowseScene()
{
	//CCScene* scene = CCTransitionSplitRows::create(0.5f, RecCardGroupScene::scene());
	CCScene* scene = CCTransitionSplitRows::create(0.5f, HeroBrowseScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2BattleScene()
{
	CCScene* scene = CCTransitionSplitRows::create(0.5f, BattleFieldScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);
}

void KUIAssist::_switch2MainMenu()
{
	CCScene* scene = CCTransitionFadeBL::create(0.5f, MainMenuScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);
}

void KUIAssist::_moveInWidget(UIWidget* panel,CCPoint& src,CCPoint& des,float elapsed)
{
	panel->setPosition(src);


	CCPoint vTmp = des - src;

	ccBezierConfig bezier;
	bezier.controlPoint_1 = vTmp*0.25f + src;
	bezier.controlPoint_2 = vTmp*0.75f + src;
	bezier.endPosition = des;

	CCActionInterval*  ActionInter = CCSequence::create(
		CCBezierTo::create(elapsed, bezier),NULL);
	panel->runAction( ActionInter);	

}

void KUIAssist::_moveOutWidget(UIWidget* panel,CCPoint& src,CCPoint& des,float elapsed)
{
	panel->setPosition(src);


	CCPoint vTmp = des - src;

	ccBezierConfig bezier;
	bezier.controlPoint_1 = vTmp*0.90f + src;
	bezier.controlPoint_2 = vTmp*0.95f + src;
	bezier.endPosition = des;

	CCActionInterval*  ActionInter = CCSequence::create(
		CCBezierTo::create(elapsed, bezier),NULL);
	panel->runAction( ActionInter);	

}

CCAction* KUIAssist::_fadeIn(UIWidget* widget,float val)
{
	if(!widget) return NULL;

	if(val<0.01){
		widget->setOpacity(255);
		return NULL;
	}else{
		CCActionInterval*  action1 = CCFadeIn::create(val);
		widget->runAction( CCSequence::create( action1, NULL, NULL));
		return action1;
	}
}

void KUIAssist::_setButVisible(UIWidget* but,bool flag)
{
	if(!but) return;
	but->setVisible(flag);
	but->setTouchEnable(flag);
}

CCGLProgram* KUIAssist::_createGrayProgram()
{
	GLchar* pszFragSource =
	 "#ifdef GL_ES \n \
	 precision mediump float; \n \
	 #endif \n \
	 uniform sampler2D u_texture; \n \
	 varying vec2 v_texCoord; \n \
	 varying vec4 v_fragmentColor; \n \
	 void main(void) \n \
	 { \n \
	 // Convert to greyscale using NTSC weightings \n \
	 vec4 col = texture2D(u_texture, v_texCoord); \n \
	 float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.299, 0.587, 0.114)); \n \
	 gl_FragColor = vec4(grey, grey, grey, col.a); \n \
	 }";
  
	CCGLProgram* pProgram = new CCGLProgram();
	pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
	return pProgram;
}

void KUIAssist::_loadAnimationData(const char* plistName,const char* animationName,int totalFrame,float delay)
{
	CCSpriteFrameCache *frameCache=CCSpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile(plistName);

	CCArray *animations = CCArray::createWithCapacity(totalFrame);                                                               
	                                                                                               
	for(int i = 1; i<= totalFrame; i++)                                                                                           
	{  
		char str[128];
		sprintf(str,"%s_%03d.png",animationName,i);                                                                        
		CCSpriteFrame *frame = frameCache->spriteFrameByName(str);                                                           
		animations->addObject(frame);                                                                                    
	} 
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations,delay); 
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation,animationName); 
}

CCSprite* KUIAssist::CreateAnimationSprite(const char* animationName,bool bLoop)
{
	CCAnimation* pNewAnim = CCAnimationCache::sharedAnimationCache()->animationByName(animationName);
	CCSprite* pSprite = CCSprite::create(); 
	if(bLoop){
		pSprite->runAction(CCRepeatForever::create( CCAnimate::create(pNewAnim) ) );
	}else{
		pSprite->runAction( CCAnimate::create(pNewAnim) );
	}
	pSprite->setAnchorPoint(ccp(0.50f,0.50f));
	return pSprite;
}










void KUIAssist::_createAffectAction(int actorId,const char* action,K3DActionParam* p,KAction* parent,int key)
{
	KCardInst* card = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),actorId);
	if(!card) return;

	KCardActor* actor = KCardActor::create(card);
	if(!actor->GetUI()->getParent()) _activeSceneLayer()->addWidget(actor->GetUI());

	K3DActionParam param;
	param.init(p);
	param.SetAction(action);
	KAction* pAction = actor->GetActionMgr().PlayAction(&param,key);
	if(pAction) pAction->SetParent(parent);
}

CCSize KUIAssist::_getRealSize(UIWidget* widget)
{
	return widget->getContentSize()*widget->getScale();
}

CCPoint KUIAssist::_getScreenCenter()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint pt = CCDirector::sharedDirector()->getVisibleOrigin();
	pt.x += visibleSize.width/2;
	pt.y += visibleSize.height/2;
	return pt;
}

CCAction* KUIAssist::_createAtkMove(UIWidget* widgetSrc,int des,float val)
{
	KCardInst* desCard = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),des);
	KCardActor* desActor = (KCardActor*)desCard->getActor();
	CCPoint pt1 = widgetSrc->getPosition();
	CCPoint pt2;
	
	CCPoint ptLast = desActor->GetUI()->getPosition();
	CCPoint centerPt = KUIAssist::_getScreenCenter();
	if(pt1.y>centerPt.y){
		ptLast.y += _getRealSize(desActor->GetUI()).height;
		pt2= ccp(centerPt.x, centerPt.y*2);
		pt1.y+=200;
	}else{
		pt1.y-=200;
		ptLast.y -= _getRealSize(desActor->GetUI()).height;
		pt2 = ccp(centerPt.x, 0);
	}

	CCPoint pt3;
	pt3.y = pt1.y;
	pt3.x = ptLast.x;

	
	ccBezierConfig bezierCon;
	bezierCon.controlPoint_1=pt1;
	bezierCon.controlPoint_2=pt2;
	bezierCon.endPosition =pt3;

	CCActionInterval * action = CCBezierTo::create(0.8*val, bezierCon);


	CCActionInterval* move = CCMoveTo::create(0.2*val, ptLast);
	CCActionInterval* move_ease_out = CCEaseElasticOut::create(move,0.05f);

	CCSequence* seq1 = CCSequence::create(action, move_ease_out, NULL);
	widgetSrc->runAction(seq1);
	return seq1;
}

CCParticleSystem* KUIAssist::_createParticle(const char* name)
{
	CCParticleSystem* emitter = new CCParticleSystemQuad();
	char msg[128];
	sprintf(msg,"data/Particles/%s.plist",name);
	emitter->initWithFile(msg);
	emitter->autorelease();
	return emitter;
}

KCardActor* KUIAssist::_getCardActor(int realId)
{
	 KCardInst* card = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),realId);
	 if(!card) return NULL;
	 KCardActor* actor = KCardActor::create(card);
	 if(!actor->GetUI()->getParent()){
		 actor->GetUI()->setPosition(_queryCardPos(NULL,card));
	 }
	 return actor;
}

void KUIAssist::_updateSecretIcon(bool bMy,KCardInstList* lst)
{
	UIWidget* panel = _activeScene->getRootWidget();

	for(int i=0;i<MAX_SECRET_POS_NUM;i++){
		UIWidget* widget = GetIndexWidget(panel,(bMy)?"my_secret_slot":"your_secret_slot",i);
		if(widget) widget->setVisible(false);
	}
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardInst* card = *it;
		const char* widgetName = card->GetBasePosName(bMy);
		UIWidget* widget = UIHelper::seekWidgetByName(panel,widgetName);
		if(widget){
			if(bMy) widget->addPushDownEvent(card->getActor(), coco_pushselector(KCardActor::DoSelect));
			widget->setTag(card->GetRealId());
			widget->setVisible(true);
		}
	}
}



void KUIAssist::_soldierShow(KCardInst* card)
{
	if(!card->IsKindOf(KCardStatic::card_soldier)) return;
	if(card->GetSlot()!=KCardInst::enum_slot_fight) return;
	KCardActor* actor = (KCardActor*)card->getActor();
	if(!actor) return;
	//actor->GetActionMgr().PlayAction("enterfight_dust");
	KCardStatic* pST = card->GetST();
	if(!pST->IsShow()) return;
	actor->GetActionMgr().PlayAction(pST->GetShow());
}

void KUIAssist::_updateCardListBuf(KCardInstList* lst)
{
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		if(actor) actor->UpdateCardBuf();
	}
}

void KUIAssist::_resortCardSet(FBattleGuy* guy,int slot)
{
	KCardInstList* lst = guy->QueryCardSet(slot);
	KCardInstList tmpLst;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		if(actor&&actor->GetUI()) tmpLst.push_back(*it);
	}
	_moveCardSet(&tmpLst,"card_move");
}


void KUIAssist::_resortHandCardSet(FBattleGuy* guy)
{
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		if(actor&&actor->GetUI()) tmpLst.push_back(*it);
	}
	_moveCardSet(&tmpLst,"card_resort");
}

void KUIAssist::_delayResortHandCardSet(FBattleGuy* guy)
{
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		if(actor&&actor->GetUI()) tmpLst.push_back(*it);
	}
	_moveCardSet(&tmpLst,"card_delayMove");
}

void KUIAssist::_setActionParamSlot(K3DActionParam* param)
{
	KCardInst* card = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),param->_srcID);
	if(card) param->_srcSlot = card->GetSlot();
	for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
		card = KBattleCtrlAssist::GetCard(KClientBattleCtrl::getInstance(),param->_desArr[i]);
		if(!card) break;
		param->_destSlot[i] = card->GetSlot();
	}

}

bool KUIAssist::_IsPlayCardAble()
{
	if(!KClientBattleCtrl::getInstance()->IsMyTurn()) return false;
	if(KAction::GetTotalClassActionNum()>0){
		return false;
	}
	if(KClientBattleCtrl::getInstance()->IsWaitDrama()) return false;
	return true;
}

bool KUIAssist::_IsValidateSrcCard(KCardInst* card)
{
	if(!KClientBattleCtrl::getInstance()->IsMyTurn()) return false;
	KCardInstList curActive;

	FBattleGuy* pMainPlayer = KClientBattleCtrl::getInstance()->GetMainPlayer();
	pMainPlayer->QueryValidateHandCards(&curActive);
	pMainPlayer->QueryValidateFightCards(&curActive);
	
	return (_getIndexOfCard(&curActive,card)>=0);
}

bool KUIAssist::_IsValidateDesCard(KCardInst* card)
{
	if(!KClientBattleCtrl::getInstance()->IsMyTurn()) return false;
	KCardInst* pSrc = KClientBattleCtrl::getInstance()->GetCurSrcCard();
	if(!pSrc) return false;

	KCardInstList curActiveGreen;
	KCardInstList curActiveRed;
	KBattleGuy* defGuy = KClientBattleCtrl::getInstance()->GetDefGuy();
	if(pSrc->IsKindOf(KCardStatic::card_soldier)){



		KBattleCtrlAssist::QueryEnterFightTarget(KClientBattleCtrl::getInstance(),pSrc,&curActiveGreen,&curActiveRed);
		if(pSrc->GetSlot()==KCardInst::enum_slot_fight){
			defGuy->QueryActiveDefendCards(&curActiveRed);
		}
	}else if(pSrc->IsKindOf(KCardStatic::card_skill)){
		KBattleCtrlAssist::QuerySkillTarget(KClientBattleCtrl::getInstance(),pSrc,&curActiveGreen,&curActiveRed);
	}
	if(_getIndexOfCard(&curActiveGreen,card)>=0)  return true;
	if(_getIndexOfCard(&curActiveRed,card)>=0)  return true;
	return false;
}

void KUIAssist::_playClickCardAction(KCardInst* card)
{
	if(!card) return;
	KActor& actor = _activeSceneActor();
	K3DActionParam* pCurClickCardParam = actor.GetActionMgr().ExistAction("click_card");
	if(pCurClickCardParam && pCurClickCardParam->GetDesId(0)==card->GetRealId()) return;
	if(actor.GetActionMgr().FoundClassAction()){
		actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
		actor.GetActionMgr().breathe(0.1f);
		return;
	}
	K3DActionParam param;
	param.init("click_card");
	param.SetDestVal(card->GetRealId(),0);
	actor.GetActionMgr().PlayAction(&param);
}

void KUIAssist::_playClickSlotAction(int slot)
{
	KActor& actor = _activeSceneActor();
	K3DActionParam* pCurClickSlotParam = actor.GetActionMgr().ExistAction("click_fight_slot");
	if(pCurClickSlotParam && pCurClickSlotParam->GetDesId(0)==slot) return;
	if(actor.GetActionMgr().FoundClassAction()){
		actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
		actor.GetActionMgr().breathe(0.1f);
		return;
	}
	K3DActionParam param;
	param.init("click_fight_slot");
	param.SetDestVal(slot,0);
	actor.GetActionMgr().PlayAction(&param);

}

void KUIAssist::_playClickFightArea()
{
	KActor& actor = _activeSceneActor();
	if(actor.GetActionMgr().ExistAction("click_fight_area")) return;
	if(actor.GetActionMgr().FoundClassAction()){
		actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
		actor.GetActionMgr().breathe(0.1f);
		return;
	}
	actor.GetActionMgr().PlayAction("click_fight_area");
}

void KUIAssist::_playClickWidget(const char* widgetName)
{
	KActor& actor = _activeSceneActor();
	if(actor.GetActionMgr().ExistAction("click_widget")) return;
	if(actor.GetActionMgr().FoundClassAction()){
		actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
		actor.GetActionMgr().breathe(0.1f);
		return;
	}
	K3DActionParam param;
	param.init("click_widget");
	param.SetString(widgetName);
	actor.GetActionMgr().PlayAction(&param);
}

void KUIAssist::_playClickTurnEnd()
{
	KActor& actor = _activeSceneActor();
	if(actor.GetActionMgr().ExistAction("click_turnend_but")) return;
	if(actor.GetActionMgr().FoundClassAction()){
		actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
		actor.GetActionMgr().breathe(0.1f);
		return;
	}
	actor.GetActionMgr().PlayAction("click_turnend_but");
}

void KUIAssist::_stopClickAction()
{
	KActor& actor = _activeSceneActor();
	actor.GetActionMgr().LimitClassAction(KActionStatic::class_click);
	actor.GetActionMgr().breathe(0.1f);
}

void KUIAssist::_stopAdviceMsg()
{
	KActor& actor = _activeSceneActor();
	actor.GetActionMgr().LimitAlive("hero_talk");
}

void KUIAssist::_playAdviceMsg(int id)
{
	if(KGameRecordMgr::getSingleton().IsPlaying()&& id<1000) return;

	static DWORD lastAdviceTime = 0;
	if((GetTickCount()-lastAdviceTime) <2000) return;
	lastAdviceTime = GetTickCount();
	//if(lastAdviceId ==id) return;
	//lastAdviceId = id;
	_playLessonMsg(id);
}

void KUIAssist::_playLessonMsg(int id)
{
	KActor& actor = _activeSceneActor();
	K3DActionParam* pCurTalkParam = actor.GetActionMgr().ExistAction("hero_talk");

	if(pCurTalkParam && pCurTalkParam->GetDesId(0)==id) return;
	
	KHelpStringStatic* helpString = KGameStaticMgr::getSingleton().GetHelpString(id);
	if(!helpString) return;
	actor.GetActionMgr().LimitAlive("hero_talk");
	K3DActionParam param;
	param.init("hero_talk");
	param.SetDestVal(id,0);
	actor.GetActionMgr().PlayAction(&param);
}

void KUIAssist::ShowWidgetArr(UILayer* layer,const char* name,int num,bool flag)
{
	for(int i=0;i<num;i++){
		char sz[64];
		sprintf(sz,"%s_%d",name,i);
		UIWidget* widget = layer->getWidgetByName(sz);
		if(widget) widget->setVisible(flag);
	}
}

void KUIAssist::ShowButton(UIWidget* pBut,bool flag)
{
	pBut->setVisible(flag);
	pBut->setTouchEnable(flag);
}

KSceneLayerBase* KUIAssist::_activeSceneLayer()
{
	return _activeScene;
}

KActor& KUIAssist::_activeSceneActor()
{
	return _activeSceneLayer()->GetActor();
}

KGameRecordPanel& KUIAssist::_recordPanel()
{
	return _activeSceneLayer()->RecordPanel();
}

void KUIAssist::_popNotifyMsg(int dlgId)
{
	KPopupLayer::DoModal(UI_NOTIFY_STR,dlgId,KPopupLayer::DT_Ok);
}

UIWidget* KUIAssist::_createNormalItemWidget(KPlayerTmpBag::ItemDef item)
{
	KItemAbout::KCreateInfo_Item* itemInfo = KItemAbout::KItemCreate::GetInstancePtr()->GetCreateInfo_Item(item._id);
	if(!itemInfo) return NULL;
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(itemInfo->GetIcon());

	UIWidget* panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/Item_normal.json");
	UILayout* pBkWidget = (UILayout*)UIHelper::seekWidgetByName(panel,"bk");
	pBkWidget->setBackGroundImage(fullPath.c_str());
	UILabelBMFont* pCountWidget = (UILabelBMFont*)UIHelper::seekWidgetByName(panel,"count_txt");
	char sz[64];
	sprintf(sz,"X%d",item._count);
	pCountWidget->setText(sz);
	//itemWidget->loadTexture(fullPath.c_str());
	panel->setAnchorPoint(ccp(0.5f, 0.5f));
	return panel;
}

UIWidget* KUIAssist::_createBagItemWidget(KPlayerTmpBag::ItemDef item)
{

	UIWidget* widget = NULL;
	switch(item._type){
	case KPlayerTmpBag::item_hero_card:
		{
			const KHeroDef* heroDef = KMainPlayer::RealPlayer()->m_cardDepot.FindHero(item._id);
			widget = KUICardAssist::_createHero(*heroDef,false);
		}
		break;
	case KPlayerTmpBag::item_normal:
		{
			widget = _createNormalItemWidget(item);
		}
		break;
	default:
		break;
	}
	return widget;
}