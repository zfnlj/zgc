/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include "System/KType.h"
#include "Inc/Lua/KLuaWrap.h"
namespace KWorldObjAbout
{
	class KPlayer;
	class KCharacter;
	class KNpc;
	class KTreasureCase;
	class KBattlePet;
	class KTransport;
	class KGroundItem;
	class KStall;
	class KBattlePetBaby;
}

namespace KSkillAbout
{
	class KSkillStaticData;
	class KEffectStaticData;
	class KEffectResult;
}

namespace KSceneAbout
{
	class KAreaTrigger;
	class KWorld;
}

class KQuestNew;

/**
 * 增加一个事件需要做的工作：
 *
 * 1 增加相应枚举（在KEventAbout::KEventID中）
 * 2 定义相应的事件参数（需要继承自KEventData，定义在本文件中）
 * 3 在产生事件的地方抛出事件
 *
 * 如果需要脚本支持，需要做额外如下工作：
 * 1 KToLua.cpp中导出事件枚举（可选）
 * 2 定义事件结构体的导出（BeginDefLuaClassNoCon）
 * 3 在KToLua.cpp中导出结构体
 * 4 在KToLua.cpp的EventTypeToLua中填充导出方式
 * 5 如果是副本脚本要用的话，在KLuaWorld::LuaWorld_OnEvent中(在KLuaWorldBash.h中定义)抛出到脚本
 */
namespace KEventAbout
{
	// 事件ID枚举
	enum KEventID
	{
		enumInvalidEvent = 0,				// 无效事件id

		enumServerEvent_Begin = 0,			// 服务器级消息
		enumServerEvent_PlayerEnterGame,	// 玩家刚刚进入游戏
		enumServerEvent_PlayerLeaveGame,	// 玩家将要离开游戏
		enumServerEvent_TeamDismiss,		// 队伍解散服务器级事件
		enumServerEvent_TeamMemberChange,	// 队员离开服务器级事件
		enumServerEvent_End,
		
		//////////////////////////////////////////////////////////////////////////
		enumWorldEvent_Begin = 1000,		// 世界级消息
		enumWorldEvent_PlayerEnterWorld,	// 玩家刚刚进入游戏
		enumWorldEvent_PlayerLeaveWorld,	// 玩家将要离开游戏
		enumWorldEvent_NpcEnterWorld,		// NPC刚刚进入游戏
		enumWorldEvent_NpcLeaveWorld,		// NPC将要离开游戏
		enumWorldEvent_TreasureCaseEnterWorld,		// 宝箱刚刚进入游戏
		enumWorldEvent_TreasureCaseLeaveWorld,		// 宝箱将要离开游戏
		enumWorldEvent_LuaWorldTimer,
		enumWorldEvent_CharacterBeKilled,	// 世界中角色被杀事件

		enumWorldEvent_CharacterEnterWorld,	// 有角色进入世界
		enumWorldEvent_CharacterLeaveWorld,	// 有角色离开世界

		enumWorldEvent_PlayerQuestOver,		// 世界中的玩家完成任务(成功,失败,放弃)
		enumWorldEvent_SendNPCFailed,		// 废弃

		enumWorldEvent_TeamDismiss,			// 队伍解散事件
		enumWorldEvent_TeamMemberLeave,		// 队伍有玩家离开

		enumWorldEvent_DestroyWorld,		// 销毁世界事件，在世界销毁前调用
		
		enumWorldEvent_KillMonster,
		enumWorldEvent_BattleFailed,
		enumWorldEvent_QuestSession,

		enumWorldEvent_End,
		
		//////////////////////////////////////////////////////////////////////////
		enumCharEvent_Begin = 2000,			// 角色级消息

		enumCharEvent_NpcDialog,			// 交任务NPC对话
		enumCharEvent_QuestOver,			// 任务完成(成功,失败,放弃)
		enumCharEvent_QuestTimer,

		enumCharEvent_GetItem,				// 获取物品
		enumCharEvent_DelItem,

		enumCharEvent_LevelUp,				// 升级
		enumCharEvent_Dialog,				// 任务中与NPC对话

		enumCharEvent_OnKilled,				// 角色杀人

		enumCharEvent_UseItem,				// 使用物品
		enumCharEvent_HpChanged,			// hp变化了

		enumCharEvent_QuestAccept,			// 角色接受一个任务

		enumCharEvent_PlayerOperate,		// 角色完成某项操作
		enumCharEvent_TeamChanged,			// 队伍变更

		// 任务组队事件
		enumCharEvent_TeamMateDialog,		// 任务中队友与NPC对话


		enumCharEvent_ReadNoteFinished,		// 阅读完成
		enumCharEvent_End,
	};

	inline bool IsServerEvent(KEventID id)
	{
		return id > enumServerEvent_Begin && id < enumServerEvent_End;
	}
	inline bool IsWorldEvent(KEventID id)
	{
		return id > enumWorldEvent_Begin && id < enumWorldEvent_End;
	}
	inline bool IsCharEvent(KEventID id)
	{
		return id > enumCharEvent_Begin && id < enumCharEvent_End;
	}

	// 事件数据，每个struct对应一个事件ID枚举
	struct KEventData
	{
		virtual KEventID GetEventID() const {return enumInvalidEvent;};
		bool CheckEvent(KEventID id) const
		{
			return GetEventID() == id;
		}
		// 根据flag得到角色
		KWorldObjAbout::KCharacter* GetCharByFlag(int flag) const;
		// 获得C1
		virtual KWorldObjAbout::KCharacter* GetC1() const {return NULL;}
		// 获得C2
		virtual KWorldObjAbout::KCharacter* GetC2() const {return GetC1();}
		// 得到buf字符串
		virtual int GetBufString(char* buf, int len) { return 0; }
	};
#define SET_C1(x) virtual KWorldObjAbout::KCharacter* GetC1() const { return (KWorldObjAbout::KCharacter*)x;}
#define SET_C2(x) virtual KWorldObjAbout::KCharacter* GetC2() const { return (KWorldObjAbout::KCharacter*)x;}
#define SET_ID(n) \
	virtual KEventID GetEventID() const {return n;};	\
	const static int EVENT_ID = n;

	struct KEDKillMonster : public KEventData
	{
		SET_ID(enumWorldEvent_KillMonster);
		KWorldObjAbout::KPlayer* m_pChar;
		KWorldObjAbout::KNpc* m_pDead;
		DWORD	m_nMonsterID;
		int GetBufString(char* buf, int len)
		{
			int pos = 0;
			if ( sizeof(DWORD) >= len )
			{
				return pos;
			}
			memcpy(buf, &m_nMonsterID, sizeof(DWORD) );
			pos += sizeof(DWORD);
			return pos;
		}

		SET_C1(m_pChar);

		BeginDefLuaClassNoCon(KEDKillMonster)
			InheritFrom(KEventData);
		DefMemberVar(m_pChar);
		DefMemberVar(m_pDead);
		DefMemberVar(m_nMonsterID);
		EndDef
	};

	struct KEDBattleFailed : public KEventData
	{
		SET_ID(enumWorldEvent_BattleFailed);
		DWORD	m_nID;
		int GetBufString(char* buf, int len)
		{
			int pos = 0;
			if ( sizeof(DWORD) >= len )
			{
				return pos;
			}
			memcpy(buf, &m_nID, sizeof(DWORD) );
			pos += sizeof(DWORD);
			return pos;
		}

		BeginDefLuaClassNoCon(KEDBattleFailed)
			InheritFrom(KEventData);
		DefMemberVar(m_nID);
		EndDef
	};

	struct KEDQuestSession : public KEventData
	{
		SET_ID(enumWorldEvent_QuestSession);
		KQuestNew* m_pQuest;
	};


#undef SET_ID
#undef SET_C1
#undef SET_C2
};
