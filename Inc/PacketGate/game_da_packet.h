#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "common_packet.h"

enum SD_Protocol
{
	s_nSD_Connected = s_nSD_PacketStart,
	s_nSD_ConnError,
	s_nSD_ConnClosed,

	s_nSD_PlayerIn,
	s_nSD_PlayerOut,

	// ´ø×´Ì¬¸±±¾
	s_nSD_LoadStateWorld,
	s_nSD_UnloadStateWorld,

	s_nSD_Clone_PlayerIn,
	s_nSD_Emergency_PlayerOut,

	s_nSD_PacketEnd,
};

enum DS_Protocol
{
	s_nDS_Connected = s_nDS_PacketStart,
	s_nDS_ConnError,
	s_nDS_ConnClosed,

	s_nDS_PlayerIn_Rsp,
	s_nDS_PlayerOut_Rsp,
	
	s_nDS_AgentPath,

	s_nDS_LoadStateWorld_Rsp,
	s_nDS_UnloadStateWorld_Rsp,

	s_nDS_PacketEnd,
};

#pragma pack(push,1)

struct SD_PlayerIn
{
	enum { s_nCode = s_nSD_PlayerIn };
	KPassport passport;
	KObjectID playerID;
	int mapID;
};

struct DS_PlayerIn_Rsp
{
	enum { s_nCode = s_nDS_PlayerIn_Rsp };
	KPassport passport;
	KObjectID playerID;
	int result;
	PlayerDataHandlers handlers;
};

struct SD_PlayerOut
{
	enum { s_nCode = s_nSD_PlayerOut };
	KObjectID playerID;
	int mapID;
	KPassport m_passport;
};

struct SD_LoadStateWorld
{
	enum { s_nCode = s_nSD_LoadStateWorld };
	DWORD worldID;
	INT64 stateID;
};

struct SD_UnloadStateWorld
{
	enum { s_nCode = s_nSD_UnloadStateWorld };
	DWORD worldID;
	INT64 stateID;
};

struct DS_LoadStateWorld_Rsp
{
	enum { s_nCode = s_nDS_LoadStateWorld_Rsp };
	DWORD worldID;
	HANDLE recHandle;
};

struct DS_UnloadStateWorld_Rsp
{
	enum { s_nCode = s_nDS_UnloadStateWorld_Rsp };
	DWORD worldID;
};

struct SD_Clone_PlayerIn
{
	enum { s_nCode = s_nSD_Clone_PlayerIn };
	KPassport srcPassport, passport;
	KObjectID srcID, playerID;
	int mapID;
};

struct SD_Emergency_PlayerOut
{
	enum { s_nCode = s_nSD_Emergency_PlayerOut };
	KObjectID playerID;
	int mapID;
	KPassport passport;
};

#pragma pack(pop)
