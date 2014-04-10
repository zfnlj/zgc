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

// Login -> Client
#define		LC_NOTIFY_MSG_LEN		256
enum LC_PROTOCOL
{
	s_nLC_PacketStart = 1 * 0x100 + 0x10,

	s_nLC_Connected = s_nLC_PacketStart,
	s_nLC_ConnError,
	s_nLC_ConnClosed,

	s_nLC_LOGIN_RSP,				// ��½Ӧ��
	s_nLC_LOGIN_NEED_QUEUE_RSP,		// ��½Ӧ�� �����Ŷ�
	s_nLC_LOGIN_PASS_QUEUE_RSP,		// ��½Ӧ�� �����Ŷ���Ϣ
	s_nLC_LOGOUT_RSP,				// �ǳ�Ӧ��
	s_nLC_SERVERINFO_RSP,			// ��������Ϣ

	s_nLC_PasswordProtect_Valid,	// ��֤�ܱ�����
	s_nLC_PasswordProtect_Error,	// �ܱ���֤ʧ����

	// �ͻ������Ϻ����ȷ�����������
	s_nLC_ProtectPack,

	// �ͻ��˷���TabFile�ļ����ٷ��������б�
	s_nLC_DIFF_TABFILE_RSP_NoneUsed,			// TabFile�Ƶ�CharacterServer�Ϸ����ˣ�����û����
	s_nLC_DIFF_TABFILE_COUNT_RSP_NoneUsed,	// TabFile�Ƶ�CharacterServer�Ϸ����ˣ�����û����
	s_nLC_LAST_CHARACTERSERVER,

	s_nLC_AreaNameAlias,			// ��������

	s_nLC_NotifyMsg,				// ��ʾ��Ϣ

	s_nLC_RecommendServer,			// �Ƽ���������Ϣ

	s_nLC_SessionKey_Rsp,			// �ظ�SessionKey

	s_nLC_EncryptScript_Rsp,		// �ظ�EncryptScript


	s_nLC_3rdCommonMsg,				// ��Passport���صĵ�����ƽ̨��Ϣ

	s_nLC_ExtraOperation_Rsp,		// �������Ӧ��

	s_nLC_ServerAreaInfos,

	s_nLC_PacketEnd,
};

// Client -> Login
enum CL_PROTOCOL
{
	s_nCL_PacketStart =  1 * 0x100 + 0x10,
	s_nCL_LOGIN_REQ,				// �����½
	s_nCL_LOGOUT_REQ,				// ����ǳ�
	s_nCL_LINK_START,				// ���ӳɹ�
	s_nCL_LINK_OVER,				// �Ͽ�����
	s_nCL_WORLD_LINK_START,			// World���ӳɹ�
	s_nCL_PasswordProtect_Rsp,		// �ܱ�����ظ�
	s_nCL_PasswordProtect_Cancel,	// ȡ���ܱ�����
	s_nCL_RecommendServer,			// ��Ҷ��Ƽ���������ѡ��
	s_nCL_SessionKey_Req,			// ����SessionKey
	s_nCL_EncryptScript_Req,		// ����EncryptScript
	s_nCL_ExtraOperation_Req,		// ����������
	s_nCL_LoginExtra_Req,			// �����¼�������汾�ţ�Ϊ�����¼��ݣ�
	s_nCL_LoginREQ_Stream,			// �����¼ �滻 s_nCL_LOGIN_REQ �ĳ���ģʽ �����̫�������
	s_nCL_PacketEnd,
};

// Login ->Character
enum LCh_PROTOCOL
{
	s_nLCh_PacketStart = s_nCL_PacketEnd,
	s_nLCh_Connected = s_nLCh_PacketStart,

	s_nLCh_Character_Count_Req,
	s_nLCh_Character_Index,

	s_nLCh_ConnClosed,
	s_nLCh_PacketEnd
};

// Character -> Login
enum ChL_PROTOCOL
{
	s_nChL_PacketStart = s_nLCh_PacketEnd,
	s_nChL_CharacterServer_Notarize,
	s_nChL_Character_Count_Rsp,
	s_nChL_PacketEnd
};

#pragma pack(push,1)
// Login Paket Client - Server
struct CL_LoginReq
{
	enum { s_nCode = s_nCL_LOGIN_REQ, };
	st_LoginInfo loginInfo;
	DWORD m_dwMainVersion;
	DWORD m_dwSubVersion;
	FLOAT m_fVersion;
	enum { enum_Mac_Addr_Len = 32};
	char m_MacAddr[enum_Mac_Addr_Len];
	INT64	m_nRandomKey;
	char	s_chRecorver[16];
	st_Accelerate_Platform m_AccePlatform;
	int language;
};

struct CL_LoginExtra_Req 
{
	enum { s_nCode = s_nCL_LoginExtra_Req, };
	INT64 m_LoginPacketVer;	
};

// Login Paket Server - Client
struct LC_LoginRsp
{
	enum { s_nCode = s_nLC_LOGIN_RSP, };
	enum 
	{
		error_Succeed = 0,
		error_User_Not_Found,	// �û���δ�ҵ�
		error_Psd_Error,		// ���벻��ȷ
		error_Already_Online,	// �Ѿ���½
		error_OTP_error,		// OTP����
		error_Version_error,	// �汾����
		error_DB_error,
		error_IllegalCharValid,	// ���������ַ�
		error_ServerClose,		// ϵͳά����
		error_AccountBlocked,	// �ʺű���ͣ
		error_WallowProtect,	// ʢ�������
		error_AdultUnApply,		// ʢ��δ�Ǽ�
		error_NoGetway,			// û�����ؿ���
	};
	st_PassportKey m_passportKey;
	DWORD m_dwOTPCode;
	INT miResult;
	DWORD m_dwWallowSec;
	st_Accelerate_Platform m_AccePlatform;
	DWORD mdwGatewayIP;
	WORD mwGatewayPort;
};

// Extra Login Operation Paket Client - Server
struct CL_ExtraOperationReq
{
	enum { s_nCode = s_nCL_ExtraOperation_Req, };
	st_LoginInfo loginInfo;
	DWORD m_dwMainVersion;
	DWORD m_dwSubVersion;
	FLOAT m_fVersion;
	DWORD m_dwOperationType;
	enum { enum_Mac_Addr_Len = 32};
	char m_MacAddr[enum_Mac_Addr_Len];
	st_Accelerate_Platform m_AccePlatform;
};

// Extra Login Operation Server - Client
struct LC_ExtraOperationRsp
{
	enum { s_nCode = s_nLC_ExtraOperation_Rsp, };
	st_PassportKey m_passportKey;
	DWORD m_dwOTPCode;
	INT miResult;
	DWORD m_dwWallowSec;
	DWORD m_dwOperationType;
	st_Accelerate_Platform m_AccePlatform;
};

struct LC_LoginNeedQueueRsp
{
	enum { s_nCode = s_nLC_LOGIN_NEED_QUEUE_RSP, };
	st_PassportKey m_passportKey;
	DWORD m_CurrentQueueCount;		// ��ǰ��������
};

struct LC_LoginPlayerPassQueueCountRsp
{
	enum { s_nCode = s_nLC_LOGIN_PASS_QUEUE_RSP, };
	DWORD	m_PassQueueCount;		// �Ӷ����е�½��Ϸ�ɹ�������
};

struct LC_PasswordProtect_Valid 
{
	enum { s_nCode = s_nLC_PasswordProtect_Valid,};
	KProtectAskInfo m_ProtectResult;
};

// Login Paket Client - Server
struct CL_Link_Over //: public KPacket
{
	enum { s_nCode = s_nCL_LINK_OVER, };
	DWORD m_dwConnectIndex;
};

struct CL_World_Link_Start //: public KPacket
{
	enum { s_nCode = s_nCL_WORLD_LINK_START, };
	DWORD m_dwConnectIndex;
};

struct CL_Link_Start //: public KPacket
{
	enum { s_nCode = s_nCL_LINK_START, };
	DWORD m_dwConnectIndex;
};

struct LC_ServerInfoRsp
{
	enum { s_nCode = s_nLC_SERVERINFO_RSP, };
	STRU_SERVER_INFO m_struServerInfo;
};

struct LC_LAST_CHARACTERSERVER
{
	enum { s_nCode = s_nLC_LAST_CHARACTERSERVER, };
	INT m_nAreaID;
	INT	m_nServerID;
};

struct LC_ProtectPack
{
	enum { s_nCode = s_nLC_ProtectPack };
	// byte sequence of protect pack data
};

struct LC_PasswordProtect_Error
{
	enum { s_nCode = s_nLC_PasswordProtect_Error, };
	int byResult;
};

struct CL_LogoutReq
{
	enum { s_nCode = s_nCL_LOGOUT_REQ, };
};

struct LC_LogoutRsp
{
	enum { s_nCode = s_nLC_LOGOUT_RSP, };
};

struct CL_PasswordProtect_Rsp 
{
	enum { s_nCode = s_nCL_PasswordProtect_Rsp,	};
	int	m_Result[3];
};

struct CL_PasswordProtect_Cancel
{
	enum { s_nCode = s_nCL_PasswordProtect_Cancel,	};
};


struct LC_AreaNameAlias 
{
	enum { s_nCode = s_nLC_AreaNameAlias,			};
};

struct LC_NotifyMsg 
{
	enum { s_nCode = s_nLC_NotifyMsg,			};
	LC_NotifyMsg()
	{
		memset(this, 0 ,sizeof(LC_NotifyMsg));
	}
	DWORD m_dwMsgID;
	CHAR m_achMsg[LC_NOTIFY_MSG_LEN];
};

struct LC_RecommendServer 
{
	enum { s_nCode = s_nLC_RecommendServer,		};
	LC_RecommendServer()
	{
		memset(this, 0 , sizeof(LC_RecommendServer));
	}
};

struct LC_SessionKey_Rsp 
{
	enum { s_nCode = s_nLC_SessionKey_Rsp,		};
	LC_SessionKey_Rsp()
	{
		memset(this, 0 , sizeof(LC_SessionKey_Rsp));
	}
	st_OP_KEY ucPubKey;
};


struct LC_EncryptScript_Rsp 
{
	enum { s_nCode = s_nLC_EncryptScript_Rsp,		};
	LC_EncryptScript_Rsp()
	{
		memset(this, 0 , sizeof(LC_EncryptScript_Rsp));
	}
};



struct CL_RecommendServer
{
	enum { s_nCode = s_nCL_RecommendServer,		};
	BYTE mbySelectRecommendServer;
};

struct CL_SessionKey_Req
{
	enum { s_nCode = s_nCL_SessionKey_Req,		};
};

struct CL_EncryptScript_Req
{
	enum { s_nCode = s_nCL_EncryptScript_Req,		};
	int		s_nRandomKey;		//�Ի��Ե������
	char	s_chRecorver[16];
	CL_EncryptScript_Req()
		: s_nRandomKey(0)
	{
		memset( s_chRecorver, 0, sizeof(s_chRecorver) );
	}
};

struct LCh_CharacterCountReq
{
	enum { s_nCode = s_nLCh_Character_Count_Req, };
	st_PassportKey m_passportKey;
	DWORD m_dwAccountType;
};

struct LCh_Character_Index
{
	enum { s_nCode = s_nLCh_Character_Index, };
	DWORD m_nIndex;
};

struct ChL_CharacterServer_NotarizeReq
{
	enum { s_nCode = s_nChL_CharacterServer_Notarize, };
	DWORD m_dwIndex;
};

struct ChL_CharacterCountRsp
{
	enum { s_nCode = s_nChL_Character_Count_Rsp, };
	st_PassportKey m_passportKey;
	STRU_SERVER_INFO m_struServerInfo;
	int m_Result;
};


struct LC_3rdCommonMsg : public K3rdCommonMsg
{
	enum { s_nCode = s_nLC_3rdCommonMsg};
};


struct CL_LoginREQ_Stream
{
	enum { s_nCode = s_nCL_LoginREQ_Stream,	};

	enum
	{
		enum_Key_PassportType = 1,	// �ʺ����� DWORD
		enum_Key_PassportName,		// �ʺ��� string
		enum_Key_Password,			// ����	string
		enum_Key_MainVersion,		// ���汾�� DWORD
		enum_Key_SubVersion,		// �Ӱ汾�� DWORD
		enum_Key_Version,			// �汾�� FLOAT
		enum_Key_MacAddr,			// ����MAC��ַ string
		enum_Key_RandomKey,			// ������� INT64
		enum_Key_Accelerate_Platform,// ���������� st_Accelerate_Platform(BYTE,BYTE,SHORT,DWORD)
		enum_Key_Langueage,			 // ���� int
		enum_Key_NetType,			 // �������� int
	};
	/*
	st_LoginInfo loginInfo;
	DWORD m_dwMainVersion;
	DWORD m_dwSubVersion;
	FLOAT m_fVersion;
	enum { enum_Mac_Addr_Len = 32};
	char m_MacAddr[enum_Mac_Addr_Len];
	INT64	m_nRandomKey;
	char	s_chRecorver[16];
	st_Accelerate_Platform m_AccePlatform;
	int language;
	*/
};

#pragma pack(pop)

