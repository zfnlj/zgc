#ifndef _IOSIAP_CPLUS_H
#define _IOSIAP_CPLUS_H

#include "IOSiAP.h"


class IOSiAP_Bridge : public IOSiAPDelegate
{
public:
    IOSiAP_Bridge();
    ~IOSiAP_Bridge();
    IOSiAP *iap;
    
    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity);
};


#endif /* defined(__iAP_JSBinding__IOSiAP_JSB__) */
