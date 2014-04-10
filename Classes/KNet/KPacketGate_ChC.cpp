#include "KPacketGate_ChC.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include <System/Misc/KStream.h>
#include "KUserData.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"
#include "../WorldObject/KMainPlayer.h"

KPacketGate_ChC::KPacketGate_ChC()
{
	memset(&m_methods, 0, sizeof(m_methods));
	this->Register(s_nChrC_Connected, &KPacketGate_ChC::Process_ChC_Connected);
	this->Register(s_nChrC_ConnError, &KPacketGate_ChC::Process_ChC_ConnError);
	this->Register(s_nChrC_ConnClosed, &KPacketGate_ChC::Process_ChC_ConnClosed);
	this->Register(s_nChrC_Message, &KPacketGate_ChC::Process_ChrC_Message);
	this->Register(s_nChrC_AccountInfo, &KPacketGate_ChC::Process_ChrC_AccountInfo);
	this->Register(s_nChrC_CharacterInfo, &KPacketGate_ChC::Process_ChrC_CharacterInfo);
	this->Register(s_nChrC_CheckName_Rsp, &KPacketGate_ChC::Process_ChrC_CheckName_Rsp);
	this->Register(s_nChrC_CreateCharacter_Rsp, &KPacketGate_ChC::Process_ChrC_CreateCharacter_Rsp);
	this->Register(s_nChrC_DeleteCharacter_rsp, &KPacketGate_ChC::Process_ChrC_DeleteCharacter_rsp);
	this->Register(s_nChrC_EnterGame_Rsp, &KPacketGate_ChC::Process_ChrC_EnterGame_Rsp);
}

KPacketGate_ChC::~KPacketGate_ChC()
{

}

BOOL KPacketGate_ChC::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_ChC::Process(KCharacterSocket* pSock, const void* pPacket)
{
	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	if(command < CommandBegin || command >= CommandEnd)
	{
		Log(LOG_ERROR, "error: unexpected ChC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method)
	{
		Log(LOG_ERROR, "error: no proc found for ChC packet cmd:%u length:%u", command, dataLength);
		return;
	}

	Log(LOG_DEBUG, "Process Packet From Character PacketKey = [%d] Length = [%d]", command, dataLength);
	(this->*method)(pSock, pData, dataLength);
}

void KPacketGate_ChC::Process_ChC_Connected(KCharacterSocket* pSock, const void* pData, int len)
{
	Log(LOG_WARN, "warn: Character Server Connected");
}

void KPacketGate_ChC::Process_ChC_ConnError(KCharacterSocket* pSock, const void* pData, int len)
{
	pSock->m_shouldRemove = TRUE;
	Log(LOG_WARN, "warn: Character Server ConnError");
}

void KPacketGate_ChC::Process_ChC_ConnClosed(KCharacterSocket* pSock, const void* pData, int len)
{
	if(!pSock->IsReady()) pSock->m_shouldRemove = TRUE;
	Log(LOG_WARN, "warn: Character Server Closed");
}

void KPacketGate_ChC::Process_ChrC_Message(KCharacterSocket* pSock, const void* pData, int len)
{
}

void KPacketGate_ChC::Process_ChrC_AccountInfo(KCharacterSocket* pSock, const void* pData, int len)
{
	KUserData* ud = KUserData::GetInstancePtr();
	ChrC_AccountInfo* packet = (ChrC_AccountInfo*)pData;
	if(packet->result != 0)
	{
		Log(LOG_ERROR, "error: ChrC_AccountInfo result:%d", packet->result);
		return;
	}
	ud->m_expectPlayerCount = packet->playerCount;
	//Program.g_app.showPlayers();
}

void KPacketGate_ChC::Process_ChrC_CharacterInfo(KCharacterSocket* pSock, const void* pData, int len)
{
	PlayerData player;
	KUserData* ud = KUserData::GetInstancePtr();
	ChrC_CharacterInfo& packet = *(ChrC_CharacterInfo*)pData;

	player.index = packet.playerIndex;
	strcpy_k(player.name, sizeof(player.name), packet.playerName);
	player.playerID = packet.playerID;
	player.map = packet.map;
	ud->addPlayer(player);

	KMainPlayer::Instance()->initPlayer(packet.playerID);
	KDynamicWorld::getSingleton().SendGatewayMsg(LOGIC_GW_CHARINFO,0,0);
}

void KPacketGate_ChC::Process_ChrC_CheckName_Rsp(KCharacterSocket* pSock, const void* pData, int len)
{
	ChrC_CheckName_Rsp& packet = *(ChrC_CheckName_Rsp*)pData;
	if(packet.result != 0)
	{
		Log(LOG_ERROR, "playerName:%s can not be used.", packet.playerName);
	}
	else
	{
		Log(LOG_WARN, "you can use playerName:%s.", packet.playerName);
	}
}

void KPacketGate_ChC::Process_ChrC_CreateCharacter_Rsp(KCharacterSocket* pSock, const void* pData, int len)
{
	ChrC_CreateCharacter_Rsp* packet =(ChrC_CreateCharacter_Rsp*)pData;
	if(packet->result != 0)
	{
		Log(LOG_ERROR, "create char %s failed, result:%d", packet->playerName, packet->result);
		return;
	}

	Log(LOG_ERROR, "success create char %s", packet->playerName);

	PlayerData player;
	KUserData* ud = KUserData::GetInstancePtr();

	player.index = packet->playerIndex;
	strcpy_k(player.name, sizeof(player.name), packet->playerName);
	player.playerID = packet->playerID;
	player.map = 1;
	ud->addPlayer(player);

	//Program.g_app.showPlayers();
}

void KPacketGate_ChC::Process_ChrC_DeleteCharacter_rsp(KCharacterSocket* pSock, const void* pData, int len)
{
	ChrC_DeleteCharacter_rsp& packet = *(ChrC_DeleteCharacter_rsp*)pData;
	if(packet.result != 0)
	{
		Log(LOG_ERROR, "error: delete player, result:%d", packet.result);
		return;
	}

	Log(LOG_ERROR, "delete player %llu", packet.playerID);

	KUserData* ud = KUserData::GetInstancePtr();
	ud->remove(packet.playerID);
	//Program.g_app.showPlayers();
}

void KPacketGate_ChC::Process_ChrC_EnterGame_Rsp(KCharacterSocket* pSock, const void* pData, int len)
{
	ChrC_EnterGame_Rsp& packet = *(ChrC_EnterGame_Rsp*)pData;
	if(packet.result != 0)
	{
		Log(LOG_ERROR, "error: enter game, result:%d", packet.result);
		return;
	}
}
