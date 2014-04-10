/********************************************************************
	created:	2008/05/12
	created:	12:5:2008   13:46
	filename: 	Athena\Main\Src\Common\LogicWorld\Src\Item\KItemDef.h
	file path:	Athena\Main\Src\Common\LogicWorld\Src\Item
	file base:	KItemDef
	file ext:	h
	author:		jungle
	
	purpose:	����Ʒ��صĶ��嶼��������
*********************************************************************/

#pragma once

#include "Inc/KCommonDefine.h"
#include "../KGlobalFunction.h"
// ���󷵻�ֵ��������ʱ
//-1 ��Ʒ���ݶ�ȡʧ��
//-2 ��Ʒ������Ϣ��ȡʧ��
//-3 ״̬���
//-4 λ�ü��
//-5 ������
//-6 �������
//-7 ְҵ���
//-8 ���Ǽ��
//-9 �������

enum eItem_Element_Type
{
	enumElement_None = 0,
	enumElement_Golden,		// ��1
	enumElement_Wood,		// ľ2
	enumElement_Water,		// ˮ3
	enumElement_Fire,		// ��4
	enumElement_Earth,		// ��5
	enumElement_Poison,		// ��6
	enumElement_Immortal,	// ��7
	enumElement_Demon,		// ħ8
	enumElement_Count,
};

enum ITEM_OPERATOR
{
	enumIO_sucess = 0,				// �ɹ�
	enumIO_failed,					// ͨ�ô���
	enumIO_logicError,				// ������ֵĴ���
	enumIO_createInfoError,			// CreateInfo����
	enumIO_noDestoryError,			// ��������
	enumIO_equipableItemError,		// ��װ��(Ӧ����װ����δ��װ��)
	enumIO_notFindBag,				// δ�ҵ�ָ������
	enumIO_noGm,					// δGM����GM��Ʒ
	enumIO_noUsableItem,			// ��UsableItem��Ʒ
	enumIO_setItemError,			// ��Ʒ�����д�
	
	enumIO_positionNotEmpty,		// ����λ�÷ǿ�
	enumIO_positionEmpty,			// ����λ�ÿ�
	
	enumIO_bagNotEnough,			// �����ռ䲻��
	enumIO_bagNotEnoughOther,		// �Է������ռ䲻��
	enumIO_forbidOperatorEquip,		// ��ֹ����װ������
	enumIO_addItem2BagCheck,		// �������ǰ���ʧ��

	enumIO_noDropError,
	enumIO_spit,					// ��ֳ���
	eunmIO_noLockItemIDOfBag,		// û���ܽ�����������Ʒ(Ŀǰ�ǽ���ʹ��)
	enumIO_noStall,					// ���ɰ�̯
	enumIO_beyondMaxKeep,			// ����MaxKeep����
	
	enumIO_CancelSucess,			// ȡ���ɹ�
	enumIO_CancelFail,				// ȡ��ʧ��
	eunuIO_costTooBig,				// �۳�����
	
	enumIM_count
};

/**
** tooltip����
**/
enum ENUM_ITEM_TOOLTIP_TYPE
{
	enumITT_myself,			// �Լ���
	enumITT_store,			// �̵��
	enumITT_otherPlayer,	// ���˵�
};

/**
** ������Ʒʱ�Ŀ۳�����
**/
enum eItem_CostType
{
	enumCT_None = 0,	// �޲���
	enumCT_Frequency,	// ��������
	enumCT_Number,		// ��������
	
	enumCT_Count
};

/**
** ����������
**/
enum LOCK_TYPE
{
	enumLT_stall,		// ��̯��
};

enum ITEM_EVENT_TYPE
{
	eItemEvent_Type_Start = 0,
	eItemEvent_Type_DisappearByGameTimeUpdate,	// ��Ʒ��ʱ��ʧBy��ʵʱ�䣬������ʵʱ���ж�
	eItemEvent_Type_DisappearByRealTimeUpdate,	// ��Ʒ��ʱ��ʧBy��Ϸʱ�䣬���ߺ󲻼���
	eItemEvent_Type_End,
};

#define MAIL_CONVERT_TO_ID				(40000)		// �ʼ���ƷID

#define BAG_NORMAL_CELL_MAXCOUNT		(60)		// ��ͨ�������ӵ��������
#define BAG_QUEST_CELL_MAXCOUNT			(60)		// ���񱳰����ӵ��������
// #define BAG_MAT_CELL_MAXCOUNT		(300)		// ���ã������Ѷ���
#define BAG_BATTLEPET_MAT_MAXCOUNT		(50)		// ������ϱ������ӵ��������
#define BANK_MAT_MAXCOUNT				(60)
#define BANK_NORMAL_MAXCOUNT			(60)

// �����չ��������
#define MAX_SUB_PACKAGE					3
#define MAX_BANK_PAGE					3
#define	MAX_ITEM_CREATE_COUNT			65535

// �����ֿ�λ�ú����
#define	ITEM_BANK_FLAG					(enumBlock_Bank << 15)
#define	IS_IN_INVENTORY(pos)			((pos & ITEM_BANK_FLAG) == 0)
#define	IS_IN_BANK(pos)					((pos & ITEM_BANK_FLAG) != 0)
#define	SET_ITEM_POS(pos, isbank)		( isbank ? (pos | ITEM_BANK_FLAG) : pos)
#define	GET_ITEM_POS(pos)				( pos & ~ITEM_BANK_FLAG )

// ������С����ʼλ��
// ��������С 7*7
#define	MAIN_PACKAGE_WIDTH				7
#define	MAIN_PACKAGE_HEIGHT				7
// ����������ʼ��Ʒ���
#define	MAIN_PACKAGE_FIRST_ITEM			0
#define	MAIN_PACKAGE_LAST_ITEM			(MAIN_PACKAGE_WIDTH * MAIN_PACKAGE_HEIGHT - 1)
// ��չ������С
#define	SUB_PACKAGE_WIDTH				4
#define	SUB_PACKAGE_HEIGHT				8
// ����չ������Ʒ����ʼ���
#define	SUB_PACKAGE_1ST_FIRST_ITEM		(MAIN_PACKAGE_LAST_ITEM + 1)
#define	SUB_PACKAGE_1ST_LAST_ITEM		(SUB_PACKAGE_1ST_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
#define	SUB_PACKAGE_2ND_FIRST_ITEM		(SUB_PACKAGE_1ST_LAST_ITEM + 1)
#define	SUB_PACKAGE_2ND_LAST_ITEM		(SUB_PACKAGE_2ND_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
#define	SUB_PACKAGE_3RD_FIRST_ITEM		(SUB_PACKAGE_2ND_LAST_ITEM + 1)
#define	SUB_PACKAGE_3RD_LAST_ITEM		(SUB_PACKAGE_3RD_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
// װ����

#define EQUIPMENT_LAST_ITEM			(enum_EQUIPMENT_COUNT - 1)

#define IsEquipmentPos(pos , byPosType)		(pos >= EQUIPMENT_FIRST_ITEM && pos <= EQUIPMENT_LAST_ITEM && byPosType == enumBlock_BagEquip)

// ���д�С����ʼλ��
#define	BANK_WIDTH						8
#define	BANK_HEIGHT						8
#define	BANK_MAX_ITEM					BANK_WIDTH * BANK_HEIGHT * MAX_BANK_PAGE
// ������Ʒ����ʼ���
#define BANK_FIRST_ITEM					0
#define BANK_LAST_ITEM					(BANK_MAX_ITEM - 1)
#define	BANK_1ST_FIRST_ITEM				0
#define	BANK_1ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 1 - 1)
#define	BANK_2ST_FIRST_ITEM				(BANK_1ST_LAST_ITEM + 1)
#define	BANK_2ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 2 - 1)
#define	BANK_3ST_FIRST_ITEM				(BANK_2ST_LAST_ITEM + 1)
#define	BANK_3ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 3 - 1)

// ��Ʒ������ʻ���
#define ITEM_DROP_PROBABILITY_BASE		1000000


// ������Ϣ

#define ITEMGEAR_TAB_FILENAME		"item/GearInfo.txt"			// �ͻ��˷���������Ʒ���������ļ�
#define ITEM_TAB_FILENAME			"item/ItemInfo.txt"			// �ͻ��˷���������Ʒ���������ļ�
#define ITEM_MATERIAL_TAB_FILENAME	"item/ItemMaterial.txt"		// �ͻ��˷���������Ʒ���������ļ�
#define ITEMGEM_TAB_FILENAME		"item/GemInfo.txt"			// �ͻ��˷���������Ʒ��ʯ�����ļ�
#define ITEMHAMMER_TAB_FILENAME		"item/HammerInfo.txt"		// �ͻ��˷���������Ʒ��ͷ�����ļ�
#define ITE_BLUEPRINT_TAB_FILENAME	"Building/TuzhiInfo.txt"	// �ͻ��˷���������Ʒͼֽ�����ļ�
#define ITEM_MILL_DEVICE_TAB_FILENAME	"Building/ZhuangZhiInfo.txt"	// �ͻ��˷���������Ʒͼֽ�����ļ�
#define ITEM_PETBAG_TAB_FILENAME	"item/PetBag.txt"			// �ͻ��˷������˳��ﱳ�������ļ�

#define ITEM_PETEGG_TAB_FILENAME	"item/PetEggInfo.txt"			// ���ﵰ�����ļ�
#define ITEM_SKILLBOOK_TAB_FILENAME	"item/PetSkillBook.txt"		// �����������ļ�
#define ITEM_SKILLSTAR_TAB_FILENAME	"item/PetSkillStar.txt"		// �����������ļ�

#define ITEM_TAB_DISPLAY_FILENAME	"item/ItemDisplayInfo.txt"	// �ͻ�����Ʒ��ʾ�����ļ�
#define DROP_GROUP_TAB_FILENAME		"item/DropGroup.txt"		// ����������Ʒ�������ñ�
#define STORE_TAB_FILENAME			"item/StateStores.txt"		// ��������npc������Ʒ���ñ�

#define ITEM_TAB_VIPITEM_FILENAME	"item/VIPCreateInfo.txt"	// �ͻ�����Ʒ��ʾ�����ļ�

#define ITEM_CYCEVENT_FILENAME		"item/CycEvent.txt"			// ��Ʒ�ض��¼����ñ�
#define ITEM_ROLL_LIST_FILENAME		"item/Roll.txt"				// �����Щ�ȼ�����ЩItem��Ҫroll���ļ�

#define ITEM_COOK_MEAT_TAB_FILENAME	"cook/Meat.txt"				// ����ʳ�������ļ�
#define ITEM_COOK_INGREDIENT_TAB_FILENAME "cook/Ingredient.txt"	// �������������ļ�
#define ITEM_COOK_FOOD_TAB_FILENAME	"cook/Food.txt"				// ������������ļ�

#define	ITEM_PICK_PROTECTED_TIME	60 * 1000
#define	ITEM_DISAPPEAR_TIME			61 * 1000
#define	ITEM_MAX_PICK_DISTANCE		36	// ���ʰȡ�����ƽ����


#define	ITEM_STRENGTHEN_MAX_LEVEL	10
#define	ITEM_STRENGTHEN_MAX_MAGIC	1
#define ITEM_INVALID_ID				0
#define ITEM_INVALID_POS			(WORD)-1
#define ITEM_INVALID_POSTYPE		(WORD)-1
#define ITEM_INVALID_ITEMNUM		0
#define ITEM_MAX_ITEMNUM			(WORD)-1
#define	ITEM_MAX_REPURCHASE_COUNT	8	// ��ҹ�����Ʒ�������

#define ITEM_MAX_CHAIN_STORE		1024
#define ITEM_MAX_STORE_ITEM			0x10000
#define ITEM_INVALID_STORE_COUNT	(WORD)(-1)
#define ITEM_STORE_COUNT_PER_PAGE	10				// �̵�ÿҳ��10����Ʒ



enum enum_Strengthen_Posision
{
	enum_STRENGTHEN_FIRST = 0,
	enum_STRENGTHEN_EQUIP = enum_STRENGTHEN_FIRST,	// ����װ��
	enum_STRENGTHEN_MAGIC,							// Ԫ��	ǿ��ʯ
	enum_STRENGTHEN_SPECIAL_EX1,					// ��1(���ʷ�)
	enum_STRENGTHEN_SPECIAL,						// ����3(���˷�)
	enum_STRENGTHEN_COUNT,
};

//enum enum_Strengthen_Command
//{
//	enum_STRENGTHEN_PUTON = 1,			// ����Ʒ
//	enum_STRENGTHEN_DOIT,				// ǿ��
//	enum_STRENGTHEN_CANCEL,				// ȡ��ǿ��
//};

enum enum_Strengthen_Result
{
	enum_STRENGTHEN_ERROR,
	enum_STRENGTHEN_SUCCESS,		// ǿ���ɹ�
	enum_STRENGTHEN_FAILED,			// ǿ��ʧ��
	enum_STRENGTHEN_FAILED_EQUIP_DESTROY,			// ǿ��ʧ��װ��δ����
	enum_STRENGTHEN_FAILED_EQUIP_NOT_DESTROY,		// ǿ��ʧ��װ��δ����
	enum_STRENGTHEN_PUTONOK,		// ������Ʒ�ɹ�
	enum_STRENGTHEN_PUTOUTOK,		// �ó���Ʒ�ɹ�
	enum_STRENGTHEN_CANCEL,			// ȡ��ǿ��
	enum_STRENGTHEN_NOMONEY,		// ��Ǯ����
	enum_STRENGTHEN_NOEQUIP,		// �����װ��*
	enum_STRENGTHEN_NOTEQUIP,		// ����Ʒ���ɱ�ǿ��
	enum_STRENGTHEN_NOMAGIC,		// �����ħ��
	enum_STRENGTHEN_NOTMAGIC,		// ���ڴ˴�����ħ��
	enum_STRENGTHEN_NOTSPECIAL,		// ��������˷�
	enum_STRENGTHEN_NOTSPECIAL_EX,	// �������ʷ�
	enum_STRENGTHEN_STRNOPLACE,		// ǿ������λ������Ʒ�����޷�����
	enum_STRENGTHEN_BAGNOPLACE,		// װ������λ������Ʒ�����޷�ȡ��
	enum_STRENGTHEN_EQUIP_FIRST,	// ���ȷ���װ��
	enum_STRENGTHEN_MAGIC_TYPE,		// ħ�����Ͳ���ȷ
	enum_STRENGTHEN_MAGIC_LEVEL,	// ħ��ǿ���ȼ�����
	enum_STRENGTHEN_SUCCESS_SLOT,	// ǿ���ɹ� �ȼ�n ��ò�۽���
	enum_STRENGTHEN_FULL,			// ǿ����λ����
	enum_STRENGTHEN_LEVEL,			// �Ѿ��ﵽǿ���ȼ�����
	enum_STRENGTHEN_EX_LEVEL,		// ���ʷ��ȼ�����
	enum_STRENGTHEN_EX2_LEVEL,		// �������ȼ�����
	enum_STR_ERR_QUALITY,			// ʯͷ��Ʒ�ʲ��ܵ���װ����Ʒ��
	enum_STRENGTHEN_CANNOTSPECIAL,	// ���ܷ��ñ�����

	enum_STRENGTHEN_PUT_SUCCEED,	// ǿ�������óɹ�
	enum_STRENGTHEN_GET_SUCCEED,	// ǿ����ȡ�سɹ�
	enum_STR_ERR_PUTON_EQUIP_FIRST,	// �ȷ���װ��

	enum_STR_ERR_MAXLEVEL = -1000,	// �Ѿ��ﵽǿ����ߵȼ�
	enum_STR_ERR_NOSTONE,			// û��ǿ��ʯ
	enum_STR_ERR_NOEQUIP,			// û��װ��
	enum_STR_ERR_LEVEL,				// Ԫ���ȼ�������װ����ƥ��
	enum_STR_ERR_LEVELEX,			// ���ʷ��ȼ�������װ����ƥ��
	enum_STR_ERR_LEVELEX2,			// �������ȼ�������װ����ƥ��
};

enum enum_Inlay_Position_Type
{
	enum_INLAY_EQUIP = 0,
	enum_INLAY_SLOT1,
	enum_INLAY_SLOT2,
	enum_INLAY_SLOT3,
	enum_INLAY_POLYCHROME_SLOT,
	enum_INLAY_COUNT,
};

enum enum_Inlay_Result_Type
{
	enum_INLAY_SUCCESS = 1,				// ��Ƕ�ɹ�
	enum_INLAY_FAILED,					// ��Ƕʧ��
	enum_INLAY_NOT_EQUIP,				// ����װ��
	enum_INLAY_NOT_GEM,					// ���Ǳ�ʯ
	enum_INLAY_NOT_POLYCHROME_GEM,		// ���Ƕ�ʱ�ʯ
	enum_INLAY_NO_EQUIP,				// �����װ��
	enum_INLAY_NO_GEM,					// ����뱦ʯ
	enum_INLAY_NO_POLYCHROME_GEM,		// ������ʱ�ʯ
	enum_INLAY_GEM_LV_QUALITY_TOO_LOW,	// ��ʯ�ȼ�����Ʒ�ʵ���װ��
	enum_INLAY_NO_MONEY,				// ûǮ
	enum_INLAY_SLOT_NOT_EXIST,			// �����ڲ��
	enum_INLAY_GEM_EXIST,				// ��ʯ�Ѿ�����

	enum_INLAY_PUT_SUCCEED,				// ��Ƕ�����óɹ�
	enum_INLAY_PUT_EQUIP_SUCCEED,		// ��Ƕ������װ���ɹ�
	enum_INLAY_GET_SUCCEED,				// ��Ƕ��ȡ�سɹ�
	enum_INLAY_CANCEL_SUCCEED,
};

enum enum_Fanshion_Inlay_Position_Type
{
	enum_Fanshion_INLAY_EQUIP = 0,
	enum_Fanshion_INLAY_SLOT1,
	enum_Fanshion_INLAY_SLOT2,
	enum_Fanshion_INLAY_SLOT3,
	enum_Fanshion_INLAY_POLYCHROME_SLOT,
	enum_Fanshion_INLAY_COUNT,
};

enum enum_Fanshion_Inlay_Result_Type
{
	enum_Fanshion_INLAY_SUCCESS = 1,			// ��Ƕ�ɹ�
	enum_Fanshion_INLAY_FAILED,					// ��Ƕʧ��
	enum_Fanshion_INLAY_NOT_EQUIP,				// ����װ��
	enum_Fanshion_INLAY_NOT_GEM,				// ���Ǳ�ʯ
	enum_Fanshion_INLAY_NOT_POLYCHROME_GEM,		// ���Ƕ�ʱ�ʯ
	enum_Fanshion_INLAY_NO_EQUIP,				// �����װ��
	enum_Fanshion_INLAY_NO_GEM,					// ����뱦ʯ
	enum_Fanshion_INLAY_NO_POLYCHROME_GEM,		// ������ʱ�ʯ
	enum_Fanshion_INLAY_GEM_LV_QUALITY_TOO_LOW,	// ��ʯ�ȼ�����Ʒ�ʵ���װ��
	enum_Fanshion_INLAY_NO_MONEY,				// ûǮ
	enum_Fanshion_INLAY_SLOT_NOT_EXIST,			// �����ڲ��
	enum_Fanshion_INLAY_GEM_EXIST,				// ��ʯ�Ѿ�����

	enum_Fanshion_INLAY_PUT_SUCCEED,			// ��Ƕ�����óɹ�
	enum_Fanshion_INLAY_PUT_EQUIP_SUCCEED,		// ��Ƕ������װ���ɹ�
	enum_Fanshion_INLAY_GET_SUCCEED,			// ��Ƕ��ȡ�سɹ�
	enum_Fanshion_INLAY_CANCEL_SUCCEED,
};

enum enum_Rabbet_Position_Type
{
	enum_Rabbet_Pos_Equip = 0,
	enum_Rabbet_Pos_Fu,

	enum_Rabbet_Pos_Count,
};

enum enum_Fanshion_Rabbet_Position_Type
{
	enum_Fanshion_Rabbet_Pos_Equip = 0,
	enum_Fanshion_Rabbet_Pos_Fu,

	enum_Fanshion_Rabbet_Pos_Count,
};

enum enum_Rabbet_Result_Type
{
	enum_Rabbet_Error = 0,
	enum_Rabbet_Succee,
	enum_Rabbet_Not_Equip,				// ����װ��
	enum_Rabbet_Not_Fu,					// ���ǿ��۾�
	enum_Rabbet_NO_Equip,				// �����װ��
	enum_Rabbet_NO_Fu,					// ����뿪�۾�
	enum_Rabbet_NO_Money,				// ûǮ
	enum_Rabbet_Slot_Exist,				// ���ѿ�

	enum_Rabbet_Put_Succee,				// ���������óɹ�
	enum_Rabbet_Get_Succee,				// ������ȡ�سɹ�
	enum_Rabbet_Cancel_Succee,			// ������ȡ���ɹ�
};

enum enum_Fanshion_Rabbet_Result_Type
{
	enum_Fanshion_Rabbet_Error = 0,
	enum_Fanshion_Rabbet_Succee,
	enum_Fanshion_Rabbet_Not_Equip,				// ����װ��
	enum_Fanshion_Rabbet_Not_Fu,				// ���ǿ��۾�
	enum_Fanshion_Rabbet_NO_Equip,				// �����װ��
	enum_Fanshion_Rabbet_NO_Fu,					// ����뿪�۾�
	enum_Fanshion_Rabbet_NO_Money,				// ûǮ
	enum_Fanshion_Rabbet_Slot_Exist,			// ���ѿ�

	enum_Fanshion_Rabbet_Put_Succee,			// ���������óɹ�
	enum_Fanshion_Rabbet_Get_Succee,			// ������ȡ�سɹ�
	enum_Fanshion_Rabbet_Cancel_Succee,			// ������ȡ���ɹ�
};

enum enum_Break_Position_Type
{
	enum_Break_Pos_Equip = 0,
	enum_Break_Pos_Fu,

	enum_Break_Pos_Count,
};

enum enum_Break_Result_Type
{
	enum_Break_Error = 0,
	enum_Break_Succee,
	enum_Break_Not_Equip,				// ����װ��
	enum_Break_Slot_Not_Exist,			// δ����
	enum_Break_Not_Fu,					// �������۾�
	enum_Break_NO_Equip,				// �����װ��
	enum_Break_NO_Fu,					// ��������۾�
	enum_Break_NO_Money,				// ûǮ
	enum_Break_Equip_No_Gem,			// װ������Ҫ����

	enum_Break_Put_Succee,				// ���������óɹ�
	enum_Break_Get_Succee,				// ������ȡ�سɹ�
	enum_Break_Cancel_Succee,
};

enum enum_Spar_Position_Type
{
	enum_Spar_Pos_Gem_1 = 0,
	enum_Spar_Pos_Gem_2,
	enum_Spar_Pos_Gem_3,
	enum_Spar_Pos_Gem_4,
	enum_Spar_Pos_Gem_5,
	enum_Spar_Pos_Special,
	enum_Spar_Pos_Result,

	enum_Spar_Pos_Count,
};

enum enum_Spar_Result_Type
{
	enum_Spar_Error = 0,
	enum_Spar_Succee,
	enum_Spar_Not_Gem,				// ���Ǳ�ʯ
	enum_Spar_Not_Special,			// ���Ǽ��ʷ�
	enum_Spar_Not_Role1,			// �����Ϲ���1
	enum_Spar_Not_Role2,			// �����Ϲ���2
	enum_Spar_Not_Role3,			// �����Ϲ���3
	enum_Spar_Not_Role4,			// �����Ϲ���4
	enum_Spar_Not_Role5,			// �����Ϲ���5
	enum_Spar_Not_Role6,			// �����Ϲ���6
	enum_Spar_NO_Money,				// ûǮ

	enum_Spar_Put_Succee,			// ��ʯ�ϳ������óɹ�
	enum_Spar_Get_Succee,			// ��ʯ�ϳ���ȡ�سɹ�
	enum_Spar_Cancel_Succee,
};

enum enum_Compose_Result_Type
{
	enum_Compose_Error = 0,
	enum_Compose_Succee,
	enum_Compose_Short,
	enum_Compose_Put_Succee,		// װ���ϳ������óɹ�
	enum_Compose_Get_Succee,		// װ���ϳ���ȡ�سɹ�
	enum_Compose_Clear_Succee,		// �Ż�������Ʒ�ɹ�
	enum_Compose_Cancel_Succee,		// ȡ���ϳɳɹ�
	enum_Compose_Show_UI,			// ��ʾUI����
	enum_Compose_ComposeDo_Error,	// �ϳ�ʧ��
};

enum enum_Compose_Position_Type
{
	enum_Compose_Pos_1 = 0,
	enum_Compose_Pos_2,
	enum_Compose_Pos_3,
	enum_Compose_Pos_4,
	enum_Compose_Pos_Special,
	enum_Compose_Pos_Count,
};

enum enum_Repair_Position_Type
{
	enum_Repair_Pos_Equip = 0,		// װ��
	enum_Repair_Pos_RepairStone,	// �޲�ʯ

	enum_Repair_Pos_Count,
};

enum enum_Repair_Result_Type
{
	enum_Repair_Error = 0,
	enum_Repair_Succee,
	enum_Repair_Not_Equip,				// ����װ��
	enum_Repair_Not_RepairStone,		// �����޲�ʯ
	enum_Repair_NO_Equip,				// �����װ��
	enum_Repair_NO_RepairStone,			// ������޲�ʯ
	enum_Repair_NO_Money,				// ûǮ
	enum_Repair_CANNOT_Repair,			// ��������
	enum_Repair_DuraMax,				// ��������
	enum_Repair_RepairStone_Lv,			// �޲�ʯ�ȼ�����װ��
	enum_Repair_RepairStone_Qu,			// �޲�ʯƷ�ʲ�����װ��
	enum_Repair_Equip_IMM,				// ����װ�����ܱ���ͨ����

	enum_Repair_Put_Succee,				// ���������óɹ�
	enum_Repair_Get_Succee,				// ������ȡ�سɹ�
	enum_Repair_Cancel_Succee,			// ������ȡ���ɹ�
};

enum enum_MBStrengthen_Position_Type
{
	enum_MBStrengthen_Pos_Stone = 0,	// �ڻ����
	enum_MBStrengthen_Pos_Count,
};

enum enum_MBStrengthen_Result_Type
{
	enum_MBStrengthen_Error = 0,
	enum_MBStrengthen_Succee,
	enum_MBStrengthen_Not_CurrMB,
	enum_MBStrengthen_Not_Stone,
	enum_MBStrengthen_No_Stone,
	enum_MBStrengthen_No_Money,
	enum_MBStrengthen_Level_Error,
	enum_MBStrengthen_FLevel_Error,
	enum_MBStrengthen_Faith_Disable,
	enum_MBStrengthen_No_Element1,
	enum_MBStrengthen_No_Element2,
	enum_MBStrengthen_No_Element3,
	enum_MBStrengthen_No_Element4,
	enum_MBStrengthen_No_Element5,

	enum_MBStrengthen_Put_Succee,		// ���������óɹ�
	enum_MBStrengthen_Get_Succee,		// ������ȡ�سɹ�
	enum_MBStrengthen_Cancel_Succee,	// ������ȡ���ɹ�
};

enum enum_Store_Operator
{
	esOpenStore = 1,
	esCloseStore,
	esGetItemList,
	esBuyItem,
	esSellItem,
	esRepurchaseItem,
	esRepairItem,
	esRepairAll,
	esSpeicalRepair,
	esIncreaseAttack,
	esIncreaseDefence,
	esBuyItemReq,				// ������Ʒ����
	esSellItemReq,				// ������Ʒ����
	
	esClientUpdatePageUp = 100,
	esClientUpdatePageDown,
	esClientSellRep,			// ����Ʒȷ��
	esLuaBuyReq,				// ����Ʒ����
	esLuaSellReq,				// ����Ʒ����
	esLuaAutoSellReq,			// �Զ������� Quality = 1
	esLuaBuyMultiReq,			// ��������
	esIsGuildStore,				// �Ƿ��ǰ����̵�
	esIsHonourStore,			// �Ƿ��������̵�
	esIsMaterialStore,			// �Ƿ��������̵�
	esIsQuestBuyStore,			// �Ƿ�������ֻ���̵�
	esIsQuestSellStore,			// �Ƿ�������ֻ���̵�
	esIsAffectionStore,			// �Ƿ��Ƕ���ֵ�̵�
	//����֮�𾧾���
	esGetMobileTradeItems,		//�𾧾�����Ʒ�б�
};

enum enum_Store_Result
{
	esSuccess			= 1,// �����ɹ�
	esbFailed			,	// ϵͳ���󣬹���ʧ��
	esbNotEnoughMoney	,	// ��Ǯ���㣬����ʧ��
	esbBagFull			,	// ������λ���㣬����ʧ��
	esbNotEnoughAttribute	,	// ���׶Ȳ��㣬����ʧ��
	esbNotEnoughHonor,
	esbNotEnoughSilver,		// û���㹻����Ʊ
	esbInOnlySellStore	,	// ��ֻ���̵��������
	esbLackOfItem		,	// ������Ʒ�������㣬����ʧ��   ������Ϣ
	essFailed			,	// ����Ʒʧ��
	essNoSold,				// ���ɳ���
	esbNotEnoughMaterial,	// û���㹻����Ʒ
	essNotEnoughSilverPlace,	// û���㹻�Ŀռ������Ʊ���޷�����
	essInOnlyBuyStore	,	// ��ֻ���̵��������
	esbNotEnoughAffection,	// ����ֵ����, �޷�����
	essStoreErrBase = 300,	// �̵�����������Ϣ��׼���
};

enum enum_Pick_Item_Result
{
	epSuccess			= 1,// ��ʰȡ��%s
	epFailed			,	// �޷�ʰȡ����Ʒ
	epBagFull			,	// �����������޷�ʰȡ��Ʒ
	epNotYours			,	// �����Ʒ��������
	epTooFar			,	// �����Զ
	epNoItem			,	// ��Ʒ������
	epOwnerNotOnline	,	// �����߲�����
	epDeath				,	// ����״̬
	epStateErr			,	// ��ǰ״̬�޷�ʰȡ
};


enum enum_Delete_Item_Reason
{
	edNoReason			= 0,	// ϵͳԭ��
	epTimeUpdate,				// ��Ʒ����ʱ�䵽
	epItemTrade,				// ��Ʒ���׳�ȥ��
	epItemDiceWinner,			// Ͷ�����ӻ�ʤ
};


enum enum_BindType
{
	eBT_NoBind			= 0,	// ���󶨣� ���ܱ��̳ǳ������
	eBT_PickBind		= 1,	// ʰȡ��
	eBT_UseBind			= 2,	// ʹ�ð�
	eBT_EquipBind		= 3,	// װ����
	eBT_NeverBind		= 4,	// ��Զ����
	eBT_MBBind			= 5,	// ��Զ����
};


enum enum_VanishType
{
	eVT_NeverDisappear	= 0,	// ���޴�ʹ��
	eVT_Disappear		= 1,	// ʹ�ô���Ϊ��ʱ��ʧ
	eVT_NoDisappear		= 2,	// ʹ�ô���Ϊ��ʱ����ʧ	
	
};

//�������ʵĸ�����Ʒ��������
enum enum_ItemOperator_ByItemFlags
{
	eItemEvent_DestoryByDead = 1,	//������ʧ
	eItemEvent_DropByDead = 2,		//��������
	eItemEvent_DestoryByLogin = 4,	//����ʱ��ʧ
	eItemEvent_DestoryByLoginMoreThen15 =8,	//����ʱ(����ʱ�䳬��15����)��ʧ
	eItemEvent_DestoryByLogout =16,	//�ǳ�����
	eItemEvent_NoDrop = 32,			//���ɶ���
	eItemEvent_NoDestory = 64,		//��������
	eItemEvent_NoSave2Bank = 128,	//���ɴ���ڲֿ�
	eItemEvent_NoSell = 256,		//���ɳ���
	eItemEvent_NoTradeWhenCD = 512,	//δ��ȴʱ�ɽ���(Ĭ��Ϊ���ɽ���)
	eItemEvent_EquipOnlyOne = 1024,	//װ��Ψһ(��ָ������)
	eItemEvent_IsQuestItem = 2048,	//���������
	eItemEvent_TempShortcut = 4096,	//��ʾ����ʱ�������
	eItemEvent_IBTips		= 8192,	//IB������ʾ
	eItemEvent_TalentLearn  = 16384,//�츳ѧϰ����

};

// ��Ʒ�����Ϊ�ᴥ������ű�����
enum enum_OpenQuestWithItemOperator
{
	eOpenQuest_None = 0x0,					// �����񴥷�
	eOpenQuest_ClientClickItem = 0x01,		// �ͻ���ʹ��ʱ����
	eOpenQuest_ServerPickUpItem = 0x02,		// ��������ʰȡ�󴥷�
	eOpenQuest_ServerUsedItem = 0x04,		// ������ʹ�ú󴥷�
	eOpenQuest_ServerDestoryItem = 0x08,	// ���������ٺ󴥷�
};

//enum enum_ItemBlockType
//{
//	eItemBlockType_None		= 0,
//	eItemBlockType_NormalBag,
//	eItemBlockType_NormalQuest,
//	eItemBlockType_NormalMat,
//	eItemBlockType_SpecialBag,
//	eItemBlockType_Count,
//};

enum enum_ItemDura_OP_TargetType
{
	enumItemDOPT_Begin			= 0x00,
	enumItemDOPT_All_Item		= 0x00,	// ������Ʒ
	enumItemDOPT_Equip_Weapon	= 0x01,	// װ��������
	enumItemDOPT_Equip_Helm		= 0x02,	// װ����ͷ��
	enumItemDOPT_Equip_Coat		= 0x04,	// װ�����·�
	enumItemDOPT_Equip_Belt		= 0x08,	// װ��������
	enumItemDOPT_Equip_Cuff		= 0x10,	// װ��������
	enumItemDOPT_Equip_Boot		= 0x20,	// װ����Ь
	enumItemDOPT_Bag_All_Item	= 0x40,	// ����������Ʒ
	enumItemDOPT_End			= 0x40,
};

enum enum_ItemDura_OP_Reason
{
	enumItemDOPR_None = 0,
	enumItemDOPR_KillByMonster,		// ��npcɱ��
	enumItemDOPR_Normal_Attack,		// ����ͨ����
	enumItemDOPR_Deadlyl_Attack,	// ������һ��
	enumItemDOPR_Critical_Attack,	// ������һ��
};


enum enum_Item_Quality_Level
{
	enum_Item_Quality_Invalid,	// ��Ч
	enum_Item_Quality_Grey,		// ��ɫ
	enum_Item_Quality_White,	// ��ɫ
	enum_Item_Quality_Green,	// ��ɫ
	enum_Item_Quality_Blue,		// ��ɫ
	enum_Item_Quality_Purple,	// ��ɫ
	enum_Item_Quality_Golden,	// ��ɫ
};


// ��Ʒ������־
enum enum_Item_Operator_Type
{
	E_Item_Operator_Unknown = 0,					// δ֪����
	E_Item_Operator_FirstLogin = 1,					// �״ε�½��־
	E_Item_Operator_Init = 2,						// ��ʼ����־
	E_Item_Operator_Trade = 3,						// ��ҽ���
	E_Item_Operator_MovableShop = 4,				// ��Ұ�̯
	E_Item_Operator_RealTimeOverEvent = 5,			// ��ʵʱ�䵽��
	E_Item_Operator_GameTimeOverEvent = 6,			// ��Ϸʱ�䵽��
	E_Item_Operator_Pick = 7,						// ʰȡ
	E_Item_Operator_Destory = 8,					// ����
	E_Item_Operator_Quest = 9,						// ����
	E_Item_Operator_Repair = 10,					// ����
	E_Item_Operator_Split = 11,						// ���
	E_Item_Operator_Billing = 12,					// �̳�
	E_Item_Operator_PickAndBind = 13,				// �����
	E_Item_Operator_MailAttach = 14,				// �ʼ�ϵͳ
	E_Item_Operator_VIPCyberCafe = 15,				// VIP����
	E_Item_Operator_CAST_SKILL = 16,				// ʹ�ü���
	E_Item_Operator_MoveItem = 17,					// �ƶ�����
	E_Item_Operator_UserDelete = 18,				// �û�����ɾ��(����)
	E_Item_Operator_NestSacrifice = 19,				// ��Ѩ�׼�
	E_Item_Operator_GemStudded = 20,				// ��ʯ��Ƕ
	E_Item_Operator_GemMerge = 21,					// ��ʯ�ϳ�
	E_Item_Operator_GemCarve = 22,					// ��ʯ���
	E_Item_Operator_GemDismount = 23,				// ��ʯ���
	E_Item_Operator_CatchNpc = 24,					// ץ��NPC
	E_Item_Operator_ConvertTo_Mail = 25,			// ��ͨ�ʼ�ת��Ʒ
	E_Item_Operator_GetAttachment = 26,				// ��ȡ����
	E_Item_Operator_Business = 27,					// ����
	E_Item_Operator_Store = 28,						// �̵�
	E_Item_Operator_ExploreGive = 29,				// ̽��֮���轱��
	E_Item_Operator_MakeBlueprint = 30,				// ����ͼֽ����
	E_Item_Operator_Stall = 31,						// ��̯
	E_Item_Operator_Hatch = 32,						// ����
	E_Item_Operator_StallOffline = 33,				// ���߰�̯
	E_Item_Operator_MillDevice = 34,				// ����װ����Ʒ
	E_Item_Operator_PetBag = 35,					// ���ﱳ����Ʒ�û�
	E_Item_Operator_TribeSystem = 36,				// ����ϵͳ
	E_Item_Operator_BpItem = 37,					// ���ﵰ
	E_Item_Operator_NpcDrop = 38,					// ɱ��npc����
	E_Item_Operator_License = 39,					// ����ִ��
	E_Item_Operator_PetSkill = 40,					// ���＼��
	E_Item_Operator_CampQuest = 41,					// Ӫ������
	E_Item_Operator_FeedPet = 42,					// ι�������淨
	E_Item_Operator_Salesroom = 43,					// ����������
	E_Item_Operator_Exploration = 44,				// ��̽����
	E_Item_Operator_BpLearnSkill = 45,				// ս��ѧϰ����
	E_Item_Operator_BpOpenSlotSkill = 46,			// ս�迪���ܲ�
	E_Item_Operator_MakeFood = 47,					// ��������
};


// ��Ǯ������־
enum enum_Money_Operator_Type
{
	E_Money_Operator_Unknown = 0,				// δ֪����
	//E_Money_Operator_FirstLogin,				// �״ε�½��־
	//E_Money_Operator_Init,						// ��ʼ����־
	//E_Money_Operator_ItemStore,					// Npc�̵�
	//E_Money_Operator_Trade,						// ��ҽ���
	//E_Money_Operator_MovableShop,				// ��Ұ�̯
	//E_Money_Operator_Strength,					// ǿ��
	//E_Money_Operator_Inlay,						// �ϳ�
	//E_Money_Operator_Rabbet,					// ����
	//E_Money_Operator_Break,						// ����ʯ
	//E_Money_Operator_Spar,						// ��Ƕ
	//E_Money_Operator_Relive,					// ����
	//E_Money_Operator_Quest,						// ����
	//E_Money_Operator_LearnSkill,				// ѧϰ����
	//E_Money_Operator_SkillAction,				// ����
	//E_Money_Operator_GMCmd,						// GMָ��
	//E_Money_Operator_Repair,					// ����
	//E_Money_Operator_Securicar,					// �ڳ�
	//E_Money_Operator_GuildCreate,				// ���ᴴ��
	//E_Money_Operator_Billing,					// �̳�
	//E_Money_Operator_Mail,						// �ʼ�ϵͳ
	//E_Money_Operator_Trans,						// ���Ϳ۳�
	//E_Money_Operator_MagicBabyFuse,				// �����ں�
	//E_Money_Operator_QuizDouble,				// �ʴ�ϵͳ(���ּӱ�����)
	//E_Money_Operator_ClearTalent,				// ����츳��۳�
	//E_Money_Operator_BillTradeCost,				// ���׵��۳��ɱ�
	//E_Money_Operator_BillTradeIncome,			// ���׵���������
	//E_Money_Operator_NpcTalk,					// NPC�Ի�
	//E_Money_Operator_NationBF,					// ��ս����
	//E_Money_Operator_GuildTalentSpeed,			// ���ٰ����츳����
	//E_Money_Operator_EatMagicBaby,				// ��������
	//E_Money_Operator_WorldChat,					// ���纰��
};

// �����������
enum enum_Cook_Food_Type
{
	E_Cook_Food_Normal,			// ��ͨ
	E_Cook_Food_Fancy,			// ���
};

#define CFG_MAX_VIP_ITEM_BY_LINE	40
