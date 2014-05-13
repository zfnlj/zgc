#ifndef __KABILITYSTATIC_H__
#define __KABILITYSTATIC_H__

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include <System/File/KTabfileLoader.h>
#include "KConditionStruct.h"

#define BUF_GUIDE_ID 10001
#define BUF_HIDE_ID 10002
#define BUF_DIST_ID 10003
#define BUF_CAN_RUSH_ID 10004
#define BUF_HURTED_ID 10005
class KAbilityStatic
{
public:
	enum Enum_AblityType{
		ability_good,
		ability_bad,
		ability_neutral,
	};
	enum Enum_When{
		when_enter,
		when_use,
		when_turn_begin,
		when_turn_end,
		when_damaged,
		when_dead,
		when_soldier_dead,
		when_mysoldier_dead,
		when_soldier_hurted,
		when_mysoldier_hurted,
		when_soldier_healed,
		when_mysoldier_healed,
		when_hero_hurted,
		when_myhero_hurted,
		when_atked,
		when_use_skill,
		when_i_use_skill,
		when_use_card,
		when_i_use_card,
		when_do_damage,
		when_healed,
		when_hero_healed,
		when_ever,
		when_all,
	};
	enum Enum_Which{
		which_mysoldier,
		which_myhero,
		which_my,
		which_yoursoldier,
		which_yourhero,
		which_your,
		which_soldier,
		which_src_nearby,
		which_des_nearby,
		which_hero,
		which_i,
		which_u,
		which_owner,
		which_all,
		which_null,
	};
	enum Enum_What{
		what_damage,
		what_heal,
		what_damage_atkadd,
		what_atk_add,
		what_add_atk_hp,
		what_pray,
		what_stun,
		what_hp_add,
		what_hp_double,
		what_hp_set,
		what_atkhp_sw,
		what_atk_set,
		what_immune,
		what_res_add,
		what_draw_card,
		what_summon,
		what_summon_guider,
		what_replace,
		what_return,
		what_kill,
		what_cast_add,
		what_sp_rate,
		what_atk_equ_hp,
		what_copy_hand,
		what_copy_fight,
		what_dispel_buf,
		what_control,
		what_get_card,
		what_mp_cost,
		what_buf,
		what_hide,
		what_dist,
		what_guide,
		what_rush,
		what_can_rush,
		what_hurted,
		what_bless_hp,
		what_angry,
		what_noready,
		what_null,
		
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
	//int GetVal(){ return m_val;}
	int GetVal2(){ return m_val2;}
	KValDef& GetMax(){ return m_max;}
	int GetMaxNormalVal(){ return m_max._val;}
	int GetId(){ return m_AbilityId;}
	bool IsLoop(){ return m_loop==0;}
	int LoopNum(){ return m_loop;}
	Enum_AblityType GetAbilityType();
	const char* GetAction(){ return m_Action;}
	const char* GetBufIcon(){ return m_bufIcon;}
	bool ActionIsEmpty(){ return strlen(m_Action)<2;}
	bool BufIconEmpty(){ return strlen(m_bufIcon)<2;}
	bool ToSelfEnable(){ return m_mySelf>0;}
	void Init(System::File::KTabFile2* fileReader);
	static Enum_What Str2What(const char* str);
	KConditionDef& GetCond(){ return m_cond;}
	KValDef& GetVal(){ return m_val;}
	int GetNormalVal(){ return m_val._val;}
	bool IsTargetSure();
	void Clone(KAbilityStatic&);
private:
	int m_AbilityId;
	Enum_When m_when;
	Enum_Which m_which;
	Enum_What m_what;
	int m_val2;
	int m_loop;
	KValDef m_max;
	int m_area;
	int m_mySelf;//是否对自己起效
	char m_Action[64];
	char m_bufIcon[32];
	KConditionDef m_cond;
	KValDef m_val;
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
