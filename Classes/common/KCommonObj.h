#ifndef _KCOMMON_OBJ_H
#define _KCOMMON_OBJ_H
#include "cocos2d.h"
#include "System/Random/KRandom.h"
#include <list>
#include "System/Misc/KStream.h"

extern System::Random::KRandom g_rnd;

typedef std::list<int > KIntegerList;
#define MAX_HERO_SKILL_NUM  3
#define MAX_HERO_LEV_NUM	5
extern KMsgOutputStream g_ostream;

void _CopyIntegerList(KIntegerList& src,KIntegerList& des,int rate =1);
void _RndIntegerList(KIntegerList& src,KIntegerList& des);
void _RndPick(KIntegerList& src,KIntegerList& des,int num);

int _RndNormal(int low,int height);
time_t _GetSystemTimeVal();
void _GetTimeInterval(time_t& t1,time_t& t2,tm& interval);
#endif