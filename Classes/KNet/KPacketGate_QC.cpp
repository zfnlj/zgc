#include "KPacketGate_QC.h"
#include "KPacketGate_LC.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include "../KApplication.h"
#include "../KLogin/KLoginServiceInterface.h"
#include "../KAppData.h"

KPacketGate_QC::KPacketGate_QC()
{
	memset(&m_methods, 0, sizeof(m_methods));
	this->Register(s_nQueryC_Connected, &KPacketGate_QC::Process_QC_Connected);
	this->Register(s_nQueryC_ConnError, &KPacketGate_QC::Process_QC_ConnError);
	this->Register(s_nQueryC_ConnClosed, &KPacketGate_QC::Process_QC_ConnClosed);
	this->Register(s_nQueryC_CharsCountRsp, &KPacketGate_QC::Process_QC_CharsCountRsp);
}

KPacketGate_QC::~KPacketGate_QC()
{

}

BOOL KPacketGate_QC::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_QC::Process(KSocket* pSock, const void* pPacket)
{
	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	//if(pSock->GetUserData() != enum_character_socket_Ping)
	//{
	//	Log(LOG_ERROR, "error: invalid QC tcp socket %s, cmd:%u, length:%u",
	//		pSock->ToString().c_str(),
	//		command, dataLength);
	//	return;
	//}

	KCharacterSocket2* pAiSock = (KCharacterSocket2*)pSock;

	if(command < CommandBegin || command >= CommandEnd)
	{
		Log(LOG_ERROR, "error: unexpected QC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method)
	{
		Log(LOG_ERROR, "error: no proc found for QC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	(this->*method)(pAiSock, pData, dataLength);
}

void KPacketGate_QC::Process_QC_Connected(KCharacterSocket2* pSock, const void* pData, int len)
{
	KClientGateListener* cgl = (KClientGateListener*)KClientGateListener::GetInstancePtr();
	if(cgl->m_pCharacterSocket_Ping == pSock)
	{
		KClientGateListener::GetInstancePtr()->OnServerConnect(enum_Server_Character_Server2);
		KLoginServiceInterface::GetInstancePtr()->ResponceLoginMsg(LOGIC_LOGIN_QUERY_SERVER_CONNECTED, 0, 0);
	}
}

void KPacketGate_QC::Process_QC_ConnError(KCharacterSocket2* pSock, const void* pData, int len)
{
	KClientGateListener* cgl = (KClientGateListener*)KClientGateListener::GetInstancePtr();
	if(cgl->m_pCharacterSocket_Ping == pSock)
	{
		KClientGateListener::GetInstancePtr()->OnServerConnError(enum_Server_Character_Server2);
		KLoginServiceInterface::GetInstancePtr()->ResponceLoginMsg(LOGIC_LOGIN_QUERY_SERVER_CONNERROR, 0, 0);
		if(cgl->m_pCharacterSocket_Ping)
		{
			cgl->m_pCharacterSocket_Ping->ReleaseRef();
			cgl->m_pCharacterSocket_Ping = NULL;
		}
	}
}

void KPacketGate_QC::Process_QC_ConnClosed(KCharacterSocket2* pSock, const void* pData, int len)
{
	KClientGateListener* cgl = (KClientGateListener*)KClientGateListener::GetInstancePtr();
	if(cgl->m_pCharacterSocket_Ping == pSock)
	{
		KClientGateListener::GetInstancePtr()->OnServerClose(enum_Server_Character_Server2);
		KLoginServiceInterface::GetInstancePtr()->ResponceLoginMsg(LOGIC_LOGIN_QUERY_SERVER_CONNCLOSED, 0, 0);
		if(cgl->m_pCharacterSocket_Ping)
		{
			cgl->m_pCharacterSocket_Ping->ReleaseRef();
			cgl->m_pCharacterSocket_Ping = NULL;
		}
	}
}

void KPacketGate_QC::Process_QC_CharsCountRsp(KCharacterSocket2* pSock, const void* pData, int len)
{
	KLoginServiceInterface::GetInstancePtr()->ResponceLoginMsg(LOGIC_LOGIN_QUERY_RESULT, (unsigned long)pData, (unsigned long)len);
}
