#include "KAffectorAdvExecutor.h"
#include "../UI/KCardActor.h"
#include <math.h>
#include "../UI/KUIAssist.h"
#include "GameRoot.h"
KAffectorExecutor::KAffectorExecutor()

{
	
}

KAffectorExecutor::~KAffectorExecutor()
{
	StopEmitter();
	if(m_sprite){
		GetActor()->RemoveSprite(m_sprite,m_AffectorStatic->GetObj());
	}
	if(m_armature){
		m_armature->removeFromParent();
	}
}

void KAffectorExecutor::init(KAffectorStatic* st)
{
	Reset();
	m_AffectorStatic = st;
	m_emitter = NULL;
	m_sprite = NULL;
	m_armature = NULL;
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



void KAffectorExecutor::OnPlay(K3DActionParam* param)
{
	m_param = param;
	mState = AdvExecutor::EXE_PLAYING;
	switch(m_AffectorStatic->Type())
	{
	case Affector_layer:
		if(GetActor()) GetActor()->setZOrder(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_summon:
		if(GetActor()) GetActor()->SummonCard(m_param->GetDesId(0));
		break;
	case Affector_summonSelf:
		if(GetActor()) GetActor()->SummonSelf();
		break;
	case Affector_anim:
		if(GetActor()) m_sprite = GetActor()->CreateAnim(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),
															m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal());
		break;;
	case Affector_replace:
		if(GetActor()) GetActor()->ShowCard(m_param->GetDesId(0));
		break;
	case Affector_move:
		if(GetActor()) GetActor()->Move(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_updateHit:
		if(GetActor()) {
			GetActor()->UpdateCardAttr(GetActor()->GetUI(),false,m_param);
			GetActor()->UpdateCardBuf();
		}
		break;
	case Affector_update:
		if(GetActor()) {
			GetActor()->UpdateCardAttr();
			GetActor()->UpdateCardBuf();
		}
		break;
	case Affector_drawCard:
		if(GetActor()) GetActor()->DrawCard();
		break;
	case Affector_hit:
		if(GetActor()){
			GetActor()->ShowHit(m_AffectorStatic->GetSlot(),m_param,m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal()>0);
		}
		break;
	case Affector_visible:
		if(GetActor()) GetActor()->SetVisible(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal()>0);
		break;
	case Affector_createSprite:
		if(GetActor()) m_sprite = GetActor()->CreateSprite(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_armature:
		if(GetActor()) m_armature = GetActor()->CreateArmature(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetFloatVal(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_fadein:
		if(GetActor()) GetActor()->FadeIn(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_updateSecret:
		if(GetActor()) GetActor()->updateSecret();
		break;
	case Affector_addWidget:
		if(GetActor()) GetActor()->addWidget(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_delWidget:
		if(GetActor()) GetActor()->delWidget(m_AffectorStatic->GetObj());
		break;
	case Affector_addwidget:
		if(GetActor()) GetActor()->AddWidget(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot());
		break;
	case Affector_delwidget:
		if(GetActor()) GetActor()->DelWidget(m_AffectorStatic->GetObj());
		break;
	case Affector_fadeout:
		if(GetActor()) GetActor()->FadeOut(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_atkMove:
		if(GetActor()) GetActor()->AtkMove(m_param->GetDesId(0),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_scale:
		if(GetActor()) GetActor()->Scale(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),mSurviveTime);
		break;
	case Affector_scaleX:
		if(GetActor()) GetActor()->ScaleX(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),mSurviveTime);
		break;
	case Affector_eff:
		if(GetActor()) m_emitter = GetActor()->CreateEff(m_AffectorStatic->GetObj(),m_AffectorStatic->GetSlot(),m_AffectorStatic->GetIntVal(),m_AffectorStatic->GetFloatVal());
		break;
	case Affector_moveback:
		if(GetActor()) GetActor()->MoveBack(m_AffectorStatic->GetFloatVal());
		break;
	case Affector_shakex:
		if(GetActor()) GetActor()->Shake(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0,mSurviveTime);
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
}

void KAffectorExecutor::Stop()
{
	OnStop();
}

void KAffectorExecutor::LimitAlive(float val)
{
	if(GetActor()){
		GetActor()->RemoveCCAction(m_AffectorStatic->GetObj());
	}
	switch(m_AffectorStatic->Type())
	{
	case Affector_layer:
		if(GetActor()) GetActor()->setZOrder(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal());
		break;
	case Affector_visible:
		if(GetActor()) GetActor()->SetVisible(m_AffectorStatic->GetObj(),m_AffectorStatic->GetIntVal()>0);
		break;
	case Affector_fadein:
		if(GetActor()) GetActor()->FadeIn(m_AffectorStatic->GetObj(),0);
		break;
	case Affector_fadeout:
		if(GetActor()) GetActor()->FadeOut(m_AffectorStatic->GetObj(),0);
		break;
	case Affector_atkMove:
		if(GetActor())  GetActor()->AtkMove(m_param->GetDesId(0),m_AffectorStatic->GetFloatVal());
		break; 
	case Affector_scale:
		if(GetActor()) GetActor()->Scale(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0);
		break;
	case Affector_scaleX:
		if(GetActor()) GetActor()->ScaleX(m_AffectorStatic->GetObj(),m_AffectorStatic->GetFloatVal(),0);
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
	CCPoint ptDes = GetActor()->GetDestPosition(param,m_AffectorStatic->GetSlot(),0);
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

bool KAffectorActionExecutor::IsTargetFlagMatch(int id)
{
    KAffectorActionStatic* pStatic = (KAffectorActionStatic*)m_AffectorStatic;

    KClientBattleCtrl& battleCtrl = GameRoot::getSingleton().BattleCtrl();
    KCardActor* actor = dynamic_cast<KCardActor*>(m_action->GetActor());
    FBattleGuy* me = (actor)?battleCtrl.GetCardOwner(actor->GetCard()):NULL;
    FBattleGuy* you = battleCtrl.GetCardOwner(id);
    bool ret = false;
    switch(pStatic->GetTarFlag()){
        case KAffectorActionStatic::tar_des:
            ret = true;
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
			if(pStatic->GetTarFlag()==KAffectorActionStatic::tar_src){
				KUIAssist::_createAffectAction(param->SrcID(),pStatic->GetObj(),param,m_action,pStatic->GetIntVal());
			}else{
				bool bFound = false;
				for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
					int actorId = param->GetDesId(i);
					if(actorId==0) break;
                    if(!IsTargetFlagMatch(actorId)) continue;;
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
	CCPoint ptSrc = GetActor()->GetDestPosition(param,"card_center",0);
	CCPoint ptDes = GetActor()->GetDestPosition(param,m_AffectorStatic->GetSlot(),0);
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
    m_bar->runAction(CCSequence::create(CCMoveTo::create(fTime,ptDes),funcall,NULL));

	KUIAssist::MainLayer()->addChild(m_bar,100);
	KAffectorExecutor::OnPlay(param);
}

void KAffectorIndicateExecutor::OnStop(void)
{
	//if(m_pChar)
	//	KObjEffAlgorithm::_setBright(m_pChar->GetEntity(),Ogre::ColourValue::ZERO);
}
