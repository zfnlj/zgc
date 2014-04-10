#include "KLoginMsgHub.h"
#include "../Inc/KLogicMsg.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../KNet/KClientGateListener.h"
#include "../KNet/KUserData.h"
#include "ccMacros.h"
KLoginMsgHub::KLoginMsgHub()
{
	
}

KLoginMsgHub::~KLoginMsgHub()
{

}

void KLoginMsgHub::onLoginServerConnected(unsigned long long p1, unsigned long long p2)
{
	//GameRoot::getSingleton().getBattleScene()->onBattleInit();
	KClientGateListener::getSingleton().SendLogin();
}

void KLoginMsgHub::onLoginServerList(unsigned long long p1, unsigned long long p2)
{
	//// 记录所有服务器信息
	//KMsgInputStream lPacket((char*)p1, p2);
	//KUserData::GetInstancePtr()->GetAreaSetting().deserialize(&lPacket);
}

void KLoginMsgHub::onLoginRsp(unsigned long long p1, unsigned long long p2)
{
	const KServerAreaSettings::Server* server = KUserData::GetInstancePtr()->m_serverAreaSettings.getAreaAt(0)->getServerAt(0);
	CCAssert(server,"gateway is null!");
	if(server){
		const KServerAreaSettings::Service* gateway = server->getService("gateway");
		KSocketAddress addr;
		addr.m_ip = gateway->m_publicAddr.m_ip;
		addr.m_port = gateway->m_publicAddr.m_port;
		KClientGateListener::getSingleton().ConnectGatewayServer(addr);
	}
}
