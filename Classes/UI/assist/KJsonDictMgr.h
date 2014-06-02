#ifndef _KJSONDICTMGR_H
#define _KJSONDICTMGR_H
#include "System/Singleton.h"
#include "cocos-ext.h"
#include <list>
#define  MAX_CACHE_CARD_ELEM 10
USING_NS_CC;
USING_NS_CC_EXT;

class KJsonDictObj : public CCObject
{
public:
	KJsonDictObj():m_json(NULL){}
	~KJsonDictObj();

	cs::CSJsonDictionary* m_json;
};

typedef std::list<UIWidget*> KCardWidgetList;
class KJsonDictMgr :  public Singleton<KJsonDictMgr> 
{
public :
	static KJsonDictMgr& getSingleton(void);
	void init();

	void onDestory();
	KJsonDictObj* GetJsonDict(const char *fileName);
	UIWidget* widgetFromJsonFile(const char* fileName);
	UIWidget* CreateCardWidget();
	void OnCardWidgetDestory(UIWidget*);
	UIWidget* CreateMiniCardWidget();
	void OnMiniCardWidgetDestory(UIWidget*);
private :
	CCDictionary m_jsonDict;
	KCardWidgetList m_cacheCardWidget;
	KCardWidgetList m_miniCardWidget;
};


class KParticleCacheMgr :  public Singleton<KParticleCacheMgr> 
{
public :
	static KParticleCacheMgr& getSingleton(void);
	void init(){}

	void onDestory();
	CCParticleSystem* CreateParticle(const char* name);
	void RemvoeParticle(CCParticleSystem*,const char* name);
private :
	CCDictionary m_particleDict;
};

#endif