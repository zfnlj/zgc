#include "KPacketGate_WC.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include <System/Misc/KStream.h>

KPacketGate_WC::KPacketGate_WC()
{
	memset(&m_methods, 0, sizeof(m_methods));
	this->Register(s_nWC_Connected, &KPacketGate_WC::Process_WC_Connected);
	this->Register(s_nWC_ConnError, &KPacketGate_WC::Process_WC_ConnError);
	this->Register(s_nWC_Closed, &KPacketGate_WC::Process_WC_Closed);
	this->Register(s_nWC_OtpCode_Result, &KPacketGate_WC::Process_WC_OtpCode_Result);
	this->Register(s_nWC_UpdateEnterQueueInfo, &KPacketGate_WC::Process_WC_UpdateEnterQueueInfo);
}

KPacketGate_WC::~KPacketGate_WC()
{

}

BOOL KPacketGate_WC::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_WC::Process(KWorldSocket* pSock, const void* pPacket)
{
	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	if(command < CommandBegin || command >= CommandEnd)
	{
		Log(LOG_ERROR, "error: unexpected ChC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method)
	{
		Log(LOG_ERROR, "error: no proc found for WC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	(this->*method)(pSock, pData, dataLength);
}

void KPacketGate_WC::Process_WC_Connected(KWorldSocket* pSock, const void* pData, int len)
{
	
}

void KPacketGate_WC::Process_WC_ConnError(KWorldSocket* pSock, const void* pData, int len)
{
	pSock->m_shouldRemove = TRUE;
	
}

void KPacketGate_WC::Process_WC_Closed(KWorldSocket* pSock, const void* pData, int len)
{
	if(!pSock->IsReady()) pSock->m_shouldRemove = TRUE;
}

void KPacketGate_WC::Process_WC_OtpCode_Result(KWorldSocket* pSock, const void* pData, int len)
{
	WC_OtpCode_Result* rsp = (WC_OtpCode_Result*)pData;
	if(rsp->result)
	{
		pSock->m_pGatewaySocket->Close();
		return;
	}

}

void KPacketGate_WC::Process_WC_UpdateEnterQueueInfo(KWorldSocket* pSock, const void* pData, int len)
{
	
}
