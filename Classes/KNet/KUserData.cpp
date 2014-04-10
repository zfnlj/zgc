#include "KUserData.h"
#include <System/Misc/md5new.h>

KUserData::KUserData()
{
	memset(&m_password, 0, sizeof(m_password));
	m_otpCode = 0;
	memset(&m_worldServerAddr, 0, sizeof(m_worldServerAddr));
	m_expectPlayerCount = 0;
	memset(&m_playerList, 0, sizeof(m_playerList));
}

KUserData::~KUserData()
{
}

void KUserData::clear()
{
	m_passport.value = 0;
	m_acctName.clear();
	memset(&m_password, 0, sizeof(m_password));
	m_otpCode = 0;
	memset(&m_worldServerAddr, 0, sizeof(m_worldServerAddr));
	m_expectPlayerCount = 0;
	memset(&m_playerList, 0, sizeof(m_playerList));
	m_serverAreaSettings.clear();
}

void KUserData::setPassword(const char* passwd)
{
	CMD5 PasswordMd5;
	PasswordMd5.Update((char*)passwd);
	PasswordMd5.Final();
	memcpy(&m_password, PasswordMd5.Get(), 16);
}

int KUserData::getPlayerCount() const
{
	int c = 0;
	for(int i=0; i<MAX_PLAYER_COUNT_PRE_ACCOUNT; i++)
	{
		const PlayerData& player = m_playerList[i];
		if(player.playerID) c++;
	}
	return c;
}

PlayerData* KUserData::get(int index)
{
	if(index < 0 || index >= MAX_PLAYER_COUNT_PRE_ACCOUNT)
		return NULL;
	PlayerData& player = m_playerList[index];
	if(!player.playerID) return NULL;
	return &player;
}

PlayerData* KUserData::get(KObjectID playerID)
{
	for(int i=0; i<MAX_PLAYER_COUNT_PRE_ACCOUNT; i++)
	{
		PlayerData& player = m_playerList[i];
		if(player.playerID == playerID) return &player;
	}
	return NULL;
}

bool KUserData::remove(KObjectID playerID)
{
	for(int i=0; i<MAX_PLAYER_COUNT_PRE_ACCOUNT; i++)
	{
		PlayerData& player = m_playerList[i];
		if(player.playerID == playerID)
		{
			memset(&player, 0, sizeof(player));
			return true;
		}
	}
	return false;
}

bool KUserData::addPlayer(const PlayerData& player)
{
	memcpy(&m_playerList[player.index], &player, sizeof(player));
	return true;
}

KObjectID KUserData::getMainPlayerID()
{
	PlayerData* player = get(0);
	if(player){
		return player->playerID;
	}else{
		return 0;
	}
}