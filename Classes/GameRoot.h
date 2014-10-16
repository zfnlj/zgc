#ifndef _GAME_ROOT_H_
#define _GAME_ROOT_H_
#include "cocos2d.h"
#include "System/Singleton.h"
#include "GameLogic/KClientBattleCtrl.h"
#include "platform/KIOSiAP_Bridge.h"
class BattleFieldScene;
class MainMenuScene;

USING_NS_CC;
class GameRoot : public CCObject , public Singleton<GameRoot> 
{
public :
	enum SCENE_DEF{
		BATTLE_FIELD_SCENE=911,
	};
	static GameRoot& getSingleton(void);
	
	void InitializeResource();
	BattleFieldScene* getBattleScene(){ return _battleScene;}
	void SetBattleScene(BattleFieldScene* scene){ _battleScene= scene;}
	MainMenuScene* getMainMenuScene(){ return _mainMenuScene;}
	void SetMainMenuScene(MainMenuScene* scene){ _mainMenuScene= scene;}
	int GetInc(){ return m_inc++;}
	void update(float dt);
	void init();
	const char* GetRootPath(){return m_rootPath;}
	KBattleCtrlBase* BattleCtrl();
	IOSiAP_Bridge* IAP_Bridge(){ return &m_iap;}
	void ParseCmd(char* cmdLine);
	void RunAutoTest();
	void onGameEnd(unsigned long long Param1);
	bool m_autoTest;
	bool m_bDebug;
	bool m_bRecMode;
private :
	void LoadStringDef(const char* filename);
	BattleFieldScene* _battleScene;
	MainMenuScene* _mainMenuScene;
	int m_inc;
	char m_rootPath[256];
	IOSiAP_Bridge m_iap;

};

#endif