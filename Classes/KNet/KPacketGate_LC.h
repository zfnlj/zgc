#ifndef _K_PACKETGATE_LC_H_
#define _K_PACKETGATE_LC_H_

#include "KSocketDefine.h"
#include "Inc/PacketGate/c_login_packet.h"

class KPacketGate_LC
{
public:
	enum { CommandBegin = s_nLC_PacketStart	};
	enum { CommandEnd   = s_nLC_PacketEnd	};
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_LC::*ProcMethod)(KLoginSocket* pSock, const void* pData, int len);

public:
	KPacketGate_LC();
	~KPacketGate_LC();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KSocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];

private:
	void Process_LC_Connected(KLoginSocket* pSock, const void* pData, int len);
	void Process_LC_ConnError(KLoginSocket* pSock, const void* pData, int len);
	void Process_LC_ConnClosed(KLoginSocket* pSock, const void* pData, int len);
	void Process_LC_LoginRsp(KLoginSocket* pSock, const void* pData, int len);
	void Process_LC_ServerAreaInfos(KLoginSocket* pSock, const void* pData, int len);
	void Process_LC_Message_Rsp(KLoginSocket* pSock, const void* pData, int len);
};

#endif