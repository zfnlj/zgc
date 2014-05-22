#pragma once

#include "Inc/lua/KLuaWrap.h"
namespace KAttributeAbout
{
	enum ATTR_1
	{
		ca_1Begin,
		ca_level,
		ca_exp,
		ca_hp,
		ca_mp,
		ca_str,//体力值
		ca_mag,
		ca_strAttack,//攻击强度
		ca_strDef,
		ca_magAttack,
		ca_magDef,
		ca_hit,//命中
		ca_miss,//闪避
		ca_crit,//暴击
		ca_tough,//韧性
		ca_attackSpeed,
		ca_attackRange,
		ca_moveSpeed,
		ca_baseHit,//基础命中
		ca_critNum,//暴击倍率
 		ca_cny,
 		ca_gameMoney,
		ca_selectPos=88,
		ca_1End
	};

	enum ATTR_2
	{
		ca_2Begin =ca_1End,
		ca_maxHP,//最大血量qwe
		ca_maxMP,//最大魔法
		ca_2End
	};

	enum ATTR_3
	{
		ca_3Begin = ca_2End,
		ca_CanNotBeSelectTarget,	// 不能被选中
		ca_CanNotBeSkillTarget,		// 不能被作为技能目标
		ca_CanNotBeEffectTarget,	// 不能被作为技能作用目标
		ca_Combat ,// 战斗状态
		ca_Deathless,				// 不会死亡
		ca_Size,					// 大小
		ca_HastePoint,				// 加速参数
		ca_WalkState,				// 走跑状态
		ca_CanNotRun,				// 疲惫
		ca_CanNotMove,				// 定身
		ca_Stun,					// 昏迷
		ca_SlipDown,				// 倒地
		ca_Fear,					// 恐惧
		ca_Indistinct,				// 迷惑
		ca_Sleep,					// 沉睡
		ca_Powerless,				// 无力
		ca_Cripple,					// 残废
		ca_Panic,					// 慌乱
		ca_Weak,					// 虚弱
		ca_Lax,						// 涣散
		ca_Invis,					// 隐身
		ca_Hide,					// 潜伏
		ca_AntiInvis,				// 真视
		ca_AntiHide,				// 明目
		ca_Crazy,					// 狂乱
		ca_RideStatus,				// 是否处于骑乘状态
		ca_TeamID,
		ca_morph,					//变形 羊刀:)
		ca_3End
	};
	
	int CompactIndex(int idx, int param);
	void DecompackIndex(int compackedIdx, int& idx, int& param);

template <int x> void export_gameserver_attribute_to_lua()
{
    
	export_enum_to_lua(ca_level);
	export_enum_to_lua(ca_exp);
	export_enum_to_lua(ca_hp);
	export_enum_to_lua(ca_mp);
	export_enum_to_lua(ca_str);
	export_enum_to_lua(ca_mag);
	export_enum_to_lua(ca_strAttack)
	export_enum_to_lua(ca_strDef);
	export_enum_to_lua(ca_magAttack);
	export_enum_to_lua(ca_magDef);
	export_enum_to_lua(ca_hit);//命中
	export_enum_to_lua(ca_miss);//闪避
	export_enum_to_lua(ca_crit);//暴击
	export_enum_to_lua(ca_tough);//韧性
	export_enum_to_lua(ca_attackSpeed);
	export_enum_to_lua(ca_attackRange);
	export_enum_to_lua(ca_moveSpeed);
	export_enum_to_lua(ca_baseHit);//基础命中
	export_enum_to_lua(ca_critNum);//暴击倍率
	
     //export_enum_to_lua(ca_money1);
	//export_enum_to_lua(ca_money2);
}}

#define export_GS_attributes() KAttributeAbout::export_gameserver_attribute_to_lua<0>();
