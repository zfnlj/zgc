#include "KPacketGate_LC.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include "KUserData.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "cocos2d.h"

USING_NS_CC;

KPacketGate_LC::KPacketGate_LC()
{
	memset(&m_methods, 0, sizeof(m_methods));
	this->Register(s_nLC_Connected, &KPacketGate_LC::Process_LC_Connected);
	this->Register(s_nLC_ConnError, &KPacketGate_LC::Process_LC_ConnError);
	this->Register(s_nLC_ConnClosed, &KPacketGate_LC::Process_LC_ConnClosed);
	this->Register(s_nLC_LoginRsp, &KPacketGate_LC::Process_LC_LoginRsp);
	this->Register(s_nLC_ServerAreaInfos, &KPacketGate_LC::Process_LC_ServerAreaInfos);
	this->Register(s_nLC_Message_Rsp, &KPacketGate_LC::Process_LC_Message_Rsp);
}

KPacketGate_LC::~KPacketGate_LC()
{

}

BOOL KPacketGate_LC::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_LC::Process(KSocket* pSock, const void* pPacket)
{
	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	KLoginSocket* loginSocket = (KLoginSocket*)pSock;

	if(command < CommandBegin || command >= CommandEnd)
	{
		Log(LOG_ERROR, "error: unexpected LC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method)
	{
		Log(LOG_ERROR, "error: no proc found for LC packet cmd:%u length:%u", command, dataLength);
		return;
	}
	
	(this->*method)(loginSocket, pData, dataLength);
}

void KPacketGate_LC::Process_LC_Connected(KLoginSocket* pSock, const void* pData, int len)
{
	CCLog("Connect Login server:%s", pSock->ToString().c_str());
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pLoginSocket == pSock)
	{
		KUserData* ud = KUserData::GetInstancePtr();
		CL_LoginReq packet;
		strcpy_k(packet.acctName, sizeof(packet.acctName), ud->m_acctName.c_str());
		memcpy(&packet.passwdMd5, &ud->m_passport, 16);
		pSock->Send(s_nCL_LoginReq, &packet, sizeof(packet));
	}
}

void KPacketGate_LC::Process_LC_ConnError(KLoginSocket* pSock, const void* pData, int len)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pLoginSocket == pSock)
	{
		cgl->m_pLoginSocket->ReleaseRef();
		cgl->m_pLoginSocket = NULL;
	}
	CCLog("Error Connect Login server:%s", pSock->ToString().c_str());
}

void KPacketGate_LC::Process_LC_ConnClosed(KLoginSocket* pSock, const void* pData, int len)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pLoginSocket == pSock)
	{
		cgl->m_pLoginSocket->ReleaseRef();
		cgl->m_pLoginSocket = NULL;
	}
	CCLog("Login Server Closed:%s!", pSock->ToString().c_str());
}

void KPacketGate_LC::Process_LC_LoginRsp(KLoginSocket* pSock, const void* pData, int len)
{
	LC_LoginRsp* rsp = (LC_LoginRsp*)pData;
	CCLog("Login Server Rsp result:%d", rsp->result);

	if(rsp->result)
	{
		pSock->Close();
		return;
	}
	KUserData* ud = KUserData::GetInstancePtr();
	ud->m_passport = rsp->passport;
	ud->m_otpCode = rsp->otpCode;

	KDynamicWorld::getSingleton().SendLoginMsg(LOGIC_LOGIN_LOGIN_RSP,(unsigned long long)pData,0);
}

void KPacketGate_LC::Process_LC_ServerAreaInfos(KLoginSocket* pSock, const void* pData, int len)
{
	KMsgInputStream si(pData, len);
	KUserData* ud = KUserData::GetInstancePtr();
	ud->m_serverAreaSettings.deserialize(&si);
}

void KPacketGate_LC::Process_LC_Message_Rsp(KLoginSocket* pSock, const void* pData, int len)
{

}
