#ifndef _KBATTLECTRLASSIST_H
#define _KBATTLECTRLASSIST_H

#include "System/KType.h"
#include "../KCardInst.h"
class KBattleCtrlBase;
class KCardInst;
class KBattleGuy;
class KBattleCtrlAssist
{
public:
	KBattleCtrlAssist(){ }
	~KBattleCtrlAssist(){}

	static KCardInst* GetCard(KBattleCtrlBase*,int id);
	static KBattleGuy* GetOtherGuy(KBattleCtrlBase*,UINT64 id);
	static KBattleGuy* GetGuy(KBattleCtrlBase*,UINT64 id);

	static void QuerySkillTarget(KBattleCtrlBase* ctrl,KCardInst* skill,KCardInstList* arrGreen,KCardInstList* arrRed);
	static bool QueryEnterFightTarget(KBattleCtrlBase* ctrl,KCardInst*  card,KCardInstList* arrGreen,KCardInstList* arrRed);
	static KCardInstList* GetCardSet(KBattleCtrlBase* ctrl,KCardInst* card);
	
};

#endif // __HELLOWORLD_SCENE_H__
