#include "KServerSetting.h"

KServerSetting::KServerSetting()
{

}

KServerSetting::~KServerSetting()
{

}

BOOL KServerSetting::Init()
{
	if(!InitializeServerSettings(SETTINGS_FILE_PATH))
	{
		Log(LOG_ERROR , "InitializeServerSettings %s Error" , SETTINGS_FILE_PATH);
		return FALSE;
	}

	if(!InitializeDatabaseSettings(DATABASE_FILE_PATH))
	{
		Log(LOG_ERROR , "InitializeDatabaseSettings %s Error" , SETTINGS_FILE_PATH);
		return FALSE;
	}
	return TRUE;
}

BOOL KServerSetting::InitializeServerSettings( const char* xmlCfgFile )
{
	XMLElementNode m_xmlSetting;

	XMLDomParser parser;
	parser.Initialize("GB2312");

	KInputFileStream fi;
	if(!fi.Open(xmlCfgFile))
	{
		return FALSE;
	}
	if(!parser.Parse(fi, m_xmlSetting))
	{
		Log(LOG_FATAL, "Error %s" , parser.GetErrorMsg());
		return FALSE;
	}

	XMLNodeColl coll;
	coll = m_xmlSetting.QueryChildren("ServerSettings/Server");
	for(int i=0; i < coll.Size(); i++)
	{
		XMLElementNode* pXml = (XMLElementNode*)coll.GetNode(i);
		int type	= (int)pXml->QueryAtt("Type")->Integer();
		m_ServerSettings[type].ID			= pXml->QueryAtt("ID")->Integer();
		m_ServerSettings[type].Name			= pXml->QueryAtt("Name")->c_str();
		m_ServerSettings[type].PrivateAddr	= pXml->QueryAtt("PrivateIP")->c_str();
		m_ServerSettings[type].PrivatePort	= pXml->QueryAtt("PrivatePort")->Integer();
		m_ServerSettings[type].PublicAddr	= pXml->QueryAtt("PublicIP")->c_str();
		m_ServerSettings[type].PublicPort	= pXml->QueryAtt("PublicPort")->Integer();
		m_ServerSettings[type].LogLevel		= pXml->QueryAtt("LogLevel")->Integer();

		switch(type)
		{
		case enum_SSI_LoginServer:
		case enum_SSI_CharacterServer:
		case enum_SSI_WorldServer:
		case enum_SSI_GameServer:
		case enum_SSI_ChatServer:
			{
				m_ServerSettings[type].MaxPlayer = (int)pXml->QueryAtt("MaxPlayer")->Integer();
			};
		}

		switch(type)
		{
		case enum_SSI_LoginServer:
		case enum_SSI_CharacterServer:
		case enum_SSI_MailServer:
			{
				m_ServerSettings[type].MaxDBLink = (int)pXml->QueryAtt("MaxDBLink")->Integer();
			}
			break;
		}			
	}

	return TRUE;
}

BOOL KServerSetting::InitializeDatabaseSettings( const char* xmlCfgFile )
{
	XMLElementNode m_xmlSetting;

	XMLDomParser parser;
	parser.Initialize("GB2312");

	KInputFileStream fi;
	if(!fi.Open(xmlCfgFile))
	{
		return FALSE;
	}
	if(!parser.Parse(fi, m_xmlSetting))
	{
		Log(LOG_FATAL, "Error %s" , parser.GetErrorMsg());
		return FALSE;
	}

	XMLNodeColl coll;
	coll = m_xmlSetting.QueryChildren("DatabaseSettings/Database");
	for(int i=0; i < coll.Size(); i++)
	{
		XMLElementNode* pXml = (XMLElementNode*)coll.GetNode(i);
		int type	= (int)pXml->QueryAtt("Type")->Integer();
		m_DBSettings[type].Addr		= pXml->QueryAtt("IP")->c_str();
		m_DBSettings[type].Port		= pXml->QueryAtt("Port")->Integer();
		m_DBSettings[type].DBName		= pXml->QueryAtt("DBName")->c_str();
		m_DBSettings[type].DBUser		= pXml->QueryAtt("DBUser")->c_str();
		m_DBSettings[type].DBPassword	= pXml->QueryAtt("DBPassword")->c_str();
		m_DBSettings[type].CharSet		= pXml->QueryAtt("DBChatSet")->c_str();
	}

	return TRUE;
}

void KServerSetting::Finalize()
{

}

void KServerSetting::SetServerID( enum SERVER_SETTINGS_INFO eServerType,INT nID )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	m_ServerSettings[eServerType].ID = nID;
}

void KServerSetting::SetServerIP( enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType,char* strIP )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	if (eListenType == enum_SBS_Public)		 m_ServerSettings[eServerType].PublicAddr = strIP;
	else		 m_ServerSettings[eServerType].PrivateAddr = strIP;
}

void KServerSetting::SetServerPort( enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType,WORD wPort )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	if (eListenType == enum_SBS_Public)		m_ServerSettings[eServerType].PublicPort = wPort;
	else		m_ServerSettings[eServerType].PrivatePort = wPort;
}

INT KServerSetting::GetServerID( enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	return m_ServerSettings[eServerType].ID;
}

const char* KServerSetting::GetServerName( enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	return m_ServerSettings[eServerType].Name.c_str();
}

const char* KServerSetting::GetServerIP( enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	if (eListenType == enum_SBS_Public)		return m_ServerSettings[eServerType].PublicAddr.c_str();
	else		return m_ServerSettings[eServerType].PrivateAddr.c_str();
}

INT KServerSetting::GetServerPort( enum SERVER_LISTEN_TYPE eListenType , enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	if (eListenType == enum_SBS_Public)		return m_ServerSettings[eServerType].PublicPort;
	else		return m_ServerSettings[eServerType].PrivatePort;
}

INT KServerSetting::GetMaxPlayer( enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	return m_ServerSettings[eServerType].MaxPlayer;
}

INT KServerSetting::GetMaxDBLink( enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	return m_ServerSettings[eServerType].MaxDBLink;
}

INT KServerSetting::GetLogLevel( enum SERVER_SETTINGS_INFO eServerType )
{
	ASSERT(eServerType >= enum_SSI_ServerStart && eServerType < enum_SSI_Count);
	return m_ServerSettings[eServerType].LogLevel;
}

const char* KServerSetting::GetDBIp( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].Addr.c_str();
}

const char* KServerSetting::GetDBName( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].DBName.c_str();
}

const char* KServerSetting::GetDBUser( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].DBUser.c_str();
}

const char* KServerSetting::GetDBPassword( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].DBPassword.c_str();
}

const char* KServerSetting::GetDBCharSet( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].CharSet.c_str();
}

INT KServerSetting::GetDBPort( enum DB_SETTINGS_INFO eDBType )
{
	ASSERT(eDBType >= enum_DSI_DBStart && eDBType < enum_DSI_Count);
	return m_DBSettings[eDBType].Port;
}