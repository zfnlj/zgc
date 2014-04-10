#ifndef __KGATEWAYMSGDEALER_H__
#define __KGATEWAYMSGDEALER_H__
#include "KMsgDelegateBase.h"

class KGatewayMsgDealer : public KMsgDelegate
{
public:
	KGatewayMsgDealer();
	virtual ~KGatewayMsgDealer();
	virtual void Setup();
private:
	void RegisterAllMsg();
};

#endif