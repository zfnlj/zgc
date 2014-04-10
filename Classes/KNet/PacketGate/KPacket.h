/********************************************************************
	created:	2008/01/11
	created:	11:1:2008   11:32
	filename: 	Athena\Main\Src\Common\ServerSocket\include\KPacket.h
	file path:	Athena\Main\Src\Common\ServerSocket\include
	file base:	KPacket
	file ext:	h
	author:		xuqin
	
	purpose:	
*********************************************************************/
#pragma once

#include "System/KType.h"

#pragma pack(push,1)
 struct KPacket
 {
 	enum	PredefineCommand
 	{
 		PredefineCommandStart	,
 		Assemble				,
 		Ping					,
 		PingAck					,
 		Ack						,
 		PredefineCommandEnd
 	};
 
 	enum	PredefineLength
 	{
 		BasePacketSize	= 256,
 		AssembleSize	= 200,
 	};
 
 private:
 	UINT	m_unSeq;
 	SHORT	m_nLength;
 	SHORT	m_nCommand;
 	UINT	m_unCheck1;
 	UINT	m_unCheck2;
 public:
 	KPacket	( SHORT nLength, SHORT nCommand )
 		:	m_nLength	( nLength	)
 		,	m_nCommand	( nCommand	)
 	{
 	}
 	KPacket()
 	{
 	}
 
 public:
 	UINT	GetSeq()
 	{
 		return m_unSeq;
 	}
 	VOID	SetSeq( UINT unSeq )
 	{
 		m_unSeq = unSeq;
 	}
 	SHORT	GetLength()
 	{
 		return m_nLength;
 	}
 	VOID	SetLength( INT nLength )
 	{
 		m_nLength = nLength;
 	}
 	SHORT	GetCommand()
 	{
 		return m_nCommand;
 	}
 	VOID 	SetCommand( SHORT nNewCommand )
 	{
 		m_nCommand = nNewCommand;
 	}
 	VOID 	SetCheck1( UINT nCheck1 )
 	{
 		m_unCheck1 = nCheck1;
 	}
 	UINT	GetCheck1()
 	{
 		return m_unCheck1;
 	}
 };

struct KPacketPing // : public KPacket
{
	DWORD m_TickCount;
};

struct KPacketPingAck //: public KPacket
{
	DWORD m_TickCount;
};

 struct KPacketAck : public KPacket
 {
 public :
 	KPacketAck		()
 		:	KPacket	( sizeof( KPacketAck ), KPacket::Ack )
 	{
 	}
 };

 struct KPacketAssemble : public KPacket
 {
 public:
 	enum	Type
 	{
 		More	= 1,
 		End		= 2,
 	};
 
 private:
 	SHORT	m_Type;
 	SHORT	m_nBodySize;
 	CHAR	m_Body[ KPacket::AssembleSize ];
 	SHORT	m_shCheckSum;
 
 public :
 	KPacketAssemble	( Type type, CHAR* pBody, INT nBodyLen )
 		// 这里要特别把Checksum减去，这里是由于接收方的特殊处理而导致的。by xueyan
 		:	KPacket	( sizeof( KPacketAssemble )-sizeof(m_shCheckSum),KPacket::Assemble )
 		,	m_Type	( type )
 	{
 		if( nBodyLen > KPacket::AssembleSize )
 			nBodyLen = KPacket::AssembleSize;
 		memcpy( m_Body, pBody, nBodyLen );
 		m_nBodySize		= nBodyLen;
 	}
 
 	Type	GetType()		{ return (Type)m_Type;	}
 	SHORT	GetBodySize()	{ return m_nBodySize;	}
 	CHAR*	GetBody()		{ return m_Body;		}
 
 	INT		GetRealLength()
 	{
 		INT nAll = sizeof( KPacketAssemble );
 		INT nSize = KPacket::AssembleSize - m_nBodySize;
 		INT nCheckSum = sizeof(m_shCheckSum);
 
 		INT nResult = nAll - nSize - nCheckSum;
 		return nResult;
 		//return ( sizeof( KPacketAssemble ) - (KPacket::AssembleSize - m_nBodySize) - sizeof(m_shCheckSum) );
 	}
 
 	void	SetRealLength()
 	{
 		INT nLen = GetRealLength();
 
 		SetLength(nLen);
 	}
 };
#pragma pack(pop)
