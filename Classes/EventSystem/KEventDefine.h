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
 * ����һ���¼���Ҫ���Ĺ�����
 *
 * 1 ������Ӧö�٣���KEventAbout::KEventID�У�
 * 2 ������Ӧ���¼���������Ҫ�̳���KEventData�������ڱ��ļ��У�
 * 3 �ڲ����¼��ĵط��׳��¼�
 *
 * �����Ҫ�ű�֧�֣���Ҫ���������¹�����
 * 1 KToLua.cpp�е����¼�ö�٣���ѡ��
 * 2 �����¼��ṹ��ĵ�����BeginDefLuaClassNoCon��
 * 3 ��KToLua.cpp�е����ṹ��
 * 4 ��KToLua.cpp��EventTypeToLua����䵼����ʽ
 * 5 ����Ǹ����ű�Ҫ�õĻ�����KLuaWorld::LuaWorld_OnEvent��(��KLuaWorldBash.h�ж���)�׳����ű�
 */
namespace KEventAbout
{
	// �¼�IDö��
	enum KEventID
	{
		enumInvalidEvent = 0,				// ��Ч�¼�id

		enumServerEvent_Begin = 0,			// ����������Ϣ
		enumServerEvent_PlayerEnterGame,	// ��Ҹոս�����Ϸ
		enumServerEvent_PlayerLeaveGame,	// ��ҽ�Ҫ�뿪��Ϸ
		enumServerEvent_TeamDismiss,		// �����ɢ���������¼�
		enumServerEvent_TeamMemberChange,	// ��Ա�뿪���������¼�
		enumServerEvent_End,
		
		//////////////////////////////////////////////////////////////////////////
		enumWorldEvent_Begin = 1000,		// ���缶��Ϣ
		enumWorldEvent_PlayerEnterWorld,	// ��Ҹոս�����Ϸ
		enumWorldEvent_PlayerLeaveWorld,	// ��ҽ�Ҫ�뿪��Ϸ
		enumWorldEvent_NpcEnterWorld,		// NPC�ոս�����Ϸ
		enumWorldEvent_NpcLeaveWorld,		// NPC��Ҫ�뿪��Ϸ
		enumWorldEvent_TreasureCaseEnterWorld,		// ����ոս�����Ϸ
		enumWorldEvent_TreasureCaseLeaveWorld,		// ���佫Ҫ�뿪��Ϸ
		enumWorldEvent_LuaWorldTimer,
		enumWorldEvent_CharacterBeKilled,	// �����н�ɫ��ɱ�¼�

		enumWorldEvent_CharacterEnterWorld,	// �н�ɫ��������
		enumWorldEvent_CharacterLeaveWorld,	// �н�ɫ�뿪����

		enumWorldEvent_PlayerQuestOver,		// �����е�����������(�ɹ�,ʧ��,����)
		enumWorldEvent_SendNPCFailed,		// ����

		enumWorldEvent_TeamDismiss,			// �����ɢ�¼�
		enumWorldEvent_TeamMemberLeave,		// ����������뿪

		enumWorldEvent_DestroyWorld,		// ���������¼�������������ǰ����
		
		enumWorldEvent_KillMonster,
		enumWorldEvent_BattleFailed,
		enumWorldEvent_QuestSession,

		enumWorldEvent_End,
		
		//////////////////////////////////////////////////////////////////////////
		enumCharEvent_Begin = 2000,			// ��ɫ����Ϣ

		enumCharEvent_NpcDialog,			// ������NPC�Ի�
		enumCharEvent_QuestOver,			// �������(�ɹ�,ʧ��,����)
		enumCharEvent_QuestTimer,

		enumCharEvent_GetItem,				// ��ȡ��Ʒ
		enumCharEvent_DelItem,

		enumCharEvent_LevelUp,				// ����
		enumCharEvent_Dialog,				// ��������NPC�Ի�

		enumCharEvent_OnKilled,				// ��ɫɱ��

		enumCharEvent_UseItem,				// ʹ����Ʒ
		enumCharEvent_HpChanged,			// hp�仯��

		enumCharEvent_QuestAccept,			// ��ɫ����һ������

		enumCharEvent_PlayerOperate,		// ��ɫ���ĳ�����
		enumCharEvent_TeamChanged,			// ������

		// ��������¼�
		enumCharEvent_TeamMateDialog,		// �����ж�����NPC�Ի�


		enumCharEvent_ReadNoteFinished,		// �Ķ����
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

	// �¼����ݣ�ÿ��struct��Ӧһ���¼�IDö��
	struct KEventData
	{
		virtual KEventID GetEventID() const {return enumInvalidEvent;};
		bool CheckEvent(KEventID id) const
		{
			return GetEventID() == id;
		}
		// ����flag�õ���ɫ
		KWorldObjAbout::KCharacter* GetCharByFlag(int flag) const;
		// ���C1
		virtual KWorldObjAbout::KCharacter* GetC1() const {return NULL;}
		// ���C2
		virtual KWorldObjAbout::KCharacter* GetC2() const {return GetC1();}
		// �õ�buf�ַ���
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
