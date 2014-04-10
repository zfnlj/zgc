#ifndef _KCLIENTBATTLECTRL_H__
#define _KCLIENTBATTLECTRL_H__

#include "KBattleCtrlBase.h"

class KClientBattleCtrl : public KBattleCtrlBase
{
public:
	KClientBattleCtrl(){ m_pBattleQuest = NULL;}
	~KClientBattleCtrl(){}
	void onTurnBegin(KBattleGuy*);
	
};

#endif // __HELLOWORLD_SCENE_H__
