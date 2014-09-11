#ifndef __KGAMEDEF_H__
#define __KGAMEDEF_H__

#define MAX_ACTION_TARGET_NUM 12


#define MAX_GAME_PLAY_CARD 31

class KCardInst;
class KAbilityStatic;
class FBattleGuy;
struct strCardDuelResult{   //卡片对打的结果
	KCardInst* _atker;	  //进攻方
	KCardInst* _defender;//防御方
	int _val1; //进攻方受到伤害
	int _val2; //防御方受到伤害
};

struct strSummonCardResult{
	KCardInst* _src;
	KCardInst* _des;
};
struct strReplaceCardResult{
	KCardInst* _src;
	KCardInst* _des;
};


struct strCardAbilityResult{   //卡片对打的结果
	strCardAbilityResult();
	int  _src;	  //进攻方
	int _actor;
	int _destArr[MAX_ACTION_TARGET_NUM];
	int _destValArr[MAX_ACTION_TARGET_NUM];
	void SetDestVal(int dest,int val);
	void init(int actor,int src,KAbilityStatic* ability);
	KAbilityStatic* _pAbility;
};

struct strCardModifyBufResult{   //卡片增减buf
	KCardInst* _card;	  //目标卡片，
	KAbilityStatic* _pBuf;//buf
};

/**
 * 任务增删消息
 */
struct KQuestChangeParam
{
	enum E_CHANGE
	{
		REMOVE = 0,		// 删除
		ADD = 1,		// 增加
	};

	enum E_TYPE
	{
		ACCEPTED = 0,			// 已接受过的任务,主线
		CAN_ACCEPT = 1,			// 未接受但可接受的
		ACCEPTED_ZHIXIAN = 2,	// 已接受过的任务,支线
		ACCEPTED_OTHER = 3,		// 已接受过的任务,除了主线和支线
	};

	int change;			// 增删类型
	int type;			// 任务已接或者可接,主线或支线
	int qid;			// 任务id
};

enum Scene_type{
		scene_battle, //战役
		scene_daily,  //日常
		scene_tower,  //魔窟
};

struct strGameResult{
	strGameResult():_winner(0),_questId(0),_money(0),_exp(0),_type(scene_battle){}
	FBattleGuy* _winner;
	int _questId;
	int _money;
	int _exp;
	Scene_type _type;
};

#endif // __HELLOWORLD_SCENE_H__
