

#pragma once
/********************************************************************
history:
created:	2013/05/14
created:	05:14:2013

filename: 	AccountDefine.h
file path:	
file base:	AccountDefine
file ext:	h

author:		Calvin
copyright:  

purpose:	定义了登录相关的一些常用结构体
comment:    
*********************************************************************/

#include "GlobalDefine.h"

// PassportServer -> WorldServer
enum PA_PROTOCOL
{
	s_nPA_PacketStart = 50 * 0x100,
	s_nPA_Connected,
	s_nPA_ConnClosed,
	s_nPA_CheckPassport_Req,

	s_nPA_PacketEnd
};

// WorldServer -> PassportServer
enum AP_PROTOCOL
{
	s_nAP_PacketStart = 51 * 0x100,
	s_nAP_Connected,
	s_nAP_ConnClosed,
	s_nAP_CheckPassport_Rsp,

	s_nAP_PacketEnd,
};


#pragma pack(push,1)

struct PA_CheckPassport_Req
{
	enum { s_nCode = s_nPA_CheckPassport_Req,	};

	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_dwIP;

	// Reserved
	enum { enum_Reserved_DWORD_Count = 4, enum_Reserved_String0_Len = 32, enum_Reserved_String1_Len = 64,};
	DWORD  m_adwReserved[enum_Reserved_DWORD_Count];
	char   m_achReserved0[enum_Reserved_String0_Len + 2];
	char   m_achReserved1[enum_Reserved_String1_Len + 2];
};


struct AP_CheckPassport_Rsp
{
	enum { s_nCode = s_nAP_CheckPassport_Rsp,	};

	int m_ErrorCode;	//存储过程返回的ErrorCode

	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;

	// Reserved
	enum { enum_Reserved_DWORD_Count = 4, enum_Reserved_String0_Len = 32, enum_Reserved_String1_Len = 64,};
	DWORD  m_adwReserved[enum_Reserved_DWORD_Count];
	char   m_achReserved0[enum_Reserved_String0_Len + 2];
	char   m_achReserved1[enum_Reserved_String1_Len + 2];
};

#pragma pack(pop)

