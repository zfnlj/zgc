#pragma once


#include <vector>
#include "AdvExecutor.h"


class  ParallelExecutorContainer : public AdvExecutorContainer
{
public:
	ParallelExecutorContainer();
	virtual ~ParallelExecutorContainer();
	virtual void Play(bool bLoop){};
	float GetTotalLifeTime() const;
	virtual float BreatheImp(float frameTime);
};
