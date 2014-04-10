

#include "KZGThread.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"

KZGThread::KZGThread()
{
	m_stopDesired = FALSE;
}

KZGThread::~KZGThread()
{

}

BOOL KZGThread::Initialize()
{
	this->Run();
	return TRUE;
}

void KZGThread::Finalize()
{
	this->Stop();
	return;
}

void KZGThread::Execute()
{
	DWORD now;
	int waitTicks;
	int checkInterval = 1000;
	DWORD lastCheckTick = GetTickCount();

	while(!m_stopDesired)
	{
		DWORD nowTicks = GetTickCount();
		int msInterval = nowTicks - lastCheckTick;

		//this->ProcessActiveQueue(nowTicks, msInterval);
		waitTicks = lastCheckTick+checkInterval - nowTicks;
		lastCheckTick = nowTicks;

		KMainPlayer::RealPlayer()->syncToRecord();
		if(waitTicks > 0) Sleep(waitTicks);
			continue;
	}
}

void KZGThread::Stop()
{
	m_stopDesired = TRUE;
	KThread::WaitFor();
}
