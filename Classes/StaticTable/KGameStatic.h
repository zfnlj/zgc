#ifndef __KCARDSTATIC_H__
#define __KCARDSTATIC_H__

#include "System/Singleton.h"

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include "KAbilityStatic.h"
#include "KDeckDefStatic.h"
#include "../common/KCommonObj.h"
#include "KBattleFieldStatic.h"
#include "KCardLayoutStatic.h"
#include "KHeroSkillStatic.h"

#define MAX_CARD_NAME 24
#define MAX_CARD_DESC_LEN 128
#define MAX_CARD_DETAIL_LEN 256

class KCardStatic
{
public:
	enum CardDef{
		card_null = 0, 
		card_hero,   //主将
		card_soldier,//士兵
		card_skill,  //技能
		card_weapon, //武器
		card_equip,  //装备
        card_secret,
	};
	enum CardRace{
		race_null,
		race_gold,
		race_tree,
		race_water,
		race_fire,
		race_mud,
	};

	KCardStatic(){}
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void SetID(int val);
	int GetID(){ return m_Id;}
	int GetAtk(){ return m_Atk;}
	int GetHp() {return m_Hp;}
	int GetCost(){return m_Cost;}
	int GetDef(){ return m_Def;}
	int GetRank(){ return m_Rank;}
	int GetRace(){ return m_Race;}
	CardDef GetType(){ return m_type;}
	char* GetName(){ return m_Name;}
	char* GetDesc(){ return m_Desc;}
	char* GetDetail(){ return m_Detail;}
	const char* GetShow(){ return m_Show;}
	const char* GetPhoto(){ return m_Photo;}
	bool IsShow() { return (strlen(m_Show)>2);}
	static KCardStatic* create();
private:
	int m_Id;
	int m_Atk;
	int m_Hp;
	int m_Cost;
	int m_Def;
	int m_Rank;
	CardRace m_Race;
	char m_Name[MAX_CARD_NAME];
	char m_Desc[MAX_CARD_DESC_LEN];
	char m_Detail[MAX_CARD_DETAIL_LEN];
	char m_Show[32];
	char m_Photo[MAX_CARD_NAME];
	CardDef m_type;
	friend class KGameStaticMgr;
};


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
	bool InitRank(const char* m_FileName);
	bool InitHeroSkill(const char* m_FileName);
	KCardStatic* GetCard(int id);
	void GetAbilityList(int id,KCardAbilityList&,KAbilityStatic::Enum_When when=KAbilityStatic::when_all);
	KAbilityStatic* GetAbilityOnId(int id);
	KAttrStatic* GetAttr(int id);
	KDeckDefStatic* GetDeckDef(int id);
	KBattleFieldStatic* GetBattleField(int id);
	KCardLayoutStatic* GetCardLayout(int idx);
	KHeroSkillStatic* GetHeroSkill(int id);
	void RndGetNormalCard(int rank,int count,KIntegerList& lst);
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
};

#endif // __HELLOWORLD_SCENE_H__
