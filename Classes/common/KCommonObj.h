#ifndef _KCOMMON_OBJ_H
#define _KCOMMON_OBJ_H
#include "System/Random/KRandom.h"
#include <list>
#include "System/Misc/KStream.h"
extern System::Random::KRandom g_rnd;

typedef std::list<int > KIntegerList;

extern KMsgOutputStream g_ostream;

#endif