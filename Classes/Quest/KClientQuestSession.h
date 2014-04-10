
#pragma once

#include <map>
#include "CommonLogic/Quest/KQuestSession.h"
#include <System/Memory/KStepObjectPool.h>

class KClientQuestSession
	: public KQuestSession
	, public System::Memory::KPortableMemoryPool<KClientQuestSession,16>
{
public:
	KClientQuestSession(void);
	~KClientQuestSession(void);

public:
	// Set/Get quest session.
	BOOL SetSession(DWORD dwKey, int nValue);
	int GetSession(DWORD key, int defVal);
	void RemoveSession(DWORD key);

	int QueryData(char* buf);
	bool InitSessions(KQuestSessionItem* pSessionItem, int nCount);
	void ClearSessions(void);

	void Reset();
	void Destroy();

protected:
	typedef std::map<DWORD, int> SessionMap;
	typedef SessionMap::iterator SessionMapIter;
	typedef std::pair<DWORD, int> SessionPair;

	SessionMap m_Sessions;
};
