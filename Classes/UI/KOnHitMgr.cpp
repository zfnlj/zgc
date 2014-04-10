#include "KOnHitMgr.h"
#include "KCardActor.h"


#define MAX_ONHIT_MSG   100
#define MAX_SKILL_ONHIT_MSG 120

template<> KOnHitMgr* Singleton<KOnHitMgr>::mSingleton = 0;

KOnHitMgr& KOnHitMgr::getSingleton(void)
{  
	if(!mSingleton){
		mSingleton = new KOnHitMgr;
		mSingleton->init();
	}
	return ( *mSingleton );  
}

KOnHitMgr::KOnHitMgr()
{
}

KOnHitMgr::~KOnHitMgr()
{
}

void KOnHitMgr::init()
{
	for(int i=0;i<MAX_On_HIT_NUM;i++){
		memset(&m_onHitArr[i],0,sizeof(K3DActionParam)); 
	}
}

bool KOnHitMgr::AddOnHitMissile(K3DActionParam* pParam)//处理飞行命中
{
	//if(FindSkillHit(pParam->BulletID())) //是普通技能，不处理
	//	return false;
	K3DActionParam* p = NULL;
	for(int i=0;i<MAX_On_HIT_NUM;i++){
		 if( m_onHitArr[i]._bulletID==0){
			 p = &m_onHitArr[i];
			 break;
		 } 
	}
	if(p) p->clone(pParam);
	
	return (p!=NULL);
}

void KOnHitMgr::OnMissileOnHit(int bulletId)
{
	if(!bulletId) return;
	for(int i=0;i<MAX_On_HIT_NUM;i++){
		if( m_onHitArr[i]._bulletID==bulletId){
			KCardActor* actor = (KCardActor*)m_onHitArr[i]._desPtr;
			actor->GetActionMgr().PlayAction(&m_onHitArr[i]);
			memset(&m_onHitArr[i],0,sizeof(K3DActionParam));
			m_onHitArr[i]._bulletID = 0;
		} 
	}
}