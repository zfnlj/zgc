#include "KActor.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "cocos-ext.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "KCardActor.h"
#include "assist/KJsonDictMgr.h"
#include "KSceneLayerBase.h"
//#include "KAction/ParticleBase.h"

USING_NS_CC;
using namespace cocos2d::extension;

cocos2d::extension::UIWidget* KActor::GetUI()
{
	CCAssert( m_ui , "m_ui needs init!");
	return m_ui;
}

KActor::KActor()
{

}

KActor::~KActor()
{

}

void KActor::init(cocos2d::extension::UIWidget* ui)
{ 
	m_ActionMgr.Init(this);
	m_ui = ui;
}

void KActor::update(float dt)
{
	m_ActionMgr.breathe(dt);
}

void KActor::Remove(const char* obj)
{
	CCNode* node = GetCNode(obj);
	if(node){
		node->removeFromParent();
	}  
}

void KActor::setZOrder(const char* obj,int val)
{
	UIWidget* widget = GetWidget(obj);
	if(widget) widget->setZOrder(val);
}

void KActor::ShowEff(CCParticleSystem* emitter,CCPoint* pt)
{	
	KSceneLayerBase* sceneLayer = KUIAssist::_activeSceneLayer();

	sceneLayer->addChild(emitter);
	emitter->setPosition(*pt);
}

void KActor::RemoveEff(CCParticleSystem* emitter,const char* obj)
{
	KParticleCacheMgr::getSingleton().RemvoeParticle(emitter,obj);
}

void KActor::Move(const char* obj,const char* slot,float fSpeed,CCActionDef& actionDef)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	CCNode* node = GetCNode(obj);
	if(fSpeed>=9999.0f){
		node->setPosition(pt);
		return;
	}
	CCPoint vTmp = pt - node->getPosition();

	fSpeed = fSpeed>0.00001f?fSpeed:1.f;
	float fDistance = vTmp.getLength();
	float fTime = fDistance / fSpeed;
	if(fTime<0.2f) fTime = 0.2f;

	ccBezierConfig bezier;
	bezier.controlPoint_1 = vTmp*0.05f + node->getPosition();
	bezier.controlPoint_2 = vTmp*0.95f + node->getPosition();
	bezier.endPosition = pt;

//	CCActionInterval*  ActionInter = CCBezierTo::create(fTime, bezier);

	CCActionInterval*  ActionInter = CCSequence::create(
		CCBezierTo::create(fTime, bezier),
		CCCallFuncN::create(this, callfuncN_selector(KActor::MoveReached)), 
		NULL);
	node->runAction( ActionInter);	
	actionDef.init(ActionInter,node);
}

CCNode* KActor::GetDictObj(const char* obj)
{
	return (CCNode*)m_objectDict.objectForKey(obj);
}


CCNode* KActor::GetCNode(const char* obj)
{
	CCNode* pNode = GetDictObj(obj);
	if(pNode) return pNode;

	cocos2d::extension::UIWidget* ret = UIHelper::seekWidgetByName(m_ui, obj);;
	if(ret)
		return ret->getRenderer();
	else
		return m_ui->getRenderer();
}

UIWidget* KActor::GetWidget(const char* obj)
{
	cocos2d::extension::UIWidget* ret = UIHelper::seekWidgetByName(m_ui, obj);
	if(ret) return ret;
	ret = KUIAssist::MainLayer()->getWidgetByName(obj);
	if(ret) return ret;
	
	return m_ui;
}

CCPoint KActor::GetDestPosition(K3DActionParam* param,const char* obj,int index)
{
	CCPoint pt;
	if(strcmp(obj,"dest")==0){
		KActor* actor =  (KActor*)KUIAssist::_getCardActor(param->_desArr[index]);
		return actor->GetUI()->getPosition();
	}else if(strcmp(obj,"my_fight_slot")==0){
		char sz[32];
		sprintf(sz,"%s_%d",obj,param->_desArr[index]);
		UIWidget* widget = GetWidget(sz);
		return widget->getWorldPosition();
	}else{
		UIWidget* widget = GetWidget(obj);
		return widget->getWorldPosition();
	}
}
void KActor::SetVisible(const char* obj,bool flag)
{
	CCNode* node = GetCNode(obj);
	if(node){
		node->setVisible(flag);
	}
}

CCAction* KActor::ScaleX(const char* obj,float val,float elapse,CCActionDef& actionDef)
{
	CCNode* node = GetCNode(obj);
	if(!node) return NULL;
	if(elapse<0.01){
		node->setScaleX(val);
		return NULL;
	}else{
		if(elapse >0.1) elapse -=0.1f; // stop befor executor end
		CCActionInterval*  actionBy = CCScaleTo::create(elapse, val, node->getScaleY());
		node->runAction( actionBy);
		actionDef.init(actionBy,node);
		return actionBy;
	}
}

void KActor::StartBreathe(const char* obj,float val,float elapse,CCActionDef& actionDef)
{
	CCNode* node = GetCNode(obj);
	m_backup = node->getScale();

	CCActionInterval*  action = CCRepeatForever::create(
		CCSequence::create(
		CCScaleTo::create(elapse*0.5, m_backup*val, m_backup*val),
		CCScaleTo::create(elapse*0.5,m_backup,m_backup),
		NULL));
	node->runAction(action);
	actionDef.init(action,node);
}

void KActor::StopBreathe(const char* obj)
{
	CCActionDef tmpDef;
	Scale(obj,m_backup,0.0,tmpDef);
}

CCAction* KActor::Scale(const char* obj,float val,float elapse,CCActionDef& actionDef)
{
	CCNode* node = GetCNode(obj);
	if(!node) return NULL;
	if(elapse<0.01){
		node->setScale(val);
		return NULL;
	}else{
		if(elapse >0.1) elapse -=0.1f; // stop befor executor end
		CCActionInterval*  actionBy = CCScaleTo::create(elapse, val, val);
		node->runAction(actionBy);
		actionDef.init(actionBy,node);
		return actionBy;
	}
}

void KActor::RemoveCCAction(const char* obj)
{
	CCNode* node = GetCNode(obj);
	if(!node) return;
	node->stopAllActions();
}

CCAction* KActor::FadeIn(const char* obj,float val,CCActionDef& actionDef)
{
	CCNode* node = GetDictObj(obj);
	if(!node) return NULL;

	if(val<0.01){
		DictObjSetOpacity(obj,255);
		return NULL;
	}else{
		CCActionInterval*  action1 = CCFadeIn::create(val);
		node->runAction( CCSequence::create( action1, NULL, NULL));
		actionDef.init(action1,node);
		return action1;
	}
}

void KActor::DictObjSetOpacity(const char* objName,int val)
{
	CCObject* obj = m_objectDict.objectForKey(objName);
	if(!obj) return;
	CCSprite* sprite = dynamic_cast<CCSprite*>(obj);
	if(sprite) return sprite->setOpacity(val);

	CCLabelBMFont* bmFont = dynamic_cast<CCLabelBMFont*>(obj);
	if(bmFont) bmFont->setOpacity(val);

}
CCAction* KActor::FadeOut(const char* obj,float val,CCActionDef& actionDef)
{
	CCNode* node = GetDictObj(obj);
	if(!node) return NULL;
	if(val<0.01){
		DictObjSetOpacity(obj,0);
		return NULL;
	}else{
		CCActionInterval*  action1 = CCFadeOut::create(val);
		CCAction* seq =  CCSequence::create( action1, NULL, NULL);
		node->runAction(seq);
		actionDef.init(seq,node);
		return action1;
	}
}
void KActor::Shake(const char* obj,float valX,float valY,float elapse,CCActionDef& actionDef)
{
 	CCNode* node = GetCNode(obj);
 	if(!node) return;
	if(elapse >0.1) elapse -=0.1f; // stop befor executor end
	CCActionInterval*  action = CCRepeat::create(
		CCSequence::create(
		CCMoveBy::create(0.05f, ccp(valX,valY)),
		CCMoveBy::create(0.05f, ccp(-valX,-valY)),
		NULL),elapse/0.2f);
	node->runAction(action);
	actionDef.init(action,node);
//	m_ui->runAction(action);
}

void KActor::Filp(const char* obj,float elapse)
{
	// 	cocos2d::extension::UIWidget* widget = GetWidget(obj);
	// 	if(!widget) return;
	
	static float ftmp = 120;
	CCActionInterval*  actionInSide = (CCActionInterval*)CCSequence::create(CCHide::create(),CCDelayTime::create(elapse * 0.5f),
		CCShow::create(),
		CCOrbitCamera::create(elapse * 0.5f, 1, 0, 180 + ftmp, 180 - ftmp, 0, 0),
		NULL);

	CCActionInterval*  actionOutSide = (CCActionInterval *)CCSequence::create(CCOrbitCamera::create(elapse * 0.5f, 1, 0, 0, ftmp, 0, 0),
		CCHide::create(),
		CCDelayTime::create(elapse * 0.5f),NULL);

	CCSprite* outCard = CCSprite::create("ipad\\back.png");	
	outCard->retain();	
	GameRoot::getSingleton().getBattleScene()->addChild(outCard,1);
	
	outCard->setAnchorPoint(outCard->getAnchorPoint());
	outCard->setPosition(m_ui->getPosition());

	m_ui->runAction(actionInSide);
	outCard->runAction(actionOutSide);	
}

void KActor::ParticleFire(float elapse)
{	
	CCParticleSystem *pEmitter = new CCParticleSystemQuad;	
	std::string filename = "data/Particles/fire_wall.plist";
	pEmitter->initWithFile(filename.c_str());	
	pEmitter->setDuration(elapse);
	
	GameRoot::getSingleton().getBattleScene()->addChild(pEmitter,1);

	CCPoint pos = m_ui->getPosition();
	pos.y -= m_ui->getSize().height/2;

	pEmitter->setPosition( pos );	
}

void KActor::SummonCard(int realId)
{
	KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(realId);
	KUIAssist::_showCard(card);
}

void KActor::ShowCard(int realId)
{
	KCardInst* card = GameRoot::getSingleton().BattleCtrl().GetCard(realId);
	KUIAssist::_updateCard(card);
}

CCSprite* KActor::CreateAnim(const char* obj,const char* slot,float scale,int zOrder,float offsetY,bool bLoop)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	pt.y += offsetY;
	CCSprite* pAnim = KUIAssist::CreateAnimationSprite(obj,bLoop);
	pAnim->setAnchorPoint(ccp(0.50f,0.50f));
	pAnim->setPosition(pt);
	pAnim->setScale(scale);
	KUIAssist::MainLayer()->addChild(pAnim,zOrder);
	m_objectDict.setObject(pAnim, obj);
	return pAnim;
}

cocos2d::extension::CCArmature* KActor::CreateArmature(K3DActionParam* param,const char* obj,const char* slot,float scale,float yOffset,int zOrder)
{
	CCPoint pt = GetDestPosition(param,slot,0);
	pt.y += yOffset;
	CCArmature *armature = CCArmature::create(obj);
	if(!armature) return NULL;
	armature->setAnchorPoint(ccp(0.50f,0.50f));
	armature->setScale(scale);
	armature->setPosition(pt);
	armature->getAnimation()->playByIndex(0);
	KUIAssist::MainLayer()->addChild(armature,zOrder);
	return armature;
}

CCSprite* KActor::CreateSprite(const char* obj,const char* slot,float scale,int zOrder)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	CCSprite* sprite = CCSprite::createWithSpriteFrameName(obj);
	if(!sprite) return NULL;
	sprite->setAnchorPoint(ccp(0.50f,0.50f));
	sprite->setScale(scale);
	sprite->setPosition(pt);
	KUIAssist::MainLayer()->addChild(sprite,zOrder);
	//widgetSlot->addRenderer(sprite,zOrder);
	m_objectDict.setObject(sprite, obj);
	return sprite;
}

void KActor::AtkMove(int des,float val,CCActionDef& actionDef)
{
	CCAction* action = KUIAssist::_createAtkMove(m_ui,des,val);
	actionDef.init(action,m_ui->getRenderer());
}

void KActor::RemoveDictObj(CCNode* node,const char* obj)
{
	node->removeFromParent();
	m_objectDict.removeObjectForKey(obj);
}

CCParticleSystem* KActor::CreateEff(const char* obj,const char* slot,int zOrder,float scale)
{
	
	CCParticleSystem* emitter = KParticleCacheMgr::getSingleton().CreateParticle(obj);
	if(!emitter) return NULL;
	emitter->setScale(scale);
	//UIWidget* widget = UIHelper::seekWidgetByName(m_ui, slot);
	//if(widget){
	//	widget->getRenderer()->addChild(emitter,zOrder);
	//}else{
		CCPoint destPt = GetDestPosition(NULL,slot,0);
		emitter->setPosition(destPt);
		KUIAssist::MainLayer()->addChild(emitter,zOrder);
	//}
	
	return emitter;
}

void KActor::Move(const char* obj,CCPoint& pt,float fSpeed,CCActionDef& actionDef)
{
	CCNode* node = GetCNode(obj);
	CCPoint vTmp = pt - node->getPosition();

	float fDistance = vTmp.getLength();
	float fTime = fDistance / fSpeed;
	if(fTime<0.1f) fTime = 0.1f;

	ccBezierConfig bezier;
	bezier.controlPoint_1 = vTmp*0.05f + node->getPosition();
	bezier.controlPoint_2 = vTmp*0.95f + node->getPosition();
	bezier.endPosition = pt;

	//	CCActionInterval*  ActionInter = CCBezierTo::create(fTime, bezier);

	CCActionInterval*  ActionInter = CCSequence::create(
		CCBezierTo::create(fTime, bezier),
		CCCallFuncN::create(this, callfuncN_selector(KActor::MoveReached)), 
		NULL);
	node->runAction( ActionInter);	
	actionDef.init(ActionInter,node);
}

cocos2d::extension::UIWidget* KActor::CreateTalk(const char* obj,const char* slot,int z, int msgId)
{
	UIWidget* widget = KJsonDictMgr::getSingleton().widgetFromJsonFile(obj);
	CCPoint pt = GetDestPosition(NULL,slot,0);
	widget->setZOrder(z);

	char sz[16];
	sprintf(sz,"id=%d",msgId);
	UILabel* labelDesc = (UILabel*)widget->getChildByName("text_str");
	KHelpStringStatic* helpString = KGameStaticMgr::getSingleton().GetHelpString(msgId);
	if(helpString){
		labelDesc->setText(helpString->GetString());
	}else{
		labelDesc->setText(sz);
	}

	KUIAssist::MainLayer()->addWidget(widget);
	widget->setPosition(pt);
	return widget;
}