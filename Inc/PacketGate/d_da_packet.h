#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonStruct.h>
#include "common_packet.h"

enum DDa_PROTOCOL
{
	s_nDDa_Connected = s_nDDa_PacketStart,
	s_nDDa_ConnClosed,

	s_nDDa_LargePacket,		// 超大的包( > 10K )

	s_nDDa_ValidateRecord,	// 通知DataAgent校验处于Active状态的Record
	// ver,key,eof,tbname

	s_nDDa_ValidateRecord_Ack, // ver,key,eof,tbname,loadTime

	s_nDDa_CheckinRecord_Ack,	// CheckinRecord确认
	// ver,key,eof,seq,tbname,loadTime,errCode

	s_nDDa_LoadRecord_Rsp,	// ver,key,values,eof,seq,tbname,errCode,loadTime

	s_nDDa_FlushRecord_Ack,	// ver,key,eof,seq,tbname,errCode
	s_nDDa_DeleteRecord_Ack,// ver,key,eof,seq,tbname,errCode

	s_nDDa_cloneRecord_Rsp,	// ver,key,values,eof,seq,tbname,errCode,loadTime

	s_nDDa_Direct_DeleteRecord_Rsp,	// ver,key,eof,seq,tbname,errCode
	s_nDDa_Direct_UpdataRecord_Rsp,	// ver,key,eof,seq,tbname,errCode
	s_nDDa_Direct_InsertRecord_Rsp,	// ver,key,eof,seq,tbname,errCode

	// proxy to data agent
	s_nDDa_NotifyDataServerReady,	// 有一个DataServer连上来了

	s_nDDa_Direct_FlushRecord_Rsp,	// ver,key,eof,seq,tbname,errCode

	s_nDDa_DeprecateRecord_Rsp,		// ver,key,eof,seq,tbname,errCode

	s_nDDa_PacketEnd,
};

enum DaD_PROTOCOL
{
	s_nDaD_Connected = s_nDaD_PacketStart,
	s_nDaD_ConnClosed,

	s_nDaD_LargePacket,			// 超大的包( > 10K )

	s_nDaD_ValidateRecord_Rsp,	// ver,key,value,eof,tbname,loadTime

	s_nDaD_CheckinRecord,		// 玩家下线后，把最新数据给DataServer
	// data sequence: ver[,key,data],eof,seq,tbname,loadTime

	s_nDaD_LoadRecord,			// ver,key,eof,seq,tbname,loadTime,create(BYTE)
	s_nDaD_LoadRecord_Ack,		// ver,key,eof,tbname,loadTime

	s_nDaD_FlushRecord,			// ver,key,data,eof,seq,tbname,loadTime
	s_nDaD_DeleteRecord,		// ver,key,eof,seq,tbname,loadTime

	s_nDaD_AskValidateRecord,	// ver,key,eof,tbname

	s_nDaD_cloneRecord,			// ver,key,eof,seq,tbname,loadTime,create(BYTE)


	/// 直接的数据操作，可能不是来自于DataAgent，而是CharacterServer, WorldServer
	s_nDaD_Direct_DeleteRecord,	// ver,key,eof,seq,tbname
	s_nDaD_Direct_UpdataRecord,	// ver,key,values,eof,seq,tbname
	s_nDaD_Direct_InsertRecord,	// ver,key,values,eof,seq,tbname
	s_nDaD_Direct_FlushRecord,	// ver,key,values,eof,seq,tbname

	s_nDaD_DeprecateRecord,		// ver,key,eof,seq,tbname

	s_nDaD_PacketEnd,
};

#pragma pack(push,1)

struct DDa_LargePacket
{
	enum { s_nCode = s_nDDa_LargePacket };
	enum FLAG { begin = (1<<0), end=(1<<1), };

	DWORD seq;			// sequence of this packet
	BYTE  flags;		// flags
	union
	{
		struct
		{
			WORD  command;		// actual command
			char  tbname[64];	// for which table, wen can get KTableRecordData by this name from KRecordDataFactory
		};
		struct
		{
			char data[66];
		};
	};
	// byte sequence of real data

	//--------------------------------------
	size_t fixSize() const
	{
		if(flags & begin)
		{
			return sizeof(DWORD)+sizeof(flags)+sizeof(WORD)+strlen(tbname)+1;
		}
		else
		{
			return sizeof(DWORD)+sizeof(flags);
		}
	}
};

struct DaD_LargePacket
{
	enum { s_nCode = s_nDaD_LargePacket };
	enum FLAG { begin = (1<<0), end=(1<<1), };
	DWORD seq;			// sequence of this packet
	BYTE  flags;		// flags
	union
	{
		struct
		{
			WORD  command;		// actual command
			char  tbname[64];	// for which table, wen can get KTableRecordData by this name from KRecordDataFactory
		};
		struct
		{
			char data[66];
		};
	};
	// byte sequence of real data

	//--------------------------------------
	size_t fixSize() const
	{
		if(flags & begin)
		{
			return sizeof(DWORD)+sizeof(flags)+sizeof(WORD)+strlen(tbname)+1;
		}
		else
		{
			return sizeof(DWORD)+sizeof(flags);
		}
	}
};

#pragma pack(pop)
