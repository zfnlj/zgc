
#pragma once
#include "cocos2d.h"


USING_NS_CC;
class KActionElemBase : public CCObject
{
public:
	KActionElemBase()
	{	
	}
	virtual ~KActionElemBase(){}
	virtual void* GetBaseData()=0;
	virtual int GetBaseSize()=0;
	virtual void* GetExData()=0;
	virtual int GetExSize()=0;
};
