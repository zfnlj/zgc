#ifndef _KCLIENTBATTLECTRL_H__
#define _KCLIENTBATTLECTRL_H__

#include "KBattleCtrlBase.h"

class KClientBattleCtrl : public KBattleCtrlBase
{
public:

	static KClientBattleCtrl* getInstance(void);

	KClientBattleCtrl(){ m_pBattleQuest = NULL;}
	~KClientBattleCtrl(){}
	void onTurnBegin(KBattleGuy*);


BeginDefLuaClass(KClientBattleCtrl)
	DefMemberFunc(GetMyHero);
EndDef
};

#endif // __HELLOWORLD_SCENE_H__
