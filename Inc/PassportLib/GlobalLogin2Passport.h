
#pragma once


#include "GlobalDefine.h"

using namespace System::Collections;

// PassportServer -> LoginServer
enum PL_PROTOCOL
{
	s_nPL_PacketStart = 42 * 0x100,
	s_nPL_Connected = s_nPL_PacketStart,	// ��PassportServer�������Ժ�֪ͨ�ڲ�����Ϣ����Ϸ������ʹ���Լ��ķ�ʽ���������Ӵ˰�

	s_nPL_CheckPassword_ErrRsp,				// PassportServer���صĵ�¼��֤��ѯ�����
	s_nPL_PasswordProtect_ErrRsp,			// PassportServer���صĵ�¼��֤��ѯ�����
	s_nPL_Check_PasswordProtect_Info,		// PassportServer���ش��ܱ��Ľ����
	s_nPL_Validation_Rsp,

	s_nPL_ConnError,						// ��PassportServer���Ӵ����Ժ�֪ͨ�ڲ�����Ϣ����Ϸ������ʹ���Լ��ķ�ʽ���������Ӵ˰�
	s_nPL_ConnClosed,						// ��PassportServer���ӶϿ��Ժ�֪ͨ�ڲ�����Ϣ����Ϸ������ʹ���Լ��ķ�ʽ���������Ӵ˰�

	s_nPL_3rdCommonMsg,						// ��PassportServer���صģ�Ƕ��ĵ�����ƽ̨���ظ��ͻ��˵�ͨѶ��Ϣ��������Ϸ��ʹ�ô˰�

	s_nPL_ExtraOperation_Req,				// PassportServer���صĶ�����������
	s_nPL_ExtraOperation_ErrReq,			// PassportServer���صĶ�����������

	s_nPL_PacketEnd
};

// LoginServer -> MainLoginServer
enum LP_PROTOCOL
{
	s_nLP_PacketStart = 43 * 0x100,

	s_nLP_Connected = s_nLP_PacketStart,	// ����Ϸ������LoginServer�������Ժ�֪ͨ�ڲ�����Ϣ		
	s_nLP_ConnClosed,						// ����Ϸ������LoginServer���ӶϿ���֪ͨ�ڲ�����Ϣ

	s_nLP_ServerLogin,

	s_nLP_CheckPassword_Req,				// �������
	s_nLP_Check_PasswordProtect_Req,		// ����ܱ����

	s_nLP_Logout_Notice_Req,
	s_nLP_ExtraOperation_Req,				// �������
	s_nLP_PacketEnd,
};

#pragma pack(push,1)


// Login -> Passport �����������
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

// Passport -> Login �����������������
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

// Login -> Passport �����������
struct LP_ExtraOperation_Req
{
	enum { s_nCode = s_nLP_ExtraOperation_Req,	};
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	DWORD m_dwIP;
	DWORD m_dwOperationType;// �����������
	enum { enum_Mac_Addr_Len = 32};
	char m_MacAddr[enum_Mac_Addr_Len];
	st_Accelerate_Platform m_AccePlatform;
};

// Passport -> Login �������������������
struct PL_ExtraOperation_ErrRsp 
{
	enum { s_nCode = s_nPL_ExtraOperation_ErrReq,	};	
	st_LoginInfo m_loginInfo;
	st_PassportKey m_passportKey;
	INT m_Response;
	DWORD m_dwWallowSec;
	DWORD m_dwOperationType;// �����������
};

// Passport -> Login ������������
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
	DWORD m_dwOperationType;// �����������
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
	INT64 m_lBonusButtonState;// �콱���İ�ť״̬�������0���أ�1��ʾ���ǲ���˸��3�Ļ�������˸
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

// ������
char m_acc[_max_acc_length];		// ��Ҫ��֤���˺���
char m_pass[_max_password_length];	// ��Ҫ��֤������
unsigned long m_ip;					// �û���IP��ַ������������֤��һ������Ϣ

int	m_UserData1;					// Ϊ�û�����������������֤�첽���ص����ݶ�Ӧ��ϵ�����ڻذ���ԭ������
int m_UserData2;					// Ϊ�û�����������������֤�첽���ص����ݶ�Ӧ��ϵ�����ڻذ���ԭ������
};

struct KCheckPasswordResponseData 
{
enum E_CHECK_PASSWORD_RESULT
{
E_CHECK_SUCCESS = 1,			// ��֤�ɹ�
E_CHECK_NO_ACCOUNT = -1,		// �˺Ų����ڣ�δ���� -1
E_CHECK_ERROR_PASSWORD = -2,	// ������� -2
E_CHECK_BIND_IP_ERROR = -97,	// ��IP������Ҫ�� -97
E_CHECK_BLOCK_TIME = -98,		// ʱ���ͣ -98		
E_CHECK_BLOCK_FOREVER = -99,	// ���÷�ͣ -99		
E_CHECK_SERVER_MAIN = -100,		// ������ά�� -100
};

// Result����
int result;							// ���صĽ��ֵ
int PassportID;						// ���ص��û��˺�ID
int PassportType;					// �����û������ͣ�����Ҫ��Mall����
int PassportGiftType;				// �����˺���Ʒ
};

//////////////////////////////////////////////////////////////////////////
// �����̵�LoginServer -> PassportServer
struct LM_LoginQuery_Req_Packet
{
enum { s_nCode = s_nLM_CheckPassword_Req, };			// ������
LM_LoginQuery_Req_Packet()
{
memset(this, 0 ,sizeof(LM_LoginQuery_Req_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall�ṩ���û���֤������Ϣ
};

struct LM_Logout_Req_Packet 
{
enum { s_nCode = s_nLM_Logout_Notice	};			// ������
LM_Logout_Req_Packet()
{
memset(this, 0 ,sizeof(LM_Logout_Req_Packet));
}

DWORD		m_dwAccountID;
DWORD		m_dwPlayedSec;
};

struct ML_LoginQuery_Rsq_Packet 
{
enum { s_nCode = s_nML_LoginQuery_Rsp,	};			// ������

ML_LoginQuery_Rsq_Packet()
{
memset(this, 0 ,sizeof(ML_LoginQuery_Rsq_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall�û��ṩ����֤������Ϣ��ԭ������
KCheckPasswordResponseData m_ResponseData;			// ���ظ�Mall���û���֤�����Ϣ
};


struct ML_LoginQuery_PasswordProtect_Rsq_Packet 
{
enum { s_nCode = s_nML_LoginQuery_PasswordProtect_Rsp,	};			// ������

ML_LoginQuery_PasswordProtect_Rsq_Packet()
{
memset(this, 0 ,sizeof(ML_LoginQuery_PasswordProtect_Rsq_Packet));
}

KCheckPasswordRequestData m_RequestData;			// Mall�û��ṩ����֤������Ϣ��ԭ������
KCheckPasswordResponseData m_ResponseData;			// ���ظ�Mall���û���֤�����Ϣ
KPasswordProtect m_PasswordProtect;
};
*/

// Passport -> Login ������ƽ̨��������Ϣ
struct PL_3rdCommonMsg : public K3rdCommonMsg
{
	enum { s_nCode = s_nPL_3rdCommonMsg,	};
};
#pragma pack(pop)

