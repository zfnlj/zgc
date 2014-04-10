#include "KQuestArray.h"
#include <System/Log/log.h>

bool KQuestArray::add( int qid )
{
	if (m_capacity <= m_arr.size())
	{
		Log(LOG_ERROR, "error when associateNpc with quest %d", qid);
		return false;
	}
	int pos = m_arr.insert_unique(qid);
	if(pos >= 0)
	{
		m_header->count += 1;
		return true;
	}
	return false;
}