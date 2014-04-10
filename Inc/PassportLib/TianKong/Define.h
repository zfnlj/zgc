

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

#include "../GlobalDefine.h"

typedef UINT64 KObjectID;

////////////////////////////////////////////////////////////////////////////
// 登陆和角色逻辑的结构体
#pragma pack(push,1)
struct KChar3DParam
{
	BYTE	CT_MODEL;				// 模型(角色模型类型编号)
	BYTE	CT_SKIN;				// 肤色
	BYTE	CT_FACE;				// 脸型
	BYTE	CT_HAIR;				// 发型
	BYTE	CT_HAIR_COLOR;			// 发色
};

struct STRU_KCharacterDisplayBaseInfo
{
	BYTE	CT_SEX;					// 性别
	BYTE	CT_PRINK;				// 化妆
	BYTE	CT_NATIONALITY;			// 国籍
	BYTE	CT_FACESTYLE;			// 脸型

	BYTE	CT_HAIRSTYLE;			// 发型
	BYTE	CT_HAIRCOLOR;			// 发色
	BYTE	CT_COMPLEXION;			// 肤色
	BYTE	CT_MODEL;				// 模型(角色模型类型编号)

	BYTE	CT_Class;				// 职业
	BYTE	CT_SubClass;			// 分支
	BYTE	CT_IsShowHelm;			// 是否显示帽子
	BYTE	CT_IsShowFanshion;		// 显示时装

	TIME_T	CT_ProtectTime;			// 保护时间
	TIME_T	CT_ProtectTime2;		// 保护时间

	BYTE	CT_NeedChangeName;		// 进入游戏是否需要改名
};

struct STRU_KNpcDisplayBaseInfo
{
	BYTE mbyFightType;
	BYTE mbyAttackable;
	BYTE mbyFleeType;
	BYTE mbyMNEliteLevel;

	BYTE mbyNationality;
	BYTE mbySex;
	BYTE mbyClass;
	BYTE mbySubClass;

	int nWeaponID;

	BYTE mbyRace;
	BYTE mbyElement;
};

struct KCharacterBaseInfo
{
	KObjectID	CT_ID;					// 角色ID
	DWORD	CT_MAPID;				// 所在地图
	DWORD	CT_LAST_LOGIN_TIME;		// 上次登录时间
	DWORD	CT_LAST_LOGIN_IP;		// 上次登录地点
	BYTE	CT_LEVEL;				// 等级
	BYTE	CT_IS_ALREADY_LOGIN;	// 初次登陆
	char	CT_NAME[62];	// 名称
	STRU_KCharacterDisplayBaseInfo m_DisplayInfo;	// 角色显示基本信息
};


struct STRU_EQUIP_INFO
{
	WORD wItemID;
	WORD wPos;
	BYTE m_elementType;
	BYTE m_elementLevel;
};


struct STRU_CHARACTER_INFO
{
	void Clear() 
	{
		m_nItemCount = 0;
		memset(&m_CharacterBaseInfo, 0, sizeof(m_CharacterBaseInfo));
		memset(mItem, 0, sizeof(mItem));
	}
	KCharacterBaseInfo m_CharacterBaseInfo;
	STRU_EQUIP_INFO	mItem[20];  //CFG_EQUIP_ITEM_MAX_COUNT
	USHORT m_nItemCount;
};

struct STRU_CHARACTER_INFOS
{
	BYTE m_Count;
	BOOL m_bCharacterInfoValid[DEF_MAX_SEND_CHAR_COUNT];
	STRU_CHARACTER_INFO m_CharacterInfo[DEF_MAX_SEND_CHAR_COUNT];
};

struct STRU_SERVER_INFO
{
	STRU_SERVER_INFO()
	{
		memset(this, 0 ,sizeof(STRU_SERVER_INFO));
	}
	char m_szServerName[62];
	DWORD	m_ServerIP;			// 服务器IP
	WORD	m_ServerPort;		// 服务器端口
	BYTE	m_AreaID;			// 大区ID
	BYTE	m_ID;				// 服务器ID
	BYTE	m_ServerState;
	BYTE	m_byRecommendationState;	// 0 不推荐	>0 推荐 
	DWORD	m_worldIP;			// 要连接的对应world server的IP
	WORD	m_worldPort;		// 要连接的对应world server的端口
};
DECLARE_SIMPLE_TYPE(STRU_SERVER_INFO)

struct STRU_LOGIN_INFO
{
	STRU_LOGIN_INFO()
	{
		memset(account, 0, sizeof(account)); 
		memset(password, 0, sizeof(password));
	}
	char account[MAX_MD5_STRING];
	char password[MAX_MD5_STRING];
};

// 通知逻辑ChatServer准备好，给客户端发送的消息
struct KChatServerInfo
{
	KChatServerInfo()
	{
		memset(this , 0 ,sizeof(KChatServerInfo));
	}

	UINT64		dwPlayerID;
	CHAR		strChatServerAddr[16];
	DWORD		dwChatServerPort;
	DWORD		m_OTP;
};

struct K3rdCommonMsg 
{
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;	//st_PassportKey中的UserType标明类型

	// 以下内容根据m_passportKey中的m_dwUserType的类型来自行解析
	// 台湾：m_dwMsgID(0:通知客户端显示手机锁, 1:手机锁验证的服务器信息) m_dwMsgID2(当m_dwMsgID为1时有效，存放ErrCode) m_achMessage(提示手机号码)
	DWORD m_dwMsgID;		// Msg ID，第三方平台自己定义
	DWORD m_dwMsgID2;		// Msg ID2，第三方平台自己定义
	DWORD m_dwReserve1;		// 预留
	DWORD m_dwReserve2;		// 预留
	char m_achMessage[32];	// 字符串
};


#pragma pack(pop)
