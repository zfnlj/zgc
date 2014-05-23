#ifndef _K_QUEST_ATTRS_H_
#define _K_QUEST_ATTRS_H_

#include "Inc/Lua/KLuaWrap.h"
#include "Inc/KCommonDefine.h"

// 任务类型名称的StringID
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
	enum_normal_quest = 1,		// 主线任务
	enum_zhixian_quest = 2,		// 支线任务
	enum_daily_quest_begin = 2,	
	enum_Profession_quest = 3,	// 宗门任务暂定3
	enum_daily_quest_end = 8,
	enum_quest_singlecamp = 8,	// 单人营地任务
	enum_quest_teamcamp = 9,	// 多人营地任务
	enum_daily_mutil_times_quest = 16,
	enum_dyntalk_quest_begin = 100,	// 动态刷新的任务开始
	enum_dyntalk_quest_triberandom,	// 部落随机任务
	enum_dyntalk_quest_end	 = 200,	
};

// 任务4种状态
enum KQuestStatusCode
{
	KQ_None = 0,			// 未接受
	KQ_PreStepOver = 1,		// 任务完成没有交付
	KQ_QuestOver = 2,		// 任务完成已经交付
	KQ_QuestRuning = 3,		// 已接受，进行中
	KQ_QuestFailed = 4,		// 已经失败，需要放弃重新接受
	KQ_QuestCancel = 5,		// 任务放弃
	//KQ_QuestEnd = 6,		// 任务结束
};

// 成就任务选择结果
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

	// 内部使用
	quest_flag_or_items = 24, // 奖励物品是多选一的
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
		userData,			//任务的自定义数据
		timeperiod,

		flag,				// 任务标志

		acceptDialogId,		// 接受任务对话id, 0表示无对话
		finishDialogId,		// 完成后交任务对话id

		acceptTopLimit,		// 任务等级上限

		grade,				// 任务等阶，默认为0,1为白色，2为绿色，3为蓝色，4为紫色

		extraDescStr,		// 额外任务描述字符串
		extraDescBGP,		// 额外任务描述图片
		battleField,
		attrCount,	
	};

	// 任务目标类型, for 宗门任务
	enum {
		ProfTypeBegin,	// 开始
		KillMonster,	// 杀怪任务,杀n只怪
		Delivery,		// 送物任务
		Demand,			// 索要任务
		Pet,			// 宠物任务
		Explore,		// 探知任务
		Mill,			// 工坊任务
		Career,			// 职业任务

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
	EPQM_Begin = 1, // 与CS_PostQuestMessage::Normal_Message_Start相等,不想包含头文件
	EPQM_AfterInit,

	EPQM_End
	
};
extern void RegQuestMessageToLua();
#endif
