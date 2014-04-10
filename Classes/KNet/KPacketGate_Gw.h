#pragma once
#include "KSocketDefine.h"
#include "Inc/PacketGate/gateway_packet.h"

class KPacketGate_Gw
{
public:
	enum { CommandBegin = s_nGwClient_PacketStart };
	enum { CommandEnd   = s_nGwClient_PacketEnd   };
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_Gw::*ProcMethod)(KGatewaySocket* pSock, const void* pData, int len);

public:
	KPacketGate_Gw();
	~KPacketGate_Gw();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KGatewaySocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];

private:
	void Process_GwClient_Connect(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Close(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_ConnError(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_ConnAccepted(KGatewaySocket* pSock, const void* pData, int len);

	void Process_GwClient_World_Connected(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_World_AskClose(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_World_Closed(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_World_OtpCode_Result(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_World_UpdateEnterQueueInfo(KGatewaySocket* pSock, const void* pData, int len);

	void Process_GwClient_Character_Connected(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Character_AskClose(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Character_Closed(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Character_Forward(KGatewaySocket* pSock, const void* pData, int len);
	
	void Process_GwClient_Game_Connected(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Game_AskClose(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Game_Closed(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Game_Forward(KGatewaySocket* pSock, const void* pData, int len);
	
	void Process_GwClient_Chat_Connected(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Chat_AskClose(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Chat_Closed(KGatewaySocket* pSock, const void* pData, int len);
	void Process_GwClient_Chat_Forward(KGatewaySocket* pSock, const void* pData, int len);
};
