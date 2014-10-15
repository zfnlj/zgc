#include "KIOSiAP_Bridge.h"
#include "cocos-ext.h"

USING_NS_CC;



IOSiAP_Bridge::IOSiAP_Bridge()
{
    iap = new IOSiAP();
    iap->delegate = this;
    m_productIdentifiers[0]="gold_01";
    m_productIdentifiers[1]="gold_02";
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

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //����requestProducts�����ˣ����ܽ��к�������в�����
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    
    if (event == IOSIAP_PAYMENT_PURCHAED) {
        //����ɹ��ˣ����԰ɽ�ҷ�������ˡ�
    }
    //����״̬������������
}