#include "KIOSiAP_Bridge.h"
#include "cocos-ext.h"
#include "../Classes/GameLogic/KDynamicWorld.h"
#include "../Classes/Inc/KLogicMsg.h"

USING_NS_CC;



IOSiAP_Bridge::IOSiAP_Bridge()
{
    iap = new IOSiAP();
    iap->delegate = this;
    m_productIdentifiers.push_back("gold_01");
    m_productIdentifiers.push_back("gold_02");
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

void IOSiAP_Bridge::requestProducts()
{
    iap->requestProducts(m_productIdentifiers);
}

void IOSiAP_Bridge::onRequestProductsFinish(void)
{
    //必须在onRequestProductsFinish后才能去请求iAP产品数据。
    IOSProduct *product = iap->iOSProductByIdentifier(m_productIdentifiers[m_idx]);
    // 然后可以发起付款请求。
    iap->paymentWithProduct(product, m_count);
}

void IOSiAP_Bridge::Payment(std::string &identifier,int count)
{
	IOSProduct *product = iap->iOSProductByIdentifier(identifier);
	if(product) iap->paymentWithProduct(product, count);
}

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //这里requestProducts出错了，不能进行后面的所有操作。
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    
    if (event == IOSIAP_PAYMENT_PURCHAED) {
        //付款成功了，可以吧金币发给玩家了。
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_IAP_CALLBACK,(unsigned long long)identifier.c_str(),quantity);
    }
    //其他状态依情况处理掉。
}