#ifndef _KBATTLECTRLBASE_H__
#define _KBATTLECTRLBASE_H__

#include "System/Singleton.h"
#include "KBattleGuy.h"
#include "assist/KBattleEventAssist.h"
class KBattleGuy;
class KCardInst;
class KQuestNew;
class KBattleCtrlBase 
{
public:
	struct BattleOp{
		BattleOp():_src(-1),_des(-1),_ok(false),_slot(-1){}
		bool IsOK(){ return _ok;}
		bool IsEmpty(){
			return (_src==-1);
		}
		void Empty(){
			_src = _des = _slot = -1;
			_ok = false;
		}
		int _src;
		int _des;
		int _slot;
		bool _ok;
	};
	enum BattleState{
		battle_null,
		battle_init,
		quest_battle_init,
		battle_select_turnplayer,
		battle_select_handcard,
		battle_turn_begin,
		battle_play,
		battle_turn_end,
		battle_game_end,
	};
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(void*);  
	virtual void update(float dt);
    // implement the "static node()" method manually
	KCardInst* GetCard(int id);
	void StartGame(BattleState st,KBattleGuy* p1,KBattleGuy* p2,KBattleGuy* p3=NULL,KBattleGuy* p4=NULL);
	void DoPlayerOp(int src,int des);
	void BattleInit();
	
	void onCard2Tomb(KCardInst* card);
	KBattleGuy* GetDefGuy();
	KBattleGuy* GetCurGuy(){ return m_CurPlayGuy;}
	BattleState& GetCurState(){ return m_state;}
	BattleOp& GetCurOp(){ return m_CurOp;}
	void DoSelectHandCard(UINT64 guyId,int n);
	KBattleGuy* GetGuy(UINT64 id);
	KBattleGuy* GetOtherGuy(UINT64 id);
	KBattleGuyList* GetGuyList(){ return &m_BattleGuyList;}
	void DoSelectBeginCard(KBattleGuy* guy,KCardInstList* arr);
	BattleState GetBattleState(){return m_state;}
	int GetCurSelSrc();
	void QuerySkillTarget(KCardInst*  skill,KCardInstList* arrGreen,KCardInstList* arrRed);
	bool QueryEnterFightTarget(KCardInst*  card,KCardInstList* arrGreen,KCardInstList* arrRed);
	void DoSelectCard(KCardInst*);
	KCardInst* GetCurSrcCard();
	void onCardSwitchOwner(KCardInst* pSrc,KCardInst* pDes);
	void OpDone();
	void OpSetSlot(int slot);
	void DoEndTurn();
	
	void PlayWithAI();
	void PlayQuestBattle(KQuestNew* pQuest);
	bool IsShowBack(KCardInst* card);
	bool IsMyCard(KCardInst* obj);
	bool IsMyTurn();
	FBattleGuy* GetCardOwner(KCardInst* card);
    FBattleGuy* GetCardOwner(int);
	KBattleDeck* GetCardDeck(KCardInst* card);
	void DoPlayerOpOK(int src,int des,int slot);
	KCardInstList* GetCardSet(KCardInst* card);
	FBattleGuy* GetOtherPlayer();
	FBattleGuy* GetMainPlayer();
	FBattleGuy* GetCurPlayer();
	void* GetWorld(){ return m_world;}
	bool IsServerSide();
	bool IsSelectCard(){ return m_bSelectCard;}
	void deserializeBattleGuy(UINT64 guyId,KMemoryStream* si,bool bMainPlayer);
	void deserializeDirty(UINT64 guyId,KMemoryStream* si);
	size_t serialize(KMemoryStream* si);
	bool deserialize(KMemoryStream* si);

	void onNetClose();

	
	bool m_bNetReady;
	void AddDramaElapsed(float val){ m_waitDramaElapsed+=val;}
	bool IsWaitDrama(){ return m_waitDramaElapsed>0.0f;}
	void AddCardEvtAbility(KCardInst* card,KAbilityStatic::Enum_When when);
	void DoCardEvtList(KCardInst* actor);
protected:
	BattleOp m_CurOp;
	BattleState m_state;
	float m_WaitTime;
	bool m_bSelectCard;
	void* m_world;
	KQuestNew* m_pBattleQuest;
	void UpdateBattleGuy(float dt);
	void SelectTurnPlayer();
	void TurnBegin();
	void PlayCard(float);
	void TurnEnd();
	void GameEnd();
	bool IsGameEnd();
	void SelectHandCard(float dt);
	void UpdateDirtyInfo();
	void QuestBattleInit(KQuestNew* pQuest);
	void StateJump(BattleState newState)
	{
		m_state = newState;
		m_bDirty = true;
	}
	void onSoldier2TombMsg(KCardInst* card);
	KBattleGuyList m_BattleGuyList;
	KBattleGuy* m_CurPlayGuy;
	KBattleGuy* m_pMainPlayer;
	bool m_bFirstTurn;
	bool m_bDirty;
	float m_waitDramaElapsed;
	KDoCardWhenAbilityList m_cardWhenList;
};

#endif // __HELLOWORLD_SCENE_H__
