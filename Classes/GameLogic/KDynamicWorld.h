/********************************************************************
created:	2013/11/06
manager:	zhufanan
purpose:	表现与逻辑的交互类，
*********************************************************************/

#ifndef _KDYNAMICWORLD_H
#define _KDYNAMICWORLD_H

#include "System/Singleton.h"
#include "../CommonLogic/KLogicModuleMsgDealer.h"
#ifdef _USE_COCOS2DX
#include "../CommonLogic/KLoginMsgDealer.h"
#include "../CommonLogic/KGatewayMsgDealer.h"
#include "../EventSystem/KEventDefine.h"
#include "../EventSystem/KEventSystem.h"
#include "../EventSystem/keventholder.h"
#endif


class KBattleGuy;
class KCardInst;
class FBattleGuy;
class KMainPlayer;
class KDynamicWorld : public Singleton<KDynamicWorld>
{
public:
	KDynamicWorld( ){}

	virtual ~KDynamicWorld(){};

	static KDynamicWorld& getSingleton(void);
	void init();
	
	void SendLoginMsg(int msgID, unsigned long long Param1, unsigned long Param2);
	void SendGatewayMsg(int msgID, unsigned long long Param1, unsigned long Param2);
	void SendWorldMsg(int msgID, unsigned long long Param1, unsigned long long Param2);
	KLogicModuleMsgDealer m_LogicMsgDealer;
    const char* GetStringByID(int id);
	void onSystemMsg(int id);
	KWorldObjAbout::KPlayer* GetPlayer(int id);
#ifdef _USE_COCOS2DX
	KLoginMsgDealer m_LoginMsgDealer;
	KGatewayMsgDealer m_GatewayMsgDealer;

	public:
	KEventAbout::KEventHolder<KEventAbout::enumWorldEvent_End - KEventAbout::enumWorldEvent_Begin, KEventAbout::enumWorldEvent_Begin> m_eventList;
	DWORD RegEvent(KEventAbout::KEventID eventID, KEventAbout::IEventInterface* pEI, DWORD key, DWORD customData)
	{
		return m_eventList.RegEventHandler(eventID, pEI, key, customData);
	}

	// 取消一个事件处理
	void RemoveEvent(KEventAbout::KEventID eventID, DWORD key)
	{
		m_eventList.RemoveEventHandler(eventID, key);
	}
	void FireEvent(const KEventAbout::KEventData& evtData){m_eventList.OnEvent(&evtData);}

	void onKillMonster(int id);
	void onQuestSetSession(KQuestNew* pQuest);
#endif


};

#endif // _KDYNAMICWORLD_H
