#ifndef _K_IOSIAP_BRIDGE_H_
#define _K_IOSIAP_BRIDGE_H_


#include "IOSFunc.h"


class IOSFunc_Bridge
{
public:
    IOSFunc_Bridge();
    ~IOSFunc_Bridge();
    IOSFunc *m_ios;
    
	virtual void OpenFeedback();
};



#endif    // __CC_APPLICATION_WIN32_H__
