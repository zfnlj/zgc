#ifndef _K_IOSIAP_BRIDGE_H_
#define _K_IOSIAP_BRIDGE_H_


#include "IOSiAP.h"


class IOSiAP_Bridge : public IOSiAPDelegate
{
public:
    IOSiAP_Bridge();
    ~IOSiAP_Bridge();
    IOSiAP *iap;
    
	void requestProducts();

    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity);
	void Payment(std::string &identifier,int count);
private:
	std::vector <std::string>  m_productIdentifiers;
	std::string	m_identifier;
	int m_idx;
	int m_count;
};



#endif    // __CC_APPLICATION_WIN32_H__
