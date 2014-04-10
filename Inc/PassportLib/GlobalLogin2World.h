#pragma once
/********************************************************************
history:
created:	2013/05/15
created:	05:15:2013

filename: 	LoginPacket.h
file path:	
file base:	LoginPacket
file ext:	h

author:		Calvin
copyright:  

purpose:	LoginServer Packet Defines
comment:    
*********************************************************************/

#include "GlobalDefine.h"

enum LW_PROTOCOL
{
	s_nLW_PacketStart = 16 * 0x100,
	s_nLW_ConnConnect = s_nLW_PacketStart,
	s_nLW_Account_CheckValid_Response,
	s_nLW_ConnClose,
	s_nLW_PacketEnd
};

enum WL_PROTOCOL
{
	s_nWL_PacketStart = 17 * 0x100,
	s_nWL_ConnConnect = s_nWL_PacketStart,
	s_nWL_Login,
	s_nWL_ServerStateUpdate,
	s_nWL_Account_CheckValid_Request,
	s_nWL_Account_Logout,
	s_nWL_WorldServer_Start,
	s_nWL_ConnClose,
	s_nWL_PacketEnd
};

//////////////////////////////////////////////////////////////////////////
// Login Server --> WorldServer
//////////////////////////////////////////////////////////////////////////

#pragma pack(push,1)
struct WL_Login 
{
	enum { s_nCode = s_nWL_Login,		};
	DWORD		m_dwCharacterSrvIP;
	DWORD		m_dwCharacterSrvPort;
};

struct WL_ServerStateUpdate 
{
	enum {s_nCode = s_nWL_ServerStateUpdate		};
	ENUM_SERVERLIST_SHOW_STATE m_State;
	BYTE	m_byRecommendationState;	// 0 ²»ÍÆ¼ö	>0 ÍÆ¼ö 
};

struct WL_Account_CheckValid_Request 
{
	enum { s_nCode = s_nWL_Account_CheckValid_Request,	};

	//DWORD  m_dwAccountID;
	st_PassportKey m_passportKey;
	DWORD  m_dwOTPCode;
	int    m_charServerID;
	DWORD  m_dwIP;
};

struct WL_Account_Logout
{
	enum	{ s_nCode = s_nWL_Account_Logout,	};
	//DWORD		m_dwAccountID;
	st_PassportKey m_passportKey;
	int			m_nOnlineSeconds;
};

struct WL_WorldServer_Start
{
	enum { s_nCode = s_nWL_WorldServer_Start };
	int m_worldServerID;
};

//////////////////////////////////////////////////////////////////////////
//  AuthServer --> World Server
//////////////////////////////////////////////////////////////////////////

struct LW_Account_CheckValid_Response
{
	enum	{ s_nCode = s_nLW_Account_CheckValid_Response,	};
	int nResult;
	//DWORD m_dwAccountID;
	st_PassportKey m_passportKey;
	DWORD m_otpCode;
	int m_charServerID;
	int m_nMasterLevel;
	DWORD m_dwIP;
	INT m_nVipLevel;
};
#pragma pack(pop)
