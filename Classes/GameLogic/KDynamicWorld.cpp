#include "KDynamicWorld.h"
#include "KBattleAI.h"
#include "KBattleGuy.h"
#include "../Inc/KTypeDef.h"
#include "../WorldObject/KMainPlayer.h"
#include "../EventSystem/KEventDefine.h"
#include "../UI/assist/KPopupLayer.h"
#include "../common/KUIMsgDef.h"
#include "../PlayerCard/KTowerAssist.h"
#include "../WorldObject/KPlayer.h"
#include "../common/KPlayerRecordAssist.h"

#ifdef _USE_COCOS2DX
#include "../UI/assist/KUIAssist.h"
#include "../UI/KSceneLayerBase.h"
#endif

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
#ifdef _USE_COCOS2DX
	if(KUIAssist::_activeSceneLayer()){
		KUIAssist::_activeSceneLayer()->onSystemMsg(id);
	}else{
		KPopupLayer::DoNotifyDlg(id);
	}
#endif
}


KWorldObjAbout::KPlayer* KDynamicWorld::GetPlayer(int id)
{
	return KMainPlayer::RealPlayer();

//#ifdef _USE_COCOS2DX
//	if(id==0) return KMainPlayer::RealPlayer();
//#endif
//	return NULL; //TBD
}

void KDynamicWorld::onBattleFailed(int questId,Scene_type tp)
{
	KMainPlayer::RealPlayer()->GetResultBag()->Reset();
	if(tp==scene_tower){
		KTowerAssist::_deactive(KMainPlayer::RealPlayer()->GetPlayerRecord());
		KDynamicWorld::getSingleton().onSystemMsg(TOWER_CLOSE);
	}else{
		/*if(tp==scene_daily){
			KPlayerRecordAssist::DailyStageLost(KMainPlayer::RealPlayer()->GetPlayerRecord());
		}*/
		KEventAbout::KEDBattleFailed evt;
		evt.m_nID = questId;
		FireEvent(evt);
	}
}

#ifdef _USE_COCOS2DX
void KDynamicWorld::onBattleWin(int questId,int monsterId,Scene_type tp)
{
	KMainPlayer::RealPlayer()->GetResultBag()->Reset();
	if(tp==scene_tower){
		KTowerAssist::_win(questId);
	}else{
		KEventAbout::KEDKillMonster evt;
		evt.m_nMonsterID = monsterId;
		FireEvent(evt);
	}
}

void KDynamicWorld::onQuestSetSession(KQuestNew* pQuest)
{
	KEventAbout::KEDQuestSession evt;
	evt.m_pQuest = pQuest;
	FireEvent(evt);
}

#endif