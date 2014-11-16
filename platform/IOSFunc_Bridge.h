#ifndef _K_IOSFUNC_BRIDGE_H_
#define _K_IOSFUNC_BRIDGE_H_


#include "IOSFunc.h"


class IOSFunc_Bridge
{
public:
    IOSFunc_Bridge();
    ~IOSFunc_Bridge();
    IOSFunc *m_ios;    
    
    static void _OpenUrl(const char* url);
	static void _OpenFeedback();
	static void _CheckGCAvailability();
    static void _ShowLeaderboard();
    static bool _ReportScore(const char* ,int val);
    static bool _ReportAchievement(const char*,double percent,bool bNotify);
};



#endif    // __CC_APPLICATION_WIN32_H__
