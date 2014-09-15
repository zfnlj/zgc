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
#define MAX_PLAYER_QUEST_NUM 3
#define MAX_DAILY_AWARD_SLOT 3

struct Player_GameData{
	struct Tower{
		unsigned short _bShow;
		unsigned short _pos;
		int _lev;
		int _strong;
		int _lucky;
		int _resLucky;
		int _fate;
		int GetLev(){ return _lev;}
		void Reset(){
			memset(this,0,sizeof(Tower));
		}
	};
	int GetDailyStageLev(){ return _dailyStageLev;}
	Tower _tower;
	int _dailyStageLev;
	int _winDailyStageNum;
};


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
		_mUpdateMask = 0;
	}
	void init(){
		memset(&_gameData,0,sizeof(_gameData));
		_playerID = _lastLoginIp = _lastLoginTime = _lastLogoutTime = _money =  _pvpVal = _power = _exp = _mercy =0 ;
		_curDeck = -1;
		_cardStore.actualLength = 0;
		_heroData.actualLength = 0;
		for(int i=0;i<MAX_DECK_NUM;i++){
			_cardDeck[i].actualLength=0;
		}
		_normalItem.actualLength = 0;
	}
	int GetExp(){ return _exp;}
	int GetCurDeck(){ return _curDeck;}
	void SetCurDeck(int deck){
		_curDeck = deck;
		updateMask(_CURDECK);
	}
	DWORD _mUpdateMask;
	KObjectID _playerID;
	DWORD _lastLoginIp;
	int _lastLoginTime;
	int _lastLogoutTime;
	
	KDBBinary<2048> _cardStore;
	KDBBinary<128> _cardDeck[MAX_DECK_NUM];
	KDBBinary<400>  _heroData; //10 hero
	int _money;
	int _curDeck;
	int _pvpVal;
	int _exp;
	int _mercy;
	int _power;
	Player_GameData _gameData;

	KDBBinary<800> _normalItem;
	System::Sync::KSync_CS _lock;

	void updateMask( DWORD v )
	{
		while(_mUpdateMask&v){
			Sleep(10);
		}
		System::Sync::KSync_CSAuto ______(_lock);
		_mUpdateMask=_mUpdateMask|v;
	}
	void clearMask()
	{
		System::Sync::KSync_CSAuto ______(_lock);
		_mUpdateMask=0;
	}
	void updateUnMask(DWORD v)
	{
		System::Sync::KSync_CSAuto ______(_lock);
		_mUpdateMask=_mUpdateMask&~v;
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
		f_QHistory,
		f_QDaily,
	};
	enum MaskBit
	{
		_QID0			= BIT(0),	//
		_QSTATE0		= BIT(1),	//
		_QID1			= BIT(2),	//
		_QSTATE1		= BIT(3),	//
		_QID2			= BIT(4),	
		_QSTATE2		= BIT(5),
		_QHISTORY       = BIT(6),
		_QDAILY	        = BIT(7),
	};
	tb_playerquest_record(){
		_mUpdateMask = 0;
	}
	DWORD _mUpdateMask;
	void updateMask( DWORD v );
	void clearMask();
	void updateUnMask(DWORD v);
	int getQuestSlot(int id){
		int slot = -1;
		for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++)
		{
			if(_qid[i]==id){
				slot = i;
				break;
			}
		}
		return slot;
	}
	void init(){
		memset(_qid,0,sizeof(_qid));
		for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++){
			_qstate[i].actualLength = 0;
		}
		_qhistory.actualLength = 0;
		_qdaily.actualLength = 0;
	}
	KObjectID _playerID;
	int _qid[MAX_PLAYER_QUEST_NUM];
	KDBBinary<120> _qstate[MAX_PLAYER_QUEST_NUM];
	KDBBinary<1024> _qhistory;
	KDBBinary<40> _qdaily;
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
