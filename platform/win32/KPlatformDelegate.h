#ifndef _K_PLATFORMDELEGATE_H_
#define _K_PLATFORMDELEGATE_H_


#include "../KPlatformProtocol.h"


class KPlatformDelegate : public KPlatformProtocol
{
public:
	KPlatformDelegate(){};
	virtual ~KPlatformDelegate(){}

    virtual void OpenFeedback();
};



#endif    // __CC_APPLICATION_WIN32_H__
