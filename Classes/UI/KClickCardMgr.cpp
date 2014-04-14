#include "KClickCardMgr.h"
#include "KCardActor.h"


#define MAX_ONHIT_MSG   100
#define MAX_SKILL_ONHIT_MSG 120

template<> KClickCardMgr* Singleton<KClickCardMgr>::mSingleton = 0;

KClickCardMgr& KClickCardMgr::getSingleton(void)
{  
	if(!mSingleton){
		mSingleton = new KClickCardMgr;
		mSingleton->init();
	}
	return ( *mSingleton );  
}

KClickCardMgr::KClickCardMgr()
{
}

KClickCardMgr::~KClickCardMgr()
{
}

void KClickCardMgr::init()
{
	m_curActor = NULL;
}

void KClickCardMgr::onClickCard(KCardActor* actor)
{
	static int click_count=0;
	click_count++;
	m_curActor = actor;
}

void KClickCardMgr::onReleaseCard(KCardActor* actor)
{
	if(m_curActor==actor){
		int kk=0;
	}
}