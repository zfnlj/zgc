
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

purpose:	������ͨѶ�����а��Ķ���
comment:    
*********************************************************************/

#include "GlobalDefine.h"

// PassportServer -> WorldServer
enum PW_PROTOCOL
{
	s_nPW_PacketStart = 40 * 0x100,

	s_nPW_Connected = s_nPW_PacketStart,	// ��PassportServer�������Ժ�֪ͨ�ڲ�����Ϣ����Ϸ������ʹ���Լ��ķ�ʽ���������Ӵ˰�
	s_nPW_ConnClosed,						// ��PassportServer���ӶϿ��Ժ�֪ͨ�ڲ�����Ϣ����Ϸ������ʹ���Լ��ķ�ʽ���������Ӵ˰�

	s_nPW_GameStart_Rsp,					// ���OneTimePassword,֪ͨ��ʼ��Ϸ���������CharacterServer����ʱ�����Ĳ�����
	s_nPW_GameEnd_Rsp,						// ֪ͨ������Ϸ����������Ҳ�������
	s_nPW_KickPlayer_Req,					// ֪ͨ����

	s_nPW_UserTransfer_Req,					// ��ҳ�ֵ
	s_nPW_WallowNotify_Req,					// ������֪ͨ

	s_nPW_GamePlaying_Rsp,					// ��ҽ���GameServer

	s_nPW_GetAccountInfo_Rsp,				// ��ȡ����ʺ���Ϣ���󣬷���һ��XML��ʽ���ݽ��

	s_nPW_UpdateAccountInfo_Rsp,			// �����ʺ���Ϣ��������Ҹ��뽱����

	s_nPW_PacketEnd
};

// WorldServer -> PassportServer
enum WP_PROTOCOL
{
	s_nWP_PacketStart = 41 * 0x100,

	s_nWP_Connected = s_nWP_PacketStart,	// ����Ϸ������LoginServer�������Ժ�֪ͨ�ڲ�����Ϣ
	s_nWP_ConnClosed,						// ����Ϸ������LoginServer���ӶϿ���֪ͨ�ڲ�����Ϣ

	s_nWP_ServerLogin,

	s_nWP_GameStart_Req,					// ��ʼ��Ϸ
	s_nWP_GameEnd_Req,						// ������Ϸ

	s_nWP_UserTransfer_Rsp,					// ��ҳ�ֵ

	s_nWP_GamePlaying_Req,					// ��ҽ���GameServer
	s_nWP_GameNotPlaying_Req,				// ����뿪GameServer

	s_nWP_GetAccountInfo_Req,				// ��ȡ����ʺ���Ϣ���󣬷���һ��XML��ʽ���ݽ��
	s_nWP_UpdateAccountInfo_Req,			// ����޸��ʺ���Ϣ����

	s_nWP_PacketEnd,
};

enum E_LOGIN_USERSTATE
{	
	enum_AccountFirstLoginFlag			= 0x0001,	// ���ε�¼
	enum_AccountFirstRegAndLoginFlag	= 0x0002,	// ����ע��+��¼
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
	INT64	m_lBonusButtonState;// �콱���İ�ť״̬�������0���أ�1��ʾ���ǲ���˸��3�Ļ�������˸
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

//Sdo�û���ֵ
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
    CHAR    m_RealName[enum_realname_len];			// ��ʵ����
	CHAR	m_IdNumber[enum_idnumber_len];			// ���֤��
	CHAR	m_MailAddress[enum_mailaddress_len];		// �����ַ
	CHAR	m_MobileNumber[enum_mobilenumber_len];		// �ֻ�����
	// ��������
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
	CHAR    m_RealName[enum_realname_len];			// ��ʵ����
	CHAR	m_IdNumber[enum_idnumber_len];			// ���֤��
	CHAR	m_MailAddress[enum_mailaddress_len];		// �����ַ
	CHAR	m_MobileNumber[enum_mobilenumber_len];		// �ֻ�����
};

struct PW_UpdateAccountInfo_Rsp 
{
	enum { s_nCode = s_nPW_UpdateAccountInfo_Rsp,	};
	st_PassportKey m_passportKey;		// �ʺ�
	// ��������
};


#pragma pack(pop)

