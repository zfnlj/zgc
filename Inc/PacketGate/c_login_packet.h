#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include "Inc/KCommonStruct.h"
#include "common_packet.h"

enum CL_Protocol
{
	s_nCL_Connected = s_nCL_PacketStart,
	s_nCL_ConnError,
	s_nCL_ConnClosed,
	s_nCL_LoginReq,

	s_nCL_Message, // testing

	s_nCL_PacketEnd,
};

enum LC_Protocol
{
	s_nLC_Connected = s_nLC_PacketStart,
	s_nLC_ConnError,
	s_nLC_ConnClosed,
	s_nLC_LoginRsp,
	s_nLC_ServerAreaInfos,

	s_nLC_Message_Rsp, // tesing

	s_nLC_PacketEnd,
};

#pragma pack(push,1)

struct CL_LoginReq
{
	enum { s_nCode = s_nCL_LoginReq };
	char acctName[64];
	unsigned char passwdMd5[16];
};

struct LC_LoginRsp
{
	enum { s_nCode = s_nLC_LoginRsp };
	int result;
	DWORD otpCode;
	KPassport passport;
};

#pragma pack(pop)
