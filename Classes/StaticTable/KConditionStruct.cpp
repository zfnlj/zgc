#include "KConditionStruct.h"
#include "string.h"
#include <System/Misc/StrUtil.h>
#include "KAbilityStatic.h"

void KConditionDef::ParseString(const char* str)
{
	char strBuf[1024];
	strcpy(strBuf,str);

	char* ss[64];
	int ns = split(strBuf, " ", ss, 64);
	if(ns!=2) return;

	if(strcmp(ss[0],"ATK_LE")==0){
		_condition = con_atk_le;
		_val = atoi(ss[1]);
	}else if(strcmp(ss[0],"ATK_HE")==0){
		_condition = con_atk_he;
		_val = atoi(ss[1]);
	}else if(strcmp(ss[0],"EXIST_BUF")==0){
		_condition = con_exist_buf;
		_val = KAbilityStatic::Str2What(ss[1]);
	}
}


void KValDef::ParseString(char* str)
{
	char* ss[64];
	int ns = split(str, ";", ss, 64);
	if(ns>=2){
		_val = atoi(ss[1]);
	}
	if(ns>=3){
		_val2 = atoi(ss[2]);
	}
	if(strcmp(ss[0],"MyHurtedSoldier")==0){
		_def = val_my_hurted_soldierNum;
	}else if(strcmp(ss[0],"SoldierNum")==0){
		_def = val_soldierNum;
	}else if(strcmp(ss[0],"GetHp")==0){
		_def = val_getHp;
	}else if(strcmp(ss[0],"Rnd")==0){
		_def = val_rnd;
	}else{
		_def = val_normal;
		_val = atoi(str);
	}
}