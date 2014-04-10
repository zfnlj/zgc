
#pragma once
/********************************************************************
history:
created:	2009/12/14
created:	12:14:2009

filename: 	KSocketDefine.h
file path:	
file base:	KSocketDefine
file ext:	h

author:		Calvin
copyright:  

purpose:	服务器通讯的所有包的定义
comment:    
*********************************************************************/

#include "GlobalDefine.h"

// PassportServer -> WorldServer
enum PW_PROTOCOL
{
	s_nPW_PacketStart = 40 * 0x100,

	s_nPW_Connected = s_nPW_PacketStart,	// 当PassportServer连接上以后通知内部的消息，游戏开发商使用自己的方式，可以无视此包
	s_nPW_ConnClosed,						// 当PassportServer连接断开以后通知内部的消息，游戏开发商使用自己的方式，可以无视此包

	s_nPW_GameStart_Rsp,					// 检查OneTimePassword,通知开始游戏（玩家在于CharacterServer相连时机做的操作）
	s_nPW_GameEnd_Rsp,						// 通知结束游戏，包括踢人也用这个包
	s_nPW_KickPlayer_Req,					// 通知踢人

	s_nPW_UserTransfer_Req,					// 玩家充值
	s_nPW_WallowNotify_Req,					// 防沉迷通知

	s_nPW_GamePlaying_Rsp,					// 玩家进入GameServer

	s_nPW_GetAccountInfo_Rsp,				// 获取玩家帐号信息请求，返回一段XML格式数据结果

	s_nPW_UpdateAccountInfo_Rsp,			// 更新帐号信息结果，并且给与奖励。

	s_nPW_PacketEnd
};

// WorldServer -> PassportServer
enum WP_PROTOCOL
{
	s_nWP_PacketStart = 41 * 0x100,

	s_nWP_Connected = s_nWP_PacketStart,	// 当游戏开发商LoginServer连接上以后通知内部的消息
	s_nWP_ConnClosed,						// 当游戏开发商LoginServer连接断开后通知内部的消息

	s_nWP_ServerLogin,

	s_nWP_GameStart_Req,					// 开始游戏
	s_nWP_GameEnd_Req,						// 结束游戏

	s_nWP_UserTransfer_Rsp,					// 玩家充值

	s_nWP_GamePlaying_Req,					// 玩家进入GameServer
	s_nWP_GameNotPlaying_Req,				// 玩家离开GameServer

	s_nWP_GetAccountInfo_Req,				// 获取玩家帐号信息请求，返回一段XML格式数据结果
	s_nWP_UpdateAccountInfo_Req,			// 玩家修改帐号信息请求

	s_nWP_PacketEnd,
};

enum E_LOGIN_USERSTATE
{	
	enum_AccountFirstLoginFlag			= 0x0001,	// 初次登录
	enum_AccountFirstRegAndLoginFlag	= 0x0002,	// 初次注册+登录
};

#pragma pack(push,1)
struct WP_GameStart_Req
{
	enum { s_nCode = s_nWP_GameStart_Req,	};

	st_PassportKey m_passportKey;
	DWORD  m_OtpCode;
	int    m_charServerID;
	DWORD  m_dwIP;
	DWORD  m_dwCharacterServerIP;
};

struct PW_GameStart_Rsp 
{
	enum { s_nCode = s_nPW_GameStart_Rsp,	};
	enum
	{
		enum_GameStart_Success,
		enum_PassportID_Error,
		enum_GameStart_OtpError,		
	};
	PW_GameStart_Rsp()
	{
		memset(m_achSecretKey, 0, sizeof(m_achSecretKey));
	}
	int  m_byResult;
	st_PassportKey m_passportKey;
	DWORD m_OtpCode;
	int m_charServerID;
	int m_nMasterLevel;
	DWORD m_dwIP;
	INT m_nVipLevel;
	CHAR	m_achSecretKey[MAX_MD5_STRING];
	INT64	m_NetbarVipLevel;
	INT64	m_lQuickRegister;
	INT64	m_lBonusButtonState;// 领奖中心按钮状态，如果是0隐藏，1显示但是不闪烁，3的话就是闪烁
	INT64	m_AccountStateFlag;
	int m_language;
};

struct WP_GameEnd_Req 
{
	enum { s_nCode = s_nWP_GameEnd_Req,	};
	st_PassportKey m_passportKey;
	DWORD m_nOnlineSeconds;
	DWORD m_WallowSec;
};

struct PW_GameEnd_Rsp 
{
	enum { s_nCode = s_nPW_GameStart_Rsp,	};
	enum
	{
		enum_GameEnd_Success,
		enum_GameEnd_Failed,		
	};
	int m_Result;
	st_PassportKey m_passportKey;
};


struct PW_KickPlayer_Req 
{
	enum { s_nCode = s_nPW_KickPlayer_Req,	};
	st_PassportKey m_passportKey;
};

struct PW_WallowNotify_Req 
{
	enum { s_nCode = s_nPW_WallowNotify_Req,};
	st_PassportKey m_passportKey;
	BYTE m_wallowType;
};

//Sdo用户充值
struct PW_UserTransfer_Req
{
	enum { s_nCode = s_nPW_UserTransfer_Req,};
	st_PassportKey m_passportKey;
	DWORD  m_nValue;
	BYTE  m_byCashType;
	CHAR m_strOrderID[SDO_ORDER_BUFF_SIZE];
};

struct WP_UserTransfer_Rsp 
{
	enum { s_nCode = s_nWP_UserTransfer_Rsp,	};
	enum
	{
		eUserTransfer_Success,
		eUserTransfer_Error,
	};
	int  m_byResult;
	st_PassportKey m_passportKey;
	DWORD  m_nValue;
	BYTE  m_byCashType;
	DWORD  m_dwGuild;
	CHAR m_strOrderID[SDO_ORDER_BUFF_SIZE];
};

struct WP_GamePlaying_Req
{
	enum { s_nCode = s_nWP_GamePlaying_Req,	};
	st_PassportKey m_passportKey;
};

struct PW_GamePlaying_Rsp
{
	enum { s_nCode = s_nPW_GamePlaying_Rsp,};
	enum
	{
		eGamePlaying_Success_Rsp,
		eGamePlaying_NotFind_Rsp,
		eGamePlaying_NotAdult_Rsp,
		eGamePlaying_NotAdultApply_Rsp,
	};
	int m_byResult;
	st_PassportKey m_passportKey;
};

struct WP_GameNotPlaying_Req
{
	enum { s_nCode = s_nWP_GameNotPlaying_Req,	};
	st_PassportKey m_passportKey;
};


struct WP_GetAccountInfo_Req 
{
	enum { s_nCode = s_nWP_GetAccountInfo_Req,	};
	st_PassportKey m_passportKey;
};


struct PW_GetAccountInfo_Rsp 
{
	enum { s_nCode = s_nPW_GetAccountInfo_Rsp,	};
	enum 
	{
		enum_realname_len = 16,
		enum_idnumber_len = 20,
		enum_mailaddress_len = 64,
		enum_mobilenumber_len = 20,
	};
	st_PassportKey m_passportKey;
    CHAR    m_RealName[enum_realname_len];			// 真实姓名
	CHAR	m_IdNumber[enum_idnumber_len];			// 身份证号
	CHAR	m_MailAddress[enum_mailaddress_len];		// 邮箱地址
	CHAR	m_MobileNumber[enum_mobilenumber_len];		// 手机号码
	// 奖励道具
};


struct WP_UpdateAccountInfo_Req 
{
	enum { s_nCode = s_nWP_UpdateAccountInfo_Req,	};
	enum 
	{
		enum_realname_len = 16,
		enum_idnumber_len = 20,
		enum_mailaddress_len = 64,
		enum_mobilenumber_len = 20,
	};
	st_PassportKey m_passportKey;
	CHAR    m_RealName[enum_realname_len];			// 真实姓名
	CHAR	m_IdNumber[enum_idnumber_len];			// 身份证号
	CHAR	m_MailAddress[enum_mailaddress_len];		// 邮箱地址
	CHAR	m_MobileNumber[enum_mobilenumber_len];		// 手机号码
};

struct PW_UpdateAccountInfo_Rsp 
{
	enum { s_nCode = s_nPW_UpdateAccountInfo_Rsp,	};
	st_PassportKey m_passportKey;		// 帐号
	// 奖励道具
};


#pragma pack(pop)

