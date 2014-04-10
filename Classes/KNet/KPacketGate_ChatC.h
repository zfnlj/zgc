//#ifndef _K_PACKET_GATE_CHATC_H_
//#define _K_PACKET_GATE_CHATC_H_
//
//#include "KSocketDefine.h"
//
//class KPacketGate_ChatC
//{
//public:
//	enum { CommandBegin = s_nChatClient_PacketStart	};
//	enum { CommandEnd   = s_nChatClient_PacketEnd	};
//	enum { CommandCount = CommandEnd-CommandBegin	};
//	typedef void (KPacketGate_ChatC::*ProcMethod)(KChatSocket* pSock, const void* pData, int len);
//
//public:
//	KPacketGate_ChatC();
//	~KPacketGate_ChatC();
//
//public:
//	BOOL Register(int cmd, ProcMethod method);
//	void Process(KChatSocket* pSock, const void* pPacket);
//	void ProcessOnePacket(KChatSocket* pSocket, int command, const void* pData, int len);
//
//public:
//	ProcMethod m_methods[CommandCount];
//
//private:
//	void Process_ChatClient_Connected(KChatSocket* pSock, const void* pData, int len);
//	void Process_ChatClient_ConnError(KChatSocket* pSock, const void* pData, int len);
//	void Process_ChatClient_ConnClosed(KChatSocket* pSock, const void* pData, int len);
//	void Process_ChatClient_PingRsp(KChatSocket* pSock, const void* pData, int len);
//};
//
//#endif