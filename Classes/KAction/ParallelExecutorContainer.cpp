#include "ParallelExecutorContainer.h"



ParallelExecutorContainer::ParallelExecutorContainer()
{
}

ParallelExecutorContainer::~ParallelExecutorContainer()
{
	Stop();
}


float ParallelExecutorContainer::BreatheImp(float frameTime)
{
	if(mExecVect.count()==0)
	{
		mbPlaying = false;
		return 0;
	}
	if(!mbPlaying) //ִ���߽���
		return 0;
	bool IsPlayOver = true; //�Ƿ�ȫ�����Ž���

	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		if(!exec->IsOver())
		{
			BreatheExecutor(exec,frameTime);
			if(exec->IsOver())
				OnStopExecutor(exec);
			else
				IsPlayOver = false;
		}
	}
	if(IsPlayOver)
		OnStopContainer();
	return 0;
}

float ParallelExecutorContainer::GetTotalLifeTime() const
{
	float lifeTime = 0;
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(&mExecVect, pObj){
		AdvExecutor* exec = (AdvExecutor*)pObj;
		if(exec->GetLifeTime()>lifeTime)
			lifeTime = exec->GetLifeTime();
	}
	return lifeTime;
}