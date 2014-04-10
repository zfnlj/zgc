#include <System/KPlatform.h>
#include <System/Misc/KDatetime.h>

#include "KQuestCommon.h"

int KQuestCommon::GetHourTime(time_t t)
{
	KLocalDatetime ldt = {2009,1,1,0,0,0};
	KDatetime dt2009(ldt);

	KDatetime dt(t);
	ldt = dt.GetLocalTime();
	ldt.minute = 0;
	ldt.second = 0;
	dt.SetLocalTime(ldt);
	
	INT64 secs = dt.Time() - dt2009.Time();
	return (int)(secs/(60*60));
}

time_t KQuestCommon::GetUniTime(int time_h)
{
	KLocalDatetime ldt = {2009,1,1,0,0,0};
	KDatetime dt2009(ldt);

	INT64 secs = time_h*60*60;
	dt2009.AddSecond((int)secs);

	return (time_t)dt2009.Time();
}

bool KQuestCommon::IsSameHours(time_t t1, time_t t2, int n)
{
	ASSERT(n != 0);
	return (t1 / (n*60)) == (t2 / (n*60));
}

bool KQuestCommon::IsSameDay(time_t t1, time_t t2)
{
	KDatetime dt1(t1);
	KDatetime dt2(t2);
	return dt1.ToLocalDays() == dt2.ToLocalDays();
}

bool KQuestCommon::IsSameWeek(time_t t1, time_t t2)
{
	tm _tm = {0, 0, 0, 31, 4, 110, 0, 0, 0};
	return ((t1 - mktime(&_tm)) / (3600 * 24 * 7)) == ((t2 - mktime(&_tm)) / (3600 * 24 * 7));
}

bool KQuestCommon::IsSameMonth(time_t t1, time_t t2)
{
	tm* pt1 = gmtime(&t1);
	tm* pt2 = gmtime(&t2);

	if (pt1->tm_year != pt2->tm_year)
		return FALSE;

	if (pt1->tm_mon != pt2->tm_mon)
		return FALSE;

	return TRUE;
}
