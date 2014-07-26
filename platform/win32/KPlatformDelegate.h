#ifndef __CC_APPLICATION_WIN32_H__
#define __CC_APPLICATION_WIN32_H__


#include "../KPlatformProtocol.h"


class KPlatformDelegate : public KPlatformProtocol
{
public:
	KPlatformDelegate(){};
	virtual ~KPlatformDelegate(){}

    virtual void OpenFeedback();
};



#endif    // __CC_APPLICATION_WIN32_H__
