#ifndef _K_SOCKET_DEFINE_H_
#define _K_SOCKET_DEFINE_H_

#include <SockServer/JgPacketSpecific.h>
#include "Inc/PacketGate/c_chr_packet.h"
#include "Inc/PacketGate/c_game_packet.h"
//#include <PacketGate/>
//#include <PacketGate/KChatPacket.h>
//#include <PacketGate/KPayPacket.h>
#include <System/Memory/KStepObjectPool.h>
//#include <KLogicWorldShell.h>
//#include <Security/JgAssist.h>
#include <System/Misc/rand.h>
#include "CommonLogic/Socket/JgVirtualSocket.h"

enum Enum_Server_Type
{
	enum_Server_Unknow = -1,

	enum_Server_Login_Server = 0,
	enum_Server_Character_Server,
	enum_Server_Game_Server,
	enum_Server_PerfLog_Server,		//客户端性能日志由GameServer转发，有独立的socket  add by jiangxiaoxing
	enum_Server_Chat_Server,
	enum_Server_Character_Server2,	// 只接受ping包

	enum_Server_World_Server,
	enum_Server_Gateway_Server,

	enum_Server_Type_Count,
};

class KLoginSocket
	: public JgClientTcpSocket
	, public System::Memory::KPortableStepPool<KLoginSocket,8,JG_S::KMTLock>
{
public:
	KLoginSocket();
	~KLoginSocket();

public:
	BOOL _Build(JgPacketBuffer* pBuffer, int cmd, const void* pData, int len);

public:
	BOOL OnConnectError(DWORD errCode);
	BOOL OnConnected();
	BOOL OnClosed();
	void Reset();
	void Destroy();

public:
	volatile BOOL m_cnnReady;
};

class KGatewaySocket;
class KWorldSocket : public JgVirtualSocket
{
public:
	KWorldSocket();
	~KWorldSocket();

public:
	BOOL IsReady() const;
	void Reset();
	void Destroy() { this->Reset(); delete this; }

	//BOOL Connect(const KSocketAddress& addr);
	BOOL Close();

	BOOL OnConnected();
	BOOL OnConnectError(DWORD errCode);
	BOOL OnClosed();

	void Breathe(int msec);

	BOOL Build(JgPacketBuffer* pPacketBuffer, int cmd, const void* pData, int len);

	BOOL Send(int cmd, const void* data, int len);
	BOOL SendImmediate(int cmd, const void* data, int len);
	SocketDesc ToString() const;

	const KSocketAddress& GetRemoteAddress() const { return m_remoteAddr; }

public:
	BOOL m_shouldRemove;
	BOOL m_recnnOnClose;
	KGatewaySocket* m_pGatewaySocket;
	KSocketAddress m_remoteAddr;
};

class KCharacterSocket : public JgVirtualSocket
{
public:
	KCharacterSocket();
	~KCharacterSocket();

public:
	BOOL IsReady() const;
	void Reset();
	void Destroy() { this->Reset(); delete this; }

	//BOOL Connect(const KSocketAddress& addr);
	BOOL Close();

	BOOL OnConnected();
	BOOL OnConnectError(DWORD errCode);
	BOOL OnClosed();

	void Breathe(int msec);
	
	BOOL Build(JgPacketBuffer* pPacketBuffer, int cmd, const void* pData, int len);

	BOOL Send(int cmd, const void* data, int len);
	BOOL SendImmediate(int cmd, const void* data, int len);
	SocketDesc ToString() const;

	const KSocketAddress& GetRemoteAddress() const { return m_remoteAddr; }
	
public:
	BOOL m_shouldRemove;
	BOOL m_recnnOnClose;
	KGatewaySocket* m_pGatewaySocket;
	KSocketAddress m_remoteAddr;
};

// 需要在多线程中被访问
class KGameSocket
	: public JgVirtualSocket
{
public:
	KGameSocket();
	~KGameSocket();

public:
	int AddRef();
	int ReleaseRef();

public:
	BOOL IsReady() const;
	void Reset();
	void Destroy() { this->Reset(); delete this; }

	//BOOL Connect(const KSocketAddress& addr);
	BOOL Close();

	BOOL OnConnected();
	BOOL OnConnectError(DWORD errCode);
	BOOL OnClosed();

	void Breathe(int msec);

	BOOL Build(JgPacketBuffer* pPacketBuffer, int cmd, const void* pData, int len);

	BOOL Send(int cmd, const void* data, int len);
	BOOL SendImmediate(int cmd, const void* data, int len);
	SocketDesc ToString() const;

	const KSocketAddress& GetRemoteAddress() const { return m_remoteAddr; }

public: // ISocketConnection
	BOOL SendPacket(int cmd, const void* pData, int len) { return this->Send(cmd, pData, len); }
	void AddRef_2()     { this->AddRef(); }
	void ReleaseRef_2() { this->ReleaseRef(); }

public:
	JG_S::KThreadMutex m_mx;
	BOOL m_shouldRemove;
	BOOL m_recnnOnClose;
	KGatewaySocket* m_pGatewaySocket;
	BOOL m_clientRelease;
	KSocketAddress m_remoteAddr;

public:
	BOOL m_ping;
	DWORD m_lastPingTick;
	UINT64 m_recvBytes;
	UINT64 m_lastPingRecv;
	//KSocketPingStatistic m_pingStatistic;

public:
	void sendPing();
};

class KChatSocket : public JgVirtualSocket
{
public:
	KChatSocket();
	~KChatSocket();

public:
	BOOL IsReady() const;
	void Reset();
	void Destroy() { this->Reset(); delete this; }

	//BOOL Connect(const KSocketAddress& addr);
	BOOL Close();

	BOOL OnConnected();
	BOOL OnConnectError(DWORD errCode);
	BOOL OnClosed();

	void Breathe(int msec);

	BOOL Build(JgPacketBuffer* pPacketBuffer, int cmd, const void* pData, int len);

	BOOL Send(int cmd, const void* data, int len);
	BOOL SendImmediate(int cmd, const void* data, int len);
	SocketDesc ToString() const;

	const KSocketAddress& GetRemoteAddress() const { return m_remoteAddr; }

public:
	BOOL m_shouldRemove;
	BOOL m_recnnOnClose;
	KGatewaySocket* m_pGatewaySocket;
	KSocketAddress m_remoteAddr;
};

class KGatewaySocket : public JgClientTcpSocket
{
public:
	KGatewaySocket();
	~KGatewaySocket();

public:
	BOOL OnConnectError(DWORD errCode);
	BOOL OnConnected();
	BOOL OnClosed();
	void Reset();
	void Destroy();

public:
	BOOL isCharReady() const { return m_c2char && m_c2char->m_cnnReady; }
	BOOL isGameReady() const { return m_c2game && m_c2game->m_cnnReady; }
	BOOL isChatReady() const { return m_c2chat && m_c2chat->m_cnnReady; }

public:
	void setWorldCnn(KWorldSocket* cnn);
	void setCharCnn(KCharacterSocket* cnn);
	void setGameCnn(KGameSocket* cnn);
	void setChatCnn(KChatSocket* cnn);

public:
	volatile BOOL m_cnnReady;
	KWorldSocket* m_c2world;
	KCharacterSocket* m_c2char;
	KGameSocket* m_c2game;
	KChatSocket* m_c2chat;
	Enum_Server_Type m_targetOnConnected;
};

#endif
