
/********************************************************************
created:	2012/10/28
created:	
filename: 	
file path:	
file base:	KAppData
file ext:	h
author:		

purpose:	�洢Ӧ�ó��������Ϣ��������IP������....
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

	std::string m_strGameName;				// ��Ϸ��
	std::string m_strDistrictName;			// ������
	std::string m_strServerName;				// ��������
	std::string m_strVersion;				// �汾��

	std::string m_strLoginServerIp;			// �ʺŷ�����IP
	unsigned short m_nLoginServerPort;			// �ʺŷ�����PORT

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
	GATEWAY_LIST_TYPE m_gatewayList;	// ���õ�gateway�б�

};
