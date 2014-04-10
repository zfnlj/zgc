#pragma once
#include <System/KType.h>
#include "packet_segment.h"
#include "Inc/KCommonDefine.h"
#include "Inc/KCommonStruct.h"
#include "common_packet.h"

enum CChr_Protocol
{
	s_nCChr_Connected = s_nCChr_PacketStart,
	s_nCChr_ConnError,
	s_nCChr_ConnClosed,
	s_nCChr_CheckName,
	s_nCChr_CreateCharacter,
	s_nCChr_DeleteCharacter,
	s_nCChr_EnterGame,
	s_nCChr_SelectServer,
	s_nCChr_PacketEnd,
};

enum ChrC_Protocol
{
	s_nChrC_Connected = s_nChrC_PacketStart,
	s_nChrC_ConnError,
	s_nChrC_ConnClosed,
	s_nChrC_Message,
	s_nChrC_AccountInfo,
	s_nChrC_CharacterInfo,
	s_nChrC_CheckName_Rsp,
	s_nChrC_CreateCharacter_Rsp,
	s_nChrC_DeleteCharacter_rsp,
	s_nChrC_EnterGame_Rsp,
	s_nChrC_PacketEnd,
};

#pragma pack(push,1)
//---------------------------------------------

struct CChr_CheckName
{
	enum { s_nCode = s_nCChr_CheckName };
	char playerName[MAX_NAME_DEF];
};

struct CChr_CreateCharacter
{
	enum { s_nCode = s_nCChr_CreateCharacter };
	char playerName[MAX_NAME_DEF];
	BYTE playerIndex;
	// other info
	// ...
};

struct CChr_DeleteCharacter
{
	enum { s_nCode = s_nCChr_DeleteCharacter };
	int playerIndex;
};

struct CChr_EnterGame
{
	enum { s_nCode = s_nCChr_EnterGame };
	KObjectID playerID;
	int mapID; // 关卡号, 从 1 开始
};

struct ChrC_AccountInfo
{
	enum { s_nCode = s_nChrC_AccountInfo };
	int result;
	int playerCount;
};

struct ChrC_CharacterInfo
{
	enum { s_nCode = s_nChrC_CharacterInfo };
	int playerIndex;
	KObjectID playerID;
	char playerName[MAX_NAME_DEF];
	int map; // 当前最大关卡号
	int x, y, z;
	// other info
	// ...
};

struct ChrC_CheckName_Rsp
{
	enum { s_nCode = s_nChrC_CheckName_Rsp };
	char playerName[MAX_NAME_DEF];
	int result;
};

struct ChrC_CreateCharacter_Rsp
{
	enum { s_nCode = s_nChrC_CreateCharacter_Rsp };
	int result;
	KObjectID playerID;
	char playerName[MAX_NAME_DEF];
	BYTE playerIndex;
};

struct ChrC_DeleteCharacter_rsp
{
	enum { s_nCode = s_nChrC_DeleteCharacter_rsp };
	KObjectID playerID;
	int result;
};

struct ChrC_EnterGame_Rsp
{
	enum { s_nCode = s_nChrC_EnterGame_Rsp };
	int result;
	char sceneXml[MAX_SCENE_XML_NAME];
};

//---------------------------------------------
#pragma pack(pop)
