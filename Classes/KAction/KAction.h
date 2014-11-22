//////////////////////////////////////////////////////////////////////////////
//
// FileName	:	KAction.h
// Creator	:	Dolaham
// Date		:	2008-4-28 13:58:32
// Comment	:	KAction��
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
	K3DActionParam	 m_Param;   //�������
	std::string m_Name;                  //������ 
	KActionMgr* m_Mgr; // ACTION�����Ľ�ɫ
	int m_key;
	bool m_bPlay;                   // 
	bool m_bLoop;
	float  m_delayTime;             //�����ӳ����С�
	const KActionStatic*  m_pActionStatic; //ָ��ǰʹ�õľ�̬������Ϣ
	KAction* m_parent;
	//CCArray m_EffectMgr; //��Ч������
	
	//CCArray m_SoundMgr; // ��Ч add by DuanGuangxiang
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
	void UpdataPlayStatus();    //����Action��m_bPlay���
	const KActionStatic* GetStatic(){ return m_pActionStatic;}
	KActionStatic::classEnum GetClass(void)const{ return m_pActionStatic->GetClass();}
	static int GetTotalClassActionNum(){ return g_totalClassAction;}
public:

	KAffectorVectExecutor m_AffectMgr;//Ч��������;

	bool IsPlay();

	const char* GetName(void) { return m_Name.c_str();}
	KActor* GetActor();
	bool IsLoop(void){ return m_bLoop;}   //�Ƿ�ѭ��
	bool Stop(void);
	void LimitAlive(float val=-1); //��������
	void breathe(float timeElapsed);
	K3DActionParam* GetParam(){ return &m_Param;}
	void Reset();
	void SetDelayTime(float delay);
	static int g_totalClassAction;
	float m_elapsed;
};