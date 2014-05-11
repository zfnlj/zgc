#include "KJsonDictMgr.h"

IMPLEMENT_SINGLETON(KJsonDictMgr)

KJsonDictObj::~KJsonDictObj()
{
	CC_SAFE_DELETE(m_json);
}

void KJsonDictMgr::init()
{
}

UIWidget* KJsonDictMgr::widgetFromJsonFile(const char* fileName)
{
	KJsonDictObj* dictObj = GetJsonDict(fileName);
	if(!dictObj) return NULL;
	return GUIReader::shareReader()->widgetFromJsonDict(dictObj->m_json,fileName);
}

KJsonDictObj* KJsonDictMgr::GetJsonDict(const char *fileName)
{
	KJsonDictObj* dictObj = (KJsonDictObj*)m_jsonDict.objectForKey(fileName);
	if(!dictObj){
		dictObj = new KJsonDictObj;
		dictObj->m_json = GUIReader::shareReader()->CreateJsonDict(fileName);
		m_jsonDict.setObject(dictObj, fileName);
	}
	return dictObj;
}

void KJsonDictMgr::onDestory()
{
	m_jsonDict.removeAllObjects(); //leak????
}


void KParticleCacheMgr::onDestory()
{
	m_particleDict.removeAllObjects(); //leak????
}

CCParticleSystem* KParticleCacheMgr::CreateParticle(const char* name)
{
	CCParticleSystem* particle = NULL;//(CCParticleSystem*)m_particleDict.objectForKey(name);
	if(particle){
		m_particleDict.removeObjectForKey(name);
		particle->resetSystem();
		particle->update(0.01f);
	}else{
		particle = new CCParticleSystemQuad();
		char msg[128];
		sprintf(msg,"data/Particles/%s.plist",name);
		particle->initWithFile(msg);
		particle->autorelease();
	}
	return particle;
}

IMPLEMENT_SINGLETON(KParticleCacheMgr)
void KParticleCacheMgr::RemvoeParticle(CCParticleSystem* particle,const char* name)
{
	m_particleDict.setObject(particle, name);
	particle->removeFromParentAndCleanup(false);
}