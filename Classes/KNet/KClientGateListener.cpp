#include "KClientGateListener.h"
#include "System/Misc/md5new.h"
#include "Inc/PacketGate/gateway_packet.h"
#include "../Inc/GlobalLogin2Client.h"
#include "System/Misc/KEncoding.h"
#include "../staticTable/StaticData.h"
#include "KUserData.h"
using namespace System::Collections;

IMPLEMENT_SINGLETON(KClientGateListener)

class ClientSocketFilter : public JgSocketFilter
{
public:
	BOOL Accept(KSocket* pSock)
	{
		DWORD ud = pSock->GetUserData();
		switch(ud)
		{
		case enum_Server_Login_Server:
			{
				KLoginSocket* sock = (KLoginSocket*)pSock;
				return sock->ReadAvail() > 0;
			}
		case enum_Server_Gateway_Server:
			{
				KGatewaySocket* sock = (KGatewaySocket*)pSock;
				return sock->ReadAvail() > 0;
			}
		default:
			{
				return FALSE;
			}
		}
	}
};

KClientGateListener::KClientGateListener(void)
{
}

KClientGateListener::~KClientGateListener(void)
{
}

void KClientGateListener::init()
{
	m_cnnState = enum_cnn_null;
	m_sockServerInited = FALSE;
	m_pLoginSocket = NULL;
	m_pGatewaySocket = NULL;
	m_sockServerInited = FALSE;	
	m_PacketStream.flush();
	if (!m_sockServerInited)
	{		
		if(!m_sockServer.Initialize(16,2)) return;
		m_sockServerInited = TRUE;
	}
}

void KClientGateListener::update(float dt)
{
	ClientSocketFilter socketFilter;
	int sockNum = m_sockServer.LockSockets(&socketFilter, m_socketes, 128);
	for(int i=0; i<sockNum; i++)
	{
		JgPacketReceiver* rv = m_socketes[i];
		KSocket* pSocket = rv->GetSocket();
		DWORD sockUdata = rv->_getUserData();

		DWORD startTick = GetTickCount();
		while(GetTickCount() < startTick + 50)
		{
			int packetNum = rv->ReadPackets(m_packetBuffer, sizeof(m_packetBuffer), m_packets, 64);
			if(!packetNum) break;
			for(int k=0; k<packetNum; k++)
			{
				JgPacketHead* packet = (JgPacketHead*)m_packets.at(k);
				switch(sockUdata)
				{
				case enum_Server_Login_Server:
					m_gateLogin.Process(pSocket, packet);
					break;
				case enum_Server_Gateway_Server:
					m_gateGateway.Process((KGatewaySocket*)pSocket, packet);
					break;
				}
			}
		}
	}
	m_sockServer.UnlockSockets(m_socketes, sockNum);
	{
		int msec = (int)(dt*1000);
		m_vsocketManager.breathe(msec);
	}
}

void KClientGateListener::Finalize()
{
	m_sockServer.Finalize();
	m_pLoginSocket = NULL;
	m_pGatewaySocket = NULL;
}

BOOL KClientGateListener::ConnectLoginServer(const char* acct, const char* pwd)
{
	DisconnectLoginServer();

	KUserData::Instance()->m_acctName = acct;
	KUserData::Instance()->setPassword(pwd);

	m_pLoginSocket = KLoginSocket::Alloc();
	m_pLoginSocket->Reset();
	m_pLoginSocket->SetRemoteAddress(STATIC_DATA_STRING("ip"), STATIC_DATA_INT("port"));
	m_pLoginSocket->AddRef();
	m_sockServer.Attach(m_pLoginSocket);

	m_cnnState = enum_cnn_login;
	return TRUE;
}

BOOL KClientGateListener::DisconnectLoginServer()
{
	if (m_pLoginSocket)
	{
		m_pLoginSocket->Close();
		m_pLoginSocket->ReleaseRef();
		m_pLoginSocket = NULL;
	}
	return TRUE;
}

BOOL KClientGateListener::SendToLoginServer( int cmd, const void* pData, int len )
{
	if (!m_pLoginSocket || !m_pLoginSocket->m_cnnReady) return FALSE;
	return m_pLoginSocket->Send(cmd, pData, len);
}

BOOL KClientGateListener::ConnectGatewayServer(const KSocketAddress& addr )
{
	DisconnectGatewayServer();
	KGatewaySocket* pGS = new KGatewaySocket();
	pGS->SetRemoteAddress(addr); pGS->AddRef();
	m_arrGwSockets.push_back(pGS);
	m_sockServer.Attach(pGS);
	return TRUE;
}

BOOL KClientGateListener::DisconnectGatewayServer()
{
	if (m_pGatewaySocket)
	{
		m_pGatewaySocket->Close();
		m_pGatewaySocket = NULL;
	}
	return TRUE;
}

BOOL KClientGateListener::SendToGatewayServer( int cmd, const void* pData, int len )
{
	if (m_pGatewaySocket && m_pGatewaySocket->m_cnnReady)
		return m_pGatewaySocket->Send(cmd, pData, len);
	return FALSE;
}

BOOL KClientGateListener::SendToCharServer(int cmd, const void* data, int len)
{
	if(!m_pGatewaySocket || !m_pGatewaySocket->isCharReady())
		return FALSE;
	return m_pGatewaySocket->m_c2char->Send(cmd, data, len);
}

BOOL KClientGateListener::SendToGameServer(int cmd, const void* data, int len)
{
	if(!m_pGatewaySocket || !m_pGatewaySocket->isGameReady())
		return FALSE;
	return m_pGatewaySocket->m_c2game->Send(cmd, data, len);
}

BOOL KClientGateListener::SendToWorldServer(int cmd, const void* data, int len)
{
	return FALSE;
	//if(!m_pGatewaySocket || !m_pGatewaySocket->isWouldBlock())
	//	return FALSE;
	//return m_pGatewaySocket->m_c2char->Send(cmd, data, len);
}

BOOL KClientGateListener::SendLogin()
{
	KUserData* ud = KUserData::GetInstancePtr();
	CL_LoginReq loginData;
	strcpy_k(loginData.acctName, sizeof(loginData.acctName), ud->m_acctName.c_str());
	memcpy(&loginData.passwdMd5, ud->m_password, 16);
	return SendToLoginServer(CL_LoginReq::s_nCode, &loginData, sizeof(CL_LoginReq));
}

BOOL KClientGateListener::SendEnterCharacter()
{
	KUserData* ud = KUserData::GetInstancePtr();
	PlayerData* player = ud->get(0);
	if(!player) return FALSE;
	CChr_EnterGame packet;
	packet.playerID = player->playerID;
	packet.mapID = player->map;
	return this->SendToCharServer(s_nCChr_EnterGame, &packet, sizeof(packet));
}

BOOL KClientGateListener::SendCheckCharacterName(const char * szName)
{
	CChr_CheckName packet;
	strcpy_k(packet.playerName, sizeof(packet.playerName), szName);
	return this->SendToCharServer(s_nCChr_CheckName, &packet, sizeof(packet));
}

BOOL KClientGateListener::SendPlayerLogin()
{
	KUserData* ud = KUserData::GetInstancePtr();
	ClientGw_PlayerLogin packetData;
	packetData.otpCode = ud->m_otpCode;
	packetData.passport = ud->m_passport;

	const KServerAreaSettings::Server* server = KUserData::GetInstancePtr()->m_serverAreaSettings.getAreaAt(0)->getServerAt(0);
	if(server){
		const KServerAreaSettings::Service* world = server->getService("world");
		packetData.worldPublicIp = world->m_publicAddr.m_ip;
		packetData.worldPublicPort = world->m_publicAddr.m_port;
	}

	return SendToGatewayServer(ClientGw_PlayerLogin::s_nCode, &packetData, sizeof(packetData));
}

BOOL KClientGateListener::EnterGameLobby()
{
	return SendToGameServer(s_nCS_PlayerEnterLobby, NULL, 0);
}

void KClientGateListener::WriteInt64( INT64 val )
{
	m_PacketStream.WriteInt64(val);
}

void KClientGateListener::WriteInt( int val )
{
	m_PacketStream.WriteInt(val);
}

void KClientGateListener::WriteShort( short val )
{
	m_PacketStream.WriteShort(val);
}

void KClientGateListener::WriteByte( BYTE val )
{
	m_PacketStream.WriteByte(val);
}

void KClientGateListener::WriteFloat( float val )
{
	m_PacketStream.WriteFloat(val);
}

void KClientGateListener::WriteString( const char* val )
{
	m_PacketStream.WriteString(val);
}

void KClientGateListener::Flush( int cmd )
{
	SendToGatewayServer(cmd, m_PacketStream.m_pBuf, m_PacketStream.m_pos);
	m_PacketStream.flush();
}

void KClientGateListener::PostPacket( int cmd, const void* packet, DWORD packetLength )
{
	if (m_pGatewaySocket && m_pGatewaySocket->m_cnnReady)
	{
		m_pGatewaySocket->PostPacket(cmd, packet, packetLength);
	}
}
