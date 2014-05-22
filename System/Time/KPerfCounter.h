#ifndef _K_PERF_MONITOR_H_
#define _K_PERF_MONITOR_H_

#include "../KType.h"
#include "../KMacro.h"
#include "../Thread/KThread.h"
#include "../Log/log.h"
#include "../Collections/KString.h"
#include "../Collections/DynArray.h"


#if defined(_MONIT_PERFORMANCE_)

#define PERF_COUNTER(id) KPerfCounter _perf_counter_##id(id);

class KPerfCounter
{
public:
	KPerfCounter(int id);
	~KPerfCounter();

public:
	int m_id;
	UINT64 m_begCycle;
	UINT64 m_endCycle;
};

class KPerfStat : public System::Thread::KThread
{
public:
	enum { max_counter = 4096 };

public:
	struct StatRec
	{
		const char* name;
		UINT64 runTimes;
		UINT64 spendCycles;
		UINT64 lastTimes,lastCycles;
		void markDump();
		System::Collections::KString<512> dumpString() const;
	};
	class cmp_t
	{
	public:
		typedef StatRec* ptr;
		int operator () (const ptr& a, const ptr& b) const
		{
			INT64 x = a->spendCycles - a->lastCycles;
			INT64 y = b->spendCycles - b->lastCycles;
			if(y > x) return 1;
			else if(y == x) return 0;
			else return -1;
		}
	};
	typedef System::Collections::KDynSortedArray<StatRec*,cmp_t> RecordArray;

public:
	KPerfStat();
	~KPerfStat();

public:
	static KPerfStat& GetInstance();
	static int SetDumpInterval(int secs);

public:
	bool Register(int id, const char* name);

public:
	void Execute();

private:
	void stop();

public:
	TIME_T m_lastDumpTime;
	int m_dumpInterval;			// second/s
	
	StatRec m_recs[max_counter];
	StatRec m_buffer[max_counter];
	StatRec* m_buffer2[max_counter];

	volatile BOOL m_stopDesired;
	DirectFileLogger m_logger;
};
DECLARE_SIMPLE_TYPE(KPerfStat::StatRec);

#else
#define PERF_COUNTER(id)

class KPerfStat
{
public:
	static KPerfStat& GetInstance() { static KPerfStat x; return x; }
	static int SetDumpInterval(int secs) { return 60; }
	bool Register(int id, const char* name) { return true; }
};

#endif

#endif