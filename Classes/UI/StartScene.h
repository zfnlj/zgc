//
//  StartScene.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#ifndef __FishingJoy__StartScene__
#define __FishingJoy__StartScene__
#include "cocos2d.h"
#include "ProgressProtocol.h"
#include "ProgressBar.h"

USING_NS_CC;

class StartScene : public cocos2d::CCScene, public ProgressDelegate
{
public:
    bool init();
    CREATE_FUNC(StartScene);
    void loadingFinished();
    void progressPercentageSetter(float percentage);

	bool LoadRes();
protected:
	void update(float delta);
    ProgressBar* _progressBar;
    cocos2d::CCLabelTTF* _progressFg;
    void transition(CCObject* pSender);
    void loading(CCObject* pObj);
    void cacheInit();
    void audioAndUserDataInit();
    void initializationCompleted();
	void LoadArmature();
	int m_curLoadPic;
	int m_totalLoadPic;
};
#endif /* defined(__FishingJoy__StartScene__) */
