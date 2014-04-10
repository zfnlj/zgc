#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "common_packet.h"

enum DW_Protocol
{
	s_nDW_Connected = s_nDW_PacketStart,
	s_nDW_ConnError,
	s_nDW_ConnClosed,

	s_nDW_Login,
	s_nDW_PlayerUnloaded,

	s_nDW_PacketEnd,
};

enum WD_Protocol
{
	s_nWD_Connected = s_nWD_PacketStart,
	s_nWD_ConnError,
	s_nWD_ConnClosed,

	s_nWD_LoginRsp,

	s_nWD_PacketEnd,
};

#pragma pack(push,1)

struct DW_Login
{
	enum { s_nCode = s_nDW_Login };
};

struct DW_PlayerUnloaded
{
	enum { s_nCode = s_nDW_PlayerUnloaded };
	KObjectID playerID;
	BYTE  success;
};

#pragma pack(pop)
