
#include "KClientQuestSession.h"

KClientQuestSession::KClientQuestSession(void)
{
}

KClientQuestSession::~KClientQuestSession(void)
{
	ClearSessions();
}

BOOL KClientQuestSession::SetSession(DWORD key, int nValue)
{
	m_Sessions[key] = nValue;
	return TRUE;
}

BOOL KClientQuestSession::GetSession(DWORD key, int defVal)
{
	SessionMapIter it = m_Sessions.find(key);
	if (it == m_Sessions.end())
	{
		return defVal;
	}
	return it->second;
}

void KClientQuestSession::RemoveSession(DWORD key)
{
	m_Sessions.erase(key);
}

bool KClientQuestSession::InitSessions(KQuestSessionItem* pSessionItem, int nCount)
{
	for (int i=0; i<nCount; i++)
	{
		KQuestSessionItem& si = pSessionItem[i];
		m_Sessions[si.key] = si.val;
	}
	return true;
}

void KClientQuestSession::ClearSessions(void)
{
	m_Sessions.clear();
}

void KClientQuestSession::Reset()
{
	m_Sessions.clear();
}

void KClientQuestSession::Destroy()
{
	this->Reset();
	KClientQuestSession::Free(this);
}

int KClientQuestSession::QueryData(char* buf)
{
	int len = m_Sessions.size()*sizeof(KQuestSessionItem);
	KQuestSessionItem* pFill = (KQuestSessionItem*)buf;
	for(SessionMap::iterator it= m_Sessions.begin();it!=m_Sessions.end();it++,pFill++){
		KQuestSessionItem si;
		si.key = it->first;
		si.val = it->second;
		memcpy(pFill,&si,sizeof(KQuestSessionItem));
	}
	return len;
}