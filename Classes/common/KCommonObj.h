#ifndef _KCOMMON_OBJ_H
#define _KCOMMON_OBJ_H
#include "cocos2d.h"
#include "System/Random/KRandom.h"
#include <list>
#include "System/Misc/KStream.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "platform/win32/KPlatformDelegate.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "platform/ios/KPlatformDelegate.h"
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_IOS

extern System::Random::KRandom g_rnd;

typedef std::list<int > KIntegerList;
#define MAX_HERO_SKILL_NUM  3

extern KMsgOutputStream g_ostream;

void _CopyIntegerList(KIntegerList& src,KIntegerList& des,int rate =1);
void _RndIntegerList(KIntegerList& src,KIntegerList& des);
void _RndPick(KIntegerList& src,KIntegerList& des,int num);

time_t _GetSystemTimeVal();
void _GetTimeInterval(time_t& t1,time_t& t2,tm& interval);
#endif