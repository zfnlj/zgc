#pragma once
#include "Inc/KCommonStruct.h"
#include "System/Patterns/KSingleton.h"
#include <System/Collections/KVector.h>
#include "../Inc/GlobalLogin2Client.h"
#include <string>
#include "CommonLogic/Configure/KServerAreaSettings.h"
#include <System/Patterns/KSingleton.h>

using namespace std;

struct PlayerData
{
	int index;
	KObjectID playerID;
	char name[MAX_NAME_DEF];
	int map;
};

class KUserData : public ::System::Patterns::KSingleton<KUserData>
{
public:
	KPassport m_passport;
	string_32 m_acctName;
	BYTE m_password[16];
	DWORD m_otpCode;
	KSocketAddress m_worldServerAddr;
	int m_expectPlayerCount;
	PlayerData m_playerList[MAX_PLAYER_COUNT_PRE_ACCOUNT];
	KServerAreaSettings m_serverAreaSettings;

public:
	KUserData();
	~KUserData();

public:
	void clear();
	void setPassword(const char* passwd);
	int getPlayerCount() const;
	PlayerData* get(int index);
	PlayerData* get(KObjectID playerID);
	bool remove(KObjectID playerID);
	bool addPlayer(const PlayerData& player);
	KObjectID getMainPlayerID();
};
