#pragma once

#include "../../Inc/GlobalDefine.h"
#include "KCommonDefine.h"
#include "KCommonStruct.h"

#include "KPacket.h"
#define	PACKET_BASE	KPacket

#define DEF_PACKET_SEGMENT_INTERVAL	(0x100)
#define MAX_SYNC_COUNT 20

enum ENUM_CLIENT_IN_PACKET_TYPE
{
	enum_Cin_Login_Packet_Begin =		1 * DEF_PACKET_SEGMENT_INTERVAL + 0x10,	// 272	- 511
	enum_Cin_Character_Packet_Begin =	2 * DEF_PACKET_SEGMENT_INTERVAL,		// 512	- 767
	enum_Cin_Gateway_Packet_Begin	=	3 * DEF_PACKET_SEGMENT_INTERVAL,		// 768	- 1023
	enum_Cin_Agent_Packet_Begin =		4 * DEF_PACKET_SEGMENT_INTERVAL,		// 1024 - 1279
	enum_Cin_World_Packet_Begin =		5 * DEF_PACKET_SEGMENT_INTERVAL,		// 1280 - 1535
	enum_Cin_Game_Packet_Begin =		6 * DEF_PACKET_SEGMENT_INTERVAL,		// 1536	- 1791
	enum_Cin_AI_Packet_Begin =			7 * DEF_PACKET_SEGMENT_INTERVAL,		// 1792	- 2047
	enum_Cin_Chat_Packet_Begin =		8 * DEF_PACKET_SEGMENT_INTERVAL,		// 2048	- 2303
	enum_Cin_Sync_Packet_Begin =		9 * DEF_PACKET_SEGMENT_INTERVAL,		// 2304	- 2559
	enum_Cin_Mail_Packet_Begin =		10 * DEF_PACKET_SEGMENT_INTERVAL,		// 2560	- 2815
	enum_Cin_QueryCharsCount_Packet_Bein = 11 * DEF_PACKET_SEGMENT_INTERVAL,	// 2816 - 3071
	enum_Cin_Salesroom_Packet_Begin =	12 * DEF_PACKET_SEGMENT_INTERVAL,	// 
};

enum ENUM_SERVER_IN_PACKET_TYPE
{
	enum_Sin_Login_Client_Packet_Begin =		1 * DEF_PACKET_SEGMENT_INTERVAL + 0x10,	// 272	- 511

	enum_Sin_Character_Client_Packet_Begin =	2 * DEF_PACKET_SEGMENT_INTERVAL,		// 512	- 767

	enum_Sin_Agent_Game_Packet_Begin =			3 * DEF_PACKET_SEGMENT_INTERVAL,		// 768	- 1023

	enum_Sin_Game_Client_Packet_Begin =			4 * DEF_PACKET_SEGMENT_INTERVAL,		// 1024	- 1279
	enum_Sin_Game_Agent_Packet_Begin =			5 * DEF_PACKET_SEGMENT_INTERVAL,		// 1280	- 1535
	enum_Sin_Game_AI_Packet_Begin =				6 * DEF_PACKET_SEGMENT_INTERVAL,		// 1536	- 1791
	enum_Sin_Game_World_Packet_Begin =			7 * DEF_PACKET_SEGMENT_INTERVAL,		// 1792	- 2047
	enum_Sin_Game_Chat_Packet_Begin =			8 * DEF_PACKET_SEGMENT_INTERVAL,		// 2048	- 2303

	enum_Sin_AI_Client_Packet_Begin =			9 * DEF_PACKET_SEGMENT_INTERVAL,		// 2304	- 2559
	enum_Sin_AI_Game_Packet_Begin =				10 * DEF_PACKET_SEGMENT_INTERVAL,		// 2560	- 2815

	enum_Sin_Chat_Client_Packet_Begin =			11 * DEF_PACKET_SEGMENT_INTERVAL,		// 2816	- 3071
	enum_Sin_Chat_Game_Packet_Begin =			12 * DEF_PACKET_SEGMENT_INTERVAL,		// 3072	- 3327
	enum_Sin_Chat_World_Packet_Begin =			13 * DEF_PACKET_SEGMENT_INTERVAL,		// 3328	- 3583

	enum_Sin_World_Game_Packet_Begin =			14 * DEF_PACKET_SEGMENT_INTERVAL,		// 3584	- 3839
	enum_Sin_World_Chat_Packet_Begin =			15 * DEF_PACKET_SEGMENT_INTERVAL,		// 3840	- 4095

	enum_Sin_Login_World_Packet_Begin =			16 * DEF_PACKET_SEGMENT_INTERVAL,		// 4096	- 4351
	enum_Sin_World_Login_Packet_Begin =			17 * DEF_PACKET_SEGMENT_INTERVAL,		// 4352	- 4607

	// 用于SERVER内部的模拟包 by xueyan
	enum_Sin_Server_Internal_Packet_Begin =		18 * DEF_PACKET_SEGMENT_INTERVAL,		// 4608	- 4864

	enum_Sin_Character_World_Packet_Begin =		19 * DEF_PACKET_SEGMENT_INTERVAL,		// 4865	- 5120
	enum_Sin_World_Character_Packet_Begin =		20 * DEF_PACKET_SEGMENT_INTERVAL,		// 5121	- 5376

	enum_Sin_Login_Auth_Packet_Begin =			21 * DEF_PACKET_SEGMENT_INTERVAL,		// 5377 - 5631
	enum_Sin_Auth_Login_Packet_Begin =			22 * DEF_PACKET_SEGMENT_INTERVAL,		// 5632 - 5887

	enum_Sin_World_Auth_Packet_Begin =			23 * DEF_PACKET_SEGMENT_INTERVAL,		// 5888 - 6143
	enum_Sin_Auth_World_Packet_Begin =			24 * DEF_PACKET_SEGMENT_INTERVAL,		// 6144 - 6399

	enum_Sin_Launcher_C_S_Packet_Begin =		25 * DEF_PACKET_SEGMENT_INTERVAL,		// 6400 - 6655
	enum_Sin_Launcher_S_C_Packet_Begin =		26 * DEF_PACKET_SEGMENT_INTERVAL,		// 6656 - 6911

	enum_Sin_LogServer_S_S_Packet_Begin =		27 * DEF_PACKET_SEGMENT_INTERVAL,		// 6912 - 7167

	// DBAgent -> WorldServer
	enum_Sin_DBAgent_World_Packet_Begin =		28 * DEF_PACKET_SEGMENT_INTERVAL,		// 7168

	// WorldServer -> DBAgent
	enum_Sin_World_DBAgent_Packet_Begin =		29 * DEF_PACKET_SEGMENT_INTERVAL,		// 7424

	// BillingServer -> WorldServer
	enum_Sin_Billing_World_Packet_Begin =		30 * DEF_PACKET_SEGMENT_INTERVAL,		// 7680

	// WorldServer -> BillingServer
	enum_Sin_World_Billing_Packet_Begin =		31 * DEF_PACKET_SEGMENT_INTERVAL,		// 7936

	//MailServer -> GameServer
	enum_Sin_Mail_Game_Packet_Begin =			32 * DEF_PACKET_SEGMENT_INTERVAL,		//8192 - 8447

	//GameServer -> MailServer
	enum_Sin_Game_Mail_Packet_Begin =			33 * DEF_PACKET_SEGMENT_INTERVAL,		//8448 - 8703

	//MailServer -> WorldServer
	enum_Sin_Mail_World_Packet_Begin =			34 * DEF_PACKET_SEGMENT_INTERVAL,		//8704 - 8959

	//WorldServer -> MailServer
	enum_Sin_World_Mail_Packet_Begin =			35 * DEF_PACKET_SEGMENT_INTERVAL,		//8960 - 9215

	enum_Sin_Game_Hack_Packet_Begin  =			36 * DEF_PACKET_SEGMENT_INTERVAL,		// 9216
	enum_Sin_Hack_Game_Packet_Begin  =			37 * DEF_PACKET_SEGMENT_INTERVAL,		// 9472

	enum_Sin_Billing_Game_Packet_Begin =		38 * DEF_PACKET_SEGMENT_INTERVAL,		// 9728
	enum_Sin_Game_Billing_Packet_Begin =		39 * DEF_PACKET_SEGMENT_INTERVAL,		// 9984

	enum_Sin_Passport_World_Packet_Begin =		40 * DEF_PACKET_SEGMENT_INTERVAL,		// 10240
	enum_Sin_World_Passport_Packet_Begin =		41 * DEF_PACKET_SEGMENT_INTERVAL,		// 10496

	enum_Sin_Passport_Login_Packet_Begin =		42 * DEF_PACKET_SEGMENT_INTERVAL,		// 10752
	enum_Sin_Login_Passport_Packet_Begin =		43 * DEF_PACKET_SEGMENT_INTERVAL,		// 11008

	enum_Sin_QueryCharsCount_Character_Packet_Begin = 44 * DEF_PACKET_SEGMENT_INTERVAL,	// 11264

	enum_Sin_Gateway_Packet_Begin		=		45 * DEF_PACKET_SEGMENT_INTERVAL,		// 11520
	
	// 客户端直接发到WorldServer的包
	enum_Sin_World_Client_Packet_Begin	=		46 * DEF_PACKET_SEGMENT_INTERVAL,		// 11776

	enum_Sin_DaD_Packet_Begin =					47 * DEF_PACKET_SEGMENT_INTERVAL,		// 12032
	enum_Sin_DDa_Packet_Begin =					48 * DEF_PACKET_SEGMENT_INTERVAL,		// 12288

	// GameServer -> SalesroomServer
	enum_Sin_Game_Salesroom_Packet_Begin =		49 * DEF_PACKET_SEGMENT_INTERVAL,		// 12544

	// SalesroomServer -> GameServer
	enum_Sin_Salesroom_Game_Packet_Begin =		50 * DEF_PACKET_SEGMENT_INTERVAL,		// 12800

	enum_Sin_Packet_Count =						51 * DEF_PACKET_SEGMENT_INTERVAL,		// 13056
};

enum ENUM_MONITOR_IN_PACKET_TYPE
{
	enum_MAgent_MManager_Packet_Begin =			1 * DEF_PACKET_SEGMENT_INTERVAL + 0x10,

	enum_MAgent_MManager_Packet_Count =			16 * DEF_PACKET_SEGMENT_INTERVAL,
};
