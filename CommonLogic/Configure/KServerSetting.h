/********************************************************************
history:
created:	2008/12/12
created:	12:12:2008

filename: 	\Athena\Main\Inc\KServerSettings.h
file path:	\Athena\Main\Inc\
file base:	KServerSettings
file ext:	h

author:		Calvin
copyright:  极光互动 2007-2008

purpose:	所有服务器的配置文件
comment:    
*********************************************************************/
#pragma once
#include <System/KPlatform.h>
#include <System/tinyxml/xmldom.h>
#include <System/Log/log.h>
#include <System/Collections/KString.h>
#include <System/Sync/KSync.h>
#include <System/Collections/DynArray.h>

using namespace System::Collections;
using namespace System::Sync;

#define CS_SETTINGS_PATH			"../Settings/ServerInfo/"
#define SETTINGS_FILE_PATH			"../Settings/ServerInfo/ServerSettings.xml"
#define DATABASE_FILE_PATH			"../Settings/ServerInfo/DatabaseSettings.xml"

enum SERVER_SETTINGS_INFO
{
	enum_SSI_ServerStart = 0,
	enum_SSI_LauncherServer,
	enum_SSI_LoginServer,
	enum_SSI_PassportServer,
	enum_SSI_CharacterServer,
	enum_SSI_LogServer,
	enum_SSI_MonitorManager,
	enum_SSI_WorldServer,
	enum_SSI_ChatServer,
	enum_SSI_DBAgent,
	enum_SSI_GameServer,
	enum_SSI_AIServer,
	enum_SSI_BillingServer,
	enum_SSI_MailServer,
	enum_SSI_HackServer,
	enum_SSI_DataServer,
	enum_SSI_AccountServer,
	enum_SSI_SalesroomServer,
	enum_SSI_Count,
};

enum DB_SETTINGS_INFO
{
	enum_DSI_DBStart = 0,
	enum_DSI_AccountDB,
	enum_DSI_GameDB,
	enum_DSI_LogDB,
	enum_DSI_CountDB,
	enum_DSI_BillingDB,
	enum_DSI_MailDB,
	enum_DSI_NickDB,
	enum_DSI_Salesroom,
	enum_DSI_Count,
};

enum SERVER_LISTEN_TYPE
{
	enum_SBS_Private,
	enum_SBS_Public,
	enum_SBS_Count,
};

struct KServerSettingsInfo
{
	KString<32>		Name;
	INT				ID;
	INT				Type;
	INT				MaxPlayer;
	KString<16>		PublicAddr;
	int				PublicPort;
	KString<16>		PrivateAddr;
	int				PrivatePort;
	INT				MaxDBLink;
	INT				LogLevel;
};
DECLARE_SIMPLE_TYPE(KServerSettingsInfo)


struct KDatabaseSettingsInfo
{
	KString<16>		Addr;
	int				Port;
	KString<32>		DBName;
	KString<32>		DBUser;
	KString<32>		DBPassword;
	KString<32>		CharSet;
};
DECLARE_SIMPLE_TYPE(KDatabaseSettingsInfo)

class KServerSetting
{
public:
	KServerSetting();
	~KServerSetting();

	BOOL Init();


public:

	void SetServerID(enum SERVER_SETTINGS_INFO eServerType,INT nID);

	void SetServerIP(enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType,char* strIP);

	void SetServerPort(enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType,WORD wPort);


	INT GetServerID(enum SERVER_SETTINGS_INFO eServerType);

	const char* GetServerName(enum SERVER_SETTINGS_INFO eServerType);

	const char* GetServerIP(enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType);

	INT GetServerPort(enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType);

	INT GetMaxPlayer(enum SERVER_SETTINGS_INFO eServerType);

	INT GetMaxDBLink(enum SERVER_SETTINGS_INFO eServerType);

	INT GetLogLevel(enum SERVER_SETTINGS_INFO eServerType);

	const char* GetDBIp(enum DB_SETTINGS_INFO eDBType);

	const char* GetDBName(enum DB_SETTINGS_INFO eDBType);

	const char* GetDBUser(enum DB_SETTINGS_INFO eDBType);

	const char* GetDBPassword(enum DB_SETTINGS_INFO eDBType);

	const char* GetDBCharSet(enum DB_SETTINGS_INFO eDBType);


	INT GetDBPort(enum DB_SETTINGS_INFO eDBType);	


private:
	BOOL InitializeServerSettings(const char* xmlCfgFile);
	BOOL InitializeDatabaseSettings(const char* xmlCfgFile);
	void Finalize();

private:
	System::Collections::DynArray<KServerSettingsInfo> m_ServerSettings;
	System::Collections::DynArray<KDatabaseSettingsInfo> m_DBSettings;
	KThreadMutex	m_mx;
};
