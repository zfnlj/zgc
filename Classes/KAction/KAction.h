//////////////////////////////////////////////////////////////////////////////
//
// FileName	:	KAction.h
// Creator	:	Dolaham
// Date		:	2008-4-28 13:58:32
// Comment	:	KAction类
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "KEffectVectExecutor.h"
#include "cocoa/CCObject.h"
#include "AdvExecutor.h"
#include "KActionStaticMgr.h"
#include "KAffectorVectExecutor.h"
#include "../common/KCommonObj.h"
//#include "KSoundEffectVectExecutor.h"

class KActionStatic;
#include <string>
//////////////////////////////////////////////////////////////////////////
// class KAction

class KActionMgr;
USING_NS_CC;
class KAction : public CCObject
{
private:
	K3DActionParam	 m_Param;   //输入参数
	std::string m_Name;                  //动作名 
	KActionMgr* m_Mgr; // ACTION所属的角色
	int m_key;
	bool m_bPlay;                   // 
	bool m_bLoop;
	float  m_delayTime;             //用于延迟运行。
	const KActionStatic*  m_pActionStatic; //指向当前使用的静态动作信息
	KAction* m_parent;
	//CCArray m_EffectMgr; //特效管理器
	
	//CCArray m_SoundMgr; // 音效 add by DuanGuangxiang
	KIntegerList m_finishAction;
	float BreatheDelayTime(float timeElapsed);
public:
	KAction();
	static KAction* create(KActionStatic*,KActionMgr*,K3DActionParam*,int key);
	void Init(KActionStatic*,KActionMgr*,K3DActionParam*,int);
	virtual ~KAction();
	void AddFinishedAction(int id);// watch other action....
	bool IsActonFinished(int id);
	void SetParent(KAction* parent){ CC_SAFE_RETAIN(parent);m_parent = parent;}
public:
	void UpdataPlayStatus();    //更新Action的m_bPlay标记
	const KActionStatic* GetStatic(){ return m_pActionStatic;}
	KActionStatic::classEnum GetClass(void)const{ return m_pActionStatic->GetClass();}
	static int GetTotalClassActionNum(){ return g_totalClassAction;}
public:

	KAffectorVectExecutor m_AffectMgr;//效果管理器;

	bool IsPlay();

	const char* GetName(void) { return m_Name.c_str();}
	KActor* GetActor();
	bool IsLoop(void){ return m_bLoop;}   //是否循环
	bool Stop(void);
	void LimitAlive(float val=-1); //限制生存
	void breathe(float timeElapsed);
	K3DActionParam* GetParam(){ return &m_Param;}
	void Reset();
	void SetDelayTime(float delay);
	static int g_totalClassAction;
	float m_elapsed;
};