#include "GameRoot.h"
#include "UI/BattleFieldScene.h"
#include "StaticTable/KGameStatic.h"
#include "common/KClientTabfileLoader.h"
#include "GameLogic/KDynamicWorld.h"
#include "KAction/KActionStaticMgr.h"
#include "KNet/KClientGateListener.h"
#include "StaticTable/StaticData.h"
#include "VirtualService.h"
#include "CommonLogic/StringManager/KStringManager.h"
#include "sqlite/sqlite3.h"
#include "Quest/KQuestManager.h"
#include "ClientLogic/KToLua.h"
#include "EventSystem/KEventDefine.h"
#include "ui/assist/KUIAssist.h"
#include <System/File/KTabfileLoader.h>
#include "System/Cipher/tea.h"
#include "DB/KPlayerDBMgr.h"
#include "sqlite/KUserSql.h"
#include "ui/assist/KQuestFacade.h"
#include "WorldObject/KMainPlayer.h"
#include "Quest/KPlayerQuestManager.h"
#include "WorldObject/KPlayer.h"

IMPLEMENT_SIMPLE_SINGLETON(GameRoot)
using namespace System::File;

void GameRoot::init()
{
	KScriptAbout::KToLua::Instance();
	sqlite3_config(SQLITE_CONFIG_SERIALIZED); //sqlite使用串行方式
	KClientGateListener::getSingleton().init();
	InitializeResource();
	KClientBattleCtrl::getInstance()->init(NULL);
	_battleScene = NULL;
	m_inc = 1;
	VirtualService::getSingleton().init();
	KQuestManager::GetInstance()->GetQuest(10001);
	KClientGateListener::getSingleton().ConnectLoginServer(STATIC_DATA_STRING("user"), STATIC_DATA_STRING("pwd"));	

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameRoot::update),this,0.1f,false);

	unsigned long long val = time(NULL);
	srand(val);//
	//KEventAbout::KEDKillMonster evt;
	//KDynamicWorld::getSingleton().FireEvent(evt);

	//tm interval;
	//time_t t1 = 0;
	//time_t t2 = _GetSystemTimeVal();
	//_GetTimeInterval(t1,t2,interval);
	//int kk=0;
}

void GameRoot::InitializeResource()
{
	memset(m_rootPath,0,sizeof(m_rootPath));
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("List.txt");
	int pos = fullPath.find("List.txt");
	memcpy(m_rootPath,fullPath.c_str(),pos-1);

	KClientTabfileLoader::GetInstance();
	KGameStaticMgr::getSingleton().LoadStaticData();
	KQuestManager::GetInstance()->Initialize();
	LoadStringDef("StringManager/System.txt");
	LoadStringDef("StringManager/ItemDesc.txt");
}

void GameRoot::update(float dt)
{
	KClientGateListener::getSingleton().update(dt);
	KClientBattleCtrl::getInstance()->update(dt);
}

void GameRoot::LoadStringDef(const char* filename)
{
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	KStringManager::Instance()->InitByDataFile(fullPath.c_str());
	//const char* t = G_STR(70001);
	//int kk=0;
}

KBattleCtrlBase* GameRoot::BattleCtrl()
{
	return KClientBattleCtrl::getInstance();
}

void GameRoot::RunAutoTest()
{
	if(!m_autoTest) return ;
	KUIAssist::_switch2BattleScene();

	KQuestManager::GetInstance()->syncAvailQuests();
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = KMainPlayer::RealPlayer()->RndQueryAdventureQuest();
	KClientBattleCtrl::getInstance()->PlayAutoQuestBattle(pQuest);
}

void GameRoot::ParseCmd(char* cmdLine)
{
	m_autoTest = false;
	char* ss[64];
	int ns = split(cmdLine, " ", ss, 64);
	for(int i=0;i<ns;i++){
		char* cmd = ss[i];
		if(strcmp(cmd,"-AT")==0){
			m_autoTest = true;
		}
	}
}

void GameRoot::onGameEnd(unsigned long long Param1)
{
	if(m_autoTest){
		KQuestManager::GetInstance()->syncAvailQuests();
		KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
		KQuestNew* pQuest = KMainPlayer::RealPlayer()->RndQueryAdventureQuest();
		KClientBattleCtrl::getInstance()->PlayAutoQuestBattle(pQuest);
	}else{
		getBattleScene()->GameResult().onGameEnd(Param1);
	}
}