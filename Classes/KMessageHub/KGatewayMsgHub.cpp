#include "KGatewayMsgHub.h"
#include "../Inc/KLogicMsg.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../KNet/KClientGateListener.h"
#include "../KNet/KUserData.h"

KGatewayMsgHub::KGatewayMsgHub()
{
	
}

KGatewayMsgHub::~KGatewayMsgHub()
{

}

void KGatewayMsgHub::onGatewayAccepted(unsigned long long p1, unsigned long long p2)
{
	KClientGateListener::getSingleton().SendPlayerLogin();

}

void KGatewayMsgHub::onCharInfoOK(unsigned long long p1, unsigned long long p2)
{
	KClientGateListener::getSingleton().SendEnterCharacter();
}