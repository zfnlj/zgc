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
		ca_str,//����ֵ
		ca_mag,
		ca_strAttack,//����ǿ��
		ca_strDef,
		ca_magAttack,
		ca_magDef,
		ca_hit,//����
		ca_miss,//����
		ca_crit,//����
		ca_tough,//����
		ca_attackSpeed,
		ca_attackRange,
		ca_moveSpeed,
		ca_baseHit,//��������
		ca_critNum,//��������
 		ca_cny,
 		ca_gameMoney,
		ca_selectPos=88,
		ca_1End
	};

	enum ATTR_2
	{
		ca_2Begin =ca_1End,
		ca_maxHP,//���Ѫ��qwe
		ca_maxMP,//���ħ��
		ca_2End
	};

	enum ATTR_3
	{
		ca_3Begin = ca_2End,
		ca_CanNotBeSelectTarget,	// ���ܱ�ѡ��
		ca_CanNotBeSkillTarget,		// ���ܱ���Ϊ����Ŀ��
		ca_CanNotBeEffectTarget,	// ���ܱ���Ϊ��������Ŀ��
		ca_Combat ,// ս��״̬
		ca_Deathless,				// ��������
		ca_Size,					// ��С
		ca_HastePoint,				// ���ٲ���
		ca_WalkState,				// ����״̬
		ca_CanNotRun,				// ƣ��
		ca_CanNotMove,				// ����
		ca_Stun,					// ����
		ca_SlipDown,				// ����
		ca_Fear,					// �־�
		ca_Indistinct,				// �Ի�
		ca_Sleep,					// ��˯
		ca_Powerless,				// ����
		ca_Cripple,					// �з�
		ca_Panic,					// ����
		ca_Weak,					// ����
		ca_Lax,						// ��ɢ
		ca_Invis,					// ����
		ca_Hide,					// Ǳ��
		ca_AntiInvis,				// ����
		ca_AntiHide,				// ��Ŀ
		ca_Crazy,					// ����
		ca_RideStatus,				// �Ƿ������״̬
		ca_TeamID,
		ca_morph,					//���� ��:)
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
	export_enum_to_lua(ca_hit);//����
	export_enum_to_lua(ca_miss);//����
	export_enum_to_lua(ca_crit);//����
	export_enum_to_lua(ca_tough);//����
	export_enum_to_lua(ca_attackSpeed);
	export_enum_to_lua(ca_attackRange);
	export_enum_to_lua(ca_moveSpeed);
	export_enum_to_lua(ca_baseHit);//��������
	export_enum_to_lua(ca_critNum);//��������
	
     //export_enum_to_lua(ca_money1);
	//export_enum_to_lua(ca_money2);
}}

#define export_GS_attributes() KAttributeAbout::export_gameserver_attribute_to_lua<0>();
