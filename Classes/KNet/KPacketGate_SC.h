#pragma once
#include "KSocketDefine.h"
#include "CommonLogic/CommSession/KStreamSession.h"

class KPacketGate_SC
{
public:
	enum { CommandBegin = s_nSC_PacketStart	};
	enum { CommandEnd   = s_nSC_PacketEnd	};
	enum { CommandCount = CommandEnd-CommandBegin };
	typedef void (KPacketGate_SC::*ProcMethod)(KGameSocket* pSock, const void* pData, int len);

public:
	KPacketGate_SC();
	~KPacketGate_SC();

public:
	BOOL Register(int cmd, ProcMethod method);
	void Process(KGameSocket* pSock, const void* pPacket);

public:
	ProcMethod m_methods[CommandCount];
	KStreamSession m_streamSession;

private:
	void Process_SC_Connected(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_ConnError(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_ConnClosed(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_PingAck(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_PlayerLogin_Rsp(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_ChangeScene(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_MessageBox(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_PlayerAppear(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_PlayerDisappear(KGameSocket* pSock, const void* pData, int len);

	//卡牌
	void Process_SC_BattleCtrlInfo(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleInit(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleTurnBegin(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleSelectHandCardOK(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleWorldUpdate(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleCardMove(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleCardDuelResult(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleUseAbilityResult(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleDrawCard(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleCard2Tomb(KGameSocket* pSock, const void* pData, int len);
	void Process_SC_BattleSummonCard(KGameSocket* pSock, const void* pData, int len);


	//quest
	void Process_SC_SyncAvailQuests(KGameSocket* pSock, const void* pData, int len);

	//store
	void Process_SC_BagOperation(KGameSocket* pSock, const void* pData, int len);
};