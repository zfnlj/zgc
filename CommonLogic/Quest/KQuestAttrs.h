#ifndef _K_QUEST_ATTRS_H_
#define _K_QUEST_ATTRS_H_

#include "Inc/Lua/KLuaWrap.h"
#include "Inc/KCommonDefine.h"

// �����������Ƶ�StringID
const int QuestType_1	= 187;
const int QuestType_2	= 188;
const int QuestType_3	= 189;
const int QuestType_4	= 190;
const int QuestType_5	= 191;
const int QuestType_6	= 192;
const int QuestType_7	= 193;
const int QuestType_8	= 194;
const int QuestType_9	= 195;
const int QuestType_10	= 196;
const int QuestType_11	= 197;
const int QuestType_12	= 198;
const int QuestType_13	= 199;
const int QuestType_14	= 200;


const char Session_Status[]			= "status";
const char Session_Group[]			= "group";
const char Session_Timeout[]		= "timeout";
const char Session_TraceTick[]		= "traceTick";
const char Session_AcceptTime[]		= "acceptTime";
const char Session_AchieveCheck[]	= "achieveCheck";
const char Session_OverTimes[]		= "overTimes";
const char Session_ProfTime[]		= "profTime";
const char Session_CurProfID[]		= "curProfID";
const char Session_ProfAwardTime[]  = "profAwardTime";

enum enumQuestResult
{
	enum_quest_ok,
	enum_quest_fail,
	enum_quest_cancel,
};

enum enumQuestType
{
	enum_normal_quest = 1,		// ��������
	enum_zhixian_quest = 2,		// ֧������
	enum_daily_quest_begin = 2,	
	enum_Profession_quest = 3,	// ���������ݶ�3
	enum_daily_quest_end = 8,
	enum_quest_singlecamp = 8,	// ����Ӫ������
	enum_quest_teamcamp = 9,	// ����Ӫ������
	enum_daily_mutil_times_quest = 16,
	enum_dyntalk_quest_begin = 100,	// ��̬ˢ�µ�����ʼ
	enum_dyntalk_quest_triberandom,	// �����������
	enum_dyntalk_quest_end	 = 200,	
};

// ����4��״̬
enum KQuestStatusCode
{
	KQ_None = 0,			// δ����
	KQ_PreStepOver = 1,		// �������û�н���
	KQ_QuestOver = 2,		// ��������Ѿ�����
	KQ_QuestRuning = 3,		// �ѽ��ܣ�������
	KQ_QuestFailed = 4,		// �Ѿ�ʧ�ܣ���Ҫ�������½���
	KQ_QuestCancel = 5,		// �������
	//KQ_QuestEnd = 6,		// �������
};

// �ɾ�����ѡ����
enum KAchieveCheckedType
{
	NoneAchieveType		= 0,
	TeamAchieveType		= 1,
	SingleAchieveType	= 2,

	AchieveTypeCount	= 3,
};

enum enum_quest_flag
{
	quest_flag_ignore_rollback = 0,
	quest_flag_ignore_tire = 1,

	// �ڲ�ʹ��
	quest_flag_or_items = 24, // ������Ʒ�Ƕ�ѡһ��
};

enum KQuestShowMsgType
{
	KQM_QUESTINFOWND	= enumECG_Info,
	KQM_CHATWND			= enumECG_Info,
	KQM_MSGBOX			= enumECG_MessageBox,
	KQM_MAINFREAM		= enumECG_MainFrameInfomation,
	KQM_NPCACTION		= 5,
	KQM_STEPOVER,
	KQM_OPENUI,
	KQM_GROUPOVER,
	KQM_TargetUpdate,
	KQM_StatusUpdate,
	KQM_ACCEPTACHIEVEQUEST,
};

enum KOpenUICode
{
	UI_QUESTDIALOG,
	UI_MOBILETRADE,
	UI_BATTLEFIELD,
	UI_ACHIEVEDIALOG,
};

struct TraceLocation
{
	int mapID, x, y;
	/////////////////////////////////////////
	TraceLocation():mapID(0),x(0),y(0)
	{
	}
	TraceLocation(int m,int _x,int _y):mapID(m),x(_x),y(_y)
	{
	}
	/////////////////////////////////////////
	//BeginDefLuaClassNoCon(TraceLocation);
	//	DefMemberVar(mapID);
	//	DefMemberVar(x);
	//	DefMemberVar(y);
	//EndDef;
};

class KQuestAttr
{
public:
	enum
	{
		questId,
		nameId,
		talk,
		effect,
		type,
		level,
		view,
		abandon,
		reuse,
		share,
		acceptLevel,
		prevQuest,
		nextQuest,

		hardDegree,
		money,
		exp,
		items,
		questItems,
		desc,
		finishDesc,
		noFinishDesc,
		achieveId,
		achievement,
		userData,			//������Զ�������
		timeperiod,

		flag,				// �����־

		acceptDialogId,		// ��������Ի�id, 0��ʾ�޶Ի�
		finishDialogId,		// ��ɺ�����Ի�id

		acceptTopLimit,		// ����ȼ�����

		grade,				// ����Ƚף�Ĭ��Ϊ0,1Ϊ��ɫ��2Ϊ��ɫ��3Ϊ��ɫ��4Ϊ��ɫ

		extraDescStr,		// �������������ַ���
		extraDescBGP,		// ������������ͼƬ
		battleField,
		attrCount,	
	};

	// ����Ŀ������, for ��������
	enum {
		ProfTypeBegin,	// ��ʼ
		KillMonster,	// ɱ������,ɱnֻ��
		Delivery,		// ��������
		Demand,			// ��Ҫ����
		Pet,			// ��������
		Explore,		// ֪̽����
		Mill,			// ��������
		Career,			// ְҵ����

		proftypeCount,
	};

	static const char* m_nameTable[attrCount];
	static const char* GetAttrname(int attrId);
	static int GetAttrId(const char* attrName);
	static const char* m_typeTable[proftypeCount];
	static const char* GetTypeAttrname(int attrId);
	static int GetTypeAttrId(const char* attrName);
};

enum EOnQuestMessage
{
	EOQM_OnLoad, 
	EOQM_OnAccept,

	EOQM_MAX
};

enum EPosQuestMessage
{
	EPQM_Begin = 1, // ��CS_PostQuestMessage::Normal_Message_Start���,�������ͷ�ļ�
	EPQM_AfterInit,

	EPQM_End
	
};
extern void RegQuestMessageToLua();
#endif
