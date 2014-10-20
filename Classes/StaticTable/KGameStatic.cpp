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


#define LOAD_TAB_TO_MAPOBJ(__classname,fileName,myMap) \
	std::string fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName); \
	KTabfileLoader& loader = KTabfileLoader::GetInstance();	\
	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str()); \
	if(!fileReader)	return false;	\
	while(true)	\
	{\
		int nRet = fileReader->ReadLine();\
		if(nRet == -1) { loader.CloseFileReader(fileReader); return false; }\
		if(nRet == 0) break;\
		__classname* pStatic = __classname::create();\
		pStatic->Init(fileReader);\
		myMap[pStatic->m_Id] = pStatic;\
	}\
	loader.CloseFileReader(fileReader);\
	return true;\

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
    //InitCard("data/card/Secret.txt");
	InitAbility("data/card/Ability.txt");
	InitAttr("data/card/Attr.txt");
	InitBattleField("data/BattleField.txt");
	InitRank("data/Rank.txt",m_playerExpMgr);
	InitCardLayout("data/card_layout.txt");
	InitHeroSkill("data/card/hero_skill.txt");
	InitHelpString("StringManager/HelpStr.txt");
	InitTipString("StringManager/TipStr.txt");
	InitStoryString("StringManager/StoryStr.txt");
	InitDeckDef("data/DeckDef.txt");
	InitLevUp("data/SkillLevUp.txt",m_skillLevUpMgr);
	InitLevUp("data/HeroLevUp.txt",m_heroLevUpMgr);
	InitLevUp("data/FateLevUp.txt",m_heroFateLevUpMgr);
	InitTowerAward("data/TowerAward.txt");

	for(int i=1;i<4;i++){
		GetRndHeroSkill(i,KCardStatic::race_gold);
		GetRndHeroSkill(i,KCardStatic::race_water);
		GetRndHeroSkill(i,KCardStatic::race_fire);
	}
}

bool KGameStaticMgr::InitRank(const char* m_FileName,KRankStaticDataManager& mgr)
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
	mgr.Init(fileReader);
	loader.CloseFileReader(fileReader);
	return true;

}

bool KGameStaticMgr::InitHeroSkill(const char* m_FileName)
{
	LOAD_TAB_TO_MAPOBJ(KHeroSkillStatic,m_FileName,m_heroSkillMap)
}

bool KGameStaticMgr::InitCardLayout(const char* m_FileName)
{

	LOAD_TAB_TO_MAPOBJ(KCardLayoutStatic,m_FileName,m_cardLayoutMap);

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

void KGameStaticMgr::RndGetHeroCard(int count,KIntegerList& lst)
{
	KIntegerList tmpLst;
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if(pCard->GetType()!=KCardStatic::card_hero) continue;
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

void KGameStaticMgr::RndGetNormalCard(int rank,int count,KIntegerList& lst)
{
	if(count==0) return;
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
	LOAD_TAB_TO_MAPOBJ(KHelpStringStatic,m_FileName,m_helpStringMap);
}

void KGameStaticMgr::FilterCardsOnCost(KIntegerList& lst,KCardStatic::CardDef def,KCardStatic::CardRace race,int maxCost,int num)
{
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if(pCard->GetType()!=def) continue;
		if(pCard->GetRace()!=race) continue;
		if(pCard->GetCost()>maxCost) continue;

		for(int i=0;i<num;i++){
			lst.push_back(pCard->GetID());
		}
	}
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

bool KGameStaticMgr::InitTipString(const char* m_FileName)
{
	LOAD_TAB_TO_MAPOBJ(KHelpStringStatic,m_FileName,m_tipStringMap);
}

bool KGameStaticMgr::InitStoryString(const char* m_FileName)
{
	LOAD_TAB_TO_MAPOBJ(KHelpStringStatic,m_FileName,m_storyStringMap);
}

KHelpStringStatic* KGameStaticMgr::GetRndTip()
{
	int nRand = g_rnd.GetRandom(1,m_tipStringMap.size()+1);
	return m_tipStringMap[nRand];
}

KHelpStringStatic* KGameStaticMgr::GetRndStory()
{
	int nRand = g_rnd.GetRandom(1,m_storyStringMap.size()+1);
	return m_storyStringMap[nRand];
}

KHeroSkillStatic* KGameStaticMgr::GetRndHeroSkill(int rank,int race)
{
	KHeroSkillMap tmpMap;
	int pos=0;
	for(KHeroSkillMap::iterator it=m_heroSkillMap.begin();it!=m_heroSkillMap.end();++it){
		KHeroSkillStatic* skill = it->second;
		if(skill->GetRank()==rank && 
			skill->IsRaceMatch(race)){
				tmpMap[pos++] = skill;
		}
	}
	if(tmpMap.empty()){
		return NULL;
	}
	int nRand = g_rnd.GetRandom(0,tmpMap.size());
	return tmpMap[nRand];
}

float KGameStaticMgr::GetLevRate(int exp)
{
	return m_playerExpMgr.GetLevRate(exp);
}

int KGameStaticMgr::PlayerExpToLevel(int exp)
{
	return m_playerExpMgr.ExpToLevel(exp);
}


void KGameStaticMgr::RndGetNormalCard(int count,int rank1Rate,int rank2Rate,int rank3Rate,int rank4Rate,
									  int heroRate,KIntegerList& outLst)
{
	KIntegerList tmpLst;

	RndGetNormalCard(0,rank1Rate*0.5,tmpLst);
	RndGetNormalCard(1,rank1Rate*0.5,tmpLst);
	RndGetNormalCard(2,rank2Rate,tmpLst);
	RndGetNormalCard(3,rank3Rate,tmpLst);
	RndGetNormalCard(4,rank4Rate,tmpLst);
	RndGetHeroCard(heroRate,tmpLst);

	_RndPick(tmpLst,outLst,count);
}

bool KGameStaticMgr::InitLevUp(const char* m_FileName,KLevUpStaticMap& mgr)
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
		KLevUpStatic* pLevUp = KLevUpStatic::create();
		pLevUp->Init(fileReader);
		mgr[pLevUp->GetLev()] = pLevUp;
	}

	loader.CloseFileReader(fileReader);
	return true;

}

int KGameStaticMgr::GetTowerLevFateStone(int lev)
{
	KTowerAwardStaticMap::iterator it = m_towerAwardMap.find(lev);
	if(it==m_towerAwardMap.end()) return 0;
	return it->second->GetFateStone();
}

int KGameStaticMgr::GetTowerLevMoney(int lev)
{
	KTowerAwardStaticMap::iterator it = m_towerAwardMap.find(lev);
	if(it==m_towerAwardMap.end()) return 0;
	return it->second->GetMoney();
}

int KGameStaticMgr::GetSkillLevUpExp(int lev)
{
	KLevUpStaticMap::iterator it = m_skillLevUpMgr.find(lev);
	if(it==m_skillLevUpMgr.end()) return 0;
	return it->second->GetExp();
}

int KGameStaticMgr::GetHeroFateLevUpExp(int lev)
{
	KLevUpStaticMap::iterator it = m_heroFateLevUpMgr.find(lev);
	if(it==m_heroFateLevUpMgr.end()) return 0;
	return it->second->GetExp();
}

int KGameStaticMgr::GetHeroLevUpExp(int lev)
{
	KLevUpStaticMap::iterator it = m_heroLevUpMgr.find(lev);
	if(it==m_heroLevUpMgr.end()) return 0;
	return it->second->GetExp();
}

int KGameStaticMgr::GetCardNum(KCardStatic::CardRace race)
{
	int count =0;
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if( race==KCardStatic::race_all || pCard->GetRace()==race){
			if(pCard->GetType()!=KCardStatic::card_hero) count++;
		}
	}
	return count;
}

int KGameStaticMgr::GetNormalCardNum()
{
	int count =0;
	for(CardMap::iterator it = m_cardMap.begin();it!=m_cardMap.end();++it){
		KCardStatic* pCard = it->second;
		if(pCard->GetType()!=KCardStatic::card_hero) count++;
	}
	return count;
}

bool KGameStaticMgr::InitTowerAward(const char* m_FileName)
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
		KTowerAwardStatic* pAward = KTowerAwardStatic::create();
		pAward->Init(fileReader);
		m_towerAwardMap[pAward->GetLev()] = pAward;
	}

	loader.CloseFileReader(fileReader);
	return true;

}