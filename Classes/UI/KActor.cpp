#include "KActor.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "cocos-ext.h"
#include "KUIAssist.h"
#include "BattleFieldScene.h"
#include "KCardActor.h"
#include "KJsonDictMgr.h"
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
	CCNode* node = GetCNode(obj);
	if(node){
		node->setZOrder(val);
	}
}

void KActor::ShowEff(CCParticleSystem* emitter,CCPoint* pt)
{	
	BattleFieldScene* layer = GameRoot::getSingleton().getBattleScene();
	layer->GetUILayer()->addChild(emitter);
	emitter->setPosition(*pt);
}

void KActor::RemoveEff(CCParticleSystem* emitter,const char* obj)
{
	KParticleCacheMgr::getSingleton().RemvoeParticle(emitter,obj);
}

void KActor::DelWidget(const char* obj)
{
	UIWidget* widget = GetWidget(obj);
	if(widget) widget->removeFromParent();
}

void KActor::AddWidget(const char* obj,const char* slot)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	cocos2d::extension::UIWidget* widget = GetWidget(obj);
	CCAssert(widget!=NULL , "Not Found widget!");
	if(widget){
		BattleFieldScene* layer = GameRoot::getSingleton().getBattleScene();
		widget->setPosition(pt);
		layer->GetUILayer()->addWidget(widget);
	}
}

void KActor::Move(const char* obj,const char* slot,float fSpeed)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	CCNode* node = GetCNode(obj);
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
}

CCNodeRGBA* KActor::GetRenderer(const char* obj)
{
	CCSprite* sprite = (CCSprite*)m_spriteDict.objectForKey(obj);
	if(sprite) return sprite;

	return NULL;
}

CCNode* KActor::GetCNode(const char* obj)
{
	CCNode* pNode = GetRenderer(obj);
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
	if(strcmp(obj,"screen_center")==0){
		return KUIAssist::_getScreenCenter();
	}else if(strcmp(obj,"dest")==0){
		KActor* actor =  (KActor*)KUIAssist::_getCardActor(param->_desArr[index]);
		return actor->GetUI()->getPosition();
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

CCAction* KActor::Scale(const char* obj,float val,float elapse)
{
	CCNode* node = GetCNode(obj);
	if(!node) return NULL;
	float oldScale = node->getScale();
	if(elapse<0.01){
		node->setScale(val);
		return NULL;
	}else{
		CCActionInterval*  actionBy = CCScaleBy::create(elapse, val/oldScale, val/oldScale);
		node->runAction( CCSequence::create(actionBy, NULL, NULL));
		return actionBy;
	}
}

void KActor::RemoveCCAction(const char* obj)
{
	CCNode* node = GetCNode(obj);
	if(!node) return;
	node->stopAllActions();
}

CCAction* KActor::FadeIn(const char* obj,float val)
{
	CCNodeRGBA* node = GetRenderer(obj);
	if(!node) return NULL;

	if(val<0.01){
		node->setOpacity(255);
		return NULL;
	}else{
		CCActionInterval*  action1 = CCFadeIn::create(val);
		node->runAction( CCSequence::create( action1, NULL, NULL));
		return action1;
	}
}

CCAction* KActor::FadeOut(const char* obj,float val)
{
	CCNodeRGBA* node = GetRenderer(obj);
	if(!node) return NULL;
	if(val<0.01){
		node->setOpacity(0);
		return NULL;
	}else{
		CCActionInterval*  action1 = CCFadeOut::create(val);
		node->runAction( CCSequence::create( action1, NULL, NULL));
		return action1;
	}
}
void KActor::Shake(const char* obj,float valX,float valY,float elapse)
{
 	CCNode* node = GetCNode(obj);
 	if(!node) return;
	
	CCActionInterval*  action = CCRepeat::create(
		CCSequence::create(
		CCMoveBy::create(0.05f, ccp(valX,valY)),
		CCMoveBy::create(0.05f, ccp(-valX,-valY)),
		NULL),elapse/0.2f);
	node->runAction(action);
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

CCSprite* KActor::CreateAnim(const char* obj,const char* slot,int zOrder)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);
	CCSprite* pAnim = KUIAssist::CreateAnimationSprite(obj,false);
	pAnim->setAnchorPoint(ccp(0.50f,0.50f));
	pAnim->setPosition(pt);
	KUIAssist::MainLayer()->addChild(pAnim,zOrder);
	m_spriteDict.setObject(pAnim, obj);
	return pAnim;
}

cocos2d::extension::CCArmature* KActor::CreateArmature(const char* obj,const char* slot,float scale,int zOrder)
{
	CCPoint pt = GetDestPosition(NULL,slot,0);

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
	m_spriteDict.setObject(sprite, obj);
	return sprite;
}

void KActor::AtkMove(int des,float val)
{
	KUIAssist::_createAtkMove(m_ui,des,val);
}

void KActor::RemoveSprite(CCSprite* sprite,const char* obj)
{
	sprite->removeFromParent();
	m_spriteDict.removeObjectForKey(obj);
}

CCParticleSystem* KActor::CreateEff(const char* obj,const char* slot,int zOrder,float scale)
{
	CCPoint destPt = GetDestPosition(NULL,slot,0);
	CCParticleSystem* emitter = KParticleCacheMgr::getSingleton().CreateParticle(obj);
	if(!emitter) return NULL;
	emitter->setScale(scale);
	emitter->setPosition(destPt);
	KUIAssist::MainLayer()->addChild(emitter,zOrder);
	return emitter;
}

