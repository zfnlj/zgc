
#ifndef _K_QUEST_SESSION_H_
#define _K_QUEST_SESSION_H_

#include <System/Memory/KStepObjectPool.h>
#include "Inc/Lua/KLuaWrap.h"

const WORD tmpSessionFlag = (WORD)'T' + (((WORD)'&')<<8);

bool isTmpSession_lua(const char* name);
LuaString buildTmpSession_lua(const char* name);

inline bool isTmpSession(const char* name)
{
	return (*(WORD*)name == tmpSessionFlag);
}

inline LuaString buildTmpSession(const char* name)
{
	LuaString s((const char*)&tmpSessionFlag, 2);
	s.append(name);
	return s;
}

struct KSessionUnit
{
	DWORD key;
	DWORD val;
};
typedef KSessionUnit KQuestSessionItem;

class SessionCmp
{
public:
	BOOL operator () (const KSessionUnit& a, const KSessionUnit& b) const
	{
		return a.key < b.key;
	}
};


typedef System::Collections::DynSortedArray<KSessionUnit, SessionCmp> KQuestSessions;

class KQuestSession
{
public:
	virtual ~KQuestSession(void);
	virtual BOOL SetSession(DWORD key, int val) = 0;
	virtual int GetSession(DWORD key, int defVal) = 0;
	virtual void RemoveSession(DWORD key) = 0;
	virtual void Reset() = 0;
	virtual void Destroy();
	virtual void EnableSync(BOOL enable);

public:
	// Set/Get quest session.
	BOOL SetSession(const char* pszSession, int nValue);
	int GetSession(const char* pszSession, int defVal);
	void RemoveSession(const char* pszSession);
	int IncSession(const char* pszSession, int delta=1);

protected:
	void Destroy_Delete();
};

class KTmpSession : public KQuestSession
{
public:
	KTmpSession();
	~KTmpSession();

public:
	BOOL SetSession(DWORD key, int val);
	int  GetSession(DWORD key, int defVal);
	void RemoveSession(DWORD key);
	void Reset();

public:
	KQuestSessions m_sessions;
};

#endif
