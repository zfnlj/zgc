/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once

namespace KAttributeAbout
{
	/**
	 * �������ԵĶ���id��������ɫ����ҵ�
	 */
	enum ATTRIBUTE_DEFINATION
	{
		// ��ɫ��ֵ����
		character_attr_start		= 1,	// ��ɫ���Կ�ʼ
		part0_ca1_start = character_attr_start,	// part��ͷ��ö�ٽ��������ж�����ʹ��

		ca_Combat = part0_ca1_start,// ս��״̬(1)
		ca_WalkState,				// ����״̬(2)
		ca_CanNotRun,				// ƣ��(3)
		ca_CanNotMove,				// ����(4)
		ca_Stun,					// ����(5)
		ca_SlipDown,				// ����(6)
		ca_Fear,					// �־�(7)
		ca_Indistinct,				// �Ի�(8)
		ca_Sleep,					// ��˯(9)
		ca_Powerless,				// ����(10)
		ca_Cripple,					// �з�(11)
		ca_Panic,					// ����(12)
		ca_Weak,					// ����(13)
		ca_Lax,						// ��ɢ(14)
		ca_Invis,					// ����(15)
		ca_Hide,					// Ǳ��(16)
		ca_AntiInvis,				// ����(17)
		ca_AntiHide,				// ��Ŀ(18)
		ca_Crazy,					// ����(19)
		ca_Deathless,				// ��������(20)
		ca_CanNotBeSelectTarget,	// ���ܱ�ѡ��(21)
		ca_CanNotBeSkillTarget,		// ���ܱ���Ϊ����Ŀ��(22)
		ca_CanNotBeEffectTarget,	// ���ܱ���Ϊ��������Ŀ��(23)
		ca_SystemFrozen,			// ϵͳ�趨�Ķ���״̬(24)



		ca_Level	= 41,			// �ȼ�(41)
		ca_Name,					// MessageID(42)
		ca_School,					// ��(43)
		ca_Grade,					// ��(44)
		ca_Class,					// Ŀ(45)
		ca_Profession,				// ְҵ(46)
		ca_CharacterDisplayID,		// ��ɫ���ֱ�ID(47)
		ca_SizeExpend,				// ��ɫ�Ŵ����(48)
		ca_Force,					// ��������(49)
		ca_Sex,						// �Ա�(50)
		ca_Size,					// ��С(51)
		ca_AttackCD,				// �������(TODO: ׼������)(52)
		ca_HP,						// ��ǰHP(53)
		ca_MP,						// ��ǰMP(54)
		ca_HairStyle,				// ����(55)
		ca_HairColor,				// ��ɫ(56)
		ca_SkinColor,				// ��ɫ(57)
		ca_Talkable,				// �ɶԻ�(58)
		ca_ImmuneStatus1,			// ����1��״̬(59)
		ca_ImmuneStatus2,			// ����2��״̬(60)
		ca_ImmuneStatus3,			// ����3��״̬(61)
		ca_ImmuneStatus4,			// ����4��״̬(62)
		ca_ImmuneStatus5,			// ����5��״̬(63)
		ca_ImmuneStatus6,			// ����6��״̬(64)
		ca_ImmuneStatus7,			// ����7��״̬(65)
		ca_ImmuneStatus8,			// ����8��״̬(66)
		ca_ImmuneStatus9,			// ����9��״̬(67)
		ca_ImmuneStatus10,			// ����10��״̬(68)
		ca_ImmuneStatus11,			// ����11��״̬(69)
		ca_ImmuneStatus12,			// ����12��״̬(70)
		ca_ImmuneStatus13,			// ����13��״̬(71)
		ca_ImmuneStatus14,			// ����14��״̬(72)
		ca_ImmuneStatus15,			// ����15��״̬(73)
		ca_ImmuneStatus16,			// ����16��״̬(74)
		ca_ImmuneStatus17,			// ����17��״̬(75)
		ca_ImmuneStatus18,			// ����18��״̬(76)
		ca_ImmuneStatus19,			// ����19��״̬(77)


		part0_ca1_end,

		// ��ɫ��ʽ����
		part0_ca2_start = 1001,
		ca_Strength = part0_ca2_start,	// ����(1001)
		ca_Stamina,						// ����(1002)
		ca_Agility,						// ����(1003)
		ca_Will,						// ��־(1004)
// 		ca_SharpEyed,					// ����(1005)

		ca_AttackPoint		= 1021,		// ��������(1021)
		ca_ArmorClass,					// ��������(����(1022))
		ca_HitStun,						// ���Ӳֱ����(1023)
		ca_HitRecovery,					// Ӳֱ�ظ�ֵ(1024)
		ca_AvoidPoint,					// ��������(1025)
		ca_CriticalPoint,				// ��������(1026)
		ca_AffectPoint,					// ��Ч����(1027)
		ca_HastePoint,					// ���ٲ���(1028)
		ca_CriticalDamageRate,			// �����˺�����(1029)
		ca_HPMax,						// Ѫ������(1030)
		ca_HPCap,						// ����Ѫ������(1031)
		ca_HPRecovery,					// Ѫ���ظ��ٶ�(1032)
		ca_MPMax,						// ��������(1033)
		ca_MPCap,						// ���÷�������(1034)
		ca_MPRecovery,					// �����ظ��ٶ�(1035)
		ca_WalkSpeed,					// �����ٶ�(1036)
		ca_RunSpeed,					// �ܲ��ٶ�(1037)
		ca_TurnRadius,					// ת��뾶(1038)
		ca_Sight,						// ��Ұ(1039)
		ca_Feeling,						// ��֪(1040)
		ca_Attack,						// ������(1041)
		ca_BeHeal,						// �ܵ���������(1042)
		ca_WeaponDPS,					// ����DPS(1043)
		ca_WeaponCD,					// ��������һ��ʱ��(1044)
		ca_AttackStability,				// �˺��ȶ���(�����ȶ���)(1045)

		ca_AffectResist		= 1046,		// ����(�ֿ�����ʱ��)(1046)
		ca_CriticalResist,				// ������(1047)
		ca_PsychoPoint,					// ������Ч(1048)
		ca_HoldPoint,					// ǣ����Ч(1049)
		ca_SealPoint,					// ��ӡ��Ч(1050)
		ca_WeakPoint,					// ������Ч(1051)
		ca_BlasterPoint,				// �ش���Ч(1052)
		ca_PsychoResist,				// ������Ӧ(1053)
		ca_HoldResist,					// ǣ����Ӧ(1054)
		ca_SealResist,					// ��ӡ��Ӧ(1055)
		ca_WeakResist,					// ������Ӧ(1056)
		ca_BlasterResist,				// �ش���Ӧ(1057)
		ca_Penetrate,					// ���״�͸(1058)
		ca_Accurate,					// ����(1059)

		ca_ExpRate			= 1061,		// �����ȡ��(1061)
		
		ca_Shield			= 1063,		// ��ǰ����ֵ(1063)
		ca_ShieldMax,					// ��������ֵ(1064)
		ca_GeneralDamageMultiple,		// ͨ���˺�����(1065)
		ca_GeneralReboundMultiple,		// ͨ�÷�������(1066)


		part0_ca2_end,
		character_attr_end = part0_ca2_end,	// ��ɫ���Խ���

		player_attr_start		= 10001,// ������Կ�ʼ

		part1_ca1_start = player_attr_start,
		ca_Exp = part1_ca1_start,	// ����(10001)
		ca_TeamID,					// ����id(10002)
		ca_TeamPopedom,				// (10003)
		ca_BluePower,				// ����֮��(10004)
		ca_CanNotRide,				// �������(�����������ɲ�ͬ)(10005)
		ca_RideStatus,				// ���״̬(10006)
		ca_BreakSpeed,				// (10007)
		ca_BreakDepth,				// (10008)
		ca_BreakQuality,			// (10009)
		ca_LicenseLevel,			// ����ִ�յȼ�(10010)
		ca_WorldGroup,				// �����Ŷӹ���(10011)
		ca_RankExp,					// ���ξ���(10012)
		ca_ForceStraight,			// ǿ��ǰ�ơ������Ա���ʱ��������ǳ�����ǰ��(10013)

		part1_ca1_end,

		part1_ca2_start = 11001,
		part1_ca2_end,

		// �߻�ǿ�����ԣ�Ԥ��200����
		part1_ca3_start = 12001,
		ca_PowerBeans				= 12006,// �����ߵ�������

		ca_HostileStrategyLevel		= 12021,// ����ս���ȼ�
		ca_DefensiveStrategyLevel	= 12022,// ����ս���ȼ�
		ca_TechnicalStrategyLevel	= 12023,// ����ս���ȼ�
		ca_AdaptiveStrategyLevel	= 12024,// ��Ӧս���ȼ�
		ca_HostileAmplifier			= 12025,// ��������
		ca_DefensiveAmplifier		= 12026,// ��������
		ca_TechnicalAmplifier		= 12027,// ��������
		ca_AdaptiveAmplifier		= 12028,// ��Ӧ����
		ca_TotalAmplifier			= 12029,// ��������
		ca_CompLevel				= 12030,// ��������ȼ�
		ca_CompExp					= 12031,// ���������ֵ

		part1_ca3_end = part1_ca3_start + 200,
		player_attr_end		= part1_ca3_end,	// ������Կ�ʼ

		npc_attr_start				= 20001,// npc���Կ�ʼ
		// NPC��������
		part2_ca1_start = npc_attr_start,
		ca_Wounded = part2_ca1_start,// ����״̬(20001)
		ca_Interactive,			// �����˵Ļ�����(20002)
		ca_Temprament,			// �Ը�0 ������1 ����(20003)
		part2_ca1_end,

		// NPC��ʽ����
		part2_ca2_start = 21001,
		part2_ca2_end,
		npc_attr_end = part2_ca2_end,	// npc���Խ���

		battle_pet_attr_start		= 30001,//ս�����Կ�ʼ
		// ս����������
		part3_ca1_start = battle_pet_attr_start,
		ca_BpCurExp = part3_ca1_start,			// ��ǰ����ֵ(30001)
		ca_SeriouslyInjuredPer		=30002,		// ���˻ָ��ٷֱ�
		ca_CarryWeight				=30003,		// ����
		ca_AISwitch					=30004,		// AI����
		ca_BpBluePower				=30005,		// ��ǰ����֮��
		ca_Favor					=30006,		// ϲ��
		
		ca_StrengthenNum			=30007,		// ǿ������
		ca_WorkStrength				=30008,		// ����(����)
		ca_WorkAgility				=30009,		// ����(����)
		ca_WorkVigour				=30010,		// ����(����)
		
// 		ca_BruteForce				=30011,		// ����
// 		ca_Psychic					=30012,		// ��־
// 		ca_StrongBody				=30013,		// ����
// 		ca_Pachyderma				=30014,		// ����
		ca_SharpEyed				=30015,		// ����
		
		ca_StrengthApt				=30016,		// ��������
		ca_WillApt					=30017,		// ��־����
		ca_StaminaApt				=30018,		// ��������
		ca_AgilityApt				=30019,		// ��������
		ca_SharpEyedApt				=30020,		// ��������
		
		ca_StrengthLvl				=30021,		// ǿ�ȵȼ�
		ca_Quality					=30022,		// Ʒ��
		ca_TakingExerciseNum		=30023,		// ����������(��������)

		ca_BpSpeed					=30036,		// ���
		ca_WorkStrengthApt			=30037,		// ��������(����)
		ca_WorkAgilityApt			=30038,		// ��������(����)
		ca_WorkVigourApt			=30039,		// ��������(����)
		
		part3_ca1_end,

		// ս�޹�ʽ����
		part3_ca2_start = 31001,
		part3_ca2_end,
		battle_pet_attr_end = part3_ca2_end,	//ս�����Խ���

		// Building��������
		building_attr_start			= 40001,//building���Կ�ʼ
		part4_ca1_start = building_attr_start,
		ca_BuildingStatus = part4_ca1_start,	// 
		part4_ca1_end,

		// Building��ʽ����
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
		building_attr_end = part4_ca2_end,	// building���Խ���

		// ��ͨ������������
		transport_attr_start		= 50001,// ��ͨ�������Կ�ʼ
		part5_ca1_start = transport_attr_start,
		part5_ca1_end,

		// ��ͨ���߹�ʽ����
		part5_ca2_start = 51001,
		part5_ca2_end,
		transport_attr_end = part5_ca2_end,// ��ͨ�������Խ���

		// ����ս����������
		tribebattlepet_attr_start		= 60001,// ����ս�����Կ�ʼ
		part6_ca1_start = tribebattlepet_attr_start,
		ca_SkillExp		= 60001,		// ���ܾ���ֵ
		ca_TriBpExp		= 60002,		// ����ֵ
		ca_TriBpQuality	= 60003,		// ����ս��Ʒ��
		ca_TriBpClass	= 60004,		// �׼�
		ca_Fatigue		= 60005,		// ƣ�Ͷ�
		
		ca_TotalAttExp		= 60006,	// �����Ծ���ֵ
		ca_StrengthAttExp	= 60007,	// ��������ֵ
		ca_StaminaAttExp	= 60008,	// ��������ֵ
		ca_AgilityAttExp	= 60009,	// ���ݾ���ֵ
		ca_WillAttExp		= 60010,	// ��־����ֵ

		part6_ca1_end,

		// ����ս�޹�ʽ����
		part6_ca2_start = 61001,
		part6_ca2_end,
		tribebattlepet_attr_end = part6_ca2_end,// ����ս�����Խ���
	};

	// ÿ��ɫ�ķ����С
	const static int COMMON_ATTRIBUTE_GROUP_SIZE = 1000;

	// ÿ����ɫ�εķָ���С
	const static int COMMON_OBJ_ATTRIBUTE_SIZE = 10000;

	// ��������λ����Ϣѹ����һ��idx
	int CompactIndex(int idx, int param);
	// ��һ��idx��ѹ��idx��λ����Ϣ
	void DecompackIndex(int compackedIdx, int& idx, int& param);
	// �ж��Ƿ�Ϊ��������
	bool IsCompositeAttr(int idx);


	// WorldPlayer������
	// �����һ�����ԣ���KGWAttrTable::KGWAttrTable()��ҲҪ���
	enum enum_WORLDPLAYER_ATTR
	{
		// KWorldPlayer����
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

// ��ɫ�����Գ�ʼ���б�
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

// ���������ƶ��������б�
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

// ���������б�
#define USING_COMPOSITE_ATTR_LIST(name)	\
	const int name[] = {				\
	KAttributeAbout::ca_Strength,		\
	KAttributeAbout::ca_Stamina,		\
	KAttributeAbout::ca_Agility,		\
	KAttributeAbout::ca_Will,			\
};

