#include "KObjectState.h"

KObjectState::KObjectState():m_sid(cs_none),m_object(NULL)
{

}

KObjectState::KObjectState(int stateID):m_sid(stateID),m_object(NULL)
{

}

KObjectState::~KObjectState()
{

}

void KObjectState::onEnterState()
{

}

void KObjectState::onLeaveState()
{

}



void KObjectState::breathe(int interval)
{

}

void KObjectState::reset()
{
	//m_sid = cs_none;
	m_object = NULL;
}

void KObjectState::destroy()
{
	this->reset();
	delete this;
}

void KObjectState::onMessage(int cmd, const void* data, int len )
{

}
