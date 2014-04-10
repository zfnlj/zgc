#pragma once
#include <System/KMacro.h>
#include "packet_segment.h"
#include "Inc/KCommonDefine.h"
#include "Inc/KCommonStruct.h"
#include "common_packet.h"

enum CW_Protocol
{
	s_nCW_Connected = s_nCW_PacketStart,
	s_nCW_ConnError,
	s_nCW_ConnClosed,

	s_nCW_PacketEnd,
};

enum WC_Protocol
{
	s_nWC_Connected = s_nWC_PacketStart,
	s_nWC_ConnError,
	s_nWC_Closed,

	s_nWC_OtpCode_Result,
	s_nWC_UpdateEnterQueueInfo,

	s_nWC_PacketEnd,
};

enum ClientGw_Protocol
{
	s_nClientGw_Connected = s_nClientGw_PacketStart,
	s_nClientGw_Closed,
	s_nClientGw_ConnError,

	s_nClientGw_PlayerLogin,
	s_nClientGw_PlayerReconnect,

	//s_nClientGw_World_Connect,
	s_nClientGw_World_AskClose,
	s_nClientGw_World_Closed,
	s_nClientGw_World_Forward,

	//s_nClientGw_Character_Connect,
	s_nClientGw_Character_AskClose,
	s_nClientGw_Character_Closed,
	s_nClientGw_Character_Forward,

	//s_nClientGw_Game_Connect,
	s_nClientGw_Game_AskClose,
	s_nClientGw_Game_Closed,
	s_nClientGw_Game_Forward,

	//s_nClientGw_Chat_Connect,
	s_nClientGw_Chat_AskClose,
	s_nClientGw_Chat_Closed,
	s_nClientGw_Chat_Forward,

	s_nClientGw_PacketEnd,
};

enum GwClient_Protocol
{
	s_nGwClient_Connect = s_nGwClient_PacketStart,
	s_nGwClient_Close,
	s_nGwClient_ConnError,

	s_nGwClient_ConnAccepted,

	s_nGwClient_World_Connected,
	s_nGwClient_World_AskClose,
	s_nGwClient_World_Closed,
	s_nGwClient_World_Forward,
	s_nGwClient_World_OtpCode_Result,

	s_nGwClient_Character_Connected,
	s_nGwClient_Character_AskClose,
	s_nGwClient_Character_Closed,
	s_nGwClient_Character_Forward,

	s_nGwClient_Game_Connected,
	s_nGwClient_Game_AskClose,
	s_nGwClient_Game_Closed,
	s_nGwClient_Game_Forward,

	s_nGwClient_Chat_Connected,
	s_nGwClient_Chat_AskClose,
	s_nGwClient_Chat_Closed,
	s_nGwClient_Chat_Forward,

	s_nGwClient_uworld_Connected,
	s_nGwClient_uworld_Closed,

	s_nGwClient_World_UpdateEnterQueueInfo,
	s_nGwClient_PacketEnd,
};

// Server (character, game, chat) to Gateway Server
enum SGw_Protocol
{
	s_nSGw_Connected = s_nSGw_Packet_Start,
	s_nSGw_Closed,

	s_nSGw_World_Login,
	s_nSGw_World_Connected,
	s_nSGw_World_AskClose,
	s_nSGw_World_Closed,
	s_nSGw_World_OtpCode_Result,

	s_nSGw_World_cnn2char,
	s_nSGw_World_cnn2game,
	s_nSGw_World_cnn2chat,
	s_nSGw_World_cnn2uw,

	s_nSGw_World_discnn2char,
	s_nSGw_World_discnn2game,
	s_nSGw_World_discnn2chat,
	s_nSGw_World_discnn2uw,

	s_nSGw_Character_Login,
	s_nSGw_Character_Connected,
	s_nSGw_Character_AskClose,
	s_nSGw_Character_Closed,
	s_nSGw_Character_Forward,

	s_nSGw_Game_Login,
	s_nSGw_Game_Connected,
	s_nSGw_Game_AskClose,
	s_nSGw_Game_Closed,
	s_nSGw_Game_Forward,

	s_nSGw_Chat_Login,
	s_nSGw_Chat_Connected,
	s_nSGw_Chat_AskClose,
	s_nSGw_Chat_Closed,
	s_nSGw_Chat_Forward,

	s_nSGw_uworld_Login,
	s_nSGw_uworld_Connected,
	s_nSGw_uworld_AskClose,
	s_nSGw_uworld_Closed,
	s_nSGw_uworld_cnn2game,
	s_nSGw_uworld_cnn2chat,
	s_nSGw_uworld_discnn2game,
	s_nSGw_uworld_discnn2chat,

	s_nSGw_World_UpdateEnterQueueInfo,
	s_nSGw_Packet_End,
};

// Gateway to Server (character, game, chat)
enum GwS_Protocol
{
	s_nGwS_Connected = s_nGwS_Packet_Start,
	s_nGwS_Closed,
	s_nGwS_ConnError,

	s_nGwS_World_Connect,
	s_nGwS_World_AskClose,
	s_nGwS_World_Closed,
	s_nGwS_World_Forward,
	s_nGwS_World_OtpCode,

	s_nGwS_World_CharConnected,
	s_nGwS_World_GameConnected,
	s_nGwS_World_ChatConnected,
	s_nGwS_World_UwConnected,

	s_nGwS_World_CharClosed,
	s_nGwS_World_GameClosed,
	s_nGwS_World_ChatClosed,
	s_nGwS_World_UwClosed,

	s_nGwS_Character_Connect,
	s_nGwS_Character_AskClose,
	s_nGwS_Character_Closed,
	s_nGwS_Character_Forward,

	s_nGwS_Game_Connect,
	s_nGwS_Game_AskClose,
	s_nGwS_Game_Closed,
	s_nGwS_Game_Forward,

	s_nGwS_Chat_Connect,
	s_nGwS_Chat_AskClose,
	s_nGwS_Chat_Closed,
	s_nGwS_Chat_Forward,

	s_nGwS_uworld_Connect,
	s_nGwS_uworld_AskClose,
	s_nGwS_uworld_Closed,
	s_nGwS_uworld_CharConnected,
	s_nGwS_uworld_GameConnected,
	s_nGwS_uworld_ChatConnected,
	s_nGwS_uworld_CharClosed,
	s_nGwS_uworld_GameClosed,
	s_nGwS_uworld_ChatClosed,

	s_nGwS_Packet_End,
};

#pragma pack(push,1)

struct WC_OtpCode_Result
{
	enum { s_nCode = s_nWC_OtpCode_Result };
	int result;
};

struct WC_UpdateEnterQueueInfo
{
	enum { s_nCode = s_nWC_UpdateEnterQueueInfo };
	INT nPosition;
	FLOAT fSpeed;
	BYTE byIsLogin;
};

struct GwClient_World_UpdateEnterQueueInfo
{
	enum { s_nCode = s_nGwClient_World_UpdateEnterQueueInfo };
	INT nPosition;
	FLOAT fSpeed;
	BYTE byIsLogin;
};

struct SGw_World_UpdateEnterQueueInfo
{
	enum { s_nCode =  s_nSGw_World_UpdateEnterQueueInfo };
	WORD gwpos; WORD token;
	INT nPosition;
	FLOAT fSpeed;
	BYTE byIsLogin;
};

struct ClientGw_Connected
{
	enum { s_nCode = s_nClientGw_Connected };
};

struct ClientGw_Closed
{
	enum { s_nCode = s_nClientGw_Closed };
};

struct ClientGw_ConnError
{
	enum { s_nCode = s_nClientGw_ConnError };
	DWORD errCode;
};

struct ClientGw_PlayerLogin
{
	enum { s_nCode = s_nClientGw_PlayerLogin };
	int language;
	DWORD otpCode;
	KPassport passport;
	DWORD worldPublicIp;
	WORD worldPublicPort;
};

struct ClientGw_PlayerReconnect
{
	enum { s_nCode = s_nClientGw_PlayerReconnect };
	DWORD otpCode;
};

struct ClientGw_World_AskClose
{
	enum { s_nCode = s_nClientGw_World_AskClose };
	int reason;
};

struct ClientGw_World_Closed
{
	enum { s_nCode = s_nClientGw_World_Closed };
};

struct ClientGw_Character_AskClose
{
	enum { s_nCode = s_nClientGw_Character_AskClose };
	int reason;
};

struct ClientGw_Character_Closed
{
	enum { s_nCode = s_nClientGw_Character_Closed };
};

struct ClientGw_Character_Forward
{
	enum { s_nCode = s_nClientGw_Character_Forward };
};

struct ClientGw_Game_AskClose
{
	enum { s_nCode = s_nClientGw_Game_AskClose };
	int reason;
};

struct ClientGw_Game_Closed
{
	enum { s_nCode = s_nClientGw_Game_Closed };
};

struct ClientGw_Game_Forward
{
	enum { s_nCode = s_nClientGw_Game_Forward };
};

struct ClientGw_Chat_AskClose
{
	enum { s_nCode = s_nClientGw_Chat_AskClose };
	int reason;
};

struct ClientGw_Chat_Closed
{
	enum { s_nCode = s_nClientGw_Chat_Closed };
};

struct ClientGw_Chat_Forward
{
	enum { s_nCode = s_nClientGw_Chat_Forward };
};

struct GwClient_World_Connected
{
	enum { s_nCode = s_nGwClient_World_Connected };
};

struct GwClient_World_AskClose
{
	enum { s_nCode = s_nGwClient_World_AskClose };
	int reason;
};

struct GwClient_World_Closed
{
	enum { s_nCode = s_nGwClient_World_Closed };
};

struct GwClient_World_OtpCode_Result
{
	enum { s_nCode = s_nGwClient_World_OtpCode_Result };
	int result;
};

struct GwClient_Character_AskClose
{
	enum { s_nCode = s_nGwClient_Character_AskClose };
	int reason;
};

struct GwClient_Character_Closed
{
	enum { s_nCode = s_nGwClient_Character_Closed };
};

struct GwClient_Character_Forward
{
	enum { s_nCode = s_nGwClient_Character_Forward };
};

struct GwClient_Game_Connected
{
	enum { s_nCode = s_nGwClient_Game_Connected };
};

struct GwClient_Game_AskClose
{
	enum { s_nCode = s_nGwClient_Game_AskClose };
	int reason;
};

struct GwClient_Game_Closed
{
	enum { s_nCode = s_nGwClient_Game_Closed };
};

struct GwClient_Game_Forward
{
	enum { s_nCode = s_nGwClient_Game_Forward };
};

struct GwClient_Chat_AskClose
{
	enum { s_nCode = s_nGwClient_Chat_AskClose };
	int reason;
};

struct GwClient_Chat_Closed
{
	enum { s_nCode = s_nGwClient_Chat_Closed };
};

struct GwClient_Chat_Forward
{
	enum { s_nCode = s_nGwClient_Chat_Forward };
};

struct SGw_World_Login
{
	enum { s_nCode = s_nSGw_World_Login };
	DWORD publicIp;
	WORD publicPort;
};

struct SGw_World_Connected
{
	enum { s_nCode = s_nSGw_World_Connected };
	WORD gwpos; WORD token;
};

struct SGw_World_AskClose
{
	enum { s_nCode = s_nSGw_World_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct SGw_World_Closed
{
	enum { s_nCode = s_nSGw_World_Closed };
	WORD gwpos; WORD token;
};

struct SGw_World_OtpCode_Result
{
	enum { s_nCode =  s_nSGw_World_OtpCode_Result };
	WORD gwpos; WORD token;
	int result;
};

struct SGw_World_cnn2char
{
	enum { s_nCode = s_nSGw_World_cnn2char };
	DWORD ip;
	WORD port;
	WORD gwpos; WORD token;
};

struct SGw_World_cnn2game
{
	enum { s_nCode = s_nSGw_World_cnn2game };
	DWORD ip;
	WORD port;
	WORD gwpos; WORD token;

	KPassport passport;
	KObjectID playerID;
	int mapID;
	DWORD worldID;
	DWORD pskey;
};

struct SGw_World_cnn2chat
{
	enum { s_nCode = s_nSGw_World_cnn2chat };

	WORD gwpos; WORD token;
	DWORD ip; WORD port;

	KPassport passport;
	KObjectID playerId;
	char playerName[MAX_NAME_DEF];
	short playerLevel;
	short playerClass;
	int gameGroupID; // 来自哪个游戏组
};

struct SGw_World_cnn2uw
{
	enum { s_nCode = s_nSGw_World_cnn2uw };
	DWORD ip; WORD port;
	WORD gwpos; WORD token;
};

struct SGw_World_discnn2char
{
	enum { s_nCode = s_nSGw_World_discnn2char };
	WORD gwpos; WORD token;
};

struct SGw_World_discnn2game
{
	enum { s_nCode = s_nSGw_World_discnn2game };
	WORD gwpos; WORD token;
};

struct SGw_World_discnn2chat
{
	enum { s_nCode = s_nSGw_World_discnn2chat };
	WORD gwpos; WORD token;
};

struct SGw_World_discnn2uw
{
	enum { s_nCode = s_nSGw_World_discnn2uw };
	WORD gwpos; WORD token;
};

struct SGw_Character_Login
{
	enum { s_nCode = s_nSGw_Character_Login };
	DWORD publicIp;
	WORD publicPort;
};

struct SGw_Character_Connected
{
	enum { s_nCode = s_nSGw_Character_Connected };
	WORD gwpos; WORD token;
};

struct SGw_Character_AskClose
{
	enum { s_nCode = s_nSGw_Character_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct SGw_Character_Closed
{
	enum { s_nCode = s_nSGw_Character_Closed };
	WORD gwpos; WORD token;
};

struct SGw_Character_Forward
{
	enum { s_nCode = s_nSGw_Character_Forward };
	WORD gwpos;	WORD token;
};

struct SGw_Game_Login
{
	enum { s_nCode = s_nSGw_Game_Login };
	DWORD publicIp;
	WORD publicPort;
};

struct SGw_Game_Connected
{
	enum { s_nCode = s_nSGw_Game_Connected };
	WORD gwpos; WORD token;
};

struct SGw_Game_AskClose
{
	enum { s_nCode = s_nSGw_Game_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct SGw_Game_Closed
{
	enum { s_nCode = s_nSGw_Game_Closed };
	WORD gwpos; WORD token;
};

struct SGw_Game_Forward
{
	enum { s_nCode = s_nSGw_Game_Forward };
	WORD gwpos;	WORD token;
};

struct SGw_Chat_Login
{
	enum { s_nCode = s_nSGw_Chat_Login };
	DWORD publicIp;
	WORD publicPort;
};

struct SGw_Chat_Connected
{
	enum { s_nCode = s_nSGw_Chat_Connected };
	WORD gwpos; WORD token;
};

struct SGw_Chat_AskClose
{
	enum { s_nCode = s_nSGw_Chat_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct SGw_Chat_Closed
{
	enum { s_nCode = s_nSGw_Chat_Closed };
	WORD gwpos; WORD token;
	DWORD errCode;
};

// chat forward to client by gateway
struct SGw_Chat_Forward
{
	enum { s_nCode = s_nSGw_Chat_Forward };
	WORD gwpos; WORD token;	
};

struct GwS_Connected
{
	enum { s_nCode = s_nGwS_Connected };
};

struct GwS_Closed
{
	enum { s_nCode = s_nGwS_Closed };
};

struct GwS_ConnError
{
	enum {s_nCode = s_nGwS_ConnError };
	DWORD errCode;
};

struct GwS_World_Connect
{
	enum { s_nCode = s_nGwS_World_Connect };
	WORD gwpos; WORD token;
	int language;
	DWORD otpCode;
	KPassport passport;
	DWORD clientIp; WORD clientPort;
};

struct GwS_World_AskClose
{
	enum { s_nCode = s_nGwS_World_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_World_Closed
{
	enum { s_nCOde = s_nGwS_World_Closed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_World_Forward
{
	enum { s_nCode = s_nGwS_World_Forward };
	WORD gwpos; WORD token;
};

struct GwS_World_OtpCode
{
	enum { s_nCode = s_nGwS_World_OtpCode };
	WORD gwpos; WORD token;
	DWORD otpCode;
};

struct GwS_World_CharConnected
{
	enum { s_nCode = s_nGwS_World_CharConnected };
	WORD gwpos; WORD token;
};

struct GwS_World_GameConnected
{
	enum { s_nCode = s_nGwS_World_GameConnected };
	WORD gwpos; WORD token;
};

struct GwS_World_ChatConnected
{
	enum { s_nCode = s_nGwS_World_ChatConnected };
	WORD gwpos; WORD token;
};

struct GwS_World_UwConnected
{
	enum { s_nCode = s_nGwS_World_UwConnected };
	WORD gwpos; WORD token;
};

struct GwS_World_CharClosed
{
	enum { s_nCode = s_nGwS_World_CharClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_World_GameClosed
{
	enum { s_nCode = s_nGwS_World_GameClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_World_ChatClosed
{
	enum { s_nCode = s_nGwS_World_ChatClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_World_UwClosed
{
	enum { s_nCode = s_nGwS_World_UwClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_Character_Connect
{
	enum { s_nCode = s_nGwS_Character_Connect };
	WORD gwpos; WORD token;
	KPassport passport;
	int language;
	DWORD clientIp;
	WORD clientPort;
};

struct GwS_Character_AskClose
{
	enum { s_nCode = s_nGwS_Character_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_Character_Closed
{
	enum { s_nCode = s_nGwS_Character_Closed };
	WORD gwpos; WORD token;
};

struct GwS_Character_Forward
{
	enum { s_nCode = s_nGwS_Character_Forward };
	WORD gwpos; WORD token;
};

struct GwS_Game_Connect
{
	enum { s_nCode = s_nGwS_Game_Connect };

	WORD gwpos; WORD token;

	int language;
	DWORD clientIp;
	WORD clientPort;

	KPassport passport;
	KObjectID playerID;
	int mapID;
	DWORD worldID;
	DWORD pskey;
};

struct GwS_Game_AskClose
{
	enum { s_nCode = s_nGwS_Game_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_Game_Closed
{
	enum { s_nCode = s_nGwS_Game_Closed };
	WORD gwpos; WORD token;
};

struct GwS_Game_Forward
{
	enum { s_nCode = s_nGwS_Game_Forward };
	WORD gwpos; WORD token;
};

struct GwS_Chat_Connect
{
	enum { s_nCode = s_nGwS_Chat_Connect };
	WORD gwpos; WORD token;
	DWORD clientIp; WORD clientPort;

	int language;
	KPassport passport;
	KObjectID playerId;
	char playerName[MAX_NAME_DEF];
	short playerLevel;
	short playerClass;
	int gameGroupID; // 来自从哪个游戏组
};

struct GwS_Chat_AskClose
{
	enum { s_nCode = s_nGwS_Chat_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_Chat_Closed
{
	enum { s_nCode = s_nGwS_Chat_Closed };
	WORD gwpos; WORD token;
};

struct GwS_Chat_Forward
{
	enum { s_nCode = s_nGwS_Chat_Forward };
	WORD gwpos; WORD token;
};

struct GwS_uworld_Connect
{
	enum { s_nCode = s_nGwS_uworld_Connect };
	WORD gwpos; WORD token;
	int language;
	KPassport passport;
	DWORD clientIp; WORD clientPort;

};

struct GwS_uworld_AskClose
{
	enum { s_nCode = s_nGwS_uworld_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_uworld_Closed
{
	enum { s_nCode = s_nGwS_uworld_Closed };
	WORD gwpos; WORD token;
};

struct GwS_uworld_CharConnected
{
	enum { s_nCode = s_nGwS_uworld_CharConnected };
	WORD gwpos; WORD token;
};

struct GwS_uworld_GameConnected
{
	enum { s_nCode = s_nGwS_uworld_GameConnected };
	WORD gwpos; WORD token;
};

struct GwS_uworld_ChatConnected
{
	enum { s_nCode = s_nGwS_uworld_ChatConnected };
	WORD gwpos; WORD token;
};

struct GwS_uworld_CharClosed
{
	enum { s_nCode = s_nGwS_uworld_CharClosed };
	WORD gwpos; WORD token;
};

struct GwS_uworld_GameClosed
{
	enum { s_nCode = s_nGwS_uworld_GameClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwS_uworld_ChatClosed
{
	enum { s_nCode = s_nGwS_uworld_ChatClosed };
	WORD gwpos; WORD token;
	int reason;
};

struct GwClient_uworld_Connected
{
	enum { s_nCode = s_nGwClient_uworld_Connected };
	WORD gepos; WORD token;
};

struct GwClient_uworld_Closed
{
	enum { s_nCode = s_nGwClient_uworld_Closed };
	WORD gwpos; WORD token;
};

struct SGw_uworld_Login
{
	enum { s_nCode = s_nSGw_uworld_Login };
	DWORD publicIp; WORD publicPort;
};

struct SGw_uworld_Connected
{
	enum { s_nCode = s_nSGw_uworld_Connected };
	WORD gwpos; WORD token;
};

struct SGw_uworld_AskClose
{
	enum { s_nCode = s_nSGw_uworld_AskClose };
	WORD gwpos; WORD token;
	int reason;
};

struct SGw_uworld_Closed
{
	enum { s_nCode = s_nSGw_uworld_Closed };
	WORD gwpos; WORD token;
};

struct SGw_uworld_cnn2game
{
	enum { s_nCode = s_nSGw_uworld_cnn2game };
	WORD gwpos; WORD token;
	DWORD ip; WORD port;


	KPassport passport;
	KObjectID playerID;
	int mapID;
	DWORD worldID;
	DWORD pskey;
};

struct SGw_uworld_cnn2chat
{
	enum { s_nCode = s_nSGw_uworld_cnn2chat };

	WORD gwpos; WORD token;
	DWORD ip; WORD port;

	KPassport passport;
	KObjectID playerId;
	char playerName[MAX_NAME_DEF];
	short playerLevel;
	short playerClass;
	int gameGroupID; // 来自从哪个游戏组
};

struct SGw_uworld_discnn2game
{
	enum { s_nCode = s_nSGw_uworld_discnn2game };
	WORD gwpos; WORD token;
};

struct SGw_uworld_discnn2chat
{
	enum { s_nCode = s_nSGw_uworld_discnn2chat };
	WORD gwpos; WORD token;
};

#pragma pack(pop)
