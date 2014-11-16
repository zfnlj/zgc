#include "IOSFunc_Bridge.h"
#include "cocos-ext.h"

USING_NS_CC;



IOSFunc_Bridge::IOSFunc_Bridge()
{
    m_ios = new IOSFunc();
}

IOSFunc_Bridge::~IOSFunc_Bridge()
{
    delete m_ios;
}

void IOSFunc_Bridge::_OpenUrl(const char* url)
{
    IOSFunc_Bridge IOSFunc;
    IOSFunc.m_ios->OpenUrl(url);
}

void IOSFunc_Bridge::_OpenFeedback()
{
	IOSFunc_Bridge IOSFunc;
	IOSFunc.m_ios->OpenFeedback();
}

void IOSFunc_Bridge::_CheckGCAvailability()
{
	IOSFunc_Bridge IOSFunc;
	IOSFunc.m_ios->CheckGCAvailability();
}

void IOSFunc_Bridge::_ShowLeaderboard()
{
    IOSFunc_Bridge IOSFunc;
    IOSFunc.m_ios->ShowLeaderboard();
}

bool IOSFunc_Bridge::_ReportScore(const char* str,int val)
{
    IOSFunc_Bridge IOSFunc;
    return IOSFunc.m_ios->ReportScore(str,val);
}

bool IOSFunc_Bridge::_ReportAchievement(const char* str,double percent,bool bNotify)
{
    IOSFunc_Bridge IOSFunc;
    return IOSFunc.m_ios->ReportAchievement(str,percent,bNotify);
}