#ifndef _KZGTHREAD_H
#define _KZGTHREAD_H

#include <System/Thread/KThread.h>


class KZGThread : public System::Thread::KThread
{
public:
	KZGThread();
	~KZGThread();

public:
	BOOL Initialize();
	void Finalize();

	void Execute();
	void Stop();

	volatile BOOL m_stopDesired;
};

#endif