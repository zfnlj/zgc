#include "KAbilityStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include "KGameStatic.h"

bool KAbilityStatic::init()
{
	m_AbilityId = m_loop = m_val = m_max = m_area = 0;
	memset(m_Action,0,sizeof(m_Action));
	memset(m_bufIcon,0,sizeof(m_bufIcon));
	return true;
}

void KAbilityStatic::SetWhen(const char* str)
{
	if(strcmp(str,"WHEN_ENTER")==0){
		m_when = when_enter;
	}else if(strcmp(str,"WHEN_USE")==0){
		m_when = when_use;
	}else if(strcmp(str,"WHEN_TURN_BEGIN")==0){
		m_when = when_turn_begin;
	}else if(strcmp(str,"WHEN_TURN_END")==0){
		m_when = when_turn_end;
	}else if(strcmp(str,"WHEN_DEAD")==0){
		m_when = when_dead;
	}else if(strcmp(str,"WHEN_SOLDIER_DEAD")==0){
		m_when = when_soldier_dead;
	}else if(strcmp(str,"WHEN_ATKED")==0){
		m_when = when_atked;
	}else if(strcmp(str,"WHEN_DO_DAMAGE")==0){
		m_when = when_do_damage;
	}else if(strcmp(str,"WHEN_DAMAGED")==0){
		m_when = when_damaged;
	}else if(strcmp(str,"WHEN_USE_SKILL")==0){
		m_when = when_use_skill;
	}else{
		CCLog("Set When isn't match!");
	}
}

KAbilityStatic* KAbilityStatic::create()
{
	KAbilityStatic* pAbility = new KAbilityStatic;
	pAbility->init();
	return pAbility;
}

bool KAbilityStatic::IsGood()
{
	bool ret = true;
	switch(m_what){
    case what_res_add:
    case what_atk_add:
	case what_hp_double:
    case what_hp_add:
            ret = (m_val>0);
            break;
	case what_stun:
	case what_damage:
		ret = false;
		break;
	case what_heal:
	case what_immune:
	case what_draw_card:
		ret = true;
		break;
    default:
        ret = false;
        break;
	}
	return ret;
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
		CCLog("Set Which isn't match!");
	}
}

void KAbilityStatic::SetWhat(const char* str)
{
	if(strcmp(str,"WHAT_DAMAGE")==0){
		m_what = what_damage;
	}else if(strcmp(str,"WHAT_HEAL")==0){
		m_what = what_heal;
	}else if(strcmp(str,"WHAT_ATK_ADD")==0){
		m_what = what_atk_add;
	}else if(strcmp(str,"WHAT_STUN")==0){
		m_what = what_stun;
	}else if(strcmp(str,"WHAT_HP_ADD")==0){
		m_what = what_hp_add;
	}else if(strcmp(str,"WHAT_HP_DOUBLE")==0){
		m_what = what_hp_double;
	}else if(strcmp(str,"WHAT_HP_SET")==0){
		m_what = what_hp_set;
	}else if(strcmp(str,"WHAT_IMMUNE")==0){
		m_what = what_immune;
	}else if(strcmp(str,"WHAT_RES_ADD")==0){
		m_what = what_res_add;
	}else if(strcmp(str,"WHAT_DRAW_CARD")==0){
		m_what = what_draw_card;
	}else if(strcmp(str,"WHAT_SUMMON")==0){
		m_what = what_summon;
	}else if(strcmp(str,"WHAT_SUMMON_GUIDER")==0){
		m_what = what_summon_guider;
	}else if(strcmp(str,"WHAT_REPLACE")==0){
		m_what = what_replace;
	}else if(strcmp(str,"WHAT_RETURN")==0){
		m_what = what_return;
	}else if(strcmp(str,"WHAT_KILL")==0){
		m_what = what_kill;
	}else if(strcmp(str,"WHAT_GUIDE")==0){
		m_what = what_guide;
	}else if(strcmp(str,"WHAT_SP_RATE")==0){
		m_what = what_sp_rate;
	}else if(strcmp(str,"WHAT_MP_COST")==0){
		m_what = what_mp_cost;
	}else if(strcmp(str,"WHAT_ATK_EQU_HP")==0){
		m_what = what_atk_equ_hp;
	}else if(strcmp(str,"WHAT_COPY_HAND")==0){
		m_what = what_copy_hand;
	}else if(strcmp(str,"WHAT_COPY_FIGHT")==0){
		m_what = what_copy_fight;
	}else if(strcmp(str,"WHAT_KILL_ATK_LE")==0){
		m_what = what_kill_atk_le;
	}else if(strcmp(str,"WHAT_KILL_ATK_HE")==0){
		m_what = what_kill_atk_he;
	}else if(strcmp(str,"WHAT_CLEAR_BUF")==0){
		m_what = what_clear_buf;
	}else if(strcmp(str,"WHAT_CONTROL")==0){
		m_what = what_control;
	}else if(strcmp(str,"WHAT_GET_CARD")==0){
		m_what = what_get_card;
	}else if(strcmp(str,"WHAT_BUF")==0){
		m_what = what_buf;
	}else if(strcmp(str,"WHAT_HIDE")==0){
		m_what = what_hide;
	}else if(strcmp(str,"WHAT_DIST")==0){
		m_what = what_dist;
	}else{
		CCAssert(false , "Set What isn't match!");
	}
}

void KAbilityStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("ID", 0, (int*)&m_AbilityId);
	char buf[64] = {0};
	fileReader->GetString("WHEN", "", buf, MAX_CARD_NAME);
	SetWhen(buf);
	fileReader->GetString("WHICH", "", buf, MAX_CARD_NAME);
	SetWhich(buf);
	fileReader->GetString("WHAT", "", buf, MAX_CARD_NAME);
	SetWhat(buf);
	fileReader->GetInteger("VAL", 0, (int*)&m_val);
	fileReader->GetInteger("LOOP", 0, (int*)&m_loop);
	fileReader->GetInteger("AREA", 0, (int*)&m_area);
	fileReader->GetInteger("MAX", 0, (int*)&m_max);
	fileReader->GetInteger("RND", 0, (int*)&m_rnd);
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

