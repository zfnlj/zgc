#ifndef _KSCRIPTAICTRL_H
#define _KSCRIPTAICTRL_H

#include "Inc/Lua/KLuaWrap.h"

class KBattleAI;
class KBattleCtrlBase;
class KCardInst;
class KScriptAICtrl
{
public:

	static KScriptAICtrl* getInstance(void);

	KScriptAICtrl(){ }
	~KScriptAICtrl(){}

	KBattleCtrlBase* m_pBattleCtrl;
	KBattleAI* m_pAI;
	int m_Target;
	float m_Val;
	void Reset(KBattleCtrlBase* ctrl,KBattleAI* pAI){
		m_pBattleCtrl = ctrl;
		m_pAI = pAI;
		m_Target = 0;
		m_Val= 0.0f;
	}

	float CalcUseTargetSkillGood(KCardInst* pCard,int abilityId);

BeginDefLuaClass(KScriptAICtrl)
	DefMemberFunc(Reset)
	DefMemberFunc(CalcUseTargetSkillGood)
	//DefMemberFunc(GetMyHero);
EndDef
};

#endif // __HELLOWORLD_SCENE_H__
