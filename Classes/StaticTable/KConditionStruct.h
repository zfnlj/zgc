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

#endif // __HELLOWORLD_SCENE_H__
