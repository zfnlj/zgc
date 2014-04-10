#include "AdvExecutor.h"
#include "KAction.h"

USING_NS_CC;

AdvExecutor::AdvExecutor()
{
}

void AdvExecutor::Reset(void)
{
	m_action = NULL;
	mDelayTime = 0;
	mbInfinite = false;
	mSurviveTime = 0.0f;
	mLoopNum = 1;
	mState= EXE_WAIT;
	ResetI();
}

void AdvExecutor::ResetI()
{
	mState = EXE_WAIT;
	mCurrentTime = 0;
	mCurLoop = 0;
}

AdvExecutor::~AdvExecutor()
{
}

void AdvExecutor::InitExecutor(float survive,unsigned char loopnum)
{
	mCurLoop = 0;
	mState = EXE_WAIT;
	mbInfinite = (loopnum==0)? true:false;
	mSurviveTime = survive;
	mLoopNum  = loopnum;
}

void AdvExecutor::Play(bool bLoop)
{
	mState = EXE_PLAYING;
	mCurrentTime = 0.0f;
	mbInfinite = bLoop;
	//mElapsedTimeFromEnd = 0;
}

void AdvExecutor::Stop()
{
	mState = EXE_END;
	mCurrentTime = 0.0f;
	mCurLoop = 0;
	//mElapsedTimeFromEnd = 0;
}

void AdvExecutor::PlayOver()
{
	mState = EXE_END;
	mCurLoop = mLoopNum;
	mCurrentTime = mSurviveTime;
}

bool AdvExecutor::IsPlay() const
{
	return (mState==EXE_PLAYING);
}

void AdvExecutor::SetCurrentTime(float fCurrentTime)
{
	if(IsPlay())
	{
		mCurrentTime = fCurrentTime;
	}
}

float AdvExecutor::GetLifeTime(void) const
{
	if(mbInfinite)
		return mSurviveTime;
	else
		return mSurviveTime*mLoopNum;
}

void AdvExecutor::MergeExecutor(AdvExecutor* exec)
{
	mCurrentTime = exec->mCurrentTime;
	if(mLoopNum !=0)
		mLoopNum +=1;

	exec->PlayOver();

}

bool AdvExecutor::StillAlive(float elapse) const 
{
	if(IsOver())
		return false;
	if(mbInfinite)
		return true;

	unsigned char tmpCurLoop = mCurLoop;
	float tmpCurTime = mCurrentTime;
	float remaintime = elapse;

	while(tmpCurLoop < mLoopNum)
	{
		if(mSurviveTime < remaintime)
		{
			remaintime -= mSurviveTime;
			tmpCurLoop++;
		}
		else
		{
			tmpCurTime += remaintime;
			if( tmpCurTime > mSurviveTime)
			{
				remaintime = tmpCurTime - mSurviveTime;
				tmpCurLoop ++;
				tmpCurTime =  0;
			}
			else
			{
				remaintime = 0;
				break;
			}
		}
	}
	return (tmpCurLoop < mLoopNum);
}

void AdvExecutor::LimitAlive(float val)
{
	if(RemainTime()<val)
		return;
	if(mState == EXE_WAIT||mState == EXE_END)
	{
		mState = EXE_END;
		return;
	}
	else
		mState = EXE_PLAYING;
	mbInfinite = false;
	mCurLoop = 0;
	mLoopNum = 1;
	//if(mSurviveTime > (mCurrentTime + MAX_FADEOUT_INTERVAL))
	
	mSurviveTime = mCurrentTime + val;
}

float AdvExecutor::RemainTime(void) const
{ 
	if(mbInfinite)
		return 100;
	else if(mCurLoop+1 < mLoopNum)
		return mSurviveTime;
	else 
		return (mSurviveTime - mCurrentTime);
}

void AdvExecutor::Init(KAction* action,float delay)
{
	m_action = action;
	mDelayTime = delay;
}
float AdvExecutor::Breathe(float elapse) //返回值为剩余的时间
{
	if(mDelayTime > elapse){
		mDelayTime -=elapse;
		return elapse;
	}
	elapse -= mDelayTime;
	mDelayTime = 0;

	if(IsOver())
		return 0;
	if(mState== EXE_WAIT)
		mState = EXE_START;
	else
		mState = EXE_PLAYING;

	float remaintime = elapse;
	if(mbInfinite)
	{
		if(mSurviveTime==0)
			mSurviveTime =1;
		while(mSurviveTime < remaintime)
			remaintime -= mSurviveTime;
		mCurrentTime += remaintime;
		if(mCurrentTime >=mSurviveTime)
			mCurrentTime -= mSurviveTime;
		remaintime = 0;
	}
	else
	{
		while(mCurLoop < mLoopNum)
		{
			if(mSurviveTime < remaintime)
			{
				remaintime -= mSurviveTime;
				mCurLoop++;
			}
			else
			{
				mCurrentTime += remaintime;
				if( mCurrentTime > mSurviveTime)
				{
					remaintime = mCurrentTime - mSurviveTime;
					mCurLoop ++;
					mCurrentTime =  0;
				}
				else
				{
					remaintime = 0;
					break;
				}
			}
		}
	}
	if(!mbInfinite)//非循环情况下，
	{
		if(mCurLoop >= mLoopNum)
		{
			mCurrentTime = mSurviveTime;
			mState = EXE_END;
		}
		else if(mCurrentTime == mSurviveTime && //如果走到尾巴，并且不是最后一轮，则置到开头
			(mCurLoop+1) <mLoopNum)
		{
			mCurrentTime = 0;
			mCurLoop++;
		}
	}
	return remaintime;
}

float  AdvExecutor::TotalLifeTime(void) const
{
	if(mLoopNum==0)
		return GetSurviveTime();
	else
		return GetSurviveTime()*mLoopNum;
}

KActor* AdvExecutor::GetActor()
{
	return m_action->GetActor();
}

AdvExecutorContainer::AdvExecutorContainer():
mbPlaying(true)
{
	
}

AdvExecutorContainer::~AdvExecutorContainer()
{
	ClearAll();
}


bool  AdvExecutorContainer::AddChild( AdvExecutor* pExecutor)
{
	if(NULL == pExecutor)
	{
		return false;
	}
	mExecVect.addObject(pExecutor);
	return true;
}

void  AdvExecutorContainer::RemoveChild( AdvExecutor* pExecutor )
{
	mExecVect.removeObject(pExecutor);
}

void  AdvExecutorContainer::ClearAll()
{
	mExecVect.removeAllObjects();
}

void AdvExecutorContainer::ResetI(bool bPlay)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		exec->ResetI();
	}
}

size_t  AdvExecutorContainer::getSize()
{
	return mExecVect.count();
}

void AdvExecutorContainer::OnPlayExecutor(AdvExecutor* pExec)
{
	pExec->OnPlay();
}

void AdvExecutorContainer::OnStopExecutor(AdvExecutor* pExec)
{
	pExec->OnStop();
}

void AdvExecutorContainer::OnStopContainer(void)
{
	mbPlaying = false;
}

void AdvExecutorContainer::StopAll(void)
{
	if(!IsPlaying())
		return;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		exec->Stop();
	}
	OnStopContainer();
}


void AdvExecutorContainer::Stop()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		exec->Stop();
	}
}

void AdvExecutorContainer::LimitAlive(float val)
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		if(exec->GetStat()!= AdvExecutor::EXE_END){
			exec->LimitAlive(val);
		}
	}
}

bool AdvExecutorContainer::IsInfinite()
{
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		if(exec->Infinite()) return true;
	}
	return false;
}

float AdvExecutorContainer::BreatheExecutor(AdvExecutor* pExec,float remainTime)
{
	AdvExecutor::EXE_STAT oldStat = pExec->GetStat();
	float ret =pExec->Breathe(remainTime);
	if(pExec->GetStat()==AdvExecutor::EXE_START ||
		(oldStat==AdvExecutor::EXE_WAIT &&  pExec->GetStat()==AdvExecutor::EXE_END))
		OnPlayExecutor(pExec); //新执行的对象
	return ret;
}

void AdvExecutorContainer::Reset()
{
	mbPlaying = true;
	ClearAll();
}

