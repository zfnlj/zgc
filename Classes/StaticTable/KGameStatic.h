#ifndef __KGAMESTATIC_H__
#define __KGAMESTATIC_H__

#include "System/Singleton.h"

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include "KAbilityStatic.h"
#include "KDeckDefStatic.h"
#include "../common/KCommonObj.h"
#include "KBattleFieldStatic.h"
#include "KCardLayoutStatic.h"
#include "KHeroSkillStatic.h"
#include "KHelpStringStatic.h"
#include "KCardStatic.h"
#include "KRankStaticMgr.h"
#include "KLevUpStatic.h"
#include "KTowerAwardStatic.h"
class KGameStaticMgr : public Singleton<KGameStaticMgr>
{
public:
	KGameStaticMgr( ){}

	virtual ~KGameStaticMgr(){};

	static KGameStaticMgr& getSingleton(void);

	void LoadStaticData();
	bool InitCard(const char* m_FileName);
	bool InitAbility(const char* m_FileName);
	bool InitAttr(const char* m_FileName);
	bool InitDeckDef(const char* m_FileName);
	bool InitBattleField(const char* m_FileName);
	bool InitHelpString(const char* m_FileName);
	bool InitRank(const char* m_FileName,KRankStaticDataManager& mgr);
	bool InitHeroSkill(const char* m_FileName);
	bool InitTipString(const char* m_FileName);
	bool InitStoryString(const char* m_FileName);
	bool InitTowerAward(const char* m_FileName);
	bool InitLevUp(const char* m_FileName,KLevUpStaticMap& mgr);

	int  PlayerExpToLevel(int exp);
	float GetLevRate(int exp);
	KCardStatic* GetCard(int id);
	int GetCardNum(KCardStatic::CardRace);
	int GetNormalCardNum();
	int GetAllCard(){ return m_cardMap.size();}
	void GetAbilityList(int id,KCardAbilityList&,KAbilityStatic::Enum_When when=KAbilityStatic::when_all);
	KAbilityStatic* GetAbilityOnId(int id);
	KAttrStatic* GetAttr(int id);
	KDeckDefStatic* GetDeckDef(int id);
	KBattleFieldStatic* GetBattleField(int id);
	KCardLayoutStatic* GetCardLayout(int idx);
	int GetSkillLevUpExp(int lev);
	int GetHeroLevUpExp(int lev);
	int GetTowerLevMoney(int lev);
	int GetTowerLevFateStone(int lev);
	KHeroSkillStatic* GetHeroSkill(int id);
	void RndGetNormalCard(int rank,int count,KIntegerList& lst);
	KHelpStringStatic* GetHelpString(int id);
	KHelpStringStatic* GetRndTip();
	KHelpStringStatic* GetRndStory();

	void RndGetHeroCard(int count,KIntegerList& lst);
	void RndGetNormalCard(int count,int rank1Rate,int rank2Rate,int rank3Rate,
									  int heroRate,KIntegerList& tmpLst);
	KHeroSkillStatic* GetRndHeroSkill(int rank,int race);
	void FilterCards(KIntegerList& lst,KCardStatic::CardDef def,KCardStatic::CardRace race,int rank,int num=1);
	void FilterCardsOnCost(KIntegerList& lst,KCardStatic::CardDef def,KCardStatic::CardRace race,int maxCost,int num);
private:
#ifdef _USE_COCOS2DX
	bool InitCardLayout(const char* m_FileName);
#endif 
	typedef System::Collections::KMapByVector<int,KCardStatic*>  CardMap;
	System::Collections::KMapByVector<int,KAbilityStatic*> m_abilityMap;
	DynAttrArr  m_attrArr;
	CardMap	m_cardMap;
	KDeckDefMap m_deckMap;
	KCardLayoutMap m_cardLayoutMap;
	KBattleFieldStaticMap m_battleFieldMap;
	KHeroSkillMap m_heroSkillMap;
	KHelpStringMap m_helpStringMap;
	KHelpStringMap m_tipStringMap;
	KHelpStringMap m_storyStringMap;
	KRankStaticDataManager m_playerExpMgr;
	KRankStaticDataManager m_skillExpMgr;

	KLevUpStaticMap m_skillLevUpMgr;
	KLevUpStaticMap m_heroLevUpMgr;
	KTowerAwardStaticMap m_towerAwardMap;
};

#endif // __HELLOWORLD_SCENE_H__
