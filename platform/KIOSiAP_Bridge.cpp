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
    //������onRequestProductsFinish�����ȥ����iAP��Ʒ���ݡ�
    IOSProduct *product = iap->iOSProductByIdentifier(m_productIdentifiers[m_idx]);
    // Ȼ����Է��𸶿�����
    iap->paymentWithProduct(product, m_count);
}

void IOSiAP_Bridge::Payment(std::string &identifier,int count)
{
	IOSProduct *product = iap->iOSProductByIdentifier(identifier);
	if(product) iap->paymentWithProduct(product, count);
}

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //����requestProducts�����ˣ����ܽ��к�������в�����
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    
    if (event == IOSIAP_PAYMENT_PURCHAED) {
        //����ɹ��ˣ����԰ɽ�ҷ�������ˡ�
		KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_IAP_CALLBACK,(unsigned long long)identifier.c_str(),quantity);
    }
    //����״̬������������
}