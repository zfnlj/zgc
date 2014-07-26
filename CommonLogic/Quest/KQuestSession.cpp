
#include "KQuestSession.h"
#include "System/KPlatform.h"

bool isTmpSession_lua(const char* name)
{
	return (*(WORD*)name == tmpSessionFlag);
}

LuaString buildTmpSession_lua(const char* name)
{
	LuaString s((const char*)&tmpSessionFlag, 2);
	s.append(name);
	return s;
}

KQuestSession::~KQuestSession(void)
{
}

void KQuestSession::Destroy()
{
	this->Destroy_Delete();
}

void KQuestSession::EnableSync(BOOL enable)
{
	return;
}

BOOL KQuestSession::SetSession(const char* pszSession, int nValue)
{
	DWORD key = ELFHash(pszSession);
	return this->SetSession(key, nValue);
}

int KQuestSession::GetSession(const char* pszSession, int defVal)
{
	DWORD key = ELFHash(pszSession);
	return this->GetSession(key, defVal);
}

void KQuestSession::RemoveSession(const char* pszSession)
{
	DWORD key = ELFHash(pszSession);
	this->RemoveSession(key);
}

int KQuestSession::IncSession(const char* pszSession, int delta)
{
	int nValue = delta;
	DWORD key = ELFHash(pszSession);
	nValue += this->GetSession(key, 0);
	this->SetSession(key, nValue);
	return nValue;
}

void KQuestSession::Destroy_Delete()
{
	delete this;
}

KTmpSession::KTmpSession()
{

}

KTmpSession::~KTmpSession()
{

}

BOOL KTmpSession::SetSession(DWORD key, int val)
{
	KQuestSessionItem u = {key, static_cast<DWORD>(val)};
	int pos = m_sessions.find(u);

	if(pos < 0)
	{
		m_sessions.insert_unique(u);
	}
	else
	{
		m_sessions[pos].val = val;
	}
	return TRUE;
}

int KTmpSession::GetSession(DWORD key, int defVal)
{
	KQuestSessionItem u = {key, 0};
	int pos = m_sessions.find(u);
	if(pos < 0)
	{
		return defVal;
	}
	else
	{
		return m_sessions[pos].val;
	}
}

void KTmpSession::RemoveSession(DWORD key)
{
	KQuestSessionItem u = {key, 0};
	int pos = m_sessions.find(u);
	if(pos >= 0) m_sessions.erase(pos);
}

void KTmpSession::Reset()
{
	m_sessions.clear();
}
