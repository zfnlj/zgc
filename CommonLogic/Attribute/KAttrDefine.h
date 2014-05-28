/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once

namespace KAttributeAbout
{
	/**
	 * 所有属性的定义id，包括角色，玩家等
	 */
	enum ATTRIBUTE_DEFINATION
	{
		// 角色数值属性
		character_attr_start		= 1,	// 角色属性开始
		part0_ca1_start = character_attr_start,	// part开头的枚举仅供程序判定界限使用

		ca_Combat = part0_ca1_start,// 战斗状态(1)
		ca_WalkState,				// 走跑状态(2)
		ca_CanNotRun,				// 疲惫(3)
		ca_CanNotMove,				// 定身(4)
		ca_Stun,					// 昏迷(5)
		ca_SlipDown,				// 倒地(6)
		ca_Fear,					// 恐惧(7)
		ca_Indistinct,				// 迷惑(8)
		ca_Sleep,					// 沉睡(9)
		ca_Powerless,				// 无力(10)
		ca_Cripple,					// 残废(11)
		ca_Panic,					// 慌乱(12)
		ca_Weak,					// 虚弱(13)
		ca_Lax,						// 涣散(14)
		ca_Invis,					// 隐身(15)
		ca_Hide,					// 潜伏(16)
		ca_AntiInvis,				// 真视(17)
		ca_AntiHide,				// 明目(18)
		ca_Crazy,					// 狂乱(19)
		ca_Deathless,				// 不会死亡(20)
		ca_CanNotBeSelectTarget,	// 不能被选中(21)
		ca_CanNotBeSkillTarget,		// 不能被作为技能目标(22)
		ca_CanNotBeEffectTarget,	// 不能被作为技能作用目标(23)
		ca_SystemFrozen,			// 系统设定的定身状态(24)



		ca_Level	= 41,			// 等级(41)
		ca_Name,					// MessageID(42)
		ca_School,					// 门(43)
		ca_Grade,					// 纲(44)
		ca_Class,					// 目(45)
		ca_Profession,				// 职业(46)
		ca_CharacterDisplayID,		// 角色表现表ID(47)
		ca_SizeExpend,				// 角色放大比例(48)
		ca_Force,					// 所属势力(49)
		ca_Sex,						// 性别(50)
		ca_Size,					// 大小(51)
		ca_AttackCD,				// 攻击间隔(TODO: 准备废弃)(52)
		ca_HP,						// 当前HP(53)
		ca_MP,						// 当前MP(54)
		ca_HairStyle,				// 发型(55)
		ca_HairColor,				// 发色(56)
		ca_SkinColor,				// 肤色(57)
		ca_Talkable,				// 可对话(58)
		ca_ImmuneStatus1,			// 免疫1组状态(59)
		ca_ImmuneStatus2,			// 免疫2组状态(60)
		ca_ImmuneStatus3,			// 免疫3组状态(61)
		ca_ImmuneStatus4,			// 免疫4组状态(62)
		ca_ImmuneStatus5,			// 免疫5组状态(63)
		ca_ImmuneStatus6,			// 免疫6组状态(64)
		ca_ImmuneStatus7,			// 免疫7组状态(65)
		ca_ImmuneStatus8,			// 免疫8组状态(66)
		ca_ImmuneStatus9,			// 免疫9组状态(67)
		ca_ImmuneStatus10,			// 免疫10组状态(68)
		ca_ImmuneStatus11,			// 免疫11组状态(69)
		ca_ImmuneStatus12,			// 免疫12组状态(70)
		ca_ImmuneStatus13,			// 免疫13组状态(71)
		ca_ImmuneStatus14,			// 免疫14组状态(72)
		ca_ImmuneStatus15,			// 免疫15组状态(73)
		ca_ImmuneStatus16,			// 免疫16组状态(74)
		ca_ImmuneStatus17,			// 免疫17组状态(75)
		ca_ImmuneStatus18,			// 免疫18组状态(76)
		ca_ImmuneStatus19,			// 免疫19组状态(77)


		part0_ca1_end,

		// 角色公式属性
		part0_ca2_start = 1001,
		ca_Strength = part0_ca2_start,	// 力量(1001)
		ca_Stamina,						// 耐力(1002)
		ca_Agility,						// 灵巧(1003)
		ca_Will,						// 意志(1004)
// 		ca_SharpEyed,					// 锐眼(1005)

		ca_AttackPoint		= 1021,		// 攻击参数(1021)
		ca_ArmorClass,					// 防御参数(护甲(1022))
		ca_HitStun,						// 打击硬直能力(1023)
		ca_HitRecovery,					// 硬直回复值(1024)
		ca_AvoidPoint,					// 躲闪参数(1025)
		ca_CriticalPoint,				// 暴击参数(1026)
		ca_AffectPoint,					// 生效参数(1027)
		ca_HastePoint,					// 加速参数(1028)
		ca_CriticalDamageRate,			// 暴击伤害比例(1029)
		ca_HPMax,						// 血量上限(1030)
		ca_HPCap,						// 可用血量上限(1031)
		ca_HPRecovery,					// 血量回复速度(1032)
		ca_MPMax,						// 法力上限(1033)
		ca_MPCap,						// 可用法力上限(1034)
		ca_MPRecovery,					// 法力回复速度(1035)
		ca_WalkSpeed,					// 步行速度(1036)
		ca_RunSpeed,					// 跑步速度(1037)
		ca_TurnRadius,					// 转弯半径(1038)
		ca_Sight,						// 视野(1039)
		ca_Feeling,						// 感知(1040)
		ca_Attack,						// 攻击力(1041)
		ca_BeHeal,						// 受到治疗能力(1042)
		ca_WeaponDPS,					// 武器DPS(1043)
		ca_WeaponCD,					// 武器攻击一次时间(1044)
		ca_AttackStability,				// 伤害稳定性(攻击稳定性)(1045)

		ca_AffectResist		= 1046,		// 挣扎(抵抗持续时间)(1046)
		ca_CriticalResist,				// 抗暴击(1047)
		ca_PsychoPoint,					// 乱神生效(1048)
		ca_HoldPoint,					// 牵制生效(1049)
		ca_SealPoint,					// 封印生效(1050)
		ca_WeakPoint,					// 弱化生效(1051)
		ca_BlasterPoint,				// 重创生效(1052)
		ca_PsychoResist,				// 乱神适应(1053)
		ca_HoldResist,					// 牵制适应(1054)
		ca_SealResist,					// 封印适应(1055)
		ca_WeakResist,					// 弱化适应(1056)
		ca_BlasterResist,				// 重创适应(1057)
		ca_Penetrate,					// 护甲穿透(1058)
		ca_Accurate,					// 命中(1059)

		ca_ExpRate			= 1061,		// 经验获取率(1061)
		
		ca_Shield			= 1063,		// 当前护盾值(1063)
		ca_ShieldMax,					// 护盾上限值(1064)
		ca_GeneralDamageMultiple,		// 通用伤害倍数(1065)
		ca_GeneralReboundMultiple,		// 通用反弹倍数(1066)


		part0_ca2_end,
		character_attr_end = part0_ca2_end,	// 角色属性结束

		player_attr_start		= 10001,// 玩家属性开始

		part1_ca1_start = player_attr_start,
		ca_Exp = part1_ca1_start,	// 经验(10001)
		ca_TeamID,					// 队伍id(10002)
		ca_TeamPopedom,				// (10003)
		ca_BluePower,				// 深蓝之力(10004)
		ca_CanNotRide,				// 不可骑乘(此项与骑乘许可不同)(10005)
		ca_RideStatus,				// 骑乘状态(10006)
		ca_BreakSpeed,				// (10007)
		ca_BreakDepth,				// (10008)
		ca_BreakQuality,			// (10009)
		ca_LicenseLevel,			// 猎人执照等级(10010)
		ca_WorldGroup,				// 世界团队归属(10011)
		ca_RankExp,					// 军衔经验(10012)
		ca_ForceStraight,			// 强制前移。该属性被置时，玩家总是尝试向前冲(10013)

		part1_ca1_end,

		part1_ca2_start = 11001,
		part1_ca2_end,

		// 策划强化属性，预留200个空
		part1_ca3_start = 12001,
		ca_PowerBeans				= 12006,// 剑舞者的能量豆

		ca_HostileStrategyLevel		= 12021,// 进攻战术等级
		ca_DefensiveStrategyLevel	= 12022,// 防御战术等级
		ca_TechnicalStrategyLevel	= 12023,// 技巧战术等级
		ca_AdaptiveStrategyLevel	= 12024,// 适应战术等级
		ca_HostileAmplifier			= 12025,// 进攻增幅
		ca_DefensiveAmplifier		= 12026,// 防御增幅
		ca_TechnicalAmplifier		= 12027,// 技巧增幅
		ca_AdaptiveAmplifier		= 12028,// 适应增幅
		ca_TotalAmplifier			= 12029,// 总增幅点
		ca_CompLevel				= 12030,// 深蓝领悟等级
		ca_CompExp					= 12031,// 深蓝领悟槽值

		part1_ca3_end = part1_ca3_start + 200,
		player_attr_end		= part1_ca3_end,	// 玩家属性开始

		npc_attr_start				= 20001,// npc属性开始
		// NPC数据属性
		part2_ca1_start = npc_attr_start,
		ca_Wounded = part2_ca1_start,// 濒死状态(20001)
		ca_Interactive,			// 与他人的互动数(20002)
		ca_Temprament,			// 性格。0 被动，1 主动(20003)
		part2_ca1_end,

		// NPC公式属性
		part2_ca2_start = 21001,
		part2_ca2_end,
		npc_attr_end = part2_ca2_end,	// npc属性结束

		battle_pet_attr_start		= 30001,//战兽属性开始
		// 战兽数据属性
		part3_ca1_start = battle_pet_attr_start,
		ca_BpCurExp = part3_ca1_start,			// 当前经验值(30001)
		ca_SeriouslyInjuredPer		=30002,		// 重伤恢复百分比
		ca_CarryWeight				=30003,		// 负重
		ca_AISwitch					=30004,		// AI开关
		ca_BpBluePower				=30005,		// 当前深蓝之力
		ca_Favor					=30006,		// 喜好
		
		ca_StrengthenNum			=30007,		// 强化次数
		ca_WorkStrength				=30008,		// 力量(工宠)
		ca_WorkAgility				=30009,		// 敏捷(工宠)
		ca_WorkVigour				=30010,		// 悟性(工宠)
		
// 		ca_BruteForce				=30011,		// 力量
// 		ca_Psychic					=30012,		// 意志
// 		ca_StrongBody				=30013,		// 耐力
// 		ca_Pachyderma				=30014,		// 灵巧
		ca_SharpEyed				=30015,		// 锐眼
		
		ca_StrengthApt				=30016,		// 力量资质
		ca_WillApt					=30017,		// 意志资质
		ca_StaminaApt				=30018,		// 耐力资质
		ca_AgilityApt				=30019,		// 灵巧资质
		ca_SharpEyedApt				=30020,		// 锐眼资质
		
		ca_StrengthLvl				=30021,		// 强度等级
		ca_Quality					=30022,		// 品质
		ca_TakingExerciseNum		=30023,		// 可培养次数(锻炼次数)

		ca_BpSpeed					=30036,		// 骑乘
		ca_WorkStrengthApt			=30037,		// 力量资质(工宠)
		ca_WorkAgilityApt			=30038,		// 敏捷资质(工宠)
		ca_WorkVigourApt			=30039,		// 智力资质(工宠)
		
		part3_ca1_end,

		// 战兽公式属性
		part3_ca2_start = 31001,
		part3_ca2_end,
		battle_pet_attr_end = part3_ca2_end,	//战兽属性结束

		// Building数据属性
		building_attr_start			= 40001,//building属性开始
		part4_ca1_start = building_attr_start,
		ca_BuildingStatus = part4_ca1_start,	// 
		part4_ca1_end,

		// Building公式属性
		part4_ca2_start = 41001,
		ca_BuildingTep = part4_ca2_start,	// 
		ca_BuildingMoi,						// 
		ca_BuildingWin,						// 
		ca_CapacityMax,						// 
		ca_Capacity,						// 
		ca_DurabilityMax,					// 
		ca_Durability,						// 
		ca_DurabilityRate,					// 
		ca_NatureWastage,					// 
		part4_ca2_end,
		building_attr_end = part4_ca2_end,	// building属性结束

		// 交通工具数据属性
		transport_attr_start		= 50001,// 交通工具属性开始
		part5_ca1_start = transport_attr_start,
		part5_ca1_end,

		// 交通工具公式属性
		part5_ca2_start = 51001,
		part5_ca2_end,
		transport_attr_end = part5_ca2_end,// 交通工具属性结束

		// 部落战兽数据属性
		tribebattlepet_attr_start		= 60001,// 部落战兽属性开始
		part6_ca1_start = tribebattlepet_attr_start,
		ca_SkillExp		= 60001,		// 技能经验值
		ca_TriBpExp		= 60002,		// 经验值
		ca_TriBpQuality	= 60003,		// 部落战兽品质
		ca_TriBpClass	= 60004,		// 阶级
		ca_Fatigue		= 60005,		// 疲劳度
		
		ca_TotalAttExp		= 60006,	// 总属性经验值
		ca_StrengthAttExp	= 60007,	// 力量经验值
		ca_StaminaAttExp	= 60008,	// 耐力经验值
		ca_AgilityAttExp	= 60009,	// 敏捷经验值
		ca_WillAttExp		= 60010,	// 意志经验值

		part6_ca1_end,

		// 部落战兽公式属性
		part6_ca2_start = 61001,
		part6_ca2_end,
		tribebattlepet_attr_end = part6_ca2_end,// 部落战兽属性结束
	};

	// 每角色的分组大小
	const static int COMMON_ATTRIBUTE_GROUP_SIZE = 1000;

	// 每个角色段的分隔大小
	const static int COMMON_OBJ_ATTRIBUTE_SIZE = 10000;

	// 将索引和位置信息压缩成一个idx
	int CompactIndex(int idx, int param);
	// 将一个idx解压成idx和位置信息
	void DecompackIndex(int compackedIdx, int& idx, int& param);
	// 判定是否为符合属性
	bool IsCompositeAttr(int idx);


	// WorldPlayer属性组
	// 这里加一个属性，在KGWAttrTable::KGWAttrTable()中也要添加
	enum enum_WORLDPLAYER_ATTR
	{
		// KWorldPlayer属性
		eWPA_Undefine = 0,

		eWPA_Sex,
		eWPA_Class,
		eWPA_Level,
		eWPA_Exp,
		eWPA_HP,
		eWPA_MP,
		eWPA_HPMax,
		eWPA_HPCap,
		eWPA_MPMax,
		eWPA_MPCap,
		eWPA_Force,
		eWPA_Profession,
		eWPA_Combat,

		//eWPA_MastLevel,
		//eWPA_GuildID,
		//eWPA_Side,
		//eWPA_EquipMark,
		//eWPA_LastLevelUpTime,
		//eWPA_LoginTime,
		//eWPA_SeekTeamTime,

		eWPA_Count,
	};	
};

// 角色的属性初始化列表
#define USING_ATTR_INIT_LIST(name) \
struct \
{ \
	int idx; \
	int initval; \
}name[] = \
{ \
	KAttributeAbout::ca_AttackPoint,		0, \
	KAttributeAbout::ca_ArmorClass,			0, \
	KAttributeAbout::ca_HitStun,			0, \
	KAttributeAbout::ca_HitRecovery,		0, \
	KAttributeAbout::ca_AvoidPoint,			0, \
	KAttributeAbout::ca_CriticalPoint,		0, \
	KAttributeAbout::ca_AffectPoint,		0, \
	KAttributeAbout::ca_HastePoint,			0, \
	KAttributeAbout::ca_CriticalDamageRate,	500, \
	KAttributeAbout::ca_HPMax,				1, \
	KAttributeAbout::ca_HPCap,				0, \
	KAttributeAbout::ca_HPRecovery,			0, \
	KAttributeAbout::ca_MPMax,				0, \
	KAttributeAbout::ca_MPCap,				0, \
	KAttributeAbout::ca_MPRecovery,			0, \
	KAttributeAbout::ca_WalkSpeed,			200, \
	KAttributeAbout::ca_RunSpeed,			600, \
	KAttributeAbout::ca_TurnRadius,			0, \
	KAttributeAbout::ca_Sight,				4000, \
	KAttributeAbout::ca_Feeling,			1000, \
	KAttributeAbout::ca_Attack,				0, \
	KAttributeAbout::ca_BeHeal,				1000, \
	KAttributeAbout::ca_WeaponDPS,			0, \
	KAttributeAbout::ca_WeaponCD,			1000, \
	KAttributeAbout::ca_Size,				100, \
};

// 限制主动移动的属性列表
#define USING_FORBID_WALK_ATTR_LIST(name)	\
const int name[] = {					\
	KAttributeAbout::ca_CanNotMove,		\
	KAttributeAbout::ca_Stun,			\
	KAttributeAbout::ca_SlipDown,		\
	KAttributeAbout::ca_Fear,			\
	KAttributeAbout::ca_Indistinct,		\
	KAttributeAbout::ca_Sleep,			\
	KAttributeAbout::ca_Crazy,			\
	KAttributeAbout::ca_SystemFrozen,	\
};

// 复合属性列表
#define USING_COMPOSITE_ATTR_LIST(name)	\
	const int name[] = {				\
	KAttributeAbout::ca_Strength,		\
	KAttributeAbout::ca_Stamina,		\
	KAttributeAbout::ca_Agility,		\
	KAttributeAbout::ca_Will,			\
};

