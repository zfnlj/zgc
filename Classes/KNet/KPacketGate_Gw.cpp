#include "KPacketGate_Gw.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include "KAppData.h"
#include "KUserData.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "cocos2d.h"
#include "GameRoot.h"
USING_NS_CC;

KPacketGate_Gw::KPacketGate_Gw()
{
	memset(&m_methods, 0, sizeof(m_methods));

	this->Register(s_nGwClient_Connect, &KPacketGate_Gw::Process_GwClient_Connect);
	this->Register(s_nGwClient_Close, &KPacketGate_Gw::Process_GwClient_Close);
	this->Register(s_nGwClient_ConnError, &KPacketGate_Gw::Process_GwClient_ConnError);
	this->Register(s_nGwClient_ConnAccepted, &KPacketGate_Gw::Process_GwClient_ConnAccepted);

	this->Register(s_nGwClient_World_Connected, &KPacketGate_Gw::Process_GwClient_World_Connected);
	this->Register(s_nGwClient_World_AskClose, &KPacketGate_Gw::Process_GwClient_World_AskClose);
	this->Register(s_nGwClient_World_Closed, &KPacketGate_Gw::Process_GwClient_World_Closed);
	this->Register(s_nGwClient_World_OtpCode_Result, &KPacketGate_Gw::Process_GwClient_World_OtpCode_Result);
	this->Register(s_nGwClient_World_UpdateEnterQueueInfo, &KPacketGate_Gw::Process_GwClient_World_UpdateEnterQueueInfo);	
	
	this->Register(s_nGwClient_Character_Connected, &KPacketGate_Gw::Process_GwClient_Character_Connected);
	this->Register(s_nGwClient_Character_AskClose, &KPacketGate_Gw::Process_GwClient_Character_AskClose);
	this->Register(s_nGwClient_Character_Closed, &KPacketGate_Gw::Process_GwClient_Character_Closed);
	this->Register(s_nGwClient_Character_Forward, &KPacketGate_Gw::Process_GwClient_Character_Forward);
	
	this->Register(s_nGwClient_Game_Connected, &KPacketGate_Gw::Process_GwClient_Game_Connected);
	this->Register(s_nGwClient_Game_AskClose, &KPacketGate_Gw::Process_GwClient_Game_AskClose);
	this->Register(s_nGwClient_Game_Closed, &KPacketGate_Gw::Process_GwClient_Game_Closed);
	this->Register(s_nGwClient_Game_Forward, &KPacketGate_Gw::Process_GwClient_Game_Forward);
	
	this->Register(s_nGwClient_Chat_Connected, &KPacketGate_Gw::Process_GwClient_Chat_Connected);
	this->Register(s_nGwClient_Chat_AskClose, &KPacketGate_Gw::Process_GwClient_Chat_AskClose);
	this->Register(s_nGwClient_Chat_Closed, &KPacketGate_Gw::Process_GwClient_Chat_Closed);
	this->Register(s_nGwClient_Chat_Forward, &KPacketGate_Gw::Process_GwClient_Chat_Forward);
}

KPacketGate_Gw::~KPacketGate_Gw()
{

}

BOOL KPacketGate_Gw::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_Gw::Process(KGatewaySocket* pSock, const void* pPacket)
{
	JgPacketHead* pHead = (JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	int serverFlag = command >> max_cmd_bits;
	switch(serverFlag)
	{
	case enum_character_server_flag:
		pHead->command = command & ((1<<max_cmd_bits) - 1);
		this->Process_GwClient_Character_Forward(pSock, pPacket, JgPacketDeclare::GetLength(pHead));
		return;
	case enum_game_server_flag:
		pHead->command = command & ((1<<max_cmd_bits) - 1);
		this->Process_GwClient_Game_Forward(pSock, pPacket, JgPacketDeclare::GetLength(pHead));
		return;
	case enum_chat_server_flag:
		pHead->command = command & ((1<<max_cmd_bits) - 1);
		this->Process_GwClient_Chat_Forward(pSock, pPacket, JgPacketDeclare::GetLength(pHead));
		return;
	}

	if(command < CommandBegin || command >= CommandEnd)
	{
		Log(LOG_ERROR, "error: unexpected gw->Client packet cmd:%u length:%u", command, dataLength);
		return;
	}

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method)
	{
		Log(LOG_ERROR, "error: no proc found for gw->client packet cmd:%u length:%u", command, dataLength);
		return;
	}

	(this->*method)(pSock, pData, dataLength);
}

void KPacketGate_Gw::Process_GwClient_Connect(KGatewaySocket* pSock, const void* pData, int len)
{
	CCLog("Connect to Gateway:%s", pSock->ToString().c_str());
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pGatewaySocket && cgl->m_pGatewaySocket != pSock){
		cgl->m_pGatewaySocket->ReleaseRef();
	}
	//pSock->AddRef(); cgl->m_pGatewaySocket = pSock;

	//KDynamicWorld::getSingleton().SendGatewayMsg(LOGIC_GW_CONNECTED,0,0);
	GameRoot::getSingleton().BattleCtrl().m_bNetReady = true;
}

void KPacketGate_Gw::Process_GwClient_Close(KGatewaySocket* pSock, const void* pData, int len)
{
	GameRoot::getSingleton().BattleCtrl().onNetClose();

	CCLog("Disconnected from gateway:%s", pSock->ToString().c_str());

	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pGatewaySocket == pSock)
	{
		if(pSock->m_c2world) pSock->m_c2world->Close();
		if(pSock->m_c2char) pSock->m_c2char->Close();
		if(pSock->m_c2game) pSock->m_c2game->Close();
		if(pSock->m_c2chat) pSock->m_c2chat->Close();
		pSock->ReleaseRef(); cgl->m_pGatewaySocket = NULL;
	}

	int n = cgl->m_arrGwSockets.size();
	for(int i=0; i<n; i++)
	{
		KGatewaySocket* sock = cgl->m_arrGwSockets.at(i);
		if(sock == pSock)
		{
			cgl->m_arrGwSockets.erase(i);
			break;
		}
	}
}

void KPacketGate_Gw::Process_GwClient_ConnError(KGatewaySocket* pSock, const void* pData, int len)
{
	DWORD errCode = *(DWORD*)pData;
	CCLog("Connect to gateway error, %u:%s", errCode, strerror_r2(errCode).c_str());
	
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pGatewaySocket == pSock)
	{
		pSock->ReleaseRef();
		cgl->m_pGatewaySocket = NULL;
	}
	int n = cgl->m_arrGwSockets.size();
	for(int i=0; i<n; i++)
	{
		KGatewaySocket* sock = cgl->m_arrGwSockets.at(i);
		if(sock == pSock)
		{
			cgl->m_arrGwSockets.erase(i);
			break;
		}
	}
}

void KPacketGate_Gw::Process_GwClient_ConnAccepted(KGatewaySocket* pSock, const void* pData, int len)
{
	CCLog("warn: GwClient_ConnAccepted %s", pSock->ToString().c_str());

	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	if(cgl->m_pGatewaySocket) return;

	ASSERT(!pSock->m_c2world);


	pSock->AddRef(); cgl->m_pGatewaySocket = pSock;

	int n = cgl->m_arrGwSockets.size();
	for(int i=n-1; i>=0; i--)
	{
		KGatewaySocket* sock = cgl->m_arrGwSockets.at(i);
		if(sock != pSock) sock->Close();
	}

	KDynamicWorld::getSingleton().SendGatewayMsg(LOGIC_GW_ACCEPTED,0,0);
}

void KPacketGate_Gw::Process_GwClient_World_Connected(KGatewaySocket* pSock, const void* pData, int len)
{
	CCLog("Connect world %s",pSock->ToString().c_str());
	KWorldSocket* cnn = pSock->m_c2world;
	if(cnn && cnn->IsReady())
	{
		Log(LOG_ERROR, "error: GwClient_World_Connected worldSocket %s already exists", cnn->ToString().c_str());
		pSock->Close();
		return;
	}

	if(!cnn)
	{
		cnn = new KWorldSocket(); cnn->Reset();
		pSock->setWorldCnn(cnn);

		KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
		cgl->m_vsocketManager.attach(cnn);

		Log(LOG_DEBUG, "debug: GwClient_World_Connected alloc %s", cnn->ToString().c_str());
	}
	
	cnn->OnConnected();
}

void KPacketGate_Gw::Process_GwClient_World_AskClose(KGatewaySocket* pSock, const void* pData, int len)
{
	// no such packet
}

void KPacketGate_Gw::Process_GwClient_World_Closed(KGatewaySocket* pSock, const void* pData, int len)
{
	// no such packet
}

void KPacketGate_Gw::Process_GwClient_World_OtpCode_Result(KGatewaySocket* pSock, const void* pData, int len)
{
	KWorldSocket* cnn = pSock->m_c2world;
	if(!cnn)
	{
		pSock->Close();
		return;
	}

	GwClient_World_OtpCode_Result* rsp = (GwClient_World_OtpCode_Result*)pData;
	WC_OtpCode_Result packet; packet.result = rsp->result;
	cnn->PostPacket(s_nWC_OtpCode_Result, &packet, sizeof(packet));
}

void KPacketGate_Gw::Process_GwClient_World_UpdateEnterQueueInfo(KGatewaySocket* pSock, const void* pData, int len)
{
	KWorldSocket* cnn = pSock->m_c2world;
	if(!cnn) return;

	GwClient_World_UpdateEnterQueueInfo* rsp = (GwClient_World_UpdateEnterQueueInfo*)pData;
	WC_UpdateEnterQueueInfo packet;
	packet.nPosition = rsp->nPosition;
	packet.fSpeed = rsp->fSpeed;
	packet.byIsLogin = rsp->byIsLogin;
	cnn->PostPacket(s_nWC_UpdateEnterQueueInfo, &packet, sizeof(packet));
}

void KPacketGate_Gw::Process_GwClient_Character_Connected(KGatewaySocket* pSock, const void* pData, int len)
{
	CCLog("Connect character ser:%s!",pSock->ToString().c_str());
	KCharacterSocket* cnn = pSock->m_c2char;
	if(cnn && cnn->IsReady())
	{
		Log(LOG_ERROR, "GwClient_Character_Connected: %s already exists", cnn->ToString().c_str());
		pSock->Close();
		return;
	}

	if(!cnn)
	{
		cnn = new KCharacterSocket(); cnn->Reset();
		pSock->setCharCnn(cnn);

		Log(LOG_DEBUG, "debug: alloc %s", cnn->ToString().c_str());

		KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
		cgl->m_vsocketManager.attach(cnn);
	}

	cnn->OnConnected();
}

void KPacketGate_Gw::Process_GwClient_Character_AskClose(KGatewaySocket* pSock, const void* pData, int len)
{
	KCharacterSocket* cnn = pSock->m_c2char;
	if(!cnn) return;
	cnn->PeerClose();
}

void KPacketGate_Gw::Process_GwClient_Character_Closed(KGatewaySocket* pSock, const void* pData, int len)
{
	KCharacterSocket* cnn = pSock->m_c2char;
	if(!cnn) return;
	cnn->OnClosed();
}

void KPacketGate_Gw::Process_GwClient_Character_Forward(KGatewaySocket* pSock, const void* pData, int len)
{
	KCharacterSocket* cnn = pSock->m_c2char;
	if(!cnn) return;
	cnn->OnAppPacket(pData, len, cnn->GetRemoteAddress());
}

void KPacketGate_Gw::Process_GwClient_Game_Connected(KGatewaySocket* pSock, const void* pData, int len)
{
	KGameSocket* cnn = pSock->m_c2game;
	if(cnn && cnn->IsReady())
	{
		Log(LOG_ERROR, "GwClient_Game_Connected: %s already exists", cnn->ToString().c_str());
		pSock->Close();
		return;
	}
	
	if(!cnn)
	{
		cnn = new KGameSocket();
		pSock->setGameCnn(cnn);

		Log(LOG_DEBUG, "debug: alloc %s", cnn->ToString().c_str());

		KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
		cgl->m_vsocketManager.attach(cnn);
	}
	CCLog("Connect Game server");
	cnn->OnConnected();
}

void KPacketGate_Gw::Process_GwClient_Game_AskClose(KGatewaySocket* pSock, const void* pData, int len)
{
	KGameSocket* cnn = pSock->m_c2game;
	if(!cnn) return;
	if(cnn->m_closeTime) return;
	cnn->PeerClose();
}

void KPacketGate_Gw::Process_GwClient_Game_Closed(KGatewaySocket* pSock, const void* pData, int len)
{
	KGameSocket* cnn = pSock->m_c2game;
	if(!cnn) return;
	cnn->OnClosed();
	CCLog("Disconnect Game server");
}

void KPacketGate_Gw::Process_GwClient_Game_Forward(KGatewaySocket* pSock, const void* pData, int len)
{
	KGameSocket* cnn = pSock->m_c2game;
	if(!cnn) return;
	cnn->OnAppPacket(pData, len, cnn->GetRemoteAddress());
}

void KPacketGate_Gw::Process_GwClient_Chat_Connected(KGatewaySocket* pSock, const void* pData, int len)
{
	KChatSocket* cnn = pSock->m_c2chat;
	if(cnn && cnn->IsReady())
	{
		Log(LOG_ERROR, "GwClient_Chat_Connected: %s already exists", cnn->ToString().c_str());
		pSock->Close();
		return;
	}

	if(!cnn)
	{
		cnn = new KChatSocket();
		pSock->setChatCnn(cnn);

		Log(LOG_DEBUG, "debug: alloc %s", cnn->ToString().c_str());

		KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
		cgl->m_vsocketManager.attach(cnn);
	}

	cnn->OnConnected();
}

void KPacketGate_Gw::Process_GwClient_Chat_AskClose(KGatewaySocket* pSock, const void* pData, int len)
{
	KChatSocket* cnn = pSock->m_c2chat;
	if(!cnn) return;
	if(cnn->m_closeTime) return;
	cnn->PeerClose();
}

void KPacketGate_Gw::Process_GwClient_Chat_Closed(KGatewaySocket* pSock, const void* pData, int len)
{
	KChatSocket* cnn = pSock->m_c2chat;
	if(!cnn) return;
	cnn->OnClosed();
}

void KPacketGate_Gw::Process_GwClient_Chat_Forward(KGatewaySocket* pSock, const void* pData, int len)
{
	KChatSocket* cnn = pSock->m_c2chat;
	if(!cnn) return;
	cnn->OnAppPacket(pData, len, cnn->GetRemoteAddress());
}
