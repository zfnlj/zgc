#pragma once

#include "System/Singleton.h"
#include "KSocketDefine.h"
#include "KPacketGate_LC.h"
#include "KPacketGate_SC.h"
#include "KPacketGate_ChC.h"
#include "KPacketGate_ChatC.h"
#include "KPacketGate_WC.h"
#include "KPacketGate_Gw.h"
#include "System/Misc/KStream.h"
#include "System/Collections/KBitset.h"
#include "CommonLogic/Socket/JgVirtualSocket.h"
#include "CommonLogic/Configure/KServerAreaSettings.h"

class KClientGateListener 
	: public Singleton<KClientGateListener> 
{
public:
	enum CNN_STATE{
		enum_cnn_null,
		enum_cnn_login,
		enum_cnn_login_ok,
		enum_cnn_enterlobby,
		enum_cnn_enterlobby_ok,
	};
	KClientGateListener(void);
	~KClientGateListener(void);
	static KClientGateListener& getSingleton(void);

public:
	void init();
	virtual void update(float dt);
	void Finalize();

public:
	BOOL ConnectLoginServer(const char* acct, const char* pwd);
	BOOL DisconnectLoginServer();
	BOOL SendToLoginServer(int cmd, const void* pData, int len);
	BOOL ConnectGatewayServer(const KSocketAddress& addr );
	BOOL DisconnectGatewayServer();
	BOOL SendToGatewayServer(int cmd, const void* pData, int len);
	void PostPacket(int cmd, const void* packet, DWORD packetLength);

	BOOL SendToCharServer(int cmd, const void* data, int len);
	BOOL SendToGameServer(int cmd, const void* data, int len);
	BOOL SendToWorldServer(int cmd, const void* data, int len);

public:
	BOOL SendLogin();
	BOOL SelectCharServer(int idx);
	BOOL SendEnterCharacter();
	BOOL SendSelectServer(int index);
	BOOL SendCheckCharacterName(const char * szName);

public:
	BOOL SendPlayerLogin();
	BOOL EnterGameLobby();
public:
	KLoginSocket* m_pLoginSocket;
	KGatewaySocket* m_pGatewaySocket;
	JG_C::DynArray<KGatewaySocket*> m_arrGwSockets;

public:
	KPacketGate_LC	m_gateLogin;
	KPacketGate_Gw	m_gateGateway;
	KPacketGate_ChC	m_gateCharater;
	KPacketGate_WC	m_gateWC;
	KPacketGate_SC	m_gateGame;
	KPacketGate_WC	m_gateWorld;

public:
	void WriteInt64(INT64 val);
	void WriteInt(int val);
	void WriteShort(short val);
	void WriteByte(BYTE val);
	void WriteFloat(float val);
	void WriteString(const char* val);
	void Flush(int cmd);

	KMsgInputStream m_PacketStream;

public:
	char m_packetBuffer[10240];
	JgPacketReceiver* m_socketes[128];
	JgPacketReceiver::PacketPtrArray m_packets;

public:
	volatile BOOL m_sockServerInited;
	JgSockServer m_sockServer;
	JgVirtualSocketManager m_vsocketManager;
	KServerAreaSettings m_serverAreaSettings;

	void SetCnnState(CNN_STATE st){ m_cnnState = st;}
	CNN_STATE m_cnnState;
};
