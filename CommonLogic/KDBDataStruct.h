#pragma once
#include "Inc/KCommonDefine.h"
#include "Inc/KCommonStruct.h"
#include "KCacheObject.h"
#include "Quest/KQuestSession.h"
#include "System/Sync/KSync.h"

#pragma pack(push, 1)

template <typename BinaryStruct, int size> class DatabaseBinaryStruct : public BinaryStruct
{
	char __data__[size-sizeof(BinaryStruct)];
};


#define MAX_DECK_NUM 5
#define MAX_PLAYER_QUEST_NUM 5
struct tb_player_record
{
	enum MaskBit
	{
		_MONEY			= BIT(0),	//
		_NORMALITEM		= BIT(1),	//
		_CARDSTORE		= BIT(2),	//
		_CARDDECK0		= BIT(3),	//
		_CARDDECK1		= BIT(4),	
		_CARDDECK2		= BIT(5),
		_CARDDECK3		= BIT(6),
		_CARDDECK4		= BIT(7),
		_CURDECK        = BIT(8),
		_HERODATA		= BIT(9),
		_CRI			= BIT(10),
	};
	tb_player_record(){
		mUpdateMask = 0;
	}
	void init(){
		playerID = lastLoginIp = lastLoginTime = lastLogoutTime = money =  pvpVal = power = exp = 0;
		curDeck = -1;
		cardStore.actualLength = 0;
		heroData.actualLength = 0;
		for(int i=0;i<MAX_DECK_NUM;i++){
			cardDeck[i].actualLength=0;
		}
		normalItem.actualLength = 0;
	}
	int GetExp(){ return exp;}
	int GetCurDeck(){ return curDeck;}
	void SetCurDeck(int deck){
		curDeck = deck;
		updateMask(_CURDECK);
	}
	DWORD mUpdateMask;
	KObjectID playerID;
	DWORD lastLoginIp;
	int lastLoginTime;
	int lastLogoutTime;
	
	KDBBinary<2048> cardStore;
	KDBBinary<128> cardDeck[MAX_DECK_NUM];
	KDBBinary<400>  heroData; //10 hero
	int money;
	int curDeck;
	int pvpVal;
	int exp;
	int power;
	KDBBinary<800> normalItem;
	System::Sync::KSync_CS m_lock;

	void updateMask( DWORD v )
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=mUpdateMask|v;
	}
	void clearMask()
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=0;
	}
	void updateUnMask(DWORD v)
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=mUpdateMask&~v;
	}
};

struct tb_playerquest_record
{
	enum field_enum
	{
		f_playerId,
		f_Qid0,
		f_Qstate0,
		f_Qid1,
		f_Qstate1,
		f_Qid2,
		f_Qstate2,
		f_Qid3,
		f_Qstate3,
		f_Qid4,
		f_Qstate4,
		f_QHistory,
		f_QPersist,
	};
	enum MaskBit
	{
		_QID0			= BIT(0),	//
		_QSTATE0		= BIT(1),	//
		_QID1			= BIT(2),	//
		_QSTATE1		= BIT(3),	//
		_QID2			= BIT(4),	
		_QSTATE2		= BIT(5),
		_QID3			= BIT(6),
		_QSTATE3		= BIT(7),
		_QID4			= BIT(8),
		_QSTATE4        = BIT(9),
		_QHISTORY       = BIT(10),
		_QPERSIST       = BIT(11),
	};
	tb_playerquest_record(){
		mUpdateMask = 0;
	}
	DWORD mUpdateMask;
	void updateMask( DWORD v )
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=mUpdateMask|v;
	}
	void clearMask()
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=0;
	}
	void updateUnMask(DWORD v)
	{
		System::Sync::KSync_CSAuto ______(m_lock);
		mUpdateMask=mUpdateMask&~v;
	}
	int getQuestSlot(int id){
		int slot = -1;
		for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++)
		{
			if(qid[i]==id){
				slot = i;
				break;
			}
		}
		return slot;
	}
	void init(){
		memset(qid,0,sizeof(qid));
		for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++){
			qstate[i].actualLength = 0;
		}
		qhistory.actualLength = 0;
		qpersist.actualLength = 0;
	}
	KObjectID playerID;
	int qid[MAX_PLAYER_QUEST_NUM];
	KDBBinary<160> qstate[MAX_PLAYER_QUEST_NUM];
	KDBBinary<2000> qhistory;
	KDBBinary<1024> qpersist;
	System::Sync::KSync_CS m_lock;
};

struct tb_worldState_record
{
	enum
	{
		f_stateID,
		f_version,
		f_stateData,
		f_expireTime,
	};

	DWORD stateID;
	unsigned short version;
	KDBBinary<1024> stateData;
	int expireTime;
};

struct KDBBagItemUnit
{
	KDBBagItemUnit(DWORD id,DWORD count):_id(id),_count(count){}
	DWORD _id;
	DWORD _count;
};

enum
{
	Max_Quest_State = 2,
	Max_Player_Active_Quest = 3,
	Max_Player_Quest_History = 1000,
	Quest_Persist_Size = 2048,
};


struct KDBQuestState
{
	KQuestSessionItem states[Max_Quest_State];
};

struct KDBQuest
{
	DWORD qid;
	KDBBinary<sizeof(KDBQuestState)> m_state;
};

struct KDBQuestHistoryDataUnit
{
	DWORD qid : 24;
	DWORD val : 8;
	DWORD  time_h;
};

DECLARE_SIMPLE_TYPE(KDBQuestHistoryDataUnit);

#pragma pack(pop)
