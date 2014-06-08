#include "KUIAssist.h"
#include "cocos2d.h"
#include "../BattleFieldScene.h"
#include "../MainMenuScene.h"
#include "cocos-ext.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "../Facade/FBattleGuy.h"
#include "../KCardActor.h"
#include "GameRoot.h"
#include "KJsonDictMgr.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "../StageWaitScene.h"
#include "../CardGroupScene.h"

#define SHOW_CARD_OFFSET 6
#define MAX_BUF_SLOT_NUM 5

static char* raceIcon[]={"null","gold","tree","water","fire","mud"};
using namespace cocos2d::extension;

BattleFieldScene* KUIAssist::_battleScene = NULL;
void KUIAssist::SetRaceIcon(UIWidget* widgetRace, int race)
{
	char sz[32];
	UIImageView* imageWidget = (UIImageView*)widgetRace;

	if(race !=(int)KCardStatic::race_null){
		sprintf(sz,"%s_icon.png",raceIcon[race]);
		imageWidget->loadTexture(sz,UI_TEX_TYPE_PLIST);
		imageWidget->setVisible(true);
	}else{
		imageWidget->setVisible(false);
	}
}


bool KUIAssist::_queryScreenPos(const char* name,cocos2d::CCPoint& pt)
{
	UIWidget* obj = _activeSceneLayer()->getWidgetByName(name);
	if(!obj) return false;
	pt = obj->getWorldPosition();
	return true;
}

cocos2d::CCPoint KUIAssist::_querySecretShowPos(KCardInst* card)
{
	if(GameRoot::getSingleton().BattleCtrl().IsMyCard(card)){
		UIWidget* widget = _battleScene->getWidgetByName("my_secret_show");
		if(widget) return widget->getWorldPosition();
	}else{
		UIWidget* widget = _battleScene->getWidgetByName("your_secret_show");
		if(widget) return widget->getWorldPosition();
	}
	return CCPoint(-500,-500);
}

cocos2d::CCPoint KUIAssist::_querySecretPos(KCardInst* card)
{
	bool bMy = GameRoot::getSingleton().BattleCtrl().IsMyCard(card);
	const char* sz = card->GetBasePosName(bMy);
	UIWidget* obj =  _battleScene->getWidgetByName(sz);
	return obj->getWorldPosition();
}

cocos2d::CCPoint KUIAssist::_queryFighterPos(KCardInst* card)
{
	bool bMy = GameRoot::getSingleton().BattleCtrl().IsMyCard(card);
	char sz[64];
	if(bMy){
		sprintf(sz,"my_fight_slot_%d",card->m_attr.getPos());
	}else{
		sprintf(sz,"your_fight_slot_%d",card->m_attr.getPos());
	}
	UIImageView* base = (UIImageView*)_battleScene->getWidgetByName(sz);
	return base->getWorldPosition();
}

void KUIAssist::_showCardSet(KCardInstList* lst)
{
	for(KCardInstList::iterator it=lst->begin();it!=lst->end();++it){
		KCardActor* actor = KCardActor::create(*it);
		cocos2d::CCPoint pt = _queryCardPos(lst,*it);
		actor->GetUI()->setPosition(pt);
		if(!actor->GetUI()->getParent()) _battleScene->addWidget(actor->GetUI());
	}
}

cocos2d::CCPoint KUIAssist::_queryCardPos(KCardInstList* lst,KCardInst* card)
{
	cocos2d::CCPoint pt(-999.0f,-999.0f);
	UIImageView* base = (UIImageView*)_battleScene->getWidgetByName(_getBasePosName(card));
	
	if(card->GetSlot()==KCardInst::enum_slot_tomb){
		if(!lst) lst = GameRoot::getSingleton().BattleCtrl().GetCardSet(card);
		KCardActor* cardActor = (KCardActor*)card->getActor();
		cardActor->GetUI()->setZOrder(50+_getIndexOfCard(lst,card));
		return base->getWorldPosition();
	}else if(card->GetSlot()==KCardInst::enum_slot_fight||
		card->GetSlot()==KCardInst::enum_slot_hero){
			return base->getWorldPosition();;
	}

	if(!lst) lst = GameRoot::getSingleton().BattleCtrl().GetCardSet(card);

	
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
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	bool bMy = (pMainPlayer->GetCard(card->GetRealId()))?true:false;
	return card->GetBasePosName(bMy);
}

void KUIAssist::_updateCard(KCardInst* card)
{
	KCardActor* cardActor = (KCardActor*)card->getActor();
	cardActor->UpdateUI();
	if(!cardActor->GetUI()->getParent()) _battleScene->addWidget(cardActor->GetUI());
}

void KUIAssist::_showCard(KCardInst* card)
{
	cocos2d::CCPoint pt = _queryCardPos(NULL,card);
	KCardActor* actor = KCardActor::create(card);
	actor->GetUI()->setPosition(pt);
	if(!actor->GetUI()->getParent()) _battleScene->addWidget(actor->GetUI());

	
	FBattleGuy* guy = GameRoot::getSingleton().BattleCtrl().GetCardOwner(card);
	KCardInstList* lst = guy->QueryCardSet(card->GetSlot());
	KUIAssist::_moveCardSet(lst,"card_move");
	
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

;

void KUIAssist::_switch2StageWaitScene()
{
	CCScene* scene = CCTransitionSplitRows::create(0.5f, StageWaitScene::scene());
	CCDirector::sharedDirector()->replaceScene(scene);

}

void KUIAssist::_switch2CardGroupScene()
{
	//CCScene* scene = CCTransitionSplitRows::create(0.5f, RecCardGroupScene::scene());
	CCScene* scene = CCTransitionSplitRows::create(0.5f, CardGroupScene::scene());
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

UIWidget* KUIAssist::_createMiniHero(KHeroDef& hero)
{
	KCardStatic* cardST = KGameStaticMgr::getSingleton().GetCard(hero._cardId);
	if(!cardST) return NULL;
	UIWidget* ui = KJsonDictMgr::getSingleton().CreateMiniCardWidget();
	ui->setAnchorPoint(CCPoint(0.5f,0.5f));

	char sz[64];
	sprintf(sz,"pp_%d.jpg",cardST->GetID());
	UILayout* panel = (UILayout*)ui->getChildByName("bk");
	panel->setBackGroundImage(sz,UI_TEX_TYPE_PLIST);

	UIWidget* widgetIcon = ui->getChildByName("cost_icon");
	widgetIcon->setVisible(false);

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost_val");
	labelCost->setVisible(false);

	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	labelNum->setVisible(false);

	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("card_title");
	sprintf(sz,"t_%d.png",cardST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);


	return ui;
}

UIWidget* KUIAssist::_createMiniCard(int cardId,int num)
{
	KCardStatic* cardST = KGameStaticMgr::getSingleton().GetCard(cardId);
	if(!cardST) return NULL;

	UIWidget* ui = KJsonDictMgr::getSingleton().CreateMiniCardWidget();
	
	ui->setAnchorPoint(CCPoint(0.5f,0.5f));
	char sz[64];
	sprintf(sz,"pp_%d.jpg",cardST->GetID());
	UILayout* panel = (UILayout*)ui->getChildByName("bk");
	panel->setBackGroundImage(sz,UI_TEX_TYPE_PLIST);

	UIWidget* widgetIcon = ui->getChildByName("cost_icon");
	widgetIcon->setVisible(true);

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost_val");
	sprintf(sz,"%d",cardST->GetCost());
	labelCost->setVisible(true);
	labelCost->setStringValue(sz);

	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	sprintf(sz,"%d",num);
	labelNum->setStringValue(sz);
	labelNum->setVisible((num>1)?true:false);

	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("card_title");
	sprintf(sz,"t_%d.png",cardST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);

	return ui;
}

void KUIAssist::UpdateMiniCardWidgetNum(UIWidget* ui,int num)
{
	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	char sz[64];
	sprintf(sz,"%d",num);
	labelNum->setStringValue(sz);
	labelNum->setVisible((num>1)?true:false);
}

void KUIAssist::_showHeroSkill(UIWidget* widget,const KHeroDef& heroDef)
{
	for(int i=0;i<MAX_BUF_SLOT_NUM;i++){
		UIImageView* widgetSlot = (UIImageView*)GetIndexWidget(widget,"buf_slot",i);
		if(widgetSlot) widgetSlot->setVisible(false);
	}

	float starNum = (float)heroDef.GetLev()/3.0f;
	int fullStarNum = (int)starNum;
	bool bHasHalfStar = (starNum-fullStarNum)>0.0f ? true:false;
	for(int n=0;n<fullStarNum;n++){
		UIImageView* widgetSlot = (UIImageView*)GetIndexWidget(widget,"buf_slot",n);
		if(widgetSlot){
			widgetSlot->setAnchorPoint(CCPoint(1.0f,0.5f));
			widgetSlot->loadTexture("star_full.png",UI_TEX_TYPE_PLIST);
			widgetSlot->setVisible(true);
		}
	}
	if(bHasHalfStar){
		UIImageView* widgetSlot = (UIImageView*)GetIndexWidget(widget,"buf_slot",fullStarNum);
		if(widgetSlot){
			widgetSlot->setAnchorPoint(CCPoint(1.0f,0.5f));
			widgetSlot->loadTexture("star_half.png",UI_TEX_TYPE_PLIST);
			widgetSlot->setVisible(true);
		}
	}
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		char sz[64]={0};
		UILabel* pLabel = (UILabel*)GetIndexWidget(widget,"hero_skill",i);
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(heroDef._skill[i]._skillId);
		if(heroDef.GetSkillLev(i)>0 && skill){
			sprintf(sz,"%s:  lev(%d)",skill->GetName(),heroDef.GetSkillLev(i));
			pLabel->setText(sz);
			pLabel->setVisible(true);
		}else{
			pLabel->setVisible(false);
		}
	}
}

UIWidget*  KUIAssist::_createHero(const KHeroDef& hero,bool bBig)
{
	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(hero._cardId);
	if(!pST) return NULL;
	UIWidget* widget = _createCardLayout(pST,bBig);
	_showHeroSkill(widget,hero);
	UILabel* labelDesc = (UILabel*)widget->getChildByName("detail");
	labelDesc->setVisible(false);
	return widget;
}

UIWidget* KUIAssist::_createCardLayout(KCardStatic* pST,bool bBig)
{
	int idx = ((int)pST->GetType())*10 + pST->GetRace();
	KCardLayoutStatic* pLayout = KGameStaticMgr::getSingleton().GetCardLayout(idx);
	if(!pLayout) pLayout =  KGameStaticMgr::getSingleton().GetCardLayout(((int)pST->GetType())*10 + 0);
	UIWidget* ui = KJsonDictMgr::getSingleton().CreateCardWidget();

	char sz[24];
	UIImageView* widgetBg =(UIImageView*)ui->getChildByName("background");
	sprintf(sz,"pic_%d.jpg",pST->GetID());
	if(widgetBg && strlen(pST->GetPhoto())>0){
		widgetBg->loadTexture(pST->GetPhoto(),UI_TEX_TYPE_PLIST);
	}
	UIImageView* widgetRace = (UIImageView*)ui->getChildByName("race_icon");

	SetRaceIcon(widgetRace, pST->GetRace());

	UIImageView* widgetMask =(UIImageView*)ui->getChildByName("card_mask");
	if(widgetMask) widgetMask->loadTexture(pLayout->GetMask(),UI_TEX_TYPE_PLIST);
	
	
	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("title");
	sprintf(sz,"t_%d.png",pST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);
	widgetTitle->setScale(0.9f);

	UIWidget* widgetIconCost = ui->getChildByName("icon_cost");
	if(widgetIconCost) widgetIconCost->setVisible(pLayout->IsShowCost());

	UIWidget* widgetIconHp = ui->getChildByName("icon_hp");
	if(widgetIconHp) widgetIconHp->setVisible(pLayout->IsShowHp());

	UIWidget* widgetIconAtk = ui->getChildByName("icon_atk");
	if(widgetIconAtk) widgetIconAtk->setVisible(pLayout->IsShowAtk());

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost");
 	if(pLayout->IsShowCost()){
		char info[64]={0};
		sprintf(info,"%d",pST->GetCost());
		labelCost->setStringValue(info);
		labelCost->setVisible(true);
	}else{
		labelCost->setVisible(false);
	}
	UIImageView* widgetStone = (UIImageView*)ui->getChildByName("stone_pos");
	if(pST->GetRank()>0){
		sprintf(sz,"stone_%d.png",pST->GetRank());
		widgetStone->loadTexture(sz,UI_TEX_TYPE_PLIST);
		widgetStone->setVisible(true);
	}else{
		widgetStone->setVisible(false);
	}

	UILabel* labelDesc = (UILabel*)ui->getChildByName("detail");
	labelDesc->setVisible(true);
	if(bBig){
		if(strlen(pST->GetDetail())>2)	labelDesc->setText(pST->GetDetail());
	}else{
		if(strlen(pST->GetDesc())>2)	labelDesc->setText(pST->GetDesc());
	}

	UILabelAtlas* labelHp = (UILabelAtlas*)ui->getChildByName("hp");
	if(pLayout->IsShowHp()){
		sprintf(sz,"%d",pST->GetHp());
		labelHp->setStringValue(sz);
		labelHp->setVisible(true);
	}else{
		labelHp->setVisible(false);
	}

	UILabelAtlas* labelAtk = (UILabelAtlas*)ui->getChildByName("atk");
	if(pLayout->IsShowAtk()){
		sprintf(sz,"%d",pST->GetAtk());
		labelAtk->setStringValue(sz);
		labelAtk->setVisible(true);
	}else{
		labelAtk->setVisible(false);
	}
	
	ui->setAnchorPoint(ccp(0.5,0.5));
	ui->setPosition(ccp(-500.0f,-500.0f));
	if(!bBig) ui->setScale(0.8f);
	return ui;
}

void KUIAssist::_createAffectAction(int actorId,const char* action,K3DActionParam* p,KAction* parent,int key)
{
	KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(actorId);
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
	KCardInst* desCard = GameRoot::getSingleton().BattleCtrl().GetCard(des);
	KCardActor* desActor = (KCardActor*)desCard->getActor();
	CCPoint pt1 = widgetSrc->getPosition();
	CCPoint pt2;
	CCPointArray *arr = CCPointArray::create(10);
	
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
	 KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(realId);
	 if(!card) return NULL;
	 return KCardActor::create(card);
}

void KUIAssist::_updateSecretIcon(bool bMy,KCardInstList* lst)
{
	UIWidget* panel = _battleScene->getRootWidget();

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

void KUIAssist::_updateBufIcon(UIWidget* panel , KCardInst* card)
{
    //MAX_BUF_SLOT_NUM;
    KCardBufferList& bufList = card->m_attr.m_bufList;
	KCardBufferList iconBufList;
	for(KCardBufferList::iterator it=bufList.begin();it!=bufList.end();++it){
		KAbilityStatic* pAbility = (*it)._pST;
		if(!pAbility->BufIconEmpty()) iconBufList.push_back(*it);
	}
    KCardBufferList::iterator it = iconBufList.begin();
    int pos = 0;
	while(it != iconBufList.end()){
        if(pos==MAX_BUF_SLOT_NUM) break;
		KAbilityStatic* pAbility = (*it)._pST;
        UIImageView* widgetSlot = (UIImageView*)GetIndexWidget(panel,"buf_slot",pos);
        if(widgetSlot){
            widgetSlot->loadTexture(pAbility->GetBufIcon(),UI_TEX_TYPE_PLIST);
            widgetSlot->setVisible(true);
        }
		it++;
        pos++;
	}
    for(int i=pos;i<MAX_BUF_SLOT_NUM;i++){
         UIImageView* widgetSlot = (UIImageView*)GetIndexWidget(panel,"buf_slot",i);
        widgetSlot->setVisible(false);
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
	KUIAssist::_moveCardSet(&tmpLst,"card_move");
}


void KUIAssist::_resortHandCardSet(FBattleGuy* guy)
{
	KCardInstList* lst = guy->QueryCardSet(KCardInst::enum_slot_hand);
	KCardInstList tmpLst;
	for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
		KCardActor* actor = (KCardActor*)(*it)->getActor();
		if(actor&&actor->GetUI()) tmpLst.push_back(*it);
	}
	KUIAssist::_moveCardSet(&tmpLst,"card_move");
}

void KUIAssist::_setActionParamSlot(K3DActionParam* param)
{
	KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(param->_srcID);
	if(card) param->_srcSlot = card->GetSlot();
	for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
		card = GameRoot::getSingleton().BattleCtrl().GetCard(param->_desArr[i]);
		if(!card) break;
		param->_destSlot[i] = card->GetSlot();
	}

}

bool KUIAssist::_IsPlayCardAble()
{
	if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()) return false;
	if(KAction::GetTotalClassActionNum()>0){
		return false;
	}
	if(GameRoot::getSingleton().BattleCtrl().IsWaitDrama()) return false;
	return true;
}

bool KUIAssist::_IsValidateSrcCard(KCardInst* card)
{
	if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()) return false;
	KCardInstList curActive;

	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	pMainPlayer->QueryValidateHandCards(&curActive);
	pMainPlayer->QueryValidateFightCards(&curActive);
	
	return (_getIndexOfCard(&curActive,card)>=0);
}

bool KUIAssist::_IsValidateDesCard(KCardInst* card)
{
	if(!GameRoot::getSingleton().BattleCtrl().IsMyTurn()) return false;
	KCardInst* pSrc = GameRoot::getSingleton().BattleCtrl().GetCurSrcCard();
	if(!pSrc) return false;

	KCardInstList curActiveGreen;
	KCardInstList curActiveRed;
	KBattleGuy* defGuy = GameRoot::getSingleton().BattleCtrl().GetDefGuy();
	if(pSrc->IsKindOf(KCardStatic::card_soldier)){
		GameRoot::getSingleton().BattleCtrl().QueryEnterFightTarget(pSrc,&curActiveGreen,&curActiveRed);
		if(pSrc->GetSlot()==KCardInst::enum_slot_fight){
			defGuy->QueryActiveDefendCards(&curActiveRed);
		}
	}else if(pSrc->IsKindOf(KCardStatic::card_skill)){
		GameRoot::getSingleton().BattleCtrl().QuerySkillTarget(pSrc,&curActiveGreen,&curActiveRed);
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

static int lastAdviceId =0;

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
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	return (KSceneLayerBase*)scene->getChildByTag(1977);
}

KActor& KUIAssist::_activeSceneActor()
{
	return _activeSceneLayer()->GetActor();
}