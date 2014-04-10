

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

#pragma once

#include "System/KType.h"
#include "System/KMacro.h"
#include "System/KPlatform.h"
#include "System/Misc/RsaNew.h"
#pragma pack(push,1)



#define	MAX_MD5_STRING				65							//  极光游戏帐号长度
#define	MAX_LOGIN_REQUEST			512							//  极光登录信息长度

#define	SDOA_MAX_ACCOUNT			64							//  盛大帐号长度
#define	SDOA_MAX_SESSION			256							//  盛大Session长度

#define	SDOA_MAX_ACCOUNT_DEF		SDOA_MAX_ACCOUNT+2			//  盛大帐号长度定义
#define	SDOA_MAX_SESSION_DEF		SDOA_MAX_SESSION+2			//  盛大Session长度定义

#define JJ_MAX_TOKEN_DEF			512							//	JJ平台Token长度定义

#define	SDO_ORDER_BUFF_SIZE			34
#define	SDO_APPENDIX_BUFF_SIZE		128

#define	COMMON_3RD_ACCOUNT_SIZE			66
#define	COMMON_3RD_PASS_SIZE				66
#define	COMMON_3RD_RESERVED_34			34
#define	COMMON_3RD_RESERVED_66			66

//////////////////////////////////////////////////////////////////////////
/*
#define	MAX_NAME			20					//  角色名字长度
#define MAX_ENABLE_NAME_LENGTH	(14)			//	角色名称允许最大长度
#define MAX_MB_NAME			12					// 法宝名字最长12字节
#define	MAX_ACCOUNT			20					//  游戏帐号长度
#define	MAX_NAME_DEF		(MAX_NAME + 2)		//  定义结构的时候，最好在名字的后面冗余2个字节，这样就可以避免使用strcpy，
#define	MAX_MODEL_NAME_DEF	(MAX_PATH)			//  定义结构的时候，最好在名字的后面冗余2个字节，这样就可以避免使用strcpy，
*/
//////////////////////////////////////////////////////////////////////////

//#define  CFG_EQUIP_ITEM_MAX_COUNT    20	// 背包中装备道具数量
#define DEF_MAX_SEND_CHAR_COUNT		(4)
#define DEF_MAX_SEND_SERVER_COUNT	(20)
//////////////////////////////////////////////////////////////////////////

#define LOADING_TYPE_CLIENT	(0)		// 客户端登陆
#define LOADING_TYPE_WEB	(1)		// 网页端登陆


enum enumAccountType // UserType
{
	eAccount_Unknown = 0,
	eAccount_Aurora = 1,	//1-10	: 极光
	eAccount_SDO = 11,		//11	: 盛大
	eAccount_Shangdu = 12,	//12	: 商都
	eAccount_Taiwan = 13,	//13	: 台湾
	eAccount_Japan = 14,	//14	: 日本
	eAccount_Jj = 15,		//15	: JJ平台
	eAccount_Korea = 16,	//16	: 韩国
	eAccount_Web = 17,

	eAccount_Count			//数目
};



//用于额外操作
enum E_EXTRA_OPERATION
{
	// 额外操作枚举
	E_EXTRA_OPERATION_START			= 0,

	// 韩国部分
	E_EXTRA_KOREA_SRDPASS_CHECK		= 1601,
	E_EXTRA_KOREA_SRDPASS_ADD		= 1602,
	E_EXTRA_KOREA_SRDPASS_DEL		= 1603,
	E_EXTRA_KOREA_SRDPASS_MODIFY	= 1604,

};

//用作Log
enum enumSDORechargeOperateLog
{
	eSDORecharge_Unknown = 0,
	eSDORecharge_FromSdoRequest = 10,
	eSDORecharge_ToBillling = 20,
	eSDORecharge_FromBillling_OK = 30,
	eSDORecharge_FromBillling_Error = 31,
};


struct st_Aurora_Login
{
	//st_Aurora_Login(){Init();}
	void Init()
	{
		memset(account, 0, sizeof(account)); 
		memset(password, 0, sizeof(password));
	}
	char account[MAX_MD5_STRING];
	char password[MAX_SESSION_STRING];
};

struct st_SDO_Login
{
	//st_SDO_Login(){Init();}
	void Init()
	{
		memset(session, 0, sizeof(session)); 
	}
	char session[SDOA_MAX_SESSION_DEF];
};

struct st_JJ_Login
{
	void Init()
	{
		memset(token, 0, sizeof(token)); 
	}
	char token[JJ_MAX_TOKEN_DEF];
};

struct st_3rd_Login_Info
{
	char account[COMMON_3RD_ACCOUNT_SIZE];
	char password[COMMON_3RD_PASS_SIZE];

	void Init()
	{
		memset(&account, 0, sizeof(account));
		memset(&password, 0, sizeof(password));
	}
};

struct st_Korea_Login_Info
{
	char account[COMMON_3RD_ACCOUNT_SIZE];
	char password[COMMON_3RD_PASS_SIZE];

	char reservedBuff0[COMMON_3RD_RESERVED_66];
	char reservedBuff1[COMMON_3RD_RESERVED_66];
	int reservedInt0;
	int reservedInt1;

	void Init()
	{
		memset(&account, 0, sizeof(account));
		memset(&password, 0, sizeof(password));
		memset(&reservedBuff0, 0, sizeof(reservedBuff0));
		memset(&reservedBuff1, 0, sizeof(reservedBuff1));
		reservedInt0 = 0;
		reservedInt1 = 0;
	}
};

struct st_LoginInfo
{
	st_LoginInfo()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(st_LoginInfo));
	}

	DWORD dwType;
	union {
		st_Aurora_Login auroraLogin;
		st_SDO_Login sdoLogin;
		st_JJ_Login jjLogin;
		st_3rd_Login_Info common3rdLogin;
		st_Korea_Login_Info koreaLogin;
	};

	DWORD GetUserType()
	{
		return dwType;
	}

	const char * GetLoginAccount()
	{
		switch(dwType)
		{
		case eAccount_Aurora:
			return auroraLogin.account;
		case eAccount_SDO:
			return sdoLogin.session;
		case eAccount_Jj:
			return jjLogin.token;
		default:
			return common3rdLogin.account;
		}
	}

	const char * GetLoginPassword()
	{
		switch(dwType)
		{
		case eAccount_Aurora:
			return auroraLogin.password;
		case eAccount_SDO:
			return "";
		case eAccount_Jj:
			return "";
		default:
			return common3rdLogin.password;
		}
	}

};



//////////////////////////////////////////////////////////////////////////


struct st_PassportKey
{
public:
	DWORD m_dwUserType;
	DWORD m_dwPassportID;
public:
	st_PassportKey()
	{
		Init();
	}
	st_PassportKey(const st_PassportKey & passportKey)
	{
		*this = passportKey;
	}
	DWORD userType()	const	{	return m_dwUserType;	}
	DWORD accountId()	const	{	return m_dwPassportID;	}

	st_PassportKey & operator = ( const st_PassportKey & passportKey )
	{
		m_dwUserType = passportKey.m_dwUserType;
		m_dwPassportID = passportKey.m_dwPassportID;
		return *this;
	}
	st_PassportKey& operator=(UINT64 ud)
	{
		m_dwUserType = (DWORD)(ud & 0xffffffff);
		m_dwPassportID = (DWORD)(ud>>32);
		return *this;
	}
	void Init()
	{
		m_dwPassportID = 0;
		m_dwUserType = 0;
	}

	BOOL IsValid()
	{
		return (m_dwPassportID > 0);
	}

	inline operator INT64() // 重载针对INT64的强制类型转换
	{
		return (((INT64)m_dwUserType)<<32) + m_dwPassportID;
	}

	inline operator UINT64() // 重载针对INT64的强制类型转换
	{
		return (((UINT64)m_dwUserType)<<32) + m_dwPassportID;
	}

	inline operator DWORD() // 重载针对DWORD的强制类型转换
	{
		return m_dwPassportID;
	}


	inline BOOL operator ==(const st_PassportKey& other) const
	{
		return (m_dwPassportID == other.m_dwPassportID && m_dwUserType == other.m_dwUserType);
	}

};


struct st_Accelerate_Platform
{
	st_Accelerate_Platform()
	{
		Init();
	}

	enum enum_Accelerate_Platform_Type{eAPT_Unknown = 0, eAPT_Xunyou = 1};
	BYTE m_byPlatformType;
	BYTE m_byParam;
	SHORT m_snParam;
	DWORD m_dwParam;

	void Init()
	{
		m_byPlatformType = 0;
		m_byParam = 0;
		m_snParam = 0;
		m_dwParam = 0;
	}
};

struct st_AreaGroup
{
	INT16 n16Area;
	INT16 n16Group;

	inline operator DWORD() // 重载针对DWORD的强制类型转换
	{
		DWORD dwTemp = n16Area;
		dwTemp = dwTemp * 0xFFFF + n16Group;
		return dwTemp;
	}
};

struct st_ServerAlias
{
	st_AreaGroup agValue;
	DWORD m_dwIp;
	char m_ServerName[32];
	//CHAR strIp[MAX_IP_BUFF_SIZE];
};


struct KPasswordProtect 
{
	KPasswordProtect()
	{
		Clear();
	}
	void Clear()
	{
		memset(this, 0 , sizeof(KPasswordProtect));
	}

	enum
	{
		max_x = 10,			// 行数 对应字母ABCDEFGH
		max_y = 8,			// 列数 对应数字12345678910
	};
	BYTE m_Key[max_y][max_x];
};


struct KProtectAskInfo
{
	static const int m_ProtectResultCount = 3;

	BOOL CheckPos(int pos)
	{
		if (pos < 0 || pos >= m_ProtectResultCount)
			return FALSE;
		return TRUE;
	}

	BOOL SetValue_X(int pos, int value)
	{
		if (!CheckPos(pos)) return FALSE;
		m_X[pos] = value;
		return TRUE;
	}

	BOOL SetValue_Y(int pos, int value)
	{
		if (!CheckPos(pos)) return FALSE;
		m_Y[pos] = value;
		return TRUE;
	}

	int GetValue_X(int pos)
	{
		if (!CheckPos(pos)) return FALSE;
		return m_X[pos];
	}

	int GetValue_Y(int pos)
	{
		if (!CheckPos(pos)) return FALSE;
		return m_Y[pos];
	}

	BYTE	m_X[m_ProtectResultCount];
	BYTE	m_Y[m_ProtectResultCount];
};
#pragma pack(pop)




enum ENUM_SERVERLIST_SHOW_STATE
{
	enumESSS_Maintenance = 0,	// 维护，无法登陆
	enumESSS_Fluency,			// 流畅
	enumESSS_Busy,				// 繁忙
	enumESSS_Full,				// 爆满
	enumESSS_NoLogin,			// 爆满，无法登陆
};

//////////////////////////////////////////////////////////////////////////

enum ENUM_WALLOW_NOTIFY_TYPE
{
	e_Wallow_Unknown = 0,
	e_Wallow_Ramain_10min,
	e_Wallow_Ramain_5min,
};



struct KAreaNameAlias
{
	int m_AreaId;
	BYTE m_ColorR;
	BYTE m_ColorG;
	BYTE m_ColorB;
	FLOAT m_MinVer;
	FLOAT m_MaxVer;
	CHAR m_AreaName[MAX_MD5_STRING];
	CHAR m_AreaState[MAX_MD5_STRING];
};

struct KRecommendServerInfo
{
public:
	KRecommendServerInfo() {Init();}
	KRecommendServerInfo(const KRecommendServerInfo &aData)
	{
		m_RecommendAreaID = aData.m_RecommendAreaID;
		m_RecommendSvrID = aData.m_RecommendSvrID;
		strcpy_k(m_RecommendMessageTitle,MAX_PATH, aData.m_RecommendMessageTitle);
		strcpy_k(m_RecommendMessage1,MAX_PATH, aData.m_RecommendMessage1);
		strcpy_k(m_RecommendMessage2,MAX_PATH, aData.m_RecommendMessage2);
	}

	void Init()
	{
		m_RecommendAreaID = 0;
		m_RecommendSvrID = 0;
		memset(m_RecommendMessageTitle, 0, sizeof(m_RecommendMessageTitle));
		memset(m_RecommendMessage1, 0, sizeof(m_RecommendMessage1));
		memset(m_RecommendMessage2, 0, sizeof(m_RecommendMessage2));
	}
	BYTE m_RecommendAreaID;
	BYTE m_RecommendSvrID;
	CHAR m_RecommendMessageTitle[MAX_PATH];
	CHAR m_RecommendMessage1[MAX_PATH];
	CHAR m_RecommendMessage2[MAX_PATH];
};
DECLARE_SIMPLE_TYPE(KRecommendServerInfo);
