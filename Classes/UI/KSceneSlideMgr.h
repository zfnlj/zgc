#ifndef _KSCENESLIDEMGR_H
#define _KSCENESLIDEMGR_H

#include "cocos2d.h"
#include "System/Singleton.h"

USING_NS_CC;

class KSceneSlideMgr :public Singleton<KSceneSlideMgr> 
{
public:
	KSceneSlideMgr(){}
	~KSceneSlideMgr();
	
	static KSceneSlideMgr& getSingleton(void);
	void OpenSceneSlide(int first,int last);
	bool NextSlide();
	bool PrevSlide();
	int GetSlideNum();
	void Clear();
private:
	int m_firstId;
	int m_lastId;
	int m_curId;
};

#endif // __BattleFieldScene_SCENE_H__
