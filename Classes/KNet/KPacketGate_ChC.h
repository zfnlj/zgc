#ifndef _K_PACKET_GATE_CHC_H_
#define _K_PACKET_GATE_CHC_H_

#include "KSocketDefine.h"

class KPacketGate_ChC
{
public:
	enum { CommandBegin = s_nChrC_PacketStart	};
	enum { CommandEnd   = s_nChrC_PacketEnd	};
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_ChC::*ProcMethod)(KCharacterSocket* pSock, const void* pData, int len);

public:
	KPacketGate_ChC();
	~KPacketGate_ChC();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KCharacterSocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];

private:
	void Process_ChC_Connected(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChC_ConnError(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChC_ConnClosed(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_Message(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_AccountInfo(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_CharacterInfo(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_CheckName_Rsp(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_CreateCharacter_Rsp(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_DeleteCharacter_rsp(KCharacterSocket* pSock, const void* pData, int len);
	void Process_ChrC_EnterGame_Rsp(KCharacterSocket* pSock, const void* pData, int len);
};

#endif