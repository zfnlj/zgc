#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include "Inc/KCommonDefine.h"
#include "Inc/KCommonStruct.h"
#include "common_packet.h"
#include "Inc/KBusinessDefine.h"

#define MAX_GEN_CARD_NUM 3
enum CS_Protocol
{
	s_nCS_Connected = s_nCS_PacketStart,
	s_nCS_ConnClosed,
	s_nCS_ConnError,

	s_nCS_Ping,
	s_nCS_PlayerLogin,
	s_nCS_ClientReady,
	s_nCS_PlayerLogout,

	s_nCS_PlayerEnterLobby, //������Ϸ����
	s_nCS_BT_SelectCardOK, //ѡ�����
	s_nCS_BattleOpDone,
	s_nCS_BT_EndTurn,



	s_nCS_PlayerAppear,
	s_nCS_NpcAppear,
	s_nCS_TriggerAppear,

	//quest
	s_nCS_OpenQuestPanel,
	s_nCS_AcceptQuest,

	//business
	s_nCS_BusinessStart,
	s_nCS_BusinessConfirm,
	s_nCS_BusinessPutOneItem,
	s_nCS_BusinessLock,
	s_nCS_BusinessCancel,
	s_nCS_BusinessRequest,

	//store
	s_nCS_StoreBuy,

	s_nCS_PacketEnd,
};

enum SC_Protocol
{
	s_nSC_Connected = s_nSC_PacketStart,
	s_nSC_ConnClosed,
	s_nSC_ConnError,

	s_nSC_PingAck,
	s_nSC_PlayerLogin_Rsp,

	s_nSC_ChangeScene,
	s_nSC_MessageBox,
	s_nSC_PlayerAppear,
	s_nSC_PlayerDisappear,

	//zgc
	s_nSC_BattleCtrlInfo, //ս������
	s_nSC_BattleInit,	  //��ʼ��
	s_nSC_Battle_TurnBegin, //�غϿ�ʼ
	s_nSC_Battle_SelectHandCardOK, //ѡ�������
	s_nSC_Battle_WorldUpdate, //ѡ�������
	s_nSC_Battle_CardMove,
	s_nSC_CardDuelResult,
	s_nSC_UseAbilityResult,
	s_nSC_BattleDrawCard,
	s_nSC_BattleStartSkill,
	s_nSC_Battle_Card2Tomb,
	s_nSC_BattleSummonCard,

	//quest
	s_nSC_SyncQuestSession,
	s_nSC_SyncQuestPersist,
	s_nSC_QuestHistoryInit,
	s_nSC_QuestHistorySync,
	s_nSC_OpenQuestPanel,
	s_nSC_SyncAvailQuests,
	s_nSC_AddQuest,
	s_nSC_LoadQuest,
	s_nSC_RemoveQuest,
	s_nSC_QuestOverEffect,
	s_nSC_QuestAchieveCheckResult,

	//bag
	s_nSC_BagOperation,
	s_nSC_BagOperationErr,

	//card store
	s_nSC_GenPlayerCard,

	//business

	s_nSC_BusinessRequest,
	s_nSC_BusinessCancel,
	s_nSC_BusinessFinish,
	s_nSC_BusinessConfirm,
	s_nSC_BusinessPutOneItem,
	s_nSC_BusinessLock,
	s_nSC_BusinessStart,

	//store
	s_nSC_StoreAddProduct,				// �̵����һ����Ʒ
	s_nSC_StoreBuy,

	s_nSC_PacketEnd,
};

#pragma pack(push,1)

struct CS_Ping
{
	enum { s_nCode = s_nCS_Ping };
	DWORD localSendTime;
};

struct SC_PingAck
{
	enum { s_nSC_PingAck };
	DWORD localSendTime;
	DWORD serverRecvTime;
	DWORD serverSendTime;
};

//��¼ʱ�����������
struct CS_PlayerLogin
{
	enum { s_nCode = s_nCS_PlayerLogin };
	KPassport m_passportKey;
	DWORD	m_dwMapID;
	DWORD	m_dwDuplicateID;
	DWORD	m_dwOTPCode;
	DWORD	m_dwMainVersion;
	DWORD	m_dwSubVersion;
	DWORD	m_dwPsKey;
};

struct CS_ClientReady 
{
	enum { s_nCode = s_nCS_ClientReady };
	BYTE mac[6];
};

//����˳�
struct CS_PlayerLogout
{
	enum { s_nCode = s_nCS_PlayerLogout };
};

struct SC_PlayerLogin_Rsp
{
	enum { s_nCode = s_nSC_PlayerLogin_Rsp };
	int result;	// error message ID
	//DWORD serverTick;
	KObjectID m_id;
	int m_mapId;
	float m_x, m_y, m_z;
	BYTE m_sex;
	char m_name[MAX_NAME_DEF];
	char m_showName[MAX_NAME_DEF];
	//DWORD m_epochTime;//��Ԫʱ��
	BYTE m_vipLevel;
	int m_fame; //����
};

struct SC_MessageBox
{
	enum { s_nCode = s_nSC_MessageBox };
	int typeID;		// ��Ϣ������� ID
	int titleID;
	int textID;
	int buttons;
};

struct SC_ChangeScene
{
	enum { s_nCode = s_nSC_ChangeScene };
	int mapID;
	int result;
};

struct CS_PlayerAppear
{
	enum { s_nCode = s_nCS_PlayerAppear };
	KObjectID targetPlayerID;
	ObjectPosition position;
	// model information ...
	// state information ...
	// other information ...
};

struct CS_NpcAppear
{
	enum { s_nCode = s_nCS_NpcAppear };
	KObjectID targetPlayerID;
	KObjectID npcID;
	ObjectPosition position;
	// model information ...
	// state information ...
	// other information ...
};

struct CS_TriggerAppear
{
	enum { s_nCode = s_nCS_TriggerAppear };
	KObjectID targetPlayerID;
	KObjectID npcID;
	ObjectPosition position;
	// model information ...
	// state information ...
	// other information ...
};

struct CS_BattleOpDone
{
	enum { s_nCode = s_nCS_BattleOpDone };
	int _src;
	int _des;
};



struct SC_PlayerAppear
{
	enum { s_nCode = s_nSC_PlayerAppear };
	DWORD flagSet; // ��Ǽ���
	KObjectID playerID;
	char playerName[MAX_NAME_DEF];
	ObjectPosition position;
	// model information ...
	// state information ...
	// other information ...
};

struct SC_BattleSelectHandCardOK
{
	enum { s_nCode = s_nSC_Battle_SelectHandCardOK };
	KObjectID playerID;
};

struct SC_Battle_CardMove
{
	enum { s_nCode = s_nSC_Battle_CardMove};
	int _realId;
};

struct SC_Battle_Card2Tomb
{
	enum { s_nCode = s_nSC_Battle_Card2Tomb};
	int _realId;
};

struct SC_Battle_TurnBegin
{
	enum { s_nCode = s_nSC_Battle_TurnBegin};
	KObjectID _playerID;
};
struct SC_PlayerDisappear
{
	enum { s_nCode = s_nSC_PlayerDisappear };
	KObjectID playerID;
};

struct SC_CardDuelResult
{
	enum { s_nCode = s_nSC_CardDuelResult };
	int _atk;
	int _def;
	int _val1;
	int _val2;
};

struct SC_UseAbilityResult
{
	enum { s_nCode = s_nSC_UseAbilityResult };
	int _src;
	int _des;
	int _abilityId;
	int _val;
};

struct SC_Battle_StartSkill
{
	enum { s_nCode = s_nSC_BattleStartSkill };
	int _realId;
};

struct SC_Battle_SummonCard
{
	enum { s_nCode = s_nSC_BattleSummonCard };
	int _src;
	int _des;
};

//quest
struct SC_SyncQuestSession
{
	enum { s_nCode = s_nSC_SyncQuestSession };
	struct UNIT { DWORD key; DWORD val; };
	int qid;
	UNIT units[1];
};

struct SC_SyncQuestPersist
{
	enum { s_nCode = s_nSC_SyncQuestPersist };
	char data[1];
};

struct SC_QuestHistoryInit
{
	enum { s_nCode = s_nSC_QuestHistoryInit };
	int count;
	char data[1];
};



struct SC_OpenQuestPanel	//by ch
{
	enum { s_nCode = s_nSC_OpenQuestPanel };
	int valid;	// ������������Ƿ���Ч
	int qid;	// ����id
	int m_type;	// 1Ϊ��������, 2Ϊ�������
};



struct SC_SyncAvailQuests
{
	enum { s_nCode = s_nSC_SyncAvailQuests };
	int qids[1];
};
struct SC_AddQuest
{
	enum { s_nCode = s_nSC_AddQuest };
	struct UNIT { DWORD key; DWORD val; };
	int qid;
	UNIT sessions[1];
};
struct SC_LoadQuest
{
	enum { s_nCode = s_nSC_LoadQuest };
	struct UNIT { DWORD key; DWORD val; };
	int qid;
	UNIT sessions[1];
};

struct SC_RemoveQuest
{
	enum { s_nCode = s_nSC_RemoveQuest };
	int qid;
};

struct SC_QuestOverEffect
{
	enum { s_nCode = s_nSC_QuestOverEffect };
	DWORD effectID;
};
struct SC_QuestAchieveCheckResult
{
	enum { s_nCode = s_nSC_QuestAchieveCheckResult };
	int qid;
	int result;
};

//����

struct SC_BagOperation
{
	enum { s_nCode = s_nSC_BagOperation };
	BYTE byteSubCmd;		// ��Ʒ��������(ɾ����)
	UINT64 nPlayerID;		// ��ұ��

	SC_BagOperation()
	{
		byteSubCmd = 0;
		nPlayerID = 0;
	}
};

struct SC_BagUpdateResponse : public SC_BagOperation
{
	BYTE byteBagID;			// ����ID
	short nPos;				// ����λ��
	BYTE byteUpdateFlag;	// ���µ���Ʒ���
	BYTE nOperatorType;		// �������ͣ���Ʒ������־

	// ���Ҫ�������
	WORD nUpdateBufferSize;	// ���±��buffer����

	SC_BagUpdateResponse() : SC_BagOperation()
	{
		byteBagID = 0;
		nPos = 0;
		byteUpdateFlag = 0;
		nUpdateBufferSize = 0;
	}
};

struct SC_BagCellUpdateLock : public SC_BagOperation
{
	BYTE byteBagID;
	short nPos;
	BYTE nLockType;

	SC_BagCellUpdateLock()
		: SC_BagOperation()
		, byteBagID(0)
		, nPos(0)
		, nLockType(0)
	{
	}
};

struct SC_BagOperationErr
{
	enum { s_nCode = s_nSC_BagOperationErr };
	BYTE byteSubCmd;		// ��Ʒ��������(ɾ����)
	UINT64 nPlayerID;		// ��ұ��

	SC_BagOperationErr()
	{
		byteSubCmd = 0;
		nPlayerID = 0;
	}
};

struct SC_BagUseErrResponse : public SC_BagOperationErr
{
	struct E_RESULT
	{
		enum
		{
			FAILED_NONE,				// ��ʼֵ
			FAILED_LEVEL_LIMIT,			// ����ȼ�����
		};
	};
	BYTE result;			// ��������

	SC_BagUseErrResponse() : SC_BagOperationErr()
	{
		result = SC_BagUseErrResponse::E_RESULT::FAILED_NONE;
	}
};



struct CS_BusinessRequest
{
	enum { s_nCode = s_nCS_BusinessRequest };

	UINT64 nBusinessID;		// �������ID
};

struct SC_BusinessRequest
{
	enum { s_nCode = s_nSC_BusinessRequest };
	enum
	{
		enumBR_Suc,				// �ɹ�������������ʾ��
		enumBR_CloseMsgBox,		// �رս�����ʾ��
		enumBR_SeflDead,		// �Լ�����
		enumBR_OtherDead,		// �Է�����
		enumBR_SeflInFight,		// �Լ�����ս��״̬
		enumBR_OtherInFight,	// �Է�����ս��״̬
		enumBR_SeflInBusiness,	// �Լ����ڽ�����
		enumBR_OtherInBusiness,	// �Է����ڽ�����

		enumBR_OtherErr,		// ��������
	};

	BusinessKey key;
	BYTE nOpen;		// 0-����/�رս�����ʾ��
};

struct CS_BusinessConfirm
{
	enum { s_nCode = s_nCS_BusinessConfirm };

	BusinessKey key;
	bool bConfirm;			// ͬ�⽻��true �ܾ�����false
};

struct SC_BusinessConfirm
{
	enum { s_nCode = s_nSC_BusinessConfirm };
	enum
	{
		enumBC_Agree,				// ͬ�⽻��
		enumBC_Refuse,				// �ܾ�����
	};

	BusinessKey key;
	BYTE nConfirm;
};

struct CS_BusinessPutOneItem
{
	enum
	{
		enumBPOI_None,		// ��ʼ��
		enumBPOI_Add,		// ���
		enumBPOI_Remove,	// ɾ��

		enumBPOI_Count
	};
	enum { s_nCode = s_nCS_BusinessPutOneItem };

	BusinessKey key;
	BYTE nOperaFlag;		// ��������
	BYTE nBagID;			// ����ID
	short nBagPos;			// ����λ��
	BYTE nBusinessPos;		// ���׿�λ��
};

struct SC_BusinessPutOneItem
{
	enum { s_nCode = s_nSC_BusinessPutOneItem };

	BYTE nOperaFlag;		// ��������
	BYTE nBagID;			// ����ID
	short nBagPos;			// ����λ��
	BYTE nBusinessPos;		// ���׿�λ��
};

struct CS_BusinessLock
{
	enum { s_nCode = s_nCS_BusinessLock };

	BusinessKey key;
	bool bLockState;		// ����״̬ true ���� false δ����
};

struct SC_BusinessLock
{
	enum { s_nCode = s_nSC_BusinessLock };
	enum
	{
		enumBL_suc,			// �ɹ�
		enumBL_allEmtpy,	// ����Ʒ�ͽ�Ǯ����
	};

	BYTE nRet;
	bool bMyLockState;		// �Լ�������״̬
	bool bOtherLockState;	// �Է�������״̬
};

struct CS_BusinessStart
{
	enum { s_nCode = s_nCS_BusinessStart };

	BusinessKey key;
};

struct SC_BusinessStart
{
	enum { s_nCode = s_nSC_BusinessStart };

	bool bMyStartState;		// �Լ��Ľ��׿�ʼ״̬
	bool bOtherStartState;	// �Է��Ľ��׿�ʼ״̬
};

struct SC_BusinessFinish
{
	enum { s_nCode = s_nSC_BusinessFinish };

	short nRet;
};

struct CS_BusinessCancel
{
	enum { s_nCode = s_nCS_BusinessCancel };

	BusinessKey key;
};

struct SC_BusinessCancel
{
	enum { s_nCode = s_nSC_BusinessCancel };

	SC_BusinessCancel() { nReason = enumBC_None; }
	enum
	{
		enumBC_None,
		enumBC_DistanceBeyond,		// �������׷�Χ
		enumBC_OtherCancel,			// �Է�ȡ������
		enumBC_SelfCancel,			// �Լ�ȡ������
		enumBC_Cancel,				// ȡ������
	};

	BYTE nReason;
};

struct SC_StoreBuy
{
	enum { s_nCode = s_nSC_StoreBuy };
	BYTE nReason;
	UINT itemID;
	SHORT count;
	int	 cost;
};
struct CS_StoreBuy
{
	enum { s_nCode = s_nCS_StoreBuy };

	int nProductIdx;
	int nNum;
};

struct SC_BagAddResponse : public SC_BagOperation
{
	BYTE byteBagID;			// ����ID
	short nPos;				// λ��
	BYTE nOperatorType;		// �������ͣ���Ʒ������־
	BYTE nLockType;			// ��Ʒ��������

	// ���Ҫ�������
	WORD nItemBufferSize;	// ��Ʒ���л��󳤶�,�����Item��Ϣ

	SC_BagAddResponse() : SC_BagOperation()
	{
		byteBagID = 0;
		nPos = 0;
		nItemBufferSize = 0;
		nOperatorType = 0;
		nLockType = 0;
	}

	BYTE* GetItemBuffer() const { return (((BYTE*)this) + sizeof(SC_BagAddResponse)); }
};

struct SC_GenPlayerCard{
	enum { s_nCode = s_nSC_GenPlayerCard };
	int cardID[MAX_GEN_CARD_NUM];
	bool newFlag[MAX_GEN_CARD_NUM];
	int count;
	int money;
	SC_GenPlayerCard(){
		count = 0;
		money = 0;
		memset(cardID,0,sizeof(count));
	}
	bool AddCard(int id,bool bNew){
		if(count==MAX_GEN_CARD_NUM) return false;
		cardID[count] = id;
		newFlag[count++] = bNew;
		return true;
	}
	void AddMoney(int val){
		money += val;
	}
};

struct CS_AcceptQuest
{
	enum { s_nCode = s_nCS_AcceptQuest };
	int qid;
};

#pragma pack(pop)
