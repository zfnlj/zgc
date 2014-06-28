#ifndef _KCONDITIONSTRUCT_H
#define _KCONDITIONSTRUCT_H


class KCardInst;
struct KConditionDef{
	enum con_def{
		con_null,
		con_atk_le,
		con_atk_he,
		con_exist_buf,
	};
	KConditionDef():_condition(con_null){}
	con_def _condition;
	int _val;
	void ParseString(const char* str);
	int GetVal(){ return _val;}
	con_def GetCond(){ return _condition;}
};

struct KValDef{
	enum val_def{
		val_normal,
		val_my_hurted_soldierNum,
		val_soldierNum,
		val_mySoldierNum,
		val_rnd,
		val_getHp,
		val_lua,
	};
	KValDef();
	void ParseString(char* str);
	val_def GetDef(){ return _def;}
	int GetVal(){ return _val;}
	val_def _def;
	int _val;
	int _val2;
	char _luaStr[24];
};
#endif // __HELLOWORLD_SCENE_H__
