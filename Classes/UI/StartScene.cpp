//
//  StartScene.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-8.
//
//

#include "StartScene.h"
#include "BattleFieldScene.h"
#include "MainMenuScene.h"
#include "GameRoot.h"
#include <System/File/KTabfileLoader.h>
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#include "assist/KUIAssist.h"
#include "assist/KJsonDictMgr.h"
#include "../StaticTable/StaticData.h"

//#include "PersonalAudioEngine.h"

USING_NS_CC;
//using namespace CocosDenshion;
using namespace System::File;

bool StartScene::init()
{
	m_curLoadPic = m_totalLoadPic = 0;

    if(CCScene::init()){
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        //CCSprite* background = CCSprite::create(STATIC_DATA_STRING("background"));
        //background->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.5));
        //this->addChild(background);
        //
        //CCSprite* title = CCSprite::create(STATIC_DATA_STRING("title"));
        //title->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.7));
        //this->addChild(title);

        
			CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
			CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
            CCSprite* bg = CCSprite::create("zgc_bk.jpg");
			bg->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
			this->addChild(bg);
			
            _progressFg = CCLabelTTF::create("0/100", "Thonburi", 16);
            _progressFg->setColor(ccc3(0, 0, 0));            

            _progressBar = ProgressBar::create(this, CCSprite::create("progress_bar.png"));
			_progressBar->setBackground(CCSprite::create("progress_bg.png"));
//            _progressBar->setBackground(progressBg);
            _progressBar->setForeground(_progressFg);
            _progressBar->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.5));
            _progressBar->setSpeed(100.0);

            this->addChild(_progressBar);

            this->audioAndUserDataInit();
			LoadRes();

        return true;
    }
    return false;
}
void StartScene::audioAndUserDataInit()
{
    //FishingJoyData::sharedFishingJoyData();
    //PersonalAudioEngine::sharedEngine();
}
void StartScene::loading(CCObject* pObj)
{
	m_curLoadPic++;
	float curPercent = ((float)m_curLoadPic / m_totalLoadPic) * 100;  

    _progressBar->progressTo(curPercent);
}
void StartScene::transition(CCObject* pSender)
{
	if(STATIC_DATA_INT("first_help")==0){
		STATIC_DATA_SET("first_help",1);
		KUIAssist::_openMainHelpScene();
	}else{
		CCScene* scene = CCTransitionFadeBL::create(2.0f, MainMenuScene::scene());
		CCDirector::sharedDirector()->replaceScene(scene);
	}
	//CCScene* scene = CCTransitionFadeBL::create(2.0f, BattleFieldScene::scene());
    
}
void StartScene::cacheInit()
{
	//KUIAssist::_loadAnimationData("GUI/active_green","active_green",31,0.3);
	//KUIAssist::_loadAnimationData("GUI/active_red","active_red",31,0.3);
	KJsonDictMgr::getSingleton().GetJsonDict("GUI/battle.json");

	LoadArmature();

	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/List.txt");
	
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return;
	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return ; }
		if(nRet == 0) break;
		char sz[128];
		fileReader->GetString("plist", "",sz, 127);
		if(strlen(sz)>0){
			 std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(sz);
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(fullPath.c_str());
		}
		char anim[128];
		fileReader->GetString("anim", "",anim, 127);
		if(strlen(anim)>2){
			int frame;
			fileReader->GetInteger("frame", 0, (int*)&frame);
			float interval;
			fileReader->GetFloat("interval", 0, (float*)&interval);
			KUIAssist::_loadAnimationData(sz,anim,frame,interval);
		}
	}
	loader.CloseFileReader(fileReader);


	//CCTextureCache::sharedTextureCache()->
    
	/*   int frameCount = STATIC_DATA_INT("fish_frame_count");
	for (int type = k_Fish_Type_Red; type < k_Fish_Type_Count; type++) {
	CCArray* spriteFrames = CCArray::createWithCapacity(frameCount);
	for(int i = 0;i < frameCount;i++){
	CCString* filename = CCString::createWithFormat(STATIC_DATA_STRING("fish_frame_name_format"),type,i);
	CCSpriteFrame* spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename->getCString());
	spriteFrames->addObject(spriteFrame);
	}
	CCAnimation* fishAnimation = CCAnimation::createWithSpriteFrames(spriteFrames);
	fishAnimation->setDelayPerUnit(STATIC_DATA_FLOAT("fish_frame_delay"));
	CCString* animationName = CCString::createWithFormat(STATIC_DATA_STRING("fish_animation"), type);
	CCAnimationCache::sharedAnimationCache()->addAnimation(fishAnimation, animationName->getCString());
	}*/

}
void StartScene::initializationCompleted()
{
	transition(NULL);
  /*  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCMenuItemSprite* start = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("start_normal")), CCSprite::createWithSpriteFrameName(STATIC_DATA_STRING("start_selected")), this, menu_selector(StartScene::transition));
    CCMenu* menu = CCMenu::create(start, NULL);
    menu->setPosition(CCPointMake(winSize.width*0.5, winSize.height*0.4));
    
    this->removeChild(_progressBar, true);
    this->addChild(menu);
    PersonalAudioEngine::sharedEngine()->playBackgroundMusic(STATIC_DATA_STRING("bg_music"));*/
}
void StartScene::loadingFinished()
{
    this->cacheInit();
    this->initializationCompleted();
}
void StartScene::progressPercentageSetter(float percentage)
{
    CCString* str = CCString::createWithFormat("%d/100",(int)percentage);
    _progressFg->setString(str->getCString());
}

void StartScene::update(float delta)
{

}

bool StartScene::LoadRes()
{
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/List.txt");
	
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;
	CCTexture2D::PVRImagesHavePremultipliedAlpha(true);
	m_totalLoadPic = 100;
	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;
		char sz[128];
		fileReader->GetString("png", "",sz, 127);
		if(strlen(sz)>0){
			CCTextureCache::sharedTextureCache()->addImageAsync(sz, this, callfuncO_selector(StartScene::loading));
			m_totalLoadPic++;
		}
	}
	loader.CloseFileReader(fileReader);
	m_totalLoadPic -= 100;
	if(m_totalLoadPic==0) loadingFinished();
	return true;
}

void StartScene::LoadArmature()
{
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/Armature.txt");
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return;
	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return ; }
		if(nRet == 0) break;
		char sz[128];
		fileReader->GetString("File", "",sz, 127);
		if(strlen(sz)>2){
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(sz);
		}
	}
	loader.CloseFileReader(fileReader);
}
