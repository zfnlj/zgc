#include "KSocketDefine.h"
#include "Inc/PacketGate/gateway_packet.h"
#include "KClientGateListener.h"
#include <System/Collections/DynArray.h>
#include <System/Sync/KSync.h>

using namespace System::Collections;
using namespace System::Sync;

static time_t g_startTime = time(NULL);
static DWORD  g_startTick = GetTickCount();

static JG_C::KString<64> tickToTime(DWORD tick)
{
	char buf[64];
	int ticks = tick - g_startTick;
	time_t t = g_startTime + ticks/1000;

	KDatetime dt(t);
	sprintf_k(buf, sizeof(buf), "%s %d", dt.ToString().c_str(), ticks%1000);

	return buf;
}

KLoginSocket::KLoginSocket()
{
	m_cnnReady = FALSE;
	this->SetUserData(enum_Server_Login_Server);
	this->SetSendBufferSize(1024);
	this->SetRecvBuffSize(1024);
	this->DisableReconnect();
}

KLoginSocket::~KLoginSocket()
{

}

// 客户端发出的所有包都带有Checksum字段
BOOL KLoginSocket::_Build(JgPacketBuffer* pBuffer, int cmd, const void* pData, int len)
{
	JgPacketHead head;

	head.flag = 1;
	head.length = len;
	head.command = cmd;

	pBuffer->m_buildBuffer.Write(&head, sizeof(JgPacketHead));
	if(len)
	{
		int nBytes = pBuffer->m_buildBuffer.Write(pData, len);
		if(nBytes != len) return FALSE;
	}

	JgPacketTail tail;
	tail.length = 0;
	tail.command = 0;
	tail.checksum = JgPacketDeclare::CalcChecksum(pData, len);

	return pBuffer->m_buildBuffer.Write(&tail, sizeof(JgPacketTail)) == sizeof(JgPacketTail);
}

BOOL KLoginSocket::OnConnectError(DWORD errCode)
{
	this->PostPacket(s_nLC_ConnError, NULL, 0);
	return TRUE;
}

BOOL KLoginSocket::OnConnected()
{
	m_cnnReady = TRUE;
	this->PostPacket(s_nLC_Connected, NULL, 0);
	return TRUE;
}

BOOL KLoginSocket::OnClosed()
{
	m_cnnReady = FALSE;
	this->PostFinalPacket(s_nLC_ConnClosed, NULL, 0);
	return TRUE;
}

void KLoginSocket::Reset()
{
	m_cnnReady = FALSE;
	JgClientTcpSocket::Reset();
}

void KLoginSocket::Destroy()
{
	this->Reset();
	KLoginSocket::Free(this);
}

KWorldSocket::KWorldSocket()
	: m_shouldRemove(FALSE)
	, m_recnnOnClose(FALSE)
	, m_pGatewaySocket(NULL)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	this->SetUserData(enum_Server_World_Server);
	this->SetRecvBufferPool(cgl->m_sockServer.m_recvPool);
	this->SetRecvBuffSize(1024);
}

KWorldSocket::~KWorldSocket()
{

}

BOOL KWorldSocket::IsReady() const
{
	return m_cnnReady && !m_closeTime && m_pGatewaySocket;
}

void KWorldSocket::Reset()
{
	m_shouldRemove = FALSE;
	m_recnnOnClose = FALSE;
	if(m_pGatewaySocket) m_pGatewaySocket->setWorldCnn(NULL);
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
	JgVirtualSocket::Reset();
}

BOOL KWorldSocket::Close()
{
	if(m_closeTime) return TRUE;

	if(!m_cnnReady)
		return this->OnConnectError(m_closeReason);

	m_closeTime = time(NULL);
	m_closeRemainMs = JgVirtualSocket::close_timeout;
	
	m_pGatewaySocket->Close();
	this->OnClosed();
	
	return TRUE;	
}

BOOL KWorldSocket::OnConnected()
{
	if(!JgVirtualSocket::OnConnected()) return FALSE;
	this->PostPacket(s_nWC_Connected, NULL, 0);
	return TRUE;
}

BOOL KWorldSocket::OnConnectError(DWORD errCode)
{
	this->PostPacket(s_nWC_ConnError, NULL, 0);
	m_pGatewaySocket->Close();
	return TRUE;
}

BOOL KWorldSocket::OnClosed()
{
	if(!JgVirtualSocket::OnClosed()) return FALSE;
	this->PostFinalPacket(s_nWC_Closed, NULL, 0);
	return TRUE;
}

void KWorldSocket::Breathe(int msec)
{
	JgVirtualSocket::Breathe(msec);

	void* packetPtrs[64];
	char packetBuffer[10240];
	DynArray<void*> packets;
	packets.attach(&packetPtrs[0], 64, 0);

	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();

	while(!m_shouldRemove)
	{
		int_r packetNum = this->ReadPackets(packetBuffer, sizeof(packetBuffer), packets, 64);
		if(packetNum < 1) break;
		for(int_r i=0; i<packetNum; i++)
		{
			JgPacketHead* head = (JgPacketHead*)packets[(int)i];
			cgl->m_gateWC.Process(this, head);
			if(m_shouldRemove) break;
		}
	}

	if(m_shouldRemove)
	{
		cgl->m_vsocketManager.detach(this);
		Log(LOG_DEBUG, "debug: free %s", this->ToString().c_str());
		this->Destroy();
	}
}

BOOL KWorldSocket::Build(JgPacketBuffer* pPacketBuffer, int cmd, const void* pData, int len)
{
	pPacketBuffer->Reset();
	return this->_Build(pPacketBuffer, cmd, pData, len);
}

BOOL KWorldSocket::Send(int cmd, const void* data, int len)
{
	ASSERT(FALSE);
	return TRUE;
}

BOOL KWorldSocket::SendImmediate(int cmd, const void* data, int len)
{
	ASSERT(FALSE);
	return TRUE;
}

KWorldSocket::SocketDesc KWorldSocket::ToString() const
{
	SocketDesc s;
	s.Printf("KWorldSocket 0x%p", this);
	return s;
}

/////

KCharacterSocket::KCharacterSocket()
	: m_shouldRemove(FALSE)
	, m_recnnOnClose(FALSE)
	, m_pGatewaySocket(NULL)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	this->SetRecvBufferPool(cgl->m_sockServer.m_recvPool);
	this->SetRecvBuffSize(1024);
	this->SetUserData(enum_Server_Character_Server);
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
}

KCharacterSocket::~KCharacterSocket()
{
	
}

BOOL KCharacterSocket::IsReady() const
{
	return m_pGatewaySocket && m_cnnReady && !m_closeTime;
}

void KCharacterSocket::Reset()
{
	m_shouldRemove = FALSE;
	m_recnnOnClose = FALSE;
	if(m_pGatewaySocket) m_pGatewaySocket->setCharCnn(NULL);
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
	JgVirtualSocket::Reset();
}

BOOL KCharacterSocket::Close()
{
	if(m_closeTime) return TRUE;

	if(!m_cnnReady)
		return this->OnConnectError(m_closeReason);

	m_closeTime = time(NULL);
	m_closeRemainMs = JgVirtualSocket::close_timeout;

	if(!m_peerClose)
	{
		ClientGw_Character_AskClose askClose;
		askClose.reason = 0;
		m_pGatewaySocket->Send(s_nClientGw_Character_AskClose, NULL, 0);
	}

	this->OnClosed();

	return TRUE;
}

BOOL KCharacterSocket::OnConnected()
{
	if(!JgVirtualSocket::OnConnected()) return FALSE;
	this->PostPacket(s_nChrC_Connected, NULL, 0);
	return TRUE;
}

BOOL KCharacterSocket::OnConnectError(DWORD errCode)
{
	this->PostPacket(s_nChrC_ConnError, &errCode, sizeof(errCode));
	return TRUE;
}

BOOL KCharacterSocket::OnClosed()
{
	if(!JgVirtualSocket::OnClosed()) return FALSE;
	
	this->PostFinalPacket(s_nChrC_ConnClosed, NULL, 0);
	m_pGatewaySocket->Send(s_nGwClient_Character_Closed, NULL, 0);
	
	return TRUE;
}

void KCharacterSocket::Breathe(int msec)
{
	JgVirtualSocket::Breathe(msec);

	void* packetPtrs[64];
	char packetBuffer[10240];
	DynArray<void*> packets;
	packets.attach(&packetPtrs[0], 64, 0);

	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();

	while(!m_shouldRemove)
	{
		int_r packetNum = this->ReadPackets(packetBuffer, sizeof(packetBuffer), packets, 64);
		if(packetNum < 1) break;
		for(int_r i=0; i<packetNum; i++)
		{
			JgPacketHead* head = (JgPacketHead*)packets[(int)i];
			cgl->m_gateCharater.Process(this, head);
			if(m_shouldRemove) break;
		}
	}

	if(m_shouldRemove)
	{
		cgl->m_vsocketManager.detach(this);
		Log(LOG_DEBUG, "debug: free %s", this->ToString().c_str());
		this->Destroy();
	}
}

BOOL KCharacterSocket::Build(JgPacketBuffer* pBuffer, int cmd, const void* pData, int len)
{
	pBuffer->Reset();
	
	//return this->_Build(pPacketBuffer, cmd, pData, len);
	JgPacketHead head;

	head.flag = 1;
	head.length = len;
	head.command = cmd;

	pBuffer->m_buildBuffer.Write(&head, sizeof(JgPacketHead));
	if(len)
	{
		int nBytes = pBuffer->m_buildBuffer.Write(pData, len);
		if(nBytes != len) return FALSE;
	}

	JgPacketTail tail;
	tail.length = 0;
	tail.command = 0;
	tail.checksum = JgPacketDeclare::CalcChecksum(pData, len);

	return pBuffer->m_buildBuffer.Write(&tail, sizeof(JgPacketTail)) == sizeof(JgPacketTail);
}

BOOL KCharacterSocket::Send(int cmd, const void* pData, int len)
{
	if(!this->IsReady()) return FALSE;

	JgSockServer* pSockServer = (JgSockServer*)m_pGatewaySocket->GetSockServer();

	int packetLen;
	const void* pPacket;
	if(!pSockServer->BuildPacket(this, cmd, pData, len, &pPacket, &packetLen))
	{
		Log(LOG_WARN, "warn: build cmd:%u len:%u sock:%s", cmd, len, this->ToString().c_str());
		return FALSE;
	}

	char buf[10240];
	KDataOutputStream so(buf, sizeof(buf));
	so.WriteByteArray(pPacket, packetLen);

	return m_pGatewaySocket->Send(s_nClientGw_Character_Forward, so.data(), so.size());
}

BOOL KCharacterSocket::SendImmediate(int cmd, const void* data, int len)
{
	return this->Send(cmd, data, len);
}

KCharacterSocket::SocketDesc KCharacterSocket::ToString() const
{
	SocketDesc s;
	s.Printf("KCharacterSocket 0x%p", this);
	return s;
}

////

KGameSocket::KGameSocket()
	: m_shouldRemove(FALSE)
	, m_recnnOnClose(FALSE)
	, m_pGatewaySocket(NULL)
	, m_clientRelease(FALSE)
	, m_ping(FALSE)
	, m_lastPingTick(0)
	, m_recvBytes(0)
	, m_lastPingRecv(0)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	this->SetRecvBufferPool(cgl->m_sockServer.m_recvPool);
	this->SetRecvBuffSize(100*1024);
	this->SetUserData(enum_Server_Game_Server);
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
}

KGameSocket::~KGameSocket()
{

}

int KGameSocket::AddRef()
{
	KAutoThreadMutex mx(m_mx);
	return ++m_refCount;
}

int KGameSocket::ReleaseRef()
{
	KAutoThreadMutex mx(m_mx);
	return --m_refCount;
}

BOOL KGameSocket::IsReady() const
{
	return m_pGatewaySocket && m_cnnReady && !m_closeTime;
}

void KGameSocket::Reset()
{
	m_shouldRemove = FALSE;
	m_recnnOnClose = FALSE;
	m_clientRelease = FALSE;

	m_ping = FALSE;
	m_lastPingTick = 0;
	m_recvBytes = 0;
	m_lastPingRecv = 0;

	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
	if(m_pGatewaySocket) m_pGatewaySocket->setGameCnn(NULL);
	JgVirtualSocket::Reset();
}

BOOL KGameSocket::Close()
{
	if(m_closeTime) return TRUE;
	if(!m_cnnReady)
		return this->OnConnectError(m_closeReason);
	
	if(!m_peerClose)
	{
		ClientGw_Game_AskClose askClose;
		askClose.reason = 0;
		m_pGatewaySocket->Send(s_nClientGw_Game_AskClose, &askClose, sizeof(askClose));
	}

	m_closeTime = time(NULL);
	m_closeRemainMs = JgVirtualSocket::close_timeout;

	this->OnClosed();

	return TRUE;
}

BOOL KGameSocket::OnConnected()
{
	if(!JgVirtualSocket::OnConnected()) return FALSE;
	
	m_recnnOnClose = FALSE;
	m_ping = FALSE;
	m_lastPingTick = 0;
	m_recvBytes = 0;
	m_lastPingRecv = 0;

	this->PostPacket(s_nSC_Connected, NULL, 0);
	return TRUE;
}

BOOL KGameSocket::OnConnectError(DWORD errCode)
{
	this->PostPacket(s_nSC_ConnError, &errCode, sizeof(errCode));
	return TRUE;
}

BOOL KGameSocket::OnClosed()
{
	if(!JgVirtualSocket::OnClosed()) return FALSE;
	this->PostFinalPacket(s_nSC_ConnClosed, NULL, 0);
	m_pGatewaySocket->Send(s_nClientGw_Game_Closed, NULL, 0);
	return TRUE;
}

void KGameSocket::Breathe(int msec)
{
	JgVirtualSocket::Breathe(msec);
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();

	if(!m_shouldRemove)
	{
		void* packetPtrs[64];
		char packetBuffer[10240];
		DynArray<void*> packets;
		packets.attach(&packetPtrs[0], 64, 0);

		while(!m_shouldRemove)
		{
			int_r packetNum = this->ReadPackets(packetBuffer, sizeof(packetBuffer), packets, 64);
			if(packetNum < 1) break;
			for(int_r i=0; i<packetNum; i++)
			{
				JgPacketHead* head = (JgPacketHead*)packets[(int)i];
				cgl->m_gateGame.Process(this, head);
				if(m_shouldRemove)
				{
					this->ClearReceiver();
					m_pGatewaySocket->setGameCnn(NULL);
					break;
				}
			}
		}

		sendPing();
	}
	else
	{
		if(this->AddRef() == 2)
		{
			cgl->m_vsocketManager.detach(this);
			Log(LOG_DEBUG, "debug: free %s", this->ToString().c_str());
			this->Destroy();
		}
		else
		{
			this->ReleaseRef();
		}
	}
}

BOOL KGameSocket::Build(JgPacketBuffer* pBuffer, int cmd, const void* pData, int len)
{
	pBuffer->Reset();

	JgPacketHead* pHead = (JgPacketHead*)pBuffer->m_buildBuffer.GetWriteBuffer();
	pHead->flag = 0;
	pHead->command = cmd;
	pHead->length = len;

	if(len > 0)
	{
		ASSERT(pData);
		memcpy(pHead+1, pData, len);
	}

	//JgPacketTail* pTail = (JgPacketTail*)((char*)(pHead+1)+len);
	//pTail->command = 0;
	//pTail->length = 0;
	//pTail->checksum = JgPacketDeclare::CalcChecksum(pHead+1, len);

	//KClientCipher& cipher = KClientCipher::GetInstance();
	//cipher.Encrypt(pHead);

	ASSERT(pHead->length == len);
	ASSERT(pHead->command == cmd);

	// 加密可能会改变Tail的长度，从而改变整个包长度
	pBuffer->m_buildBuffer.m_wp = JgPacketDeclare::GetLength(pHead);
	return TRUE;
}

BOOL KGameSocket::Send(int cmd, const void* data, int len)
{
	KAutoThreadMutex mx(m_mx);
	if(!this->IsReady()) return FALSE;

	JgSockServer* pSockServer = (JgSockServer*)m_pGatewaySocket->GetSockServer();
	
	int packetLen;
	const void* pPacket;
	if(!pSockServer->BuildPacket(this, cmd, data, len, &pPacket, &packetLen))
	{
		Log(LOG_WARN, "warn: build cmd:%u len:%u sock:%s", cmd, len, this->ToString().c_str());
		return FALSE;
	}

	char buf[10240];
	KDataOutputStream so(buf, sizeof(buf));
	so.WriteByteArray(pPacket, packetLen);

	return m_pGatewaySocket->Send(s_nClientGw_Game_Forward, so.data(), so.size());
}

BOOL KGameSocket::SendImmediate(int cmd, const void* data, int len)
{
	return this->Send(cmd, data, len);
}

KGameSocket::SocketDesc KGameSocket::ToString() const
{
	SocketDesc s;
	s.Printf("KGameSocket 0x%p", this);
	return s;
}

void KGameSocket::sendPing()
{
	if (!m_ping || !this->IsReady()) return;

	DWORD now = GetTickCount();
	if (now < m_lastPingTick + CLIENT_PING_INTERVAL) return;

	static CS_Ping packet;
	packet.localSendTime = now;
	this->SendImmediate(s_nCS_Ping, &packet, sizeof(CS_Ping));

	m_lastPingTick = now;
	m_lastPingRecv = m_recvBytes;
}
///

KChatSocket::KChatSocket()
	: m_shouldRemove(FALSE)
	, m_recnnOnClose(FALSE)
	, m_pGatewaySocket(NULL)
{
	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();
	this->SetRecvBufferPool(cgl->m_sockServer.m_recvPool);
	this->SetRecvBuffSize(1024);
	this->SetUserData(enum_Server_Chat_Server);
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
}

KChatSocket::~KChatSocket()
{

}

BOOL KChatSocket::IsReady() const
{
	return m_pGatewaySocket && m_cnnReady && !m_closeTime;
}

void KChatSocket::Reset()
{
	m_shouldRemove = FALSE;
	m_recnnOnClose = FALSE;
	m_remoteAddr.m_ip = 0;
	m_remoteAddr.m_port = 0;
	if(m_pGatewaySocket) m_pGatewaySocket->setChatCnn(NULL);
	JgVirtualSocket::Reset();
}

BOOL KChatSocket::Close()
{
	if(m_closeTime) return TRUE;

	if(!m_cnnReady)
		return this->OnConnectError(m_closeReason);

	if(!m_peerClose)
	{
		ClientGw_Chat_AskClose askClose;
		askClose.reason = 0;
		m_pGatewaySocket->Send(s_nClientGw_Chat_AskClose, &askClose, sizeof(askClose));
	}

	m_closeTime = time(NULL);
	m_closeRemainMs = JgVirtualSocket::close_timeout;

	this->OnClosed();

	return TRUE;
}

BOOL KChatSocket::OnConnected()
{
	if(!JgVirtualSocket::OnConnected()) return FALSE;
	//this->PostPacket(s_nChatClient_Connected, NULL, 0);
	return TRUE;
}

BOOL KChatSocket::OnConnectError(DWORD errCode)
{
	//this->PostPacket(s_nChatClient_ConnError, &errCode, sizeof(errCode));
	return TRUE;
}

BOOL KChatSocket::OnClosed()
{
	if(!JgVirtualSocket::OnClosed()) return FALSE;
	
	//this->PostFinalPacket(s_nChatClient_ConnClosed, NULL, 0);
	m_pGatewaySocket->Send(s_nClientGw_Chat_Closed, NULL, 0);
	
	return TRUE;
}

void KChatSocket::Breathe(int msec)
{
	JgVirtualSocket::Breathe(msec);

	void* packetPtrs[64];
	char packetBuffer[10240];
	DynArray<void*> packets;
	packets.attach(&packetPtrs[0], 64, 0);

	KClientGateListener* cgl = KClientGateListener::getSingletonPtr();

	while(!m_shouldRemove)
	{
		int_r packetNum = this->ReadPackets(packetBuffer, sizeof(packetBuffer), packets, 64);
		if(packetNum < 1) break;
		for(int_r i=0; i<packetNum; i++)
		{
			JgPacketHead* head = (JgPacketHead*)packets[(int)i];
			//cgl->m_gateChatC.Process(this, head);
			if(m_shouldRemove) break;
		}
	}

	if(m_shouldRemove)
	{
		cgl->m_vsocketManager.detach(this);
		m_pGatewaySocket->setChatCnn(NULL);
		Log(LOG_DEBUG, "debug: free %s", this->ToString().c_str());
		this->Destroy();
	}
}

BOOL KChatSocket::Build(JgPacketBuffer* pBuffer, int cmd, const void* pData, int len)
{
	pBuffer->Reset();
	JgPacketHead head;

	head.flag = 1;
	head.length = len;
	head.command = cmd;

	pBuffer->m_buildBuffer.Write(&head, sizeof(JgPacketHead));
	if(len)
	{
		int nBytes = pBuffer->m_buildBuffer.Write(pData, len);
		if(nBytes != len) return FALSE;
	}

	JgPacketTail tail;
	tail.length = 0;
	tail.command = 0;
	tail.checksum = JgPacketDeclare::CalcChecksum(pData, len);

	return pBuffer->m_buildBuffer.Write(&tail, sizeof(JgPacketTail)) == sizeof(JgPacketTail);
}

BOOL KChatSocket::Send(int cmd, const void* data, int len)
{
	if(!this->IsReady()) return FALSE;

	JgSockServer* pSockServer = (JgSockServer*)m_pGatewaySocket->GetSockServer();
	
	int packetLen;
	const void* pPacket;
	if(!pSockServer->BuildPacket(this, cmd, data, len, &pPacket, &packetLen))
	{
		Log(LOG_WARN, "warn: build cmd:%u len:%u sock:%s", cmd, len, this->ToString().c_str());
		return FALSE;
	}

	char buf[10240];
	KDataOutputStream so(buf, sizeof(buf));
	so.WriteByteArray(pPacket, packetLen);

	return m_pGatewaySocket->Send(s_nClientGw_Chat_Forward, so.data(), so.size());
}

BOOL KChatSocket::SendImmediate(int cmd, const void* data, int len)
{
	return this->Send(cmd, data, len);
}

KChatSocket::SocketDesc KChatSocket::ToString() const
{
	SocketDesc s;
	s.Printf("KChatSocket 0x%p", this);
	return s;
}

///

KGatewaySocket::KGatewaySocket()
{
	this->SetUserData(enum_Server_Gateway_Server);
	this->SetSendBufferSize(100);
	this->SetRecvBuffSize(100*1024);
	this->DisableReconnect();
	m_cnnReady = FALSE;
	m_c2world = NULL;
	m_c2char = NULL;
	m_c2game = NULL;
	m_c2chat = NULL;
	m_targetOnConnected = enum_Server_Unknow;
}

KGatewaySocket::~KGatewaySocket()
{

}

BOOL KGatewaySocket::OnConnectError(DWORD errCode)
{
	this->PostPacket(s_nGwClient_ConnError, &errCode, sizeof(errCode));
	return TRUE;
}

BOOL KGatewaySocket::OnConnected()
{
	m_cnnReady = TRUE;
	this->PostPacket(s_nGwClient_Connect, NULL, 0);
	return TRUE;
}

BOOL KGatewaySocket::OnClosed()
{
	m_cnnReady = FALSE;
	this->PostFinalPacket(s_nGwClient_Close, NULL, 0);
	return TRUE;
}

void KGatewaySocket::Reset()
{
	m_cnnReady = FALSE;
	this->setWorldCnn(NULL);
	this->setCharCnn(NULL);
	this->setGameCnn(NULL);
	this->setChatCnn(NULL);
	m_targetOnConnected = enum_Server_Unknow;
	JgClientTcpSocket::Reset();
}

void KGatewaySocket::Destroy()
{
	this->Reset();
	delete this;
}

void KGatewaySocket::setWorldCnn(KWorldSocket* cnn)
{
	if(cnn)
	{
		ASSERT(!m_c2world);
		cnn->AddRef(); m_c2world = cnn;
		this->AddRef(); cnn->m_pGatewaySocket = this;
	}
	else
	{
		cnn = m_c2world;
		if(cnn)
		{
			cnn->ReleaseRef(); m_c2world = NULL;
			this->ReleaseRef(); cnn->m_pGatewaySocket = NULL;
		}
	}
}

void KGatewaySocket::setCharCnn(KCharacterSocket* cnn)
{
	if(cnn)
	{
		ASSERT(!m_c2char);
		cnn->AddRef(); m_c2char = cnn;
		this->AddRef(); cnn->m_pGatewaySocket = this;
	}
	else
	{
		cnn = m_c2char;
		if(cnn)
		{
			cnn->ReleaseRef(); m_c2char = NULL;
			this->ReleaseRef(); cnn->m_pGatewaySocket = NULL;
		}
	}
}

void KGatewaySocket::setGameCnn(KGameSocket* cnn)
{
	if(cnn)
	{
		ASSERT(!m_c2game);
		cnn->AddRef(); m_c2game = cnn;
		this->AddRef(); cnn->m_pGatewaySocket = this;
	}
	else
	{
		cnn = m_c2game;
		if(cnn)
		{
			cnn->ReleaseRef(); m_c2game = NULL;
			this->ReleaseRef(); cnn->m_pGatewaySocket = NULL;
		}
	}
}

void KGatewaySocket::setChatCnn(KChatSocket* cnn)
{
	if(cnn)
	{
		ASSERT(!m_c2chat);
		cnn->AddRef(); m_c2chat = cnn;
		this->AddRef(); cnn->m_pGatewaySocket = this;
	}
	else
	{
		cnn = m_c2chat;
		if(cnn)
		{
			cnn->ReleaseRef(); m_c2chat = NULL;
			this->ReleaseRef(); cnn->m_pGatewaySocket = NULL;
		}
	}
}
