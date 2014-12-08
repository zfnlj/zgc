#ifndef _KAUDIOASSIST_H_
#define _KAUDIOASSIST_H_


class KAudioAssist
{
public:
	enum AudioScene{
		audio_null,
		audio_battle,
		audio_shop,
		audio_mainMenu,
		audio_cardGroup,
		audio_heroBrowse,
		audio_stageSelect,
		audio_stageWait,
	};
	//sound
	static void PlayDrawCardSound(int n);
	static void PlayClickButSound();
	static void PlayBGM(AudioScene def,float elapsed);
	static void PlaySceneBGM(AudioScene def);
	static void playSound(const char* name);

	static void StopTimeOutSound();
	static void PlayTimeOutSound();
};

#endif // __HELLOWORLD_SCENE_H__	
