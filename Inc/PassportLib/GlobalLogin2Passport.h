
#pragma once


#include "GlobalDefine.h"

using namespace System::Collections;

// PassportServer -> LoginServer
enum PL_PROTOCOL
{
	s_nPL_PacketStart = 42 * 0x100,
	s_nPL_Connected = s_nPL_PacketStart,	// 当PassportServer连接上以后通知内部的消息，游戏开发商使用自己的方式，可以无视此包

	s_nPL_CheckPassword_ErrRsp,				// PassportServer返回的登录验证查询结果包
	s_nPL_PasswordProtect_ErrRsp,			// PassportServer返回的登录验证查询结果包
	s_nPL_Check_PasswordProtect_Info,		// PassportServer返回带密保的结果包
	s_nPL_Validation_Rsp,

	s_nPL_ConnError,						// 当PassportServer连接错误以后通知内部的消息，游戏开发商使用自己的方式，可以无视此包
	s_nPL_ConnClosed,						// 当PassportServer连接断开以后通知内部的消息，游戏开发商使用自己的方式，可以无视此包

	s_nPL_3rdCommonMsg,						// 从PassportServer返回的，嵌入的第三方平台返回给客户端的通讯消息，极光游戏不使用此包

	s_nPL_ExtraOperation_Req,				// PassportServer返回的额外操作结果包
	s_nPL_ExtraOperation_ErrReq,			// PassportServer返回的额外操作错误包

	s_nPL_PacketEnd
};

// LoginServer -> MainLoginServer
enum LP_PROTOCOL
{
	s_nLP_PacketStart = 43 * 0x100,

	s_nLP_Connected = s_nLP_PacketStart,	// 当游戏开发商LoginServer连接上以后通知内部的消息		
	s_nLP_ConnClosed,						// 当游戏开发商LoginServer连接断开后通知内部的消息

	s_nLP_ServerLogin,

	s_nLP_CheckPassword_Req,				// 检查密码
	s_nLP_Check_PasswordProtect_Req,		// 检查密保结果

	s_nLP_Logout_Notice_Req,
	s_nLP_ExtraOperation_Req,				// 额外操作
	s_nLP_PacketEnd,
};

#pragma pack(push,1)


// Login -> Passport 检查密码请求
struct LP_CheckPassword_Req
{
	enum { s_nCode = s_nLP_CheckPassword_Req,	};
	enum { enum_Mac_Addr_Len = 32};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_dwIP;	
	CHAR m_MacAddr[enum_Mac_Addr_Len];
	st_Accelerate_Platform m_AccePlatform;
	int language;
};

// Passport -> Login 密码检查结果（错误包）
struct PL_CheckPassword_ErrRsp 
{
	enum { s_nCode = s_nPL_CheckPassword_ErrRsp,	};	
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	INT m_Response;
	DWORD m_dwWallowSec;
};

struct PL_PasswordProtect_ErrRsp 
{
	enum { s_nCode = s_nPL_PasswordProtect_ErrRsp,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	INT m_Response;
};

struct LP_Check_PasswordProtect_Req 
{
	enum { s_nCode = s_nLP_Check_PasswordProtect_Req,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	BYTE	m_Result[3];
};

struct PL_Check_PasswordProtect_Info
{
	enum { s_nCode = s_nPL_Check_PasswordProtect_Info,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	KProtectAskInfo m_ProtectAskInfo;
};

// Login -> Passport 额外操作请求
struct LP_ExtraOperation_Req
{
	enum { s_nCode = s_nLP_ExtraOperation_Req,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_dwIP;
	DWORD m_dwOperationType;// 额外操作类型
	enum { enum_Mac_Addr_Len = 32};
	char m_MacAddr[enum_Mac_Addr_Len];
	st_Accelerate_Platform m_AccePlatform;
};

// Passport -> Login 额外操作结果（错误包）
struct PL_ExtraOperation_ErrRsp 
{
	enum { s_nCode = s_nPL_ExtraOperation_ErrReq,	};	
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	INT m_Response;
	DWORD m_dwWallowSec;
	DWORD m_dwOperationType;// 额外操作类型
};

// Passport -> Login 额外操作结果包
struct PL_ExtraOperation_Rsp
{
	enum { s_nCode = s_nPL_ExtraOperation_Req,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_AccountFlag;
	DWORD m_GameMasterFlag;
	DWORD m_OtpCode;
	INT m_Response;
	DWORD m_LastCharacterServerIP;
	DWORD m_dwOperationType;// 额外操作类型
	st_Accelerate_Platform m_AccePlatform;
};

struct PL_Validation_Rsp
{
	enum { s_nCode = s_nPL_Validation_Rsp,				};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_AccountFlag;
	DWORD m_GameMasterFlag;
	DWORD m_OtpCode;
	INT m_Response;
	DWORD m_LastCharacterServerIP;
	st_Accelerate_Platform m_AccePlatform;
	INT64 m_NetbarVipLevel;
	INT64 m_lQuickRegister;
	INT64 m_lBonusButtonState;// 领奖中心按钮状态，如果是0隐藏，1显示但是不闪烁，3的话就是闪烁
};

struct LP_Logout_Notice_Req 
{
	enum { s_nCode = s_nLP_Logout_Notice_Req,			};
	st_PassportKey m_passportKey;
	int m_PassportID;
};



/*
struct KCheckPasswordRequestData 
{
enum _length_enum
{
_max_acc_length = 32,
_min_acc_length = 1,		
_max_password_length = 32,
_min_password_length = 1,
};

// 请求区
char m_acc[_max_acc_length];		// 需要验证的账号名
char m_pass[_max_password_length];	// 需要验证的密码
unsigned long m_ip;					// 用户的IP地址，可能做来验证的一部分信息

int	m_UserData1;					// 为用户保留的数据区，保证异步返回的数据对应关系，会在回包里原样返回
int m_UserData2;					// 为用户保留的数据区，保证异步返回的数据对应关系，会在回包里原样返回
};

struct KCheckPasswordResponseData 
{
enum E_CHECK_PASSWORD_RESULT
{
E_CHECK_SUCCESS = 1,			// 验证成功
E_CHECK_NO_ACCOUNT = -1,		// 账号不存在，未激活 -1
E_CHECK_ERROR_PASSWORD = -2,	// 密码错误 -2
E_CHECK_BIND_IP_ERROR = -97,	// 绑定IP不符合要求 -97
E_CHECK_BLOCK_TIME = -98,		// 时间封停 -98		
E_CHECK_BLOCK_FOREVER = -99,	// 永久封停 -99		
E_CHECK_SERVER_MAIN = -100,		// 服务器维护 -100
};

// Result区域
int result;							// 返回的结果值
int PassportID;						// 返回的用户账号ID
int PassportType;					// 返回用户的类型，这需要与Mall定制
int PassportGiftType;				// 返回账号礼品
};

//////////////////////////////////////////////////////////////////////////
// 开发商的LoginServer -> PassportServer
struct LM_LoginQuery_Req_Packet
{
enum { s_nCode = s_nLM_CheckPassword_Req, };			// 包命令
LM_LoginQuery_Req_Packet()
{
memset(this, 0 ,sizeof(LM_LoginQuery_Req_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall提供的用户验证请求信息
};

struct LM_Logout_Req_Packet 
{
enum { s_nCode = s_nLM_Logout_Notice	};			// 包命令
LM_Logout_Req_Packet()
{
memset(this, 0 ,sizeof(LM_Logout_Req_Packet));
}

DWORD		m_dwAccountID;
DWORD		m_dwPlayedSec;
};

struct ML_LoginQuery_Rsq_Packet 
{
enum { s_nCode = s_nML_LoginQuery_Rsp,	};			// 包命令

ML_LoginQuery_Rsq_Packet()
{
memset(this, 0 ,sizeof(ML_LoginQuery_Rsq_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall用户提供的验证请求信息，原样返回
KCheckPasswordResponseData m_ResponseData;			// 返回给Mall的用户验证结果信息
};


struct ML_LoginQuery_PasswordProtect_Rsq_Packet 
{
enum { s_nCode = s_nML_LoginQuery_PasswordProtect_Rsp,	};			// 包命令

ML_LoginQuery_PasswordProtect_Rsq_Packet()
{
memset(this, 0 ,sizeof(ML_LoginQuery_PasswordProtect_Rsq_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall用户提供的验证请求信息，原样返回
KCheckPasswordResponseData m_ResponseData;			// 返回给Mall的用户验证结果信息
KPasswordProtect m_PasswordProtect;
};
*/

// Passport -> Login 第三方平台发来的消息
struct PL_3rdCommonMsg : public K3rdCommonMsg
{
	enum { s_nCode = s_nPL_3rdCommonMsg,	};
};
#pragma pack(pop)

