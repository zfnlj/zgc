#ifndef _K_QUEST_COMMON_H_
#define _K_QUEST_COMMON_H_

#include <System/KType.h>
#include <System/KMacro.h>

class KQuestCommon
{
public:
	KQuestCommon();
	~KQuestCommon();

public:
	static int GetHourTime(time_t t);
	static time_t GetUniTime(int time_h);
	static bool IsSameHours(time_t t1, time_t t2, int n);
	static bool IsSameDay(time_t t1, time_t t2);
	static bool IsSameWeek(time_t t1, time_t t2);
	static bool IsSameMonth(time_t t1, time_t t2);
};

#endif
