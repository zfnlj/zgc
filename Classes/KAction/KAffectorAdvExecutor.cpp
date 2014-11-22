#include "KAffectorAdvExecutor.h"
#include "../UI/KCardActor.h"
#include <math.h>
#include "../UI/assist/KUIAssist.h"
#include "GameRoot.h"
#include "../ui/KSceneLayerBase.h"
#include "SimpleAudioEngine.h"

CCActionDef::~CCActionDef()
{
	empty();
}

void CCActionDef::empty()
{
	if(_action&&_node){
		if(!_action->isSingleReference()){
			_node->stopAction(_action);
		}
		_action->release();
	}
	_action = NULL;
	_node = NULL;
}

KAffectorExecutor::KAffectorExecutor()

{
	
}

KAffectorExecutor::~KAffectorExecutor()
{
	OnStop();
}

void KAffectorExecutor::init(KAffectorStatic* st)
{
	Reset();
	m_AffectorStatic = st;
	m_emitter = NULL;
	m_dictObj = NULL;
	m_armature = NULL;
	m_ccAction = NULL;
	m_widget = NULL;
	mSurviveTime = st->GetSurviveTime();
	if(mSurviveTime<=0.00001)
	{
		mSurviveTime = 1;
		mbInfinite = true;
	}
}

void KAffectorExecutor::LimitAlive(void)
{
	mSurviveTime = 0;
	mbInfinite  = false;
	if(GetActor()){
		GetActor()->RemoveCCAction(m_AffectorStatic->GetObj());
	}
}


float KAffectorExecutor::Breathe(float frameTime)
{
	if(mState==EXE_PLAYING){
		if(m_ccActionDef._action && m_ccActionDef._action->isSingleReference()){ //ccAction Done;
			LimitAlive(0.01f);
		}
	}
	return AdvExecutor::Breathe(frameTime);
}

void KAffectorExecutor::OnPlay(K3DActionParam* param)
{
	m_param = param;
	mState = AdvExecutor::EXE_PLAYING;
	switch(m_AffectorStatic->Type())
	{
	case Affector_layer:
		if(GetActor()){
			GetActor()->setZOrder(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		}
		break;
	case Affector_sound:
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_AffectorStatic->GetFloatVal());
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_AffectorStatic->GetObj());
		}
		break;
	case Affector_tmpLayer:
		if(GetActor()){
			GetActor()->setZOrder(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		}
		break;
	case Affector_summon:
		if(GetActor()) GetActor()->SummonCard(m_param->GetDesId(0));
		break;
	case Affector_summonSelf:
		if(GetActor()) GetActor()->SummonSelf();
		break;
	case Affector_resortHand:
		{
			KCardActor* cardActor = (KCardActor*)GetActor();
			KUIAssist::_resortHandCardSet(cardActor->GetCard()->GetOwner()->GetFacade());
		}
		break;
	case Affector_ZOrderCardSet:
		{
			KCardActor* cardActor = (KCardActor*)GetActor();
			KUIAssist::_ZOrderCardSet(cardActor->GetCard()->GetOwner()->GetFacade(),m_AffectorStatic->GetIntVal());
		}
		break;
	case Affector_anim:
		if(GetActor()) m_dictObj = GetActor()->CreateAnim(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),
															m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal(),m_AffectorStatic->GetFloatVal2(),mbInfinite);
		break;;
	case Affector_replace:
		if(GetActor()) GetActor()->ShowCard(m_param->GetDesId(0));
		break;
	case Affector_shader:
		if(GetActor()) GetActor()->SetShader(m_AffectorStatic->GetObj());
		break;
	case Affector_move:
		if(GetActor()) GetActor()->Move(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetFloatVal(),m_ccActionDef);
		break;
	case Affector_moveOnHit:
		if(GetActor()){
			GetActor()->MoveOnHit(m_param,m_AffectorStatic->GetFloatVal(),m_ccActionDef);
			if(!m_ccActionDef._action) LimitAlive(0.1f);
		}
		break;
	case Affector_updateAttr:
		if(GetActor()) {
			GetActor()->UpdateCardAttr();
			GetActor()->UpdateCardBuf();
		}
		break;
	case Affector_updateHit:
		if(GetActor()) {
			GetActor()->UpdateCardAttr(GetActor()->GetUI(),false,false,m_param);
			GetActor()->UpdateCardBuf();
		}
		break;
	case Affector_updateBuf:
		if(GetActor()) {
			GetActor()->UpdateCardBuf();
		}
		break;
	case Affector_drawCard:
		if(GetActor()) GetActor()->DrawCard();
		break;
	case Affector_hit:
		if(GetActor()){
			m_dictObj = GetActor()->ShowHit(m_AffectorStatic->GetSlot(),m_param,m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal()>0);
		}
		break;
	case Affector_visible:
		if(GetActor()) GetActor()->SetVisible(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal()>0);
		break;
	case Affector_createSprite:
		if(GetActor()) m_dictObj = GetActor()->CreateSprite(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_talk:
		if(GetActor()) m_widget = GetActor()->CreateTalk(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetIntVal(),m_param->GetDesId(0));
		break;
	case Affector_armature:
		if(GetActor()){
			if(!m_param->IsStrEmpty()){
				m_armature = GetActor()->CreateArmature(m_param,m_AffectorStatic->GetObj(),m_param->_string,
					m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetFloatVal2(),m_AffectorStatic->GetIntVal());
			}else{
				m_armature = GetActor()->CreateArmature(m_param,m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),
														m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetFloatVal2(),m_AffectorStatic->GetIntVal());
			}
		}
		break;
	case Affector_fadein:
		if(GetActor()) GetActor()->FadeIn(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),m_ccActionDef);
		break;
	case Affector_updateSecret:
		if(GetActor()) GetActor()->updateSecret();
		break;
	case Affector_updateRes:
		if(GetActor()) GetActor()->updateRes();
		break;
	case Affector_addWidget:
		if(GetActor()) GetActor()->addWidget(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_delWidget:
		if(GetActor()) GetActor()->delWidget(m_AffectorStatic->GetObj());
		break;
	case Affector_fadeout:
		if(GetActor()) GetActor()->FadeOut(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),m_ccActionDef);
		break;
	case Affector_breathe:
		if(GetActor()) GetActor()->StartBreathe(m_AffectorStatic->GetObj(),((float)m_AffectorStatic->GetIntVal())*0.01f,
												m_AffectorStatic->GetFloatVal(),m_ccActionDef);
		break;
	case Affector_atkMove:
		if(GetActor()){
			GetActor()->AtkMove(m_param->GetDesId(0),m_AffectorStatic->GetFloatVal(),m_ccActionDef);
		}
		break;
	case Affector_scale:
		if(GetActor()) GetActor()->Scale(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),mSurviveTime,m_ccActionDef);
		break;
	case Affector_scaleX:
		if(GetActor()) GetActor()->ScaleX(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),mSurviveTime,m_ccActionDef);
		break;
	case Affector_eff:
		if(GetActor()) m_emitter = GetActor()->CreateEff(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetIntVal(),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_moveback:
		if(GetActor()) GetActor()->MoveBack(m_AffectorStatic->GetFloatVal());
		break;
	case Affector_shakex:
		if(GetActor()) GetActor()->Shake(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0,mSurviveTime,m_ccActionDef);
		break;
    default:
        break;
	}

}

void KAffectorExecutor::StopEmitter()
{
	if(m_emitter){
		if(GetActor()) GetActor()->RemoveEff(m_emitter,m_AffectorStatic->GetObj());
		m_emitter = NULL;
	}
}

void KAffectorExecutor::OnStop(void)
{
	StopEmitter();
	if(m_dictObj && mSurviveTime>0.1f){ //self control life
		GetActor()->RemoveDictObj(m_dictObj,m_AffectorStatic->GetObj());
	}
	m_dictObj = NULL;
	if(m_armature){
		m_armature->removeFromParent();
		m_armature = NULL;
	}
	if(m_widget){
		m_widget->removeFromParent();
		m_widget = NULL;
	}
}

void KAffectorExecutor::Stop()
{
	OnStop();
}

void KAffectorExecutor::LimitAlive(float val)
{
	//if(GetActor()){
	//	GetActor()->RemoveCCAction(m_AffectorStatic->GetObj());
	//}
	if(m_dictObj){ //self control life
		GetActor()->RemoveDictObj(m_dictObj,m_AffectorStatic->GetObj());
	}
	m_dictObj = NULL;
	if(m_armature){
		m_armature->removeFromParent();
		m_armature = NULL;
	}

	m_ccActionDef.empty();
	switch(m_AffectorStatic->Type())
	{
	case Affector_layer:
		if(GetActor()) GetActor()->setZOrder(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_tmpLayer:
		if(GetActor()) GetActor()->resortZOrder(0);
		break;
	case Affector_visible:
		if(GetActor()) GetActor()->SetVisible(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal()>0);
		break;
	case Affector_fadein:
		if(GetActor()) GetActor()->FadeIn(m_AffectorStatic->GetObj(),0,m_ccActionDef);
		break;
	case Affector_fadeout:
		if(GetActor()) GetActor()->FadeOut(m_AffectorStatic->GetObj(),0,m_ccActionDef);
		break;
	case Affector_breathe:
		if(GetActor()) GetActor()->StopBreathe(m_AffectorStatic->GetObj());
		break;
	/*case Affector_atkMove:
		if(GetActor())  GetActor()->AtkMove(m_param->GetDesId(0),m_AffectorStatic->GetFloatVal());
		break; */
	case Affector_shader:
		if(GetActor()) GetActor()->RestoreShader();
		break;
	case Affector_scale:
		if(GetActor()) GetActor()->Scale(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0,m_ccActionDef);
		break;
	case Affector_scaleX:
		if(GetActor()) GetActor()->ScaleX(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0,m_ccActionDef);
		break;
    default:
        break;
	}
	AdvExecutor::LimitAlive(val);
}

KAffectorMissileExecutor::KAffectorMissileExecutor()
{

}


KAffectorMissileExecutor::~KAffectorMissileExecutor()
{
}

void KAffectorMissileExecutor::callbackMissileReach()
{
	StopEmitter();
	LimitAlive(0.1f);
}
void KAffectorMissileExecutor::OnPlay(K3DActionParam* param)
{
	m_emitter = KUIAssist::_createParticle(m_AffectorStatic->GetObj());

	cocos2d::extension::UIWidget* widget = GetActor()->GetWidget("");
	CCPoint ptSrc = widget->getPosition();
	bool ret;
	CCPoint ptDes = GetActor()->GetDestPosition(param,m_AffectorStatic->GetSlot(),0,ret);
	CCPoint vTmp = ptDes - ptSrc;
	float fDistance = vTmp.getLength();
	float fTime = fDistance / m_AffectorStatic->GetFloatVal();
	if(fTime <0.2f) fTime = 0.2f;
	GetActor()->ShowEff(m_emitter,&ptSrc);
	m_emitter->setAngle(180+vTmp.getAngle()*180*M_1_PI);
	//m_emitter->setAngle(-vTmp.getAngle()*180*M_1_PI);
	CCActionInterval * easeSineIn = CCEaseSineIn::create(CCMoveTo::create(fTime,ptDes));
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(KAffectorMissileExecutor::callbackMissileReach));
    m_emitter->runAction(CCSequence::create(easeSineIn,funcall,NULL));

	KAffectorExecutor::OnPlay(param);
}

void KAffectorMissileExecutor::OnStop(void)
{
	//if(m_pChar)
	//	KObjEffAlgorithm::_setBright(m_pChar->GetEntity(),Ogre::ColourValue::ZERO);
}

bool KAffectorActionExecutor::IsTargetFlagMatch(int id,int index)
{
    KAffectorActionStatic* pStatic = (KAffectorActionStatic*)m_AffectorStatic;

    KClientBattleCtrl* battleCtrl = KClientBattleCtrl::getInstance();
    KCardActor* actor = dynamic_cast<KCardActor*>(m_action->GetActor());
    FBattleGuy* me = (actor)?battleCtrl->GetCardOwner(actor->GetCard()):NULL;
    FBattleGuy* you = battleCtrl->GetCardOwner(id);
    bool ret = false;
    switch(pStatic->GetTarFlag()){
        case KAffectorActionStatic::tar_des:
            ret = true;
            break;
		case KAffectorActionStatic::tar_des0:
            ret = (index==0);
            break;
        case KAffectorActionStatic::tar_my:
            ret = (me==you);
            break;
        case KAffectorActionStatic::tar_your:
            ret = (me!=you);
            break;
        default:
            break;
    }
    return ret;
}

float KAffectorActionExecutor::Breathe(float frameTime)
{
	if(mState==EXE_PLAYING){
		K3DActionParam* param = m_action->GetParam();
		KAffectorActionStatic* pStatic = (KAffectorActionStatic*)m_AffectorStatic;
		if(m_action->IsActonFinished(pStatic->GetPreId())){
			if(pStatic->GetTarFlag()==KAffectorActionStatic::tar_null){
				KCardInst* card = ((KCardActor*)GetActor())->GetCard();
				KUIAssist::_createAffectAction(card->GetRealId(),pStatic->GetObj(),param,m_action,pStatic->GetIntVal());
			}else if(pStatic->GetTarFlag()==KAffectorActionStatic::tar_src){
				KUIAssist::_createAffectAction(param->SrcID(),pStatic->GetObj(),param,m_action,pStatic->GetIntVal());
			}else{
				bool bFound = false;
				for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
					int actorId = param->GetDesId(i);
					if(actorId==0) break;
                    if(!IsTargetFlagMatch(actorId,i)) continue;;
					bFound = true;
					KUIAssist::_createAffectAction(actorId,pStatic->GetObj(),param,m_action,pStatic->GetIntVal());
				}
				if(!bFound) m_action->AddFinishedAction(pStatic->GetIntVal());
			}
			LimitAlive(0.1f);
			PlayOver();
		}
	}
	return KAffectorExecutor::Breathe(frameTime);
}

//////////////////////////////////////////////////////////

KAffectorIndicateExecutor::KAffectorIndicateExecutor():m_bar(NULL)
{

}


KAffectorIndicateExecutor::~KAffectorIndicateExecutor()
{

	if(m_bar){
		m_bar->stopAllActions();
		m_bar->removeFromParent();
	}
	m_bar = NULL;
}

void KAffectorIndicateExecutor::callbackMissileReach()
{
	LimitAlive(0.5f);
}
void KAffectorIndicateExecutor::OnPlay(K3DActionParam* param)
{
	bool ret;
	CCPoint ptSrc = GetActor()->GetDestPosition(param,"card_center",0,ret);


	CCPoint ptDes = GetActor()->GetDestPosition(param,m_AffectorStatic->GetSlot(),0,ret);
	if(!ret){
		LimitAlive(0.1f);
		return;
	}
	CCPoint vTmp = ptDes - ptSrc;
	float fDistance = vTmp.getLength();
	float fTime = fDistance / m_AffectorStatic->GetFloatVal();
	if(fTime <0.2f) fTime = 0.2f;

	CCRect rc(0,0,fDistance,4.0f);
	CCSprite* sprite = CCSprite::create(m_AffectorStatic->GetObj(), rc);
	
	m_bar = CCProgressTimer::create(sprite);
    m_bar->setType(kCCProgressTimerTypeBar);
    m_bar->setMidpoint(ccp(0,0));
    m_bar->setBarChangeRate(ccp(1, 0));
	m_bar->runAction( CCProgressTo::create(fTime, 100));
	m_bar->setRotation(180-vTmp.getAngle()*180*M_1_PI);
	//m_bar->setRotation(vTmp.getAngle());
	m_bar->setPosition(ptSrc);
	m_bar->setAnchorPoint(ccp(0.0f,0.5f));
	//m_emitter->setAngle(-vTmp.getAngle()*180*M_1_PI);
    CCCallFunc * funcall= CCCallFunc::create(this, callfunc_selector(KAffectorIndicateExecutor::callbackMissileReach));
	CCAction* seq = CCSequence::create(CCMoveTo::create(fTime,ptDes),funcall,NULL);
    m_bar->runAction(seq);
	m_ccActionDef.init(seq,m_bar);

	KUIAssist::_activeSceneLayer()->addChild(m_bar,100);
	KAffectorExecutor::OnPlay(param);
}

void KAffectorIndicateExecutor::OnStop(void)
{
	m_ccActionDef.empty();
	//if(m_pChar)
	//	KObjEffAlgorithm::_setBright(m_pChar->GetEntity(),Ogre::ColourValue::ZERO);
}


//////////////////////////////////////////////////////////////////


KAffectorTimerBarExecutor::KAffectorTimerBarExecutor():m_bar(NULL)
{

}


KAffectorTimerBarExecutor::~KAffectorTimerBarExecutor()
{

	if(m_bar){
		m_bar->stopAllActions();
		m_bar->removeFromParent();
	}
	m_bar = NULL;
}

void KAffectorTimerBarExecutor::OnPlay(K3DActionParam* param)
{
	bool ret;
	CCPoint pt = GetActor()->GetDestPosition(param,m_AffectorStatic->GetSlot(),0,ret);
	if(!ret){
		LimitAlive(0.1f);
		return;
	}

	CCSprite* sprite = CCSprite::createWithSpriteFrameName(m_AffectorStatic->GetObj());

	m_bar = CCProgressTimer::create(sprite);
	m_bar->setType(kCCProgressTimerTypeBar);
	m_bar->setMidpoint(ccp(0,0));
	m_bar->setBarChangeRate(ccp(1, 0));
	m_bar->setPosition(pt);
	m_bar->setAnchorPoint(ccp(0.5f,0.5f));

	m_bar->runAction( CCProgressTo::create(mSurviveTime-0.6, 100));

	KUIAssist::_activeSceneLayer()->addChild(m_bar,100);
	KAffectorExecutor::OnPlay(param);
}

void KAffectorTimerBarExecutor::OnStop(void)
{
	
}
