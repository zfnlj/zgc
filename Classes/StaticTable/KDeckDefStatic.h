#ifndef __KDECKDEFSTATIC_H__
#define __KDECKDEFSTATIC_H__

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>
#include "KCardStatic.h"
#include "KHeroSkillStatic.h"



class KDeckDefStatic
{
public:
	struct DynamicCardDef{
		KCardStatic::CardDef _def;
		KCardStatic::CardRace _race;
		int _rank;
		int _num;
		int _rate;
		static DynamicCardDef* create(){
			DynamicCardDef* pDef = new DynamicCardDef;
			pDef->init();
			return pDef;
		}
		DynamicCardDef(){
			
		}
		void init(){
			memset(this,0,sizeof(DynamicCardDef));
		}
		bool init(char* sz);
		int size(){ return _num*_rate;}
	};
	typedef std::list<DynamicCardDef*> DynamicCardDefList;
    virtual bool init();
	static KDeckDefStatic* create();
	void setDeck(const char* buf);
	int getHero(){ return m_heroID;}
	int getRes(){ return m_res;}
	int getHeroHp(){ return m_heroHp;}
	int getDrawNum(){return m_drawNum;}
	int getHeroStrong(){ return m_heroStrong;}
	int getHeroLucky(){ return m_heroLucky;}
	int GetAtkVal();
	void Init(System::File::KTabFile2* fileReader);
	void GenCardList(KIntegerList& lst,bool bRnd=false);
	bool IsRnd(){ return m_rnd>0;}
	const HeroSkill* GetHeroSkill(){ return m_skill;}
	static int RndPickSummonSoldier(int maxCost);
private:
	int m_Id;
	int m_heroID;
	int m_heroHp;
	int m_heroStrong;
	int m_heroLucky;
	int m_res;
	int m_drawNum;
	int m_rnd;
	HeroSkill m_skill[MAX_HERO_SKILL_NUM];
	void setDynamic(const char* buf);
	void setHeroSkill(const char* buf);
	void FillOnDynamicCardDef(DynamicCardDef*,KIntegerList& lst);
	KIntegerList m_cardList;
	DynamicCardDefList m_defList;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KDeckDefStatic*> KDeckDefMap;

#endif // __HELLOWORLD_SCENE_H__
