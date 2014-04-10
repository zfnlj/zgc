#include "KGatewayMsgDealer.h"
#include "../Inc/KLogicMsg.h"
#include "../KMessageHub/KLoginMsgHub.h"
#include "../KMessageHub/KMsgHub.h"

KGatewayMsgDealer::KGatewayMsgDealer()
{
	
}

KGatewayMsgDealer::~KGatewayMsgDealer()
{

}

void KGatewayMsgDealer::RegisterAllMsg()
{
	RegisterMsg<KGatewayMsgHub>(LOGIC_GW_ACCEPTED,					&KMsgHub::getSingleton().GetGatewayMsgHub(),	&KGatewayMsgHub::onGatewayAccepted);
	RegisterMsg<KGatewayMsgHub>(LOGIC_GW_CHARINFO,					&KMsgHub::getSingleton().GetGatewayMsgHub(),	&KGatewayMsgHub::onCharInfoOK);
	
}


void KGatewayMsgDealer::Setup()
{
	m_MsgMap.init(LOGIC_GW_COUNT - LOGIC_GATEWAY_BEGIN, LOGIC_GATEWAY_BEGIN);
	RegisterAllMsg();
}
	
