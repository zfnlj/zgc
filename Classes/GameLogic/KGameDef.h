#ifndef __KGAMEDEF_H__
#define __KGAMEDEF_H__

#define MAX_ACTION_TARGET_NUM 12


#define MAX_GAME_PLAY_CARD 31

class KCardInst;
class KAbilityStatic;
class FBattleGuy;
struct strCardDuelResult{   //��Ƭ�Դ�Ľ��
	KCardInst* _atker;	  //������
	KCardInst* _defender;//������
	int _val1; //�������ܵ��˺�
	int _val2; //�������ܵ��˺�
};

struct strSummonCardResult{
	KCardInst* _src;
	KCardInst* _des;
};
struct strReplaceCardResult{
	KCardInst* _src;
	KCardInst* _des;
};


struct strCardAbilityResult{   //��Ƭ�Դ�Ľ��
	strCardAbilityResult();
	int  _src;	  //������
	int _actor;
	int _destArr[MAX_ACTION_TARGET_NUM];
	int _destValArr[MAX_ACTION_TARGET_NUM];
	void SetDestVal(int dest,int val);
	void init(int actor,int src,KAbilityStatic* ability);
	KAbilityStatic* _pAbility;
};

struct strCardModifyBufResult{   //��Ƭ����buf
	KCardInst* _card;	  //Ŀ�꿨Ƭ��
	KAbilityStatic* _pBuf;//buf
};

/**
 * ������ɾ��Ϣ
 */
struct KQuestChangeParam
{
	enum E_CHANGE
	{
		REMOVE = 0,		// ɾ��
		ADD = 1,		// ����
	};

	enum E_TYPE
	{
		ACCEPTED = 0,			// �ѽ��ܹ�������,����
		CAN_ACCEPT = 1,			// δ���ܵ��ɽ��ܵ�
		ACCEPTED_ZHIXIAN = 2,	// �ѽ��ܹ�������,֧��
		ACCEPTED_OTHER = 3,		// �ѽ��ܹ�������,�������ߺ�֧��
	};

	int change;			// ��ɾ����
	int type;			// �����ѽӻ��߿ɽ�,���߻�֧��
	int qid;			// ����id
};

enum Scene_type{
		scene_battle, //ս��
		scene_daily,  //�ճ�
		scene_tower,  //ħ��
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
