#include "KPerfCounter.h"
#include "../Memory/KBuffer64k.h"
#include "../File/KFile.h"
#include <time.h>

#if defined(_MONIT_PERFORMANCE_)

#define g_perfStat KPerfStat::GetInstance()

KPerfCounter::KPerfCounter(int id)
	: m_id(id)
	, m_begCycle(rdtsc())
	, m_endCycle(m_begCycle)
{

}

KPerfCounter::~KPerfCounter()
{
	m_endCycle = rdtsc();
	INT64 n = m_endCycle - m_begCycle;
	g_perfStat.m_recs[m_id].runTimes += 1;
	g_perfStat.m_recs[m_id].spendCycles += n > 0 ? n:0;
}

void KPerfStat::StatRec::markDump()
{
	lastTimes = runTimes;
	lastCycles = spendCycles;
}

System::Collections::KString<512> KPerfStat::StatRec::dumpString() const
{
	char buf[512];
	static DWORD cpuSecCycles = GetCpuCyclesPerSecond();
	size_t n = sprintf_k(buf, sizeof(buf), "%s    %lld(%lld)    %04f(%04f)",
		name,
		runTimes,
		runTimes-lastTimes,
		(double)spendCycles/cpuSecCycles,
		(double)(spendCycles-lastCycles)/cpuSecCycles);
	return System::Collections::KString<512>(buf, n);
}

KPerfStat::KPerfStat()
	: m_lastDumpTime(0)
	, m_stopDesired(FALSE)
	, m_dumpInterval(60)
{
	memset(&m_recs, 0, sizeof(m_recs));
	// open log file
	char modfile[512];
	char cleanname[128];
	GetModuleFileName_k(NULL, modfile, sizeof(modfile));
	System::File::KFileUtil::cleanname_r(modfile, cleanname);
	System::Collections::KString<256> logfile("./Log/");
	logfile.append(cleanname);
	logfile.append("_perf.log");
	m_logger.Open(logfile.c_str());

	// start thread
	this->Run();
}

KPerfStat::~KPerfStat()
{
	this->stop();
}

KPerfStat& KPerfStat::GetInstance()
{
	static KPerfStat* p = new KPerfStat();
	return *p;
}

int KPerfStat::SetDumpInterval(int secs)
{
	int old = g_perfStat.m_dumpInterval;
	g_perfStat.m_dumpInterval = secs;
	return old;
}

bool KPerfStat::Register(int id, const char* name)
{
	if(id < 0 || id >= max_counter) return false;
	ASSERT(m_recs[id].name == NULL);

	StatRec& rec = m_recs[id];
	rec.name = KBuffer64k::WriteData(name, (int)strlen(name)+1);
	rec.runTimes = 0;
	rec.spendCycles = 0;
	rec.lastTimes = 0;
	rec.lastCycles = 0;
	
	return true;
}

void KPerfStat::Execute()
{
	RecordArray arr;
	arr.attach(m_buffer2, max_counter, 0);

	if(!m_lastDumpTime)
	{
		m_lastDumpTime = (TIME_T)time(NULL);
	}

	while(!m_stopDesired)
	{
		TIME_T now = (TIME_T)time(NULL);
		if(now < m_lastDumpTime + m_dumpInterval) { Sleep(100); continue; }
		m_lastDumpTime = (TIME_T)time(NULL);

		arr.clear();
		memcpy(&m_buffer, &m_recs, sizeof(m_recs));
		
		for(int i=0; i<max_counter; i++)
		{
			StatRec& rec = m_buffer[i]; m_recs[i].markDump();
			if(rec.runTimes > rec.lastTimes)
				arr.insert(&rec);
		}

		int n = arr.size();
		if(n)
		{
			m_logger.WriteLog("start dump perf-stat >>>>>>>>>>>>>>>>>>>>>>>>>");
			for(int i=0; i<n; i++)
			{
				StatRec& rec = *arr[i];
				m_logger.WriteLog("no.%d	%s", i, rec.dumpString().c_str());
			}
			m_logger.WriteLog("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
		}
	}
}

void KPerfStat::stop()
{
	m_stopDesired = TRUE;
	this->WaitFor();
}

#endif