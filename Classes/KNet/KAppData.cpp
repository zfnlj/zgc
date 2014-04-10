#include "KAppData.h"

#include "System/tinyxml/xmldom.h"
#include "System/Cipher/jgencryptor.h"
#include "KUserData.h"

KAppData::KAppData(void):
 m_strGameName("")
, m_strLoginServerIp("")
, m_nLoginServerPort(0)
{
	m_dwMapID = 0;
	m_dwWorldID = 0;

	m_dwMaxLineCount = 0;
	m_dwCurrLineID = 0;
}

KAppData::~KAppData(void)
{
}

BOOL KAppData::InitWorldInfo()
{
	/*
	XMLElementNode m_WorldCfgXml;
	m_WorldCfgXml.Clear();

	XMLDomParser parser;
	parser.Initialize("GB2312");

	Ogre::DataStreamPtr dataStreamPtr = Ogre::ResourceGroupManager::getSingleton().openResource("world.xml","General");
	if (dataStreamPtr.isNull())
	{
		return false;
	}
	KStreamWrapper streamWrapper = dataStreamPtr;
	JgEncStream  encStream;
	encStream.setInputStream(&streamWrapper);

	if(!parser.Parse(encStream, m_WorldCfgXml))
	{
		Log(LOG_ERROR, "error: parse ,%s,%s", "world.xml",parser.GetErrorMsg());
		return FALSE;
	}

	XMLNodeColl collMap = m_WorldCfgXml.QueryChildren("WorldInfo/Map");
	for(int i=0; i<collMap.Size(); i++)
	{
		XMLElementNode* pMapXml = (XMLElementNode*)collMap.GetNode(i);
		KWorldInfoInGame mapInfo;
		mapInfo.m_dwMapID = pMapXml->QueryAtt("Map.value")->Integer();
		mapInfo.m_strName = pMapXml->QueryAtt("name.value")->cstr();
		mapInfo.m_strWorldFile = pMapXml->QueryAtt("WorldFile.value")->cstr();
		mapInfo.m_strL2MapFile = pMapXml->QueryAtt("L2MapPath.value")->cstr();
		m_MapInfoMap.insert_unique(mapInfo.m_dwMapID, mapInfo);
	}
	*/
	return TRUE;
}


//KWorldInfoInGame * KAppData::GetWorldInfo(DWORD dwMapID)
//{
//	//KMapInfoMap::iterator it = m_MapInfoMap.find(dwMapID);
//	//if(it == m_MapInfoMap.end())return NULL;
//	//return &it->second;
//	return NULL;
//}
//
//char* KAppData::GetWorldFileName( DWORD dwMapID )
//{
//	//KMapInfoMap::iterator it = m_MapInfoMap.find(dwMapID);
//	//if(it == m_MapInfoMap.end())return "";
//	//return it->second.m_strWorldFile.c_str();
//	return NULL;
//}