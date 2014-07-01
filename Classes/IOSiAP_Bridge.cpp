#include <stdio.h>

#include "IOSiAP_Bridge.h"

IOSiAP_Bridge::IOSiAP_Bridge()
{
    iap = new IOSiAP();
    iap->delegate = this;
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

IOSiAP_Bridge:: requestProducts()
{
    iap->requestProducts(identifiers);
}

void IOSiAP_Bridge::onRequestProductsFinish(void)
{
    //������onRequestProductsFinish�����ȥ����iAP��Ʒ���ݡ�
    IOSProduct *product = iap->iOSProductByIdentifier(identifier);
    // Ȼ����Է��𸶿�����
    iap->paymentWithProduct(product, quantity);
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
