
/********************************************************************
created:	2012/10/28
created:	
filename: 	
file path:	
file base:	KAppData
file ext:	h
author:		

purpose:	存储应用程序相关信息：服务器IP，大区....
*********************************************************************/
#pragma once
#include "Inc/KCommonStruct.h"
#include "System/Collections/DynComplexArray.h"
#include "System/Patterns/KSingleton.h"
#include <string>
using namespace std;

class KAppData : public System::Patterns::KSingleton<KAppData>
{
public:
	KAppData();
	~KAppData();
	
	static KAppData& getSingleton(void);
	const char * GetLoginIP() const { return m_strLoginServerIp.c_str(); }
	unsigned short GetLoginPort() const { return m_nLoginServerPort; }
	void SetMapID(DWORD dwMapID) {m_dwMapID = dwMapID;}
	DWORD GetMapID() const {return m_dwMapID;}

	void SetDuplicateID(DWORD dwDuplicateID) {m_dwWorldID = dwDuplicateID;}
	DWORD GetDuplicateID() const {return m_dwWorldID;}

	void SetMaxLineCount(DWORD dwMaxLineCount) {m_dwMaxLineCount = dwMaxLineCount;}
	DWORD GetMaxLineCount() const {return m_dwMaxLineCount;}

	void SetCurrLineID(DWORD dwCurrLineID) {m_dwCurrLineID = dwCurrLineID;}
	DWORD GetCurrLineID() const {return m_dwCurrLineID;}


	BOOL InitWorldInfo();
	//KWorldInfoInGame* GetWorldInfo(DWORD dwMapID); 
	//char* GetWorldFileName(DWORD dwMapID);

	int GetGatewayCount() const
	{
		return m_gatewayList.size();
	}
	const char* GetGatewayIP(int pos = 0)
	{
		if (pos >= 0 && pos < m_gatewayList.size())
		{
			return m_gatewayList[pos].ip.c_str();
		}
		return "";
	}
	WORD GetGatewayPort(int pos = 0)
	{
		if (pos >= 0 && pos < m_gatewayList.size())
		{
			return m_gatewayList[pos].port;
		}
		return 0;
	}



protected:

	std::string m_strGameName;				// 游戏名
	std::string m_strDistrictName;			// 大区名
	std::string m_strServerName;				// 服务器名
	std::string m_strVersion;				// 版本号

	std::string m_strLoginServerIp;			// 帐号服务器IP
	unsigned short m_nLoginServerPort;			// 帐号服务器PORT

	DWORD m_dwMapID;
	DWORD m_dwWorldID;

	DWORD m_dwMaxLineCount;
	DWORD m_dwCurrLineID;

	struct AddrType
	{
		std::string ip;
		WORD port;
	};
	typedef ::System::Collections::DynComplexArray<AddrType> GATEWAY_LIST_TYPE;
	GATEWAY_LIST_TYPE m_gatewayList;	// 可用的gateway列表

};
