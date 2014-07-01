#ifndef __KCARDSTATIC_H__
#define __KCARDSTATIC_H__

#include <string>
#include <System/File/KTabfileLoader.h>

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
	bool IsShow() { return (strlen(m_Show)>2);}
	static KCardStatic* create();
	void Init(System::File::KTabFile2* fileReader);

	static KCardStatic::CardRace getCardRace(const char* str);
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
	CardDef m_type;
	friend class KGameStaticMgr;
};

#endif // __HELLOWORLD_SCENE_H__
