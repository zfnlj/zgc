#ifndef _KGATEWAYMSGDELEGATE_H
#define _KGATEWAYMSGDELEGATE_H
#include "KMsgDelegate.h"

class KMsgDelegate
{
public:
	KMsgDelegate();
	virtual ~KMsgDelegate();
	
	template<typename ObjType>
	void RegisterMsg(int msgID, ObjType *pObj, void (ObjType::*pfn)(unsigned long long, unsigned long long))
	{
		m_MsgMap.RegisterType<ObjType>(msgID,pObj,pfn);
	}
	void RegisterMsg(int msgID, void (*pfn)(unsigned long long, unsigned long long));
	void ReleaseMsg();
	void SendMsg(int msgID, unsigned long long Param1, unsigned long long Param2);
	virtual void RegisterAllMsg()=0;
protected:
	KMsgDelegateNodeMap m_MsgMap;

};


#endif