#include "KGameStatic.h"
#include "KAbilityStatic.h"
#include <System/File/KTabfileLoader.h>
#include "KRankStaticMgr.h"
#ifdef _USE_COCOS2DX
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#endif

#include "../common/KCommonObj.h"

using namespace System::File;

template<> KGameStaticMgr* Singleton<KGameStaticMgr>::mSingleton = 0;


KGameStaticMgr& KGameStaticMgr::getSingleton(void)
{
	if(!mSingleton){
		mSingleton = new KGameStaticMgr;
	}
	return *mSingleton;
}

void KGameStaticMgr::LoadStaticData()
{
    InitCard("data/card/SoldierCard.txt");
	InitCard("data/card/HeroCard.txt");
	InitCard("data/card/SkillCard.txt");
    InitCard("data/card/Secret.txt");
	InitAbility("data/card/Ability.txt");
	InitAttr("data/card/Attr.txt");
	InitDeckDef("data/DeckDef.txt");
	InitBattleField("data/BattleField.txt");
	InitRank("data/Rank.txt");
	InitCardLayout("data/card_layout.txt");
	InitHeroSkill("data/card/hero_skill.txt");
	InitHelpString("data/HelpStr.txt");
}

bool KGameStaticMgr::InitRank(const char* m_FileName)
{
	std::string pathKey = m_FileName;

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;
	KRankStaticDataManager::Instance()->Init(fileReader);
	int lev = KRankStaticDataManager::Instance()->ExpToLevel(0);
	loader.CloseFileReader(fileReader);
	return true;

}

bool KGameStaticMgr::InitHeroSkill(const char* m_FileName)
{
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(m_FileName);

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KHeroSkillStatic* pStatic = KHeroSkillStatic::create();
		pStatic->Init(fileReader);
		m_heroSkillMap[pStatic->m_Id] = pStatic;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

bool KGameStaticMgr::InitCardLayout(const char* m_FileName)
{
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(m_FileName);

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KCardLayoutStatic* pStatic = KCardLayoutStatic::create();
		pStatic->Init(fileReader);
		m_cardLayoutMap[pStatic->m_Id] = pStatic;
	}

	loader.CloseFileReader(fileReader);
	return true;
}


bool KGameStaticMgr::InitBattleField(const char* m_FileName)
{
	std::string pathKey = m_FileName;

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KBattleFieldStatic* pBattle = KBattleFieldStatic::create();
		pBattle->Init(fileReader);
		m_battleFieldMap[pBattle->m_Id] = pBattle;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

bool KGameStaticMgr::InitDeckDef(const char* m_FileName)
{
	std::string pathKey = m_FileName;

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KDeckDefStatic* pDeck = KDeckDefStatic::create();
		pDeck->Init(fileReader);
		m_deckMap[pDeck->m_Id] = pDeck;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

bool KGameStaticMgr::InitAttr(const char* m_FileName)
{
	std::string pathKey = m_FileName;

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KAttrStatic* pAttr = KAttrStatic::create();

		fileReader->GetInteger("ID", 0, (int*)&pAttr->m_CardId);
		fileReader->GetInteger("GUIDE", 0, (int*)&pAttr->m_guide);
		fileReader->GetInteger("RUSH", 0, (int*)&pAttr->m_rush);
		fileReader->GetInteger("HIDE", 0, (int*)&pAttr->m_hide);
		fileReader->GetInteger("DIST", 0, (int*)&pAttr->m_dist);
		m_attrArr[pAttr->m_CardId] = pAttr;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

bool KGameStaticMgr::InitAbility(const char* m_FileName)
{
	std::string pathKey = m_FileName;

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)
	{
		return false;
	}


	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KAbilityStatic* pAbility = KAbilityStatic::create();
		pAbility->Init(fileReader);
		m_abilityMap[pAbility->m_AbilityId] = pAbility;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

bool KGameStaticMgr::InitCard(const char* m_FileName)
{
	std::string pathKey = m_FileName;

    std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
#else
	fullPath = pathKey;
#endif
	

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)
	{
		return false;
	}

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;
		char buf[128]={0};
		KCardStatic* pCard = KCardStatic::create();
		pCard->Init(fileReader);
		m_cardMap[pCard->GetID()] = pCard;
	}

	loader.CloseFileReader(fileReader);
	return true;
}


KCardStatic* KGameStaticMgr::GetCard(int id)
{
	if(m_cardMap.exists(id)){
		return m_cardMap[id];
	}else{
		return NULL;
	}
}

KHeroSkillStatic* KGameStaticMgr::GetHeroSkill(int id)
{
	KHeroSkillMap::iterator it  = m_heroSkillMap.find(id);
	if(it==m_heroSkillMap.end()) return NULL;
	return it->second;
}

KAbilityStatic* KGameStaticMgr::GetAbilityOnId(int id)
{
	KAbilityMap::iterator it  = m_abilityMap.find(id);
	if(it==m_abilityMap.end()) return NULL;
	return it->second;
}

KHelpStringStatic* KGameStaticMgr::GetHelpString(int id)
{
	KHelpStringMap::iterator it  = m_helpStringMap.find(id);
	if(it==m_helpStringMap.end()) return NULL;
	return it->second;
}

void KGameStaticMgr::GetAbilityList(int id,KCardAbilityList& lst,KAbilityStatic::Enum_When when)
{
	for(int i=0;i<5;i++)
	{
		int abilityId = id*10+i;
		KAbilityMap::iterator it  = m_abilityMap.find(abilityId);
		if(it==m_abilityMap.end()) return;
		if(when==KAbilityStatic::when_all ||it->second->GetWhen()==when) lst.push_back(it->second);
	}
}

KAttrStatic* KGameStaticMgr::GetAttr(int id)
{
	return m_attrArr.at(id);
}

void KGameStaticMgr::RndGetNormalCard(int rank,int count,KIntegerList& lst)
{
	KIntegerList tmpLst;
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if(pCard->GetType()==KCardStatic::card_hero) continue;
		if(pCard->GetRank()>rank) continue;
		tmpLst.push_back(pCard->GetID());
	}
	for(int i=0;i<count;i++){
		int nRand = g_rnd.GetRandom(0,tmpLst.size());
		int pos=0;
		for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,pos++){
			if(pos==nRand){
				lst.push_back(*it);
				break;
			}
		}
	}
}

KDeckDefStatic* KGameStaticMgr::GetDeckDef(int id)
{
	KDeckDefMap::iterator it  = m_deckMap.find(id);
	if(it==m_deckMap.end()) return NULL;
	return it->second;
}

KBattleFieldStatic* KGameStaticMgr::GetBattleField(int id)
{
	KBattleFieldStaticMap::iterator it  = m_battleFieldMap.find(id);
	if(it==m_battleFieldMap.end()) return NULL;
	return it->second;
}

KCardLayoutStatic* KGameStaticMgr::GetCardLayout(int idx)
{
	KCardLayoutMap::iterator it = m_cardLayoutMap.find(idx);
	if(it==m_cardLayoutMap.end()) return NULL;
	return it->second;
}


bool KGameStaticMgr::InitHelpString(const char* m_FileName)
{
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(m_FileName);

	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if(!fileReader)	return false;

	while(true)
	{
		int nRet = fileReader->ReadLine();
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }
		if(nRet == 0) break;

		KHelpStringStatic* pStatic = KHelpStringStatic::create();
		pStatic->Init(fileReader);
		m_helpStringMap[pStatic->m_Id] = pStatic;
	}

	loader.CloseFileReader(fileReader);
	return true;
}

void KGameStaticMgr::FilterCards(KIntegerList& lst,KCardStatic::CardDef def,KCardStatic::CardRace race,int rank,int num)
{
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if(pCard->GetType()!=def) continue;
		if(pCard->GetRace()!=race) continue;
		if(pCard->GetRank()!=rank) continue;

		for(int i=0;i<num;i++){
			lst.push_back(pCard->GetID());
		}
	}

	/*
	for(int i=0;i<count;i++){
		int nRand = g_rnd.GetRandom(0,tmpLst.size());
		int pos=0;
		for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,pos++){
			if(pos==nRand){
				lst.push_back(*it);
				break;
			}
		}
	}
	*/
}