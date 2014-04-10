#pragma once
#include "System/Singleton.h"
#include "KBattleMsgHub.h"
#include "KWorldMsgHub.h"

#ifdef _USE_COCOS2DX
#include "KLoginMsgHub.h"
#include "KGatewayMsgHub.h"
#endif

class KMsgHub : public Singleton<KMsgHub>
{	
public:
	KMsgHub();
	virtual ~KMsgHub();

	static KMsgHub& getSingleton(void);

	KBattleMsgHub&	GetBattleMsgHub() {return m_BattleMsgHub;}
	KWorldMsgHub&	GetWorldMsgHub(){ return m_WorldMsgHub;}
#ifdef _USE_COCOS2DX
	KLoginMsgHub&	GetLoginMsgHub(){ return m_LoginMsgHub;}
	KGatewayMsgHub&	GetGatewayMsgHub(){ return m_GatewayMsgHub;}
	KLoginMsgHub			m_LoginMsgHub;
	KGatewayMsgHub			m_GatewayMsgHub;
#endif
	KBattleMsgHub			m_BattleMsgHub;
	KWorldMsgHub			m_WorldMsgHub;

};