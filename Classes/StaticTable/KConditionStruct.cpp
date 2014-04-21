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
