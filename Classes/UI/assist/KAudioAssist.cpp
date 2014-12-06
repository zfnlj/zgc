#include "KAudioAssist.h"
#include "SimpleAudioEngine.h"
#include "../../StaticTable/StaticData.h"
#include "cocos-ext.h"
#include "../../GameLogic/KClientBattleCtrl.h"
using namespace cocos2d::extension;


void KAudioAssist::playSound(const char* name)
{
	int soundOn = STATIC_DATA_INT("Sound On");
	if(soundOn==0) return;
	float soundVol = (float)STATIC_DATA_INT("Sound Vol");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(soundVol*0.01f);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(name);
}

void KAudioAssist::PlaySceneBGM(AudioScene def)
{
	switch(def){
		case audio_stageWait:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bgm/battle_wait.mp3");
			break;
		case audio_mainMenu:
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bgm/bgm_3.mp3");
			break;
		default:
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bgm/bgm_2.mp3");
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			break;
	}

}

void KAudioAssist::PlayBGM(AudioScene def,float dt)
{
	return;
	static AudioScene lastDef = audio_null;
	int musicOn = STATIC_DATA_INT("Music On");
	if(musicOn==0 || def==audio_null){
		lastDef = audio_null;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		return;
	}
	float musicVol = (float)STATIC_DATA_INT("Music Vol");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(musicVol*0.01f);

	static float needTimes = 0.0f;
	
	if(lastDef == def){
		if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			needTimes -= dt;
			if(needTimes<0.0f){
				PlaySceneBGM(def);
				needTimes = 10.0f;
			}
		}
	}else{
		PlaySceneBGM(def);
		needTimes = 10.0f;
	}

	//if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) return;
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bgm/bgm_1.mp3");
	lastDef = def;
}

void KAudioAssist::PlayDrawCardSound(int n)
{
	if(KClientBattleCtrl::getInstance()->GetBattleState()==KBattleCtrlBase::battle_select_handcard) return;
	if(n>3) n=3;
	char sz[128];
	sprintf(sz,"audio/battle/card_shuffle%d.wav",n);
	playSound(sz);
}

void KAudioAssist::PlayClickButSound()
{
	playSound("audio/ui/but_click.wav");
}

static unsigned int timeOutSndId = 0;

void KAudioAssist::PlayTimeOutSound()
{
	if(timeOutSndId==0){
		timeOutSndId = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/battle/zz.wav",true);
	}else{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(timeOutSndId);
	}
}

void KAudioAssist::StopTimeOutSound()
{
	if(timeOutSndId!=0) CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(timeOutSndId);
}