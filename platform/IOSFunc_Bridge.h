#ifndef _K_IOSFUNC_BRIDGE_H_
#define _K_IOSFUNC_BRIDGE_H_


#include "IOSFunc.h"


class IOSFunc_Bridge
{
public:
    IOSFunc_Bridge();
    ~IOSFunc_Bridge();
    IOSFunc *m_ios;
    
	virtual void OpenFeedback();
	virtual void ShowLeaderboard();
	virtual void OpenUrl(const char*);
	virtual void CheckGCAvailability();
	
	static void _OpenFeedback();
	static void _CheckGCAvailability();
};



#endif    // __CC_APPLICATION_WIN32_H__
