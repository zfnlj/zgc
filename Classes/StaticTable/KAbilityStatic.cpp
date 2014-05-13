#include "KAbilityStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include "KGameStatic.h"

bool KAbilityStatic::init()
{
	m_AbilityId = m_loop = m_val2 = m_area = 0;
	memset(m_Action,0,sizeof(m_Action));
	memset(m_bufIcon,0,sizeof(m_bufIcon));
	return true;
}

void KAbilityStatic::Clone(KAbilityStatic& ab)
{
	ab.m_AbilityId = m_AbilityId;
	ab.m_when = m_when;
	ab.m_which = m_which;
	ab.m_what = m_what;
	ab.m_mySelf = m_mySelf;
	ab.m_loop = m_loop;
	ab.m_val2 = m_val2;
	ab.m_area = m_area;
	strcpy(ab.m_Action,m_Action);
	strcpy(ab.m_bufIcon,m_bufIcon);
	memcpy(&ab.m_max,&m_max,sizeof(m_max));
	memcpy(&ab.m_cond,&m_cond,sizeof(m_cond));
	memcpy(&ab.m_val,&m_val,sizeof(m_val));
}

void KAbilityStatic::SetWhat(int what)
{
	m_what = (Enum_What)what;
}

KAbilityStatic* KAbilityStatic::create()
{
	KAbilityStatic* pAbility = new KAbilityStatic;
	pAbility->init();
	return pAbility;
}


KAbilityStatic::Enum_AblityType KAbilityStatic::GetAbilityType()
{
	Enum_AblityType tp = ability_neutral;
	switch(m_what){
    case what_res_add:
    case what_atk_add:
	case what_angry:
	case what_hp_double:
    case what_hp_add:
            tp = (m_val._val>0)?ability_good:ability_bad;
            break;
	case what_stun:
	case what_damage:
		tp = ability_bad;
		break;
	case what_heal:
	case what_immune:
	case what_draw_card:
	case what_rush:
	case what_hide:
	case what_dist:
	case what_guide:
	case what_bless_hp:
		tp = ability_good;
		break;
    default:
        break;
	}
	return tp;
}

bool KAbilityStatic::IsTargetSure()
{
	return (m_which==which_i || m_which==which_myhero || m_which==which_yourhero);
}

void KAbilityStatic::SetWhich(const char* str)
{
	if(strcmp(str,"WHICH_MYSOLDIER")==0){
		m_which = which_mysoldier;
	}else if(strcmp(str,"WHICH_MYHERO")==0){
		m_which = which_myhero;
	}else if(strcmp(str,"WHICH_MY")==0){
		m_which = which_my;
	}else if(strcmp(str,"WHICH_YOURSOLDIER")==0){
		m_which = which_yoursoldier;
	}else if(strcmp(str,"WHICH_YOURHERO")==0){
		m_which = which_yourhero;
	}else if(strcmp(str,"WHICH_YOUR")==0){
		m_which = which_your;
	}else if(strcmp(str,"WHICH_SOLDIER")==0){
		m_which = which_soldier;
	}else if(strcmp(str,"WHICH_SRC_NEARBY")==0){
		m_which = which_src_nearby;
	}else if(strcmp(str,"WHICH_DES_NEARBY")==0){
		m_which = which_des_nearby;
	}else if(strcmp(str,"WHICH_HERO")==0){
		m_which = which_hero;
	}else if(strcmp(str,"WHICH_ALL")==0){
		m_which = which_all;
	}else if(strcmp(str,"WHICH_I")==0){
		m_which = which_i;
	}else if(strcmp(str,"WHICH_U")==0){
		m_which = which_u;
	}else if(strcmp(str,"WHICH_OWNER")==0){
		m_which = which_owner;
	}else if(strcmp(str,"WHICH_NULL")==0){
		m_which = which_null;
	}else{
		//CCLog("Set Which isn't match!");
		CCAssert(false , "Set Which isn't match!");
	}
}

KAbilityStatic::Enum_When KAbilityStatic::Str2When(const char* str)
{
	Enum_When when = when_null;
	if(strcmp(str,"WHEN_ENTER")==0){
		when = when_enter;
	}else if(strcmp(str,"WHEN_USE")==0){
		when = when_use;
	}else if(strcmp(str,"WHEN_TURN_BEGIN")==0){
		when = when_turn_begin;
	}else if(strcmp(str,"WHEN_TURN_END")==0){
		when = when_turn_end;
	}else if(strcmp(str,"WHEN_DEAD")==0){
		when = when_dead;
	}else if(strcmp(str,"WHEN_SOLDIER_DEAD")==0){
		when = when_soldier_dead;
	}else if(strcmp(str,"WHEN_ATKED")==0){
		when = when_atked;
	}else if(strcmp(str,"WHEN_DO_DAMAGE")==0){
		when = when_do_damage;
	}else if(strcmp(str,"WHEN_DAMAGED")==0){
		when = when_damaged;
	}else if(strcmp(str,"WHEN_USE_SKILL")==0){
		when = when_use_skill;
	}else if(strcmp(str,"WHEN_I_USE_SKILL")==0){
		when = when_i_use_skill;
	}else if(strcmp(str,"WHEN_I_USE_CARD")==0){
		when = when_i_use_card;
	}else if(strcmp(str,"WHEN_EVER")==0){
		when = when_ever;
	}else if(strcmp(str,"WHEN_ALL")==0){
		when = when_all;
	}else if(strcmp(str,"WHEN_HEALED")==0){
		when = when_healed;
	}else if(strcmp(str,"WHEN_SOLDIER_DEAD")==0){
		when = when_soldier_dead;
	}else if(strcmp(str,"WHEN_MYSOLDIER_DEAD")==0){
		when = when_mysoldier_dead;
	}else if(strcmp(str,"WHEN_SOLDIER_HURTED")==0){
		when = when_soldier_hurted;
	}else if(strcmp(str,"WHEN_MYSOLDIER_HURTED")==0){
		when = when_mysoldier_hurted;
	}else if(strcmp(str,"WHEN_SOLDIER_HEALED")==0){
		when = when_soldier_healed;
	}else if(strcmp(str,"WHEN_MYSOLDIER_HEALED")==0){
		when = when_mysoldier_healed;
	}else if(strcmp(str,"WHEN_HERO_HURTED")==0){
		when = when_hero_hurted;
	}else if(strcmp(str,"WHEN_MYHERO_HURTED")==0){
		when = when_myhero_hurted;
	}else{
		CCAssert(false , "Set When isn't match!");
		//CCLog("Set When isn't match!");
	}
	return when;
}

KAbilityStatic::Enum_What KAbilityStatic::Str2What(const char* str)
{
	Enum_What what = what_null;
	if(strcmp(str,"WHAT_DAMAGE")==0){
		what = what_damage;
	}else if(strcmp(str,"WHAT_HEAL")==0){
		what = what_heal;
	}else if(strcmp(str,"WHAT_ATK_ADD")==0){
		what = what_atk_add;
	}else if(strcmp(str,"WHAT_ANGRY")==0){
		what = what_angry;
	}else if(strcmp(str,"WHAT_NOREADY")==0){
		what = what_noready;
	}else if(strcmp(str,"WHAT_STUN")==0){
		what = what_stun;
	}else if(strcmp(str,"WHAT_HP_ADD")==0){
		what = what_hp_add;
	}else if(strcmp(str,"WHAT_HP_DOUBLE")==0){
		what = what_hp_double;
	}else if(strcmp(str,"WHAT_HP_SET")==0){
		what = what_hp_set;
	}else if(strcmp(str,"WHAT_ATKHP_SW")==0){
		what = what_atkhp_sw;
	}else if(strcmp(str,"WHAT_ATK_SET")==0){
		what = what_atk_set;
	}else if(strcmp(str,"WHAT_IMMUNE")==0){
		what = what_immune;
	}else if(strcmp(str,"WHAT_RES_ADD")==0){
		what = what_res_add;
	}else if(strcmp(str,"WHAT_DRAW_CARD")==0){
		what = what_draw_card;
	}else if(strcmp(str,"WHAT_SUMMON")==0){
		what = what_summon;
	}else if(strcmp(str,"WHAT_SUMMON_GUIDER")==0){
		what = what_summon_guider;
	}else if(strcmp(str,"WHAT_REPLACE")==0){
		what = what_replace;
	}else if(strcmp(str,"WHAT_RETURN")==0){
		what = what_return;
	}else if(strcmp(str,"WHAT_KILL")==0){
		what = what_kill;
	}else if(strcmp(str,"WHAT_GUIDE")==0){
		what = what_guide;
	}else if(strcmp(str,"WHAT_SP_RATE")==0){
		what = what_sp_rate;
	}else if(strcmp(str,"WHAT_CAST_ADD")==0){
		what = what_cast_add;
	}else if(strcmp(str,"WHAT_MP_COST")==0){
		what = what_mp_cost;
	}else if(strcmp(str,"WHAT_ATK_EQU_HP")==0){
		what = what_atk_equ_hp;
	}else if(strcmp(str,"WHAT_COPY_HAND")==0){
		what = what_copy_hand;
	}else if(strcmp(str,"WHAT_COPY_FIGHT")==0){
		what = what_copy_fight;
	}else if(strcmp(str,"WHAT_CLEAR_BUF")==0){
		what = what_dispel_buf;
	}else if(strcmp(str,"WHAT_CONTROL")==0){
		what = what_control;
	}else if(strcmp(str,"WHAT_GET_CARD")==0){
		what = what_get_card;
	}else if(strcmp(str,"WHAT_BUF")==0){
		what = what_buf;
	}else if(strcmp(str,"WHAT_HIDE")==0){
		what = what_hide;
	}else if(strcmp(str,"WHAT_DIST")==0){
		what = what_dist;
	}else if(strcmp(str,"WHAT_RUSH")==0){
		what = what_rush;
	}else if(strcmp(str,"WHAT_CAN_RUSH")==0){
		what = what_can_rush;
	}else if(strcmp(str,"WHAT_DAMAGE_ATKADD")==0){
		what = what_damage_atkadd;
	}else if(strcmp(str,"WHAT_HURTED")==0){
		what = what_hurted;
	}else if(strcmp(str,"WHAT_BLESS_HP")==0){
		what = what_bless_hp;
	}else if(strcmp(str,"WHAT_ADD_ATK_HP")==0){
		what = what_add_atk_hp;
	}else if(strcmp(str,"WHAT_RND")==0){
		what = what_rnd;
	}else{
		CCAssert(false , "Set What isn't match!");
	}
	return what;
}

void KAbilityStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("ID", 0, (int*)&m_AbilityId);
	char buf[64] = {0};
	fileReader->GetString("WHEN", "", buf, 63);
	m_when = Str2When(buf);

	fileReader->GetString("WHICH", "", buf, 63);
	SetWhich(buf);

	fileReader->GetString("Condition", "", buf, 63);
	m_cond.ParseString(buf);

	fileReader->GetString("WHAT", "", buf, 63);
	m_what = Str2What(buf);

	fileReader->GetString("VAL", "", buf, 63);
	m_val.ParseString(buf);
	fileReader->GetInteger("VAL2", 0, (int*)&m_val2);
	fileReader->GetInteger("LOOP", 0, (int*)&m_loop);
	fileReader->GetInteger("AREA", 0, (int*)&m_area);
	fileReader->GetString("MAX", "", buf, 63);
	m_max.ParseString(buf);

	fileReader->GetInteger("MYSELF", 1, (int*)&m_mySelf);
	fileReader->GetString("Action", "", m_Action, 63);
	fileReader->GetString("BUF_ICON", "", m_bufIcon, 32);
	if(strlen(m_bufIcon)<2) memset(m_bufIcon,0,sizeof(m_bufIcon));
}


bool KAttrStatic::init()
{
	m_CardId = m_guide= m_rush = m_hide = m_dist = 0;
	return true;
}

KAttrStatic* KAttrStatic::create()
{
	KAttrStatic* attr = new KAttrStatic;
	attr->init();
	return attr;
}

