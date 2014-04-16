#ifndef _KTOUCHDISPATCHER_H
#define _KTOUCHDISPATCHER_H
#include "cocos2d.h"
#include "System/Singleton.h"


USING_NS_CC;

class KTouchDispatcher : public CCTouchDispatcher
{
public :
	void init();

	virtual void touchesEnded(CCSet* touches, CCEvent* pEvent);
};

#endif