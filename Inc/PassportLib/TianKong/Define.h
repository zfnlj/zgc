

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

purpose:	�����˵�¼��ص�һЩ���ýṹ��
comment:    
*********************************************************************/

#pragma once

#include "../GlobalDefine.h"

typedef UINT64 KObjectID;

////////////////////////////////////////////////////////////////////////////
// ��½�ͽ�ɫ�߼��Ľṹ��
#pragma pack(push,1)
struct KChar3DParam
{
	BYTE	CT_MODEL;				// ģ��(��ɫģ�����ͱ��)
	BYTE	CT_SKIN;				// ��ɫ
	BYTE	CT_FACE;				// ����
	BYTE	CT_HAIR;				// ����
	BYTE	CT_HAIR_COLOR;			// ��ɫ
};

struct STRU_KCharacterDisplayBaseInfo
{
	BYTE	CT_SEX;					// �Ա�
	BYTE	CT_PRINK;				// ��ױ
	BYTE	CT_NATIONALITY;			// ����
	BYTE	CT_FACESTYLE;			// ����

	BYTE	CT_HAIRSTYLE;			// ����
	BYTE	CT_HAIRCOLOR;			// ��ɫ
	BYTE	CT_COMPLEXION;			// ��ɫ
	BYTE	CT_MODEL;				// ģ��(��ɫģ�����ͱ��)

	BYTE	CT_Class;				// ְҵ
	BYTE	CT_SubClass;			// ��֧
	BYTE	CT_IsShowHelm;			// �Ƿ���ʾñ��
	BYTE	CT_IsShowFanshion;		// ��ʾʱװ

	TIME_T	CT_ProtectTime;			// ����ʱ��
	TIME_T	CT_ProtectTime2;		// ����ʱ��

	BYTE	CT_NeedChangeName;		// ������Ϸ�Ƿ���Ҫ����
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
	KObjectID	CT_ID;					// ��ɫID
	DWORD	CT_MAPID;				// ���ڵ�ͼ
	DWORD	CT_LAST_LOGIN_TIME;		// �ϴε�¼ʱ��
	DWORD	CT_LAST_LOGIN_IP;		// �ϴε�¼�ص�
	BYTE	CT_LEVEL;				// �ȼ�
	BYTE	CT_IS_ALREADY_LOGIN;	// ���ε�½
	char	CT_NAME[62];	// ����
	STRU_KCharacterDisplayBaseInfo m_DisplayInfo;	// ��ɫ��ʾ������Ϣ
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
	DWORD	m_ServerIP;			// ������IP
	WORD	m_ServerPort;		// �������˿�
	BYTE	m_AreaID;			// ����ID
	BYTE	m_ID;				// ������ID
	BYTE	m_ServerState;
	BYTE	m_byRecommendationState;	// 0 ���Ƽ�	>0 �Ƽ� 
	DWORD	m_worldIP;			// Ҫ���ӵĶ�Ӧworld server��IP
	WORD	m_worldPort;		// Ҫ���ӵĶ�Ӧworld server�Ķ˿�
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

// ֪ͨ�߼�ChatServer׼���ã����ͻ��˷��͵���Ϣ
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
	st_PassportKey m_passportKey;	//st_PassportKey�е�UserType��������

	// �������ݸ���m_passportKey�е�m_dwUserType�����������н���
	// ̨�壺m_dwMsgID(0:֪ͨ�ͻ�����ʾ�ֻ���, 1:�ֻ�����֤�ķ�������Ϣ) m_dwMsgID2(��m_dwMsgIDΪ1ʱ��Ч�����ErrCode) m_achMessage(��ʾ�ֻ�����)
	DWORD m_dwMsgID;		// Msg ID��������ƽ̨�Լ�����
	DWORD m_dwMsgID2;		// Msg ID2��������ƽ̨�Լ�����
	DWORD m_dwReserve1;		// Ԥ��
	DWORD m_dwReserve2;		// Ԥ��
	char m_achMessage[32];	// �ַ���
};


#pragma pack(pop)
