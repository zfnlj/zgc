#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "common_packet.h"

enum SW_Protocol
{
	s_nSW_Connected = s_nSW_PacketStart,
	s_nSW_ConnError,
	s_nSW_ConnClosed,

	s_nSW_Login,
	s_nSW_OTPCheck,
	s_nSW_PlayerLogin,

	// 动态创建，销毁副本
	s_nSW_CreateWorld_Rsp,
	s_nSW_DestroyWorld_Rsp,
	s_nSW_DestroyWorld_Req,
	s_nSW_DestroyWorld_Ack,
	s_nSW_WorldStateVersion,
	s_nSW_WorldStateExpire,

	s_nSW_PlayerChangeScene,

	s_nSW_PacketEnd,
};

enum WS_Protocol
{
	s_nWS_Connected = s_nWS_PacketStart,
	s_nWS_ConnError,
	s_nWS_ConnClosed,

	s_nWS_LoginRsp,
	s_nWS_TeleportTo,
	s_nWS_OTPCheck_Rsp,
	s_nWS_PlayerChangeSceneResponse,

	// 动态创建副本
	s_nWS_CreateWorld,
	s_nWS_DestroyWorld,
	s_nWS_DestroyWorld_Ack,
	s_nWS_DestroyingWorldIdle, // 关闭中的副本进入Idle状态了
	s_nWS_SyncWorldsInfoToGame,

	s_nWS_KickPlayer,
	s_nWS_ForwardToPlayer,
	s_nWS_BroardCastToAllPlayer,
	s_nWS_BroadCastMsgToPlayer,

	s_nWS_PacketEnd,
};

#pragma pack(push,1)

struct SW_Login
{
	enum { s_nCode = s_nSW_Login };
	int serverID;
	DWORD serverIP;
	WORD serverPort;
};

struct WS_LoginRsp
{
	enum { s_nCode = s_nWS_LoginRsp };
	int gameGroupID;
	BOOL isUnion;
};

struct WS_TeleportTo 
{
	enum { s_nCode = s_nWS_TeleportTo };
	KObjectID playerID;
	int mapID;
	float fX, fY, fZ;
	int worldID;
	// stream session data (optional)
};

struct WS_CreateWorld
{
	enum { s_nCode = s_nWS_CreateWorld };
	DWORD worldID;
	INT64 stateID; // 副本状态记录
	int  initdata[1]; // 初始化数据,前四个字节为数据长度
};

struct WS_DestroyWorld
{
	enum { s_nCode = s_nWS_DestroyWorld };
	DWORD worldID;
};

struct WS_DestroyWorld_Ack
{
	enum { s_nCode = s_nWS_DestroyWorld_Ack };
	DWORD worldID;
};
struct WS_DestroyingWorldIdle
{
	enum { s_nCode = s_nWS_DestroyingWorldIdle };
	DWORD worldID;
};

struct SW_CreateWorld_Rsp
{
	enum { s_nCode = s_nSW_CreateWorld_Rsp };
	enum // error codes
	{
		err_success,
		err_memory_out,
		err_init_world,
		err_create_monitor,
		err_already_exist,
		err_dbagent_unavail,
		err_load_state,
	};
	int errCode;
	DWORD worldID;
	INT64 stateID;
	TIME_T expireTime;
	short stateVer;
};

struct SW_DestroyWorld_Rsp
{
	enum { s_nCode = s_nSW_DestroyWorld_Rsp };
	enum
	{
		err_success,
		err_no_world,
	};
	DWORD worldID;
	int errCode;
};

struct SW_DestroyWorld_Req
{
	enum { s_nCode = s_nSW_DestroyWorld_Req };
	enum { unknown,finished,create,error,worldServerDown,gameServerDown,system };
	DWORD worldID;
	int reason;
};

struct SW_DestroyWorld_Ack
{
	enum { s_nCode = s_nSW_DestroyWorld_Ack };
	DWORD worldID;
};

struct SW_WorldStateVersion
{
	enum { s_nCode = s_nSW_WorldStateVersion };
	DWORD worldID;
	INT64 stateID;
	short version;
};

struct SW_WorldStateExpire
{
	enum { s_nCode = s_nSW_WorldStateExpire };
	DWORD  worldID;
	INT64  stateID;
	TIME_T expire;
};

struct WS_OTPCheck_Rsp
{
	enum { s_nCode = s_nWS_OTPCheck_Rsp };

	int m_byResult;
	KPassport m_passportKey;
	KObjectID m_dwPlayerID;
	char m_charName[MAX_NAME_DEF];
	int m_dwMapID;
	DWORD		m_dwDuplicateID;
	DWORD		m_pointID;
	INT			m_nMaxLineCount;
	INT			m_nCurrLineID;
	INT			m_nSide;
	INT			m_nMasterLevel;
	INT			m_nVipLevel;
	DWORD		m_dwTeamID;
	BOOL		m_bIsLogin;
	int			language;

	// stream sessions
};

struct SW_PlayerLogin
{
	enum { s_nCode = s_nSW_PlayerLogin };	
	KObjectID m_dwPlayerID;
	KPassport m_passportKey;
	WORD	m_Level;
	DWORD	m_NationID;
	DWORD	m_dwMapID;
	DWORD	m_dwDuplicateID;
	KObjectID	m_GuildID;
	DWORD	m_GroupID;
	KObjectID	m_AnswerGuildID;		// 已响应的公会ID
	BYTE	m_Sex;
	BYTE	m_Class;
	BYTE	m_SubClass;
	FLOAT	m_equipMark;
	DWORD	m_dwLastLevelUpTime;
	DWORD	m_dwExp;
	int		m_matchPoint;
};

struct SW_OTPCheck
{
	enum { s_nCode = s_nSW_OTPCheck };
	KPassport m_passportKey;
	DWORD	m_dwOTPCode;
	DWORD	m_dwMainVersion;
	DWORD	m_dwSubVersion;
};

struct WS_PlayerChangeSceneResponse : public PlayerChangeSceneRsp
{
	enum {	s_nCode = s_nWS_PlayerChangeSceneResponse };
	enum
	{
		enum_success,
		enum_state_refuse,
		enum_target_not_found,
		enum_no_available,
	};
	// stream session data (optional)
};

struct WS_KickPlayer
{
	enum { s_nCode = s_nWS_KickPlayer };
	KPassport passport;
	KObjectID playerID;
	int delaySecs;
};

struct SW_PlayerChangeScene : public PlayerChangeSceneReq
{
	enum {	s_nCode = s_nSW_PlayerChangeScene };
	// stream of option data
	// ...
};

struct WS_SyncWorldsInfoToGame
{
	enum { s_nCode = s_nWS_SyncWorldsInfoToGame };

	struct World
	{
		short line;
		short worldType;
		short baseIdx;
		short count;
	};

	World worlds[1];
};

struct WS_ForwardToPlayer
{
	enum { s_nCode = s_nWS_ForwardToPlayer };
	KObjectID playerId;
	WORD scCode;
	//char data[1];
};

struct WS_BroardCastToAllPlayer
{
	enum { s_nCode = s_nWS_BroardCastToAllPlayer };
	int scCode;
	char data[1];
};

struct WS_BroadcastMsgToPlayer
{
	enum { s_nCode = s_nWS_BroadCastMsgToPlayer };
	// Stream
	//    byte type // 0 全服 1 本副本 2 同类副本
	//    
};

#pragma pack(pop)
