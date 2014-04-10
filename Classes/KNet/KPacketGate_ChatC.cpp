//#include "KPacketGate_ChatC.h"
//#include "KClientGateListener.h"
//#include "../Facade/FLogicWorld.h"
//#include <System/Log/log.h>
//#include <System/Misc/KStream.h>
//#include "../KApplication.h"
//#include "../KAppData.h"
//#include "../KUserData.h"
//
////KPacketGate_ChatC::KPacketGate_ChatC()
////{
////	memset(&m_methods, 0, sizeof(m_methods));
////
////	this->Register(s_nChatClient_Connected, &KPacketGate_ChatC::Process_ChatClient_Connected);
////	this->Register(s_nChatClient_ConnError, &KPacketGate_ChatC::Process_ChatClient_ConnError);
////	this->Register(s_nChatClient_ConnClosed, &KPacketGate_ChatC::Process_ChatClient_ConnClosed);
////	this->Register(s_nChatClient_PingRsp, &KPacketGate_ChatC::Process_ChatClient_PingRsp);
////}
////
////KPacketGate_ChatC::~KPacketGate_ChatC()
////{
////
////}
////
////BOOL KPacketGate_ChatC::Register(int cmd, ProcMethod method)
////{
////	if(cmd < CommandBegin) return FALSE;
////	if(cmd >= CommandEnd)  return FALSE;
////	m_methods[cmd-CommandBegin] = method;
////	return TRUE;
////}
////
////void KPacketGate_ChatC::Process(KChatSocket* pSock, const void* pPacket)
////{
////	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;
////
////	const void* pData = pHead + 1;
////	int command = pHead->Command();
////	int dataLength = pHead->Length();
////	ProcessOnePacket(pSock, command, pData, dataLength);
////}
////
////void KPacketGate_ChatC::ProcessOnePacket(KChatSocket* pSocket, int command, const void* pData, int len)
////{
////	if(command >= CommandBegin && command < CommandEnd)
////	{
////		ProcMethod method = m_methods[command - CommandBegin];
////		if( NULL != method )
////		{
////			(this->*method)(pSocket, pData, len);
////		}
////	}
////
////	GetLogicWorld()->getPacketDeliver()->deliverPacket(command, pData, len);
////	return;
////}
////
////void KPacketGate_ChatC::Process_ChatClient_Connected(KChatSocket* pSock, const void* pData, int len)
////{
////	KClientGateListener::GetInstancePtr()->OnServerConnect(enum_Server_Chat_Server);
////}
////
////void KPacketGate_ChatC::Process_ChatClient_ConnError(KChatSocket* pSock, const void* pData, int len)
////{
////	pSock->m_shouldRemove = TRUE;
////}
////
////void KPacketGate_ChatC::Process_ChatClient_ConnClosed(KChatSocket* pSock, const void* pData, int len)
////{
////	if(!pSock->IsReady()) pSock->m_shouldRemove = TRUE;
////	KClientGateListener::GetInstancePtr()->OnServerClose(enum_Server_Chat_Server);
////}
////
////void KPacketGate_ChatC::Process_ChatClient_PingRsp(KChatSocket* pSock, const void* pData, int len)
////{
////	//pSock->m_dwLastPingChatRsp = ::GetTickCount();
////}