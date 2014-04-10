#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include <KCommonStruct.h>
#include "common_packet.h"

enum WP_Protocol
{
	s_nWP_Connected = s_nWP_PacketStart,
	s_nWP_ConnError,
	s_nWP_ConnClosed,
	s_nWP_CheckOtpCode,
	s_nWP_PacketEnd,
};

enum PW_Protocol
{
	s_nPW_Connected = s_nPW_PacketStart,
	s_nPW_ConnError,
	s_nPW_ConnClosed,
	s_nPW_CheckOtpCode_Rsp,
	s_nPW_PacketEnd,
};

#pragma pack(push,1)

struct WP_CheckOtpCode
{
	enum { s_nCode = s_nWP_CheckOtpCode };
	KPassport passport;
	DWORD otpCode;
};

struct PW_CheckOtpCode_Rsp
{
	enum { s_nCode = s_nPW_CheckOtpCode_Rsp };
	KPassport passport;
	int result;
};

#pragma pack(pop)
