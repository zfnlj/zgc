#include "KTouchDispatcher.h"
#include "UI/KClickCardMgr.h"

void KTouchDispatcher::init()
{
	CCTouchDispatcher::init();
}

void KTouchDispatcher::touchesEnded(CCSet* touches, CCEvent* pEvent)
{
	CCTouchDispatcher::touchesEnded(touches,pEvent);

	KClickCardMgr::getSingleton().onTouchEnd();
}