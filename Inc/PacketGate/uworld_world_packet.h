#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "common_packet.h"

enum WU_PROTOCOL
{
	s_nWU_Connected = s_nWU_PacketStart,
	s_nWU_ConnClosed,
	s_nWU_PlayerUnloaded,		// 玩家数据保存好了
	s_nWU_PlayerChangeScene,	// 切服请求
	s_nWU_KickPlayer,
	s_nWU_PlayerChangeSceneRsp,

	s_nWU_serviceWorld_apply,
	s_nWU_serviceWorld_teamApply,
	s_nWU_serviceWorld_withdraw,
	s_nWU_serviceWorld_teamWithdraw,
	s_nWU_serviceWorld_inviteAck,
	s_nWU_serviceWorld_emergencyInviteAck,
	s_nWU_serviceWorld_emergencyEnterMap,
	s_nWU_serviceWorld_smartQuit,
	s_nWU_serviceWorld_AcceptInvite,

	s_nWU_notify_playerEnterGame,
	s_nWU_notify_playerLeaveGame,

	s_nWU_teleportPlayer_rsp,
	s_nWU_serviceWorld_uiAction,

	s_nWU_Org,

	s_nWU_PacketEnd,
};

enum UW_PROTOCOL
{
	s_nUW_Connected = s_nUW_PacketStart,
	s_nUW_ConnError,
	s_nUW_ConnClosed,
	s_nUW_Login,
	s_nUW_PlayerChangeSceneResponse,
	s_nUW_PlayerEnter,	// 玩家进入UnionServer
	s_nUW_PlayerLeave,	// 玩家离开UnionServer
	s_nUW_PlayerChangeScene, // 从Union切服到普通场景
	s_nUW_KickPlayer,	// 仅仅是普通WorldServer踢人，假定UnionWorldServer已经没这个人了

	s_nUW_forwardToPlayer,
	s_nUW_forwardToGame,
	s_nUW_doScript,
	s_nUW_serviceWorld_notifyCfg,

	s_nUW_teleportPlayer,

	s_nUW_forwardMessageToPlayer,
	s_nUW_forwardMessageIdToPlayer,

	s_nUW_Org,

	s_nUW_backToChar,	// 玩家返回人物界面
	s_nUW_serviceWorld_AcceptInvite,

	s_nUW_PacketEnd,
};

#pragma warning(disable:4200)
#pragma pack(push, 1)

struct WU_PlayerUnloaded
{
	enum { s_nCode = s_nWU_PlayerUnloaded };
	KObjectID playerID;
	DWORD errCode;
};

struct WU_PlayerChangeScene : public PlayerChangeSceneReq
{
	enum {	s_nCode = s_nWU_PlayerChangeScene };
	// stream of option data (optional)
	// ...
};

struct UW_PlayerChangeSceneResponse : public PlayerChangeSceneRsp
{
	enum { s_nCode = s_nUW_PlayerChangeSceneResponse };
	// stream of option data (optional)
};

struct WU_KickPlayer
{
	enum { s_nCode = s_nWU_KickPlayer };
	KObjectID playerID;
};

struct UW_PlayerEnter
{
	enum { s_nCode = s_nUW_PlayerEnter };
	KObjectID playerID;
};

struct UW_PlayerLeave
{
	enum { s_nCode = s_nUW_PlayerLeave };
	KObjectID playerID;
};

struct UW_PlayerChangeScene : public PlayerChangeSceneReq
{
	enum { s_nCode = s_nUW_PlayerChangeScene };
	// stream of option data
	// ...	
};

struct WU_PlayerChangeSceneRsp : public PlayerChangeSceneRsp
{
	enum { s_nCode = s_nWU_PlayerChangeSceneRsp };
};

struct WU_serviceWorld_apply
{
	enum { s_nCode = s_nWU_serviceWorld_apply };
	int mapID;
	KObjectID pid;

	int   nExtendVal;
	INT64 iExtendVal;
	char  szExtendVal[MAX_NAME_DEF];	

	//sServiceWorldSign sign[0];
};
struct WU_serviceWorld_teamApply
{
	enum { s_nCode = s_nWU_serviceWorld_teamApply };
	int	  mapId;
	KObjectID pid;

	DWORD fromTeamID;
	BYTE  count;
	int   nExtendVal;
	INT64 iExtendVal;
	char  szExtendVal[MAX_NAME_DEF];	

	//sServiceWorldSign sign[0];
	//int getSignsCount(size_t len) const
	//{
	//	return (len-offsetof(WU_serviceWorld_teamApply,sign))/sizeof(sServiceWorldSign);
	//}
};

struct WU_serviceWorld_withdraw
{
	enum { s_nCode = s_nWU_serviceWorld_withdraw };
	int mapId;
	KObjectID pid;
};

struct WU_serviceWorld_teamWithdraw
{
	enum { s_nCode = s_nWU_serviceWorld_teamWithdraw };
	int mapId;
	KObjectID pid;
};

struct WU_serviceWorld_inviteAck
{
	enum { s_nCode = s_nWU_serviceWorld_inviteAck };
	KObjectID pid;
	int  mapId;
	BYTE accept;
};

struct WU_serviceWorld_emergencyInviteAck
{
	enum { s_nCode = s_nWU_serviceWorld_emergencyInviteAck };
	KObjectID pid;
	int  mapId;
	BYTE accept;
};

struct WU_serviceWorld_emergencyEnterMap
{
	enum { s_nCode = s_nWU_serviceWorld_emergencyEnterMap };
	KObjectID pid;
	int mapId;
};

struct WU_serviceWorld_smartQuit
{
	enum { s_nCode = s_nWU_serviceWorld_smartQuit };
	KObjectID pid;
	int mapId;
};

struct WU_ServiceWorld_AcceptInvite
{
	enum { s_nCode = s_nWU_serviceWorld_AcceptInvite };
	KObjectID playerId;
	DWORD	  mapId;
};

struct UW_KickPlayer
{
	enum { s_nCode = s_nUW_KickPlayer };
	KObjectID playerID;
};

struct UW_Login
{
	enum { s_nCode = s_nUW_Login };
	int gameGroupID;
	DWORD serverIP; WORD serverPort;
};

struct WU_notify_playerEnterGame
{
	enum { s_nCode = s_nWU_notify_playerEnterGame };
	KObjectID pid;
	int mapID;
};

struct WU_notify_playerLeaveGame
{
	enum { s_nCode = s_nWU_notify_playerLeaveGame };
	KObjectID pid;
	int mapID;
};

struct WU_teleportPlayer_rsp
{
	enum { s_nCode = s_nWU_teleportPlayer_rsp };
	KObjectID pid;
	int mapID;
	DWORD worldID;
	float x, y, z;
};

struct WU_serviceWorld_uiAction
{
	enum { s_nCode = s_nWU_serviceWorld_uiAction };
	KObjectID pid;
	int mapID;
	int	ttype;	

	// stream session data (optional)
};

struct UW_forwardToPlayer
{
	enum { s_nCode = s_nUW_forwardToPlayer };
	KObjectID pid;
	WORD cmd;
	// data sequence of packet cmd
};

struct UW_forwardToGame
{
	enum { s_nCode = s_nUW_forwardToGame };
	KObjectID pid;
	WORD cmd;
};

struct UW_doScript
{
	enum { s_nCode = s_nUW_doScript };
	// KRemoteScript memory layout
	// use KRemoteScriptReader to process
};

struct UW_teleportPlayer
{
	enum { s_nCode = s_nUW_teleportPlayer };
	KObjectID pid;
	int mapID;
	DWORD worldID;
	float x, y, z;
	// stream session data (optional)
};

struct UW_forwardMessageToPlayer
{
	enum { s_nCode = s_nUW_forwardMessageToPlayer };
	KObjectID pid;
	// str
};

struct UM_forwardMessageIdToPlayer
{
	enum { s_nCode = s_nUW_forwardMessageIdToPlayer };
	KObjectID pid;
	int	id;
};

struct WU_Org
{
	enum{ s_nCode = s_nWU_Org };
};

struct UW_Org
{
	enum{ s_nCode = s_nUW_Org };
};

struct UW_backToChar
{
	enum { s_nCode = s_nUW_backToChar };
	KObjectID playerId;
};

struct UW_ServiceWorld_AcceptInvite
{
	enum { s_nCode = s_nUW_serviceWorld_AcceptInvite };
	KObjectID playerId;
	DWORD	  mapId;
};

#pragma pack(pop)
#pragma warning(default:4200)
