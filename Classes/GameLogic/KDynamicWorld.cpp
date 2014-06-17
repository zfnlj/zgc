#include "KDynamicWorld.h"
#include "KBattleAI.h"
#include "KBattleGuy.h"
#include "../Inc/KTypeDef.h"
#include "../WorldObject/KMainPlayer.h"
#include "../EventSystem/KEventDefine.h"
#include "../UI/assist/KPopupLayer.h"
#include "../common/KUIMsgDef.h"

template<> KDynamicWorld* Singleton<KDynamicWorld>::mSingleton = 0;

KDynamicWorld& KDynamicWorld::getSingleton(void)
{  
	if(!mSingleton){
		mSingleton = new KDynamicWorld;
		mSingleton->init();
	}
	return ( *mSingleton );  
}

void KDynamicWorld::init()
{
	
#ifdef _USE_COCOS2DX
	m_LoginMsgDealer.Setup();
	m_GatewayMsgDealer.Setup();
#endif
	m_LogicMsgDealer.Setup();
	
}

void KDynamicWorld::SendLoginMsg(int msgID, unsigned long long Param1, unsigned long Param2)
{
#ifdef _USE_COCOS2DX
	m_LoginMsgDealer.SendMsg(msgID,Param1,Param2);
#endif
}

void KDynamicWorld::SendGatewayMsg(int msgID, unsigned long long Param1, unsigned long Param2)
{
#ifdef _USE_COCOS2DX
	m_GatewayMsgDealer.SendMsg(msgID,Param1,Param2);
#endif
}

void KDynamicWorld::SendWorldMsg(int msgID, unsigned long long Param1, unsigned long long Param2)
{
	m_LogicMsgDealer.SendMsg(msgID,Param1,Param2);
}

const char* KDynamicWorld::GetStringByID(int id)
{
    return "";
}

void KDynamicWorld::onSystemMsg(int id)
{
	KPopupLayer::DoNotifyDlg(id);
}


KWorldObjAbout::KPlayer* KDynamicWorld::GetPlayer(int id)
{
	return KMainPlayer::RealPlayer();

//#ifdef _USE_COCOS2DX
//	if(id==0) return KMainPlayer::RealPlayer();
//#endif
//	return NULL; //TBD
}

#ifdef _USE_COCOS2DX
void KDynamicWorld::onKillMonster(int id)
{
	KEventAbout::KEDKillMonster evt;
	evt.m_nMonsterID = id;
	FireEvent(evt);
}

void KDynamicWorld::onQuestSetSession(KQuestNew* pQuest)
{
	KEventAbout::KEDQuestSession evt;
	evt.m_pQuest = pQuest;
	FireEvent(evt);
}
#endif