#ifndef __KLOGINMSGDEALER_H__
#define __KLOGINMSGDEALER_H__
#include "KMsgDelegateBase.h"

class KLoginMsgDealer : public KMsgDelegate
{
public:
	KLoginMsgDealer();
	virtual ~KLoginMsgDealer();
	virtual void Setup();
private:
	void RegisterAllMsg();
};

#endif