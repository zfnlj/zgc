#include "KSoundStatic.h"
#include "System/tinyxml/xmldom.h"

KSoundStatic::KSoundStatic()
{	
}

void KSoundStatic::init()
{
	m_createTime = 0.0f;
	mLifeTime = 0.0f;
	m_bLoop = false;
}

KSoundStatic* KSoundStatic::create(XMLElementNode *pDataXml)
{

	if (NULL == pDataXml)	return NULL;
	KSoundStatic* sound = new KSoundStatic;
	sound->init();
	const StringValue* pValue = pDataXml->QueryAtt("soundEffectName");
	if (NULL != pValue) sound->mSoundEffectName = pValue->c_str();

	pValue = pDataXml->QueryAtt("createTime");
	sound->m_createTime = (pValue)?pValue->Float():0;

	pValue = pDataXml->QueryAtt("loop");
	if (NULL != pValue)	sound->m_bLoop = pValue->Bool()>0;

	pValue = pDataXml->QueryAtt("lifeTime");
	sound->mLifeTime = (pValue)?pValue->Float():0;

	sound->autorelease();
	return sound;
}


const char* KSoundStatic::GetName()
{
	return mSoundEffectName.c_str();
}

const char* KSoundStatic::getSoundEffectName() const 
{
	return mSoundEffectName.c_str();
}

