#ifndef _K_PACKET_GATE_QC_H_
#define _K_PACKET_GATE_QC_H_

#include "KSocketDefine.h"

class KPacketGate_QC
{
public:
	enum { CommandBegin = s_nQueryC_PacketStart	};
	enum { CommandEnd   = s_nQueryC_PacketEnd	};
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_QC::*ProcMethod)(KCharacterSocket2* pSock, const void* pData, int len);

public:
	KPacketGate_QC();
	~KPacketGate_QC();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KSocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];

private:
	void Process_QC_Connected(KCharacterSocket2* pSock, const void* pData, int len);
	void Process_QC_ConnError(KCharacterSocket2* pSock, const void* pData, int len);
	void Process_QC_ConnClosed(KCharacterSocket2* pSock, const void* pData, int len);
	void Process_QC_CharsCountRsp(KCharacterSocket2* pSock, const void* pData, int len);
};

#endif // _K_PACKET_GATE_QC_H_