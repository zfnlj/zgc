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
#include "ui/KUIAssist.h"
#include <System/File/KTabfileLoader.h>
#include "System/Cipher/tea.h"

IMPLEMENT_SIMPLE_SINGLETON(GameRoot)
using namespace System::File;

void encry(char* str,char* pwd)
{
	int len = strlen(str);
	int pwdLen = strlen(pwd);

	int keypos=0;
	for(int i=0;i<len;i++){
		str[i] = str[i]^ pwd[keypos];
		keypos++;
		if(keypos==pwdLen) keypos=0;
	}
}

void EncryTest()
{
	const int SIZE_IN = 64, SIZE_OUT = 64, SIZE_KEY = 16;
    BYTE plain[SIZE_IN]="I am a good guy. to you be here ..gogogogoogog!";
	BYTE crypt[SIZE_OUT];
	BYTE key[SIZE_KEY]="zhufanan";


	TEA tea(key, 16);
	memset(crypt,0,sizeof(crypt));
    tea.encrypt(plain, crypt);
	memset(plain,0,sizeof(plain));
	tea.decrypt(crypt, plain);

	char testStr[64]="I am a good guy. to you be here ..gogogogoogog!";
	char pwd[8]="zhufana";
	encry(testStr,pwd);
	encry(testStr,pwd);
	int kk=0;
}

void GameRoot::init()
{
	EncryTest();
	KScriptAbout::KToLua::Instance();
	sqlite3_config(SQLITE_CONFIG_SERIALIZED); //sqlite使用串行方式
	KClientGateListener::getSingleton().init();
	InitializeResource();
	m_battleCtrl.init(NULL);
	_battleScene = NULL;
	m_inc = 1;
	VirtualService::getSingleton().init();
	KQuestManager::GetInstance()->GetQuest(10001);
	KClientGateListener::getSingleton().ConnectLoginServer(STATIC_DATA_STRING("user"), STATIC_DATA_STRING("pwd"));	

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(GameRoot::update),this,0.1f,false);

	//KEventAbout::KEDKillMonster evt;
	//KDynamicWorld::getSingleton().FireEvent(evt);
}

void GameRoot::InitializeResource()
{
	memset(m_rootPath,0,sizeof(m_rootPath));
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("zgc.dat");
	int pos = fullPath.find("zgc.dat");
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
	m_battleCtrl.update(dt);
}

void GameRoot::LoadStringDef(const char* filename)
{
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
	KStringManager::Instance()->InitByDataFile(fullPath.c_str());
	//const char* t = G_STR(70001);
	//int kk=0;
}


