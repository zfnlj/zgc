
#include "KActionStaticMgr.h"
#include <System/tinyxml/xmldom.h>
#include "System/Cipher/jgencryptor.h"
#include "System/Misc/KCrc32.h"
#include "../Inc/KTypeDef.h"
#include "../common/OgreDataStream.h"
#include "../Common/KStreamWrapper.h"
#include <fstream> 
#include <string>

#define ACTION_STATIC		"A"
#define ACTION_NAME			"B"
#define ACTION_STATIC_DATA	"C"
#define ANIM_GRP_ST_DATA    "D"
#define ANIM_GRP_DATA       "E"
#define ANIM_ST             "F"
#define EFF_GRP_DATA        "G"
#define OBJ_GRP_DATA        "H"
#define AFF_GRP_DATA        "I"
#define AFF_TYPE            "J"
#define AFF_BASE_DATA       "K"
#define AFF_DERIVE_DATA     "L"
#define SND_GRP_DATA        "M"

KAffectorStatic * createNewAffectorStatic(XMLElementNode* pDataXml)
{
	const StringValue* pClass = pDataXml->QueryAtt("class");
	CCString className = pClass->c_str();
	KAffectorStatic* pAffector = NULL;
	if(className.compare("action")==0){
		pAffector =  new KAffectorActionStatic; 
	}else{
		pAffector =  new KAffectorStatic;
	}
	pAffector->init(pDataXml);
	return pAffector;
}

void KActionStatic::ParseClass(const char* className)
{
	if(strcmp(className,"move")==0)
	{
		m_ActionData.m_class = class_move;
	}
	else if(strcmp(className,"cast")==0)
	{
		m_ActionData.m_class = class_cast;
	}
	else if(strcmp(className,"hit")==0)
	{
		m_ActionData.m_class = class_hit;
	}
	else
	{
		m_ActionData.m_class = class_null;
	}
}

bool XmlLoadAffectBody(XMLElementNode* pNode,KActionStatic& action)
{
	if(!pNode)
		return true;

	XMLNodeColl objColl = pNode->QueryChildren("affector");
	for(int i=0; i< objColl.Size();++i)
	{
		XMLElementNode* pDataXml = (XMLElementNode*)objColl.GetNode(i);
		KAffectorStatic *pAff = createNewAffectorStatic(pDataXml);
		action.AddAffector(pAff);
	}
	return true;
}

// add by DuanGuangxiang 2012 5 2
bool XmlLoadSoundEffectBody(XMLElementNode* pNode,KActionStatic& action)
{
	if(!pNode)
		return true;

	XMLNodeColl objColl = pNode->QueryChildren("sound");

	for(int i=0; i< objColl.Size();++i)
	{
		XMLElementNode* pDataXml = (XMLElementNode*)objColl.GetNode(i);
		KSoundStatic* soundStatic = KSoundStatic::create(pDataXml);
		action.AddSoundEffect(soundStatic);
	}
	return true;
}
// the end

KActionStatic::SActionData::SActionData():
m_loop(0)
,m_crc(0)
{
}



KActionStatic::KActionStatic(const char* name)
{
	memset(&m_ActionData,0,sizeof(m_ActionData));
    strcpy(m_ActionData.m_name,name);
	CalcCRC();
}

KActionStatic::KActionStatic()
{
}

KActionStatic::KActionStatic(const KActionStatic& action)
{
	*this =action;
}

KActionStatic& KActionStatic::operator=( const KActionStatic& action)
{
	m_effectSTVect = action.m_effectSTVect;
	m_AffectorVect = action.m_AffectorVect;
	m_SoundEffectVec = action.m_SoundEffectVec;

	m_ActionData = action.m_ActionData;
	return *this;
}

void KActionStatic::setName(const char* name)
{
    strcpy(m_ActionData.m_name,name);

	CalcCRC();
}

unsigned long KActionStatic::CalcCRC()
{
	const char* name = GetName();
	int len = strlen(name);
	if( len==0)
		return 0;
	m_ActionData.m_crc = crc32c((unsigned char*)name,len);
	return m_ActionData.m_crc;
}


const char* KActionStatic::GetName(void) const
{
	return m_ActionData.m_name;
}


IMPLEMENT_SINGLETON(KActionStaticMgr)

bool KActionStaticMgr::m_bNoAction = false;
KActionStatic* KActionStaticMgr::GetAction(const char* name)
{
	if(m_bNoAction)
		return NULL;
	if(strlen(name)==0)
		return NULL;
	KActionStatic* pAction = (KActionStatic*)m_ActionDict.objectForKey(name);
	if(pAction){
		return pAction;
	}else{
		return LoadActionXmlFile(name);
	}
}


void KActionStaticMgr::AddAction(KActionStatic* pST)
{
	m_ActionDict.setObject(pST,pST->GetName());
}

KActionStatic* KActionStaticMgr::LoadXmlActionData(const char* name,DataStream* dataStream)
{

	XMLDomParser parser;
	parser.Initialize("GB2312");

	XMLElementNode actionXml;
	KStreamWrapper streamWrapper;
	streamWrapper.init(dataStream);
	JgEncStream  encStream;
	encStream.setInputStream(&streamWrapper);
	if(!parser.Parse(encStream, actionXml))	return NULL;
	
	XMLNodeColl collMap = actionXml.QueryChildren("action");
	for(int j=0;j<collMap.Size();j++){
		XMLElementNode* pDataXml = (XMLElementNode*)collMap.GetNode(j);

		const StringValue* pName = pDataXml->QueryAtt("name");
		KActionStatic* pST = (j==0)? new KActionStatic(name): new KActionStatic(pName->c_str());

		KActionStatic& actionST = *pST;
		const StringValue* pLoop = pDataXml->QueryAtt("loop");
		actionST.m_ActionData.m_loop =(uchar)( (pLoop)? pLoop->Bool() : 0);

		const StringValue* pSlot = pDataXml->QueryAtt("slot");
		actionST.m_ActionData.m_slot =(uchar)( (pSlot)? pSlot->Integer() : 0);

		const StringValue* pClass = pDataXml->QueryAtt("class");
		actionST.ParseClass(pClass->c_str());

		XMLNodeColl affBodyColl = pDataXml->QueryChildren("affect_body");
		for(int i=0; i< affBodyColl.Size();++i)
		{
			XMLElementNode* affBody = (XMLElementNode*)affBodyColl.GetNode(i);
			XmlLoadAffectBody(affBody,actionST);
		}

		// add by DuanGuangxiang 2012 5 2
		XMLNodeColl soundEffectColl = pDataXml->QueryChildren("sound_body");
		for (int i=0; i< soundEffectColl.Size();++i)
		{
			XMLElementNode* soundBody = (XMLElementNode*)soundEffectColl.GetNode(0);
			XmlLoadSoundEffectBody(soundBody,actionST);
		}
		AddAction(pST);
	}

	return (KActionStatic*)m_ActionDict.objectForKey(name);

}

KActionStatic* KActionStaticMgr::LoadActionXmlFile(const char* name)
{
    char buf[128]={0};
	sprintf(buf,"data/Action/%s%s",name,".action");
	
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(buf);

	std::ifstream fp;
	fp.open(fullPath.c_str(), std::ios::in | std::ios::binary);
	if(!fp)
		return NULL;

	DataStream* stream =  new FileStreamDataStream(name, &fp, false);
	return LoadXmlActionData(name,stream);
}



KActionStatic* KActionStaticMgr::NewAction(const char* name)
{
	KActionStatic* pAction = GetAction(name);
	if(pAction)
		return pAction;
	KActionStatic* actionST = new KActionStatic(name);
	AddAction(actionST);
	return actionST;
}

void KActionStaticMgr::Clear()
{
	m_ActionDict.removeAllObjects();
}

