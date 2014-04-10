#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "common_packet.h"

enum ChW_PROTOCOL
{
	s_nChW_Connected = s_nChW_PacketStart,
	s_nChW_ConnError,
	s_nChW_ConnClosed,

	s_nChW_Login,
	s_nChW_Account_OTP_Check,

	s_nChW_EnterGame,
	s_nChW_Character_Logout,

	s_nChW_CreatePlayer_Notice,		// 玩家创建一个角色通知
	s_nChW_DeletePlayer_Notice,		// 玩家删除一个角色通知

	s_nChW_PacketEnd
};

enum WCh_PROTOCOL
{
	s_nWCh_Connected = s_nWCh_PacketStart,
	s_nWCh_ConnError,
	s_nWCh_ConnClosed,

	s_nWCh_EnterGame_Rsp,
	
	s_nWCh_PacketEnd
};

//////////////////////////////////////////////////////////////////////////
// Character Server --> WorldServer
//////////////////////////////////////////////////////////////////////////

#pragma pack(push,1)

struct ChW_Login
{
	enum { s_nCode = s_nChW_Login };
	int serverID;
	DWORD serverIP;
	WORD serverPort;
};

struct ChW_EnterGame
{
	enum { s_nCode = s_nChW_EnterGame };
	KPassport m_passportKey;
	KObjectID mdwPID;
	char nickName[MAX_NAME_DEF];
	int dwMapID;
	int nX,nY,nZ;
	BYTE firstLogin;
};

struct ChW_Character_Logout
{
	enum { s_nCode = s_nChW_Character_Logout };
	KPassport m_passport;
};

struct ChW_CreatePlayer_Notice 
{
	enum { s_nCode = s_nChW_CreatePlayer_Notice };
	KObjectID playerID;
	char name[MAX_NAME_DEF];
};

struct ChW_DeletePlayer_Notice 
{
	enum { s_nCode = s_nChW_DeletePlayer_Notice };
	KObjectID playerID;
};

//////////////////////////////////////////////////////////////////////////
//  WorldServer --> Character Server
//////////////////////////////////////////////////////////////////////////

struct WCh_EnterGame_Rsp
{
	enum { s_nCode = s_nWCh_EnterGame_Rsp };
	int result;
	KPassport passport;
	KObjectID playerID;
	int map;
};

struct ChW_Account_OTP_Check
{
	enum { s_nCode = s_nChW_Account_OTP_Check };
	KPassport m_passportKey;
	DWORD miOTPCode;
};

#pragma pack(pop)
