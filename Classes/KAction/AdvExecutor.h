#pragma once

#include "cocoa/CCObject.h"
#include "cocoa/CCArray.h"

USING_NS_CC;

class KActor;
class KAction;
class  AdvExecutor : public CCObject
{
public:
	enum EXE_STAT{
		EXE_WAIT, //δ��ʼ
		EXE_START, //��ʼ
		EXE_PLAYING,//������
		EXE_END,    //����
	};

	AdvExecutor();
	virtual ~AdvExecutor();
	virtual void Play(bool bLoop);
	virtual void Stop();
	virtual void OnPlay(){}
	virtual void OnStop(){}
	virtual void PlayOver();
	virtual float Breathe(float frameTime);
	virtual bool IsPlay() const;
	virtual void SetCurrentTime(float fCurrentTime);
	virtual void Reset(void);
	virtual float GetLifeTime(void) const;                        
	float   GetSurviveTime(void) const {return mSurviveTime;} //һ�ֵ�ʱ��
	void   SetSurviveTime(float survive){ mSurviveTime = survive;}
	float   TotalLifeTime(void) const;                              //������ʱ��    
	bool    Infinite(void) const {return mbInfinite;}

	float   CurrentTime(void)const{ return mCurrentTime;}
	float   RemainTime(void) const;
	bool    IsOver(void)const {return mState ==EXE_END;}
	void    InitExecutor(float survive,unsigned char loopnum); //loop= 0����ѭ��
	void	ResetI();
	unsigned char   LoopNum(void) const {return mLoopNum;}
	void    SetStat(EXE_STAT state) { mState = state;}
	EXE_STAT GetStat(void)const{ return mState;}
	bool	StillAlive(float elapse) const; //����elapseʱ����Ƿ񻹻���
	void    MergeExecutor(AdvExecutor* exec);//��ͬexecutor�ĺϲ�
	virtual void	LimitAlive(float val);
	virtual void OnFresh(AdvExecutor*){}
	virtual void Init(KAction* action,float delay);
	KActor* GetActor();
protected:
	CCArray AdvExecutorList;

	EXE_STAT mState;
	unsigned char   mLoopNum;
	unsigned char   mCurLoop;
	bool    mbInfinite;      //����ѭ��
	float   mCurrentTime;//time elapsed from the point sound begin.
	float   mSurviveTime;//total time;
	float   mDelayTime;
	KAction* m_action;
	//float	mElapsedTimeFromEnd;//time elapsed from the point sound last ended.
};

class  AdvExecutorContainer
{
public:

	AdvExecutorContainer();
	virtual ~AdvExecutorContainer();
	bool	AddChild( AdvExecutor* pExecutor);
	void	RemoveChild(AdvExecutor* pExecutor );
	void	ClearAll();
	size_t  getSize();
	//bool	GetItem(size_t iIndex,AdvExecutor*);
	//bool    SetItem(size_t iIndex,AdvExecutor*);
	virtual void OnPlayExecutor(AdvExecutor*); //
	virtual void OnStopExecutor(AdvExecutor*);
	virtual void OnStopContainer(void);//�������
	void	ResetI(bool bPlay =true);
	bool    IsPlaying(void){ return mbPlaying;}
	virtual void    StopAll(void);
	virtual void Stop();
	virtual void LimitAlive(float val);
	float   BreatheExecutor(AdvExecutor* pExec,float remainTime);
	void Reset();
	bool IsInfinite();
protected:
	CCArray mExecVect;
	CCArray mListeners;
	bool    mbPlaying;

}
;
