#pragma once
#include "KSocketDefine.h"
#include "Inc/PacketGate/gateway_packet.h"

class KPacketGate_WC
{
public:
	enum { CommandBegin = s_nWC_PacketStart	};
	enum { CommandEnd   = s_nWC_PacketEnd	};
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_WC::*ProcMethod)(KWorldSocket* pSock, const void* pData, int len);

public:
	KPacketGate_WC();
	~KPacketGate_WC();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KWorldSocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];

private:
	void Process_WC_Connected(KWorldSocket* pSock, const void* pData, int len);
	void Process_WC_ConnError(KWorldSocket* pSock, const void* pData, int len);
	void Process_WC_Closed(KWorldSocket* pSock, const void* pData, int len);
	void Process_WC_OtpCode_Result(KWorldSocket* pSock, const void* pData, int len);
	void Process_WC_UpdateEnterQueueInfo(KWorldSocket* pSock, const void* pData, int len);
};
