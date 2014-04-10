#include "KLoginMsgDealer.h"
#include "../Inc/KLogicMsg.h"
#include "../KMessageHub/KLoginMsgHub.h"
#include "../KMessageHub/KMsgHub.h"

KLoginMsgDealer::KLoginMsgDealer()
{
	
}

KLoginMsgDealer::~KLoginMsgDealer()
{

}

void KLoginMsgDealer::RegisterAllMsg()
{
	

	RegisterMsg<KLoginMsgHub>(LOGIC_LOGIN_SERVER_CONNECTED,			&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLoginServerConnected);
	//RegisterLoginMsg<KLoginMsgHub>(LOGIC_LOGIN_SERVER_CONNERROR,			&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLoginServerConnError);
	//RegisterLoginMsg<KLoginMsgHub>(LOGIC_LOGIN_SERVER_CONNCLOSED,			&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLoginServerConnClosed);
	RegisterMsg<KLoginMsgHub>(LOGIC_LOGIN_LOGIN_RSP,					&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLoginRsp);
	RegisterMsg<KLoginMsgHub>(LOGIC_LOGIN_SERVER_LIST_OVER,			&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLoginServerList);
	
	//RegisterLoginMsg<KLoginMsgHub>(LOGIC_LOGIN_LOGOUT_RSP,					&KMsgHub::getSingleton().GetLoginMsgHub(),	&KLoginMsgHub::onLogoutRsp);
}


void KLoginMsgDealer::Setup()
{
	m_MsgMap.init(LOGIC_LOGIN_COUNT - LOGIC_LOGIN_SERVER_CONNECTED, LOGIC_LOGIN_SERVER_CONNECTED);
	RegisterAllMsg();
}
