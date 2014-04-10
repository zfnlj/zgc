#ifndef __KABILITYSTATIC_H__
#define __KABILITYSTATIC_H__

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include <System/File/KTabfileLoader.h>

class KAbilityStatic
{
public:
	enum Enum_When{
		when_enter,
		when_use,
		when_turn_begin,
		when_turn_end,
		when_damaged,
		when_dead,
		when_soldier_dead,
		when_atked,
		when_use_skill,
		when_null,
	};
	enum Enum_Which{
		which_mysoldier,
		which_myhero,
		which_my,
		which_yoursoldier,
		which_yourhero,
		which_your,
		which_soldier,
		which_hero,
		which_i,
		which_u,
		which_owner,
		which_all,
	};
	enum Enum_What{
		what_damage,
		what_heal,
		what_atk_add,
		what_stun,
		what_hp_add,
		what_hp_double,
		what_hp_set,
		what_immune,
		what_res_add,
		what_draw_card,
		what_summon,
		what_replace,
		what_return,
		what_kill,
		what_guide,
		what_remove_buf,
		what_sp_rate,
		what_atk_equ_hp,
		what_copy_hand,
		what_copy_fight,
		what_kill_atk_le,
		what_kill_atk_he,
		what_clear_buf,
		what_control,
		what_get_card,
		what_mp_cost,
		
	};
    virtual bool init();
	static KAbilityStatic* create();
	void SetWhen(const char* str);
	void SetWhich(const char* str);
	void SetWhat(const char* str);
	Enum_What GetWhat(){ return m_what;}
	Enum_When GetWhen(){ return m_when;}
	Enum_Which GetWhich(){ return m_which;}
	bool IsArea() { return m_area>0;}
	int GetVal(){ return m_val;}
	int GetMax(){ return m_max;}
	int GetId(){ return m_AbilityId;}
	int IsLoop(){ return m_loop==0;}
	bool IsGood();//有益
	bool IsRnd(){ return m_rnd>0;}
	int GetRnd(){ return m_rnd;}
	const char* GetAction(){ return m_Action;}
	const char* GetBufIcon(){ return m_bufIcon;}
	bool ActionIsEmpty(){ return strlen(m_Action)<2;}
	bool BufIconEmpty(){ return strlen(m_bufIcon)<2;}
	bool ToSelfEnable(){ return m_mySelf>0;}
	void Init(System::File::KTabFile2* fileReader);
private:
	int m_AbilityId;
	Enum_When m_when;
	Enum_Which m_which;
	Enum_What m_what;
	int m_val;
	int m_loop;
	int m_max;
	int m_area;
	int m_rnd;
	int m_mySelf;//是否对自己起效
	char m_Action[64];
	char m_bufIcon[32];
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KAbilityStatic*> KAbilityMap;

class KAttrStatic
{
public:
	virtual bool init();
	static KAttrStatic* create();
	bool IsGuide(){ return m_guide>0;}
	bool IsDist(){ return m_dist>0;}
	bool IsRush(){ return m_rush>0;}
	bool IsHide(){ return m_hide>0;}

private:
	int m_CardId;
	int m_guide;
	int m_rush;
	int m_hide;
	int m_dist;
	friend class KGameStaticMgr;
};
typedef std::list<KAbilityStatic*> KCardAbilityList;
typedef System::Collections::DynArray<KAttrStatic*> DynAttrArr;

#endif // __HELLOWORLD_SCENE_H__
