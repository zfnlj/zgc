/********************************************************************
	created:	2008/4/15
	filename: 	System\Patterns\KPacketGate.h
	file path:	System\Patterns
	file base:	KPacketGate
	file ext:	h
	author:		jungle
	
	purpose:	实现一个网络发包模式封装类，目的是将网络协议包的修改集中化
*********************************************************************/

#pragma once

#include "../../Inc/GlobalDefine.h"
#include "KPacket.h"
#include "KPacket_Segment_Define.h"
//#include "System/Collections/KHashMap_D.h"

namespace System {			namespace TCP {

// 允许最大的数据包数量，注意：包命令字不能超过这个数字
#define	MAX_PACKET_COUNT	(enum_Sin_Packet_Count)

#define	Conn(x, y) x##y
//#define	ToChar(x) #@x
#define	_str_(x) #x

#ifdef	WIN32
typedef VOID (__cdecl  *FN_CALL)(INT, INT,CHAR*, INT);//char* pBuf, int nBufLen);
#else
typedef VOID (*FN_CALL)(INT,INT, CHAR*, INT);
#endif

#ifdef _SERVER
#define REG_CLIENT_CALL_SERVER(cmd, func) RegFunc(cmd, func)
#define REG_SERVER_CALL_CLIENT(cmd, func) RegFunc(cmd, _str_(func))
#else
#define REG_CLIENT_CALL_SERVER(cmd, func) RegFunc(cmd, _str_(func))
#define REG_SERVER_CALL_CLIENT(cmd, func) RegFunc(cmd, func)
#endif

// class KPacketGate
// {
// public:
// 	KPacketGate(void) {	memset(m_Command_Func, 0, sizeof(m_Command_Func)); }
// 	~KPacketGate(void) { }
// 
// 	void Process(int nConn,int nID,short sCommand, char* pBuf, int nLen)
// 	{
// 		PACKET_BASE* pPacket = (PACKET_BASE*)pBuf;
// 		//short cmd = pPacket->GetCommand();
// 		N_ASSERT( sCommand >= 0 && sCommand < MAX_PACKET_COUNT );
// 		if( m_Command_Func[sCommand] )
// 			m_Command_Func[sCommand](nConn,nID, pBuf, nLen);
// 	}
// 
// protected:
// 
// 	void RegFunc(int cmd, FN_CALL func)
// 	{
// 		N_ASSERT( cmd >= 0 && cmd < MAX_PACKET_COUNT );
// 		N_ASSERT( func != NULL );
// 		N_ASSERT( m_Command_Func[cmd] == NULL );
// 		m_Command_Func[cmd] = func;
// 	}
// 
// 	FN_CALL	m_Command_Func[MAX_PACKET_COUNT];
// };
// 

}};
