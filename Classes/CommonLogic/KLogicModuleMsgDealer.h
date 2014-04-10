#ifndef __KLOGICMODULEMSGDEALER_H__
#define __KLOGICMODULEMSGDEALER_H__
#include "KMsgDelegateBase.h"

class KLogicModuleMsgDealer : public KMsgDelegate
{
public:
	KLogicModuleMsgDealer();
	virtual ~KLogicModuleMsgDealer();
	virtual void Setup();
private:
	virtual void RegisterAllMsg();
};

#endif