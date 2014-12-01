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

void KAudioAssist::PlayBGM(AudioScene def,float elapsed)
{
	int musicOn = STATIC_DATA_INT("Music On");
	if(musicOn==0){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		return;
	}
	float musicVol = (float)STATIC_DATA_INT("Music Vol");
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(musicVol*0.01f);
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) return;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("audio/bgm/bgm_1.mp3");
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