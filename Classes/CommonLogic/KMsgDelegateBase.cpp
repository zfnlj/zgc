#include "KMsgDelegateBase.h"
#include "../Inc/KLogicMsg.h"

KMsgDelegate::KMsgDelegate()
{
	
}

KMsgDelegate::~KMsgDelegate()
{
	ReleaseMsg();
}

void KMsgDelegate::RegisterMsg(int msgID, void (*pfn)(unsigned long long, unsigned long long))
{
	m_MsgMap.RegisterNonType(msgID,pfn);	
}

void KMsgDelegate::ReleaseMsg()
{
	m_MsgMap.ClearMsgDelegate();
}

void KMsgDelegate::SendMsg(int msgID, unsigned long long Param1, unsigned long long Param2)
{
	m_MsgMap.Invoke(msgID, Param1, Param2);
}


