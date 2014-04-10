/********************************************************************
	created:	2008/05/12
	created:	12:5:2008   13:46
	filename: 	Athena\Main\Src\Common\LogicWorld\Src\Item\KItemDef.h
	file path:	Athena\Main\Src\Common\LogicWorld\Src\Item
	file base:	KItemDef
	file ext:	h
	author:		jungle
	
	purpose:	和物品相关的定义都放在这里
*********************************************************************/

#pragma once

#include "Inc/KCommonDefine.h"
#include "../KGlobalFunction.h"
// 错误返回值，这里暂时
//-1 物品数据读取失败
//-2 物品基础信息读取失败
//-3 状态检查
//-4 位置检查
//-5 级别检查
//-6 国籍检查
//-7 职业检查
//-8 根骨检查
//-9 灵力检查

enum eItem_Element_Type
{
	enumElement_None = 0,
	enumElement_Golden,		// 金1
	enumElement_Wood,		// 木2
	enumElement_Water,		// 水3
	enumElement_Fire,		// 火4
	enumElement_Earth,		// 土5
	enumElement_Poison,		// 毒6
	enumElement_Immortal,	// 仙7
	enumElement_Demon,		// 魔8
	enumElement_Count,
};

enum ITEM_OPERATOR
{
	enumIO_sucess = 0,				// 成功
	enumIO_failed,					// 通用错误
	enumIO_logicError,				// 不会出现的错误
	enumIO_createInfoError,			// CreateInfo错误
	enumIO_noDestoryError,			// 不可销毁
	enumIO_equipableItemError,		// 非装备(应该是装备但未是装备)
	enumIO_notFindBag,				// 未找到指定背包
	enumIO_noGm,					// 未GM操作GM物品
	enumIO_noUsableItem,			// 非UsableItem物品
	enumIO_setItemError,			// 物品配置有错
	
	enumIO_positionNotEmpty,		// 背包位置非空
	enumIO_positionEmpty,			// 背包位置空
	
	enumIO_bagNotEnough,			// 背包空间不足
	enumIO_bagNotEnoughOther,		// 对方背包空间不足
	enumIO_forbidOperatorEquip,		// 禁止操作装备背包
	enumIO_addItem2BagCheck,		// 背包添加前检测失败

	enumIO_noDropError,
	enumIO_spit,					// 拆分出错
	eunmIO_noLockItemIDOfBag,		// 没有能进背包的锁物品(目前是交易使用)
	enumIO_noStall,					// 不可摆摊
	enumIO_beyondMaxKeep,			// 超出MaxKeep设置
	
	enumIO_CancelSucess,			// 取消成功
	enumIO_CancelFail,				// 取消失败
	eunuIO_costTooBig,				// 扣除过多
	
	enumIM_count
};

/**
** tooltip类型
**/
enum ENUM_ITEM_TOOLTIP_TYPE
{
	enumITT_myself,			// 自己的
	enumITT_store,			// 商店的
	enumITT_otherPlayer,	// 别人的
};

/**
** 消耗物品时的扣除类型
**/
enum eItem_CostType
{
	enumCT_None = 0,	// 无操作
	enumCT_Frequency,	// 按次数扣
	enumCT_Number,		// 按数量扣
	
	enumCT_Count
};

/**
** 格子锁类型
**/
enum LOCK_TYPE
{
	enumLT_stall,		// 摆摊锁
};

enum ITEM_EVENT_TYPE
{
	eItemEvent_Type_Start = 0,
	eItemEvent_Type_DisappearByGameTimeUpdate,	// 物品定时消失By现实时间，根据真实时间判断
	eItemEvent_Type_DisappearByRealTimeUpdate,	// 物品定时消失By游戏时间，下线后不计算
	eItemEvent_Type_End,
};

#define MAIL_CONVERT_TO_ID				(40000)		// 邮件物品ID

#define BAG_NORMAL_CELL_MAXCOUNT		(60)		// 普通背包格子的最大数量
#define BAG_QUEST_CELL_MAXCOUNT			(60)		// 任务背包格子的最大数量
// #define BAG_MAT_CELL_MAXCOUNT		(300)		// 无用，此数已丢弃
#define BAG_BATTLEPET_MAT_MAXCOUNT		(50)		// 宠物材料背包格子的最大数量
#define BANK_MAT_MAXCOUNT				(60)
#define BANK_NORMAL_MAXCOUNT			(60)

// 最大扩展背包数量
#define MAX_SUB_PACKAGE					3
#define MAX_BANK_PAGE					3
#define	MAX_ITEM_CREATE_COUNT			65535

// 背包仓库位置宏操作
#define	ITEM_BANK_FLAG					(enumBlock_Bank << 15)
#define	IS_IN_INVENTORY(pos)			((pos & ITEM_BANK_FLAG) == 0)
#define	IS_IN_BANK(pos)					((pos & ITEM_BANK_FLAG) != 0)
#define	SET_ITEM_POS(pos, isbank)		( isbank ? (pos | ITEM_BANK_FLAG) : pos)
#define	GET_ITEM_POS(pos)				( pos & ~ITEM_BANK_FLAG )

// 背包大小、开始位置
// 主背包大小 7*7
#define	MAIN_PACKAGE_WIDTH				7
#define	MAIN_PACKAGE_HEIGHT				7
// 主背包的起始物品序号
#define	MAIN_PACKAGE_FIRST_ITEM			0
#define	MAIN_PACKAGE_LAST_ITEM			(MAIN_PACKAGE_WIDTH * MAIN_PACKAGE_HEIGHT - 1)
// 扩展背包大小
#define	SUB_PACKAGE_WIDTH				4
#define	SUB_PACKAGE_HEIGHT				8
// 各扩展背包物品的起始序号
#define	SUB_PACKAGE_1ST_FIRST_ITEM		(MAIN_PACKAGE_LAST_ITEM + 1)
#define	SUB_PACKAGE_1ST_LAST_ITEM		(SUB_PACKAGE_1ST_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
#define	SUB_PACKAGE_2ND_FIRST_ITEM		(SUB_PACKAGE_1ST_LAST_ITEM + 1)
#define	SUB_PACKAGE_2ND_LAST_ITEM		(SUB_PACKAGE_2ND_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
#define	SUB_PACKAGE_3RD_FIRST_ITEM		(SUB_PACKAGE_2ND_LAST_ITEM + 1)
#define	SUB_PACKAGE_3RD_LAST_ITEM		(SUB_PACKAGE_3RD_FIRST_ITEM + SUB_PACKAGE_WIDTH * SUB_PACKAGE_HEIGHT - 1)
// 装备栏

#define EQUIPMENT_LAST_ITEM			(enum_EQUIPMENT_COUNT - 1)

#define IsEquipmentPos(pos , byPosType)		(pos >= EQUIPMENT_FIRST_ITEM && pos <= EQUIPMENT_LAST_ITEM && byPosType == enumBlock_BagEquip)

// 银行大小、开始位置
#define	BANK_WIDTH						8
#define	BANK_HEIGHT						8
#define	BANK_MAX_ITEM					BANK_WIDTH * BANK_HEIGHT * MAX_BANK_PAGE
// 银行物品的起始序号
#define BANK_FIRST_ITEM					0
#define BANK_LAST_ITEM					(BANK_MAX_ITEM - 1)
#define	BANK_1ST_FIRST_ITEM				0
#define	BANK_1ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 1 - 1)
#define	BANK_2ST_FIRST_ITEM				(BANK_1ST_LAST_ITEM + 1)
#define	BANK_2ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 2 - 1)
#define	BANK_3ST_FIRST_ITEM				(BANK_2ST_LAST_ITEM + 1)
#define	BANK_3ST_LAST_ITEM				(BANK_WIDTH * BANK_HEIGHT * 3 - 1)

// 物品掉落概率基数
#define ITEM_DROP_PROBABILITY_BASE		1000000


// 配置信息

#define ITEMGEAR_TAB_FILENAME		"item/GearInfo.txt"			// 客户端服务器端物品共用数据文件
#define ITEM_TAB_FILENAME			"item/ItemInfo.txt"			// 客户端服务器端物品共用数据文件
#define ITEM_MATERIAL_TAB_FILENAME	"item/ItemMaterial.txt"		// 客户端服务器端物品共用数据文件
#define ITEMGEM_TAB_FILENAME		"item/GemInfo.txt"			// 客户端服务器端物品宝石数据文件
#define ITEMHAMMER_TAB_FILENAME		"item/HammerInfo.txt"		// 客户端服务器端物品榔头数据文件
#define ITE_BLUEPRINT_TAB_FILENAME	"Building/TuzhiInfo.txt"	// 客户端服务器端物品图纸数据文件
#define ITEM_MILL_DEVICE_TAB_FILENAME	"Building/ZhuangZhiInfo.txt"	// 客户端服务器端物品图纸数据文件
#define ITEM_PETBAG_TAB_FILENAME	"item/PetBag.txt"			// 客户端服务器端宠物背包数据文件

#define ITEM_PETEGG_TAB_FILENAME	"item/PetEggInfo.txt"			// 宠物蛋数据文件
#define ITEM_SKILLBOOK_TAB_FILENAME	"item/PetSkillBook.txt"		// 技能书数据文件
#define ITEM_SKILLSTAR_TAB_FILENAME	"item/PetSkillStar.txt"		// 技能星数据文件

#define ITEM_TAB_DISPLAY_FILENAME	"item/ItemDisplayInfo.txt"	// 客户端物品显示数据文件
#define DROP_GROUP_TAB_FILENAME		"item/DropGroup.txt"		// 服务器端物品掉落配置表
#define STORE_TAB_FILENAME			"item/StateStores.txt"		// 服务器端npc出售商品配置表

#define ITEM_TAB_VIPITEM_FILENAME	"item/VIPCreateInfo.txt"	// 客户端物品显示数据文件

#define ITEM_CYCEVENT_FILENAME		"item/CycEvent.txt"			// 物品特定事件配置表
#define ITEM_ROLL_LIST_FILENAME		"item/Roll.txt"				// 标记哪些等级的哪些Item需要roll的文件

#define ITEM_COOK_MEAT_TAB_FILENAME	"cook/Meat.txt"				// 料理食材数据文件
#define ITEM_COOK_INGREDIENT_TAB_FILENAME "cook/Ingredient.txt"	// 料理配料数据文件
#define ITEM_COOK_FOOD_TAB_FILENAME	"cook/Food.txt"				// 料理产物数据文件

#define	ITEM_PICK_PROTECTED_TIME	60 * 1000
#define	ITEM_DISAPPEAR_TIME			61 * 1000
#define	ITEM_MAX_PICK_DISTANCE		36	// 最大拾取距离的平方！


#define	ITEM_STRENGTHEN_MAX_LEVEL	10
#define	ITEM_STRENGTHEN_MAX_MAGIC	1
#define ITEM_INVALID_ID				0
#define ITEM_INVALID_POS			(WORD)-1
#define ITEM_INVALID_POSTYPE		(WORD)-1
#define ITEM_INVALID_ITEMNUM		0
#define ITEM_MAX_ITEMNUM			(WORD)-1
#define	ITEM_MAX_REPURCHASE_COUNT	8	// 玩家购回商品最大数量

#define ITEM_MAX_CHAIN_STORE		1024
#define ITEM_MAX_STORE_ITEM			0x10000
#define ITEM_INVALID_STORE_COUNT	(WORD)(-1)
#define ITEM_STORE_COUNT_PER_PAGE	10				// 商店每页有10个商品



enum enum_Strengthen_Posision
{
	enum_STRENGTHEN_FIRST = 0,
	enum_STRENGTHEN_EQUIP = enum_STRENGTHEN_FIRST,	// 武器装备
	enum_STRENGTHEN_MAGIC,							// 元丹	强化石
	enum_STRENGTHEN_SPECIAL_EX1,					// 辅1(几率符)
	enum_STRENGTHEN_SPECIAL,						// 辅助3(幸运符)
	enum_STRENGTHEN_COUNT,
};

//enum enum_Strengthen_Command
//{
//	enum_STRENGTHEN_PUTON = 1,			// 放物品
//	enum_STRENGTHEN_DOIT,				// 强化
//	enum_STRENGTHEN_CANCEL,				// 取消强化
//};

enum enum_Strengthen_Result
{
	enum_STRENGTHEN_ERROR,
	enum_STRENGTHEN_SUCCESS,		// 强化成功
	enum_STRENGTHEN_FAILED,			// 强化失败
	enum_STRENGTHEN_FAILED_EQUIP_DESTROY,			// 强化失败装备未销毁
	enum_STRENGTHEN_FAILED_EQUIP_NOT_DESTROY,		// 强化失败装备未销毁
	enum_STRENGTHEN_PUTONOK,		// 放入物品成功
	enum_STRENGTHEN_PUTOUTOK,		// 拿出物品成功
	enum_STRENGTHEN_CANCEL,			// 取消强化
	enum_STRENGTHEN_NOMONEY,		// 金钱不足
	enum_STRENGTHEN_NOEQUIP,		// 请放入装备*
	enum_STRENGTHEN_NOTEQUIP,		// 该物品不可被强化
	enum_STRENGTHEN_NOMAGIC,		// 请放入魔晶
	enum_STRENGTHEN_NOTMAGIC,		// 请在此处放入魔晶
	enum_STRENGTHEN_NOTSPECIAL,		// 请放入幸运符
	enum_STRENGTHEN_NOTSPECIAL_EX,	// 请放入机率符
	enum_STRENGTHEN_STRNOPLACE,		// 强化栏该位置有物品啦，无法放置
	enum_STRENGTHEN_BAGNOPLACE,		// 装备栏该位置有物品啦，无法取回
	enum_STRENGTHEN_EQUIP_FIRST,	// 请先放入装备
	enum_STRENGTHEN_MAGIC_TYPE,		// 魔晶类型不正确
	enum_STRENGTHEN_MAGIC_LEVEL,	// 魔晶强化等级不够
	enum_STRENGTHEN_SUCCESS_SLOT,	// 强化成功 等级n 获得插槽奖励
	enum_STRENGTHEN_FULL,			// 强化栏位已满
	enum_STRENGTHEN_LEVEL,			// 已经达到强化等级上限
	enum_STRENGTHEN_EX_LEVEL,		// 几率符等级不够
	enum_STRENGTHEN_EX2_LEVEL,		// 保护符等级不够
	enum_STR_ERR_QUALITY,			// 石头的品质不能低于装备的品质
	enum_STRENGTHEN_CANNOTSPECIAL,	// 不能放置保护符

	enum_STRENGTHEN_PUT_SUCCEED,	// 强化栏放置成功
	enum_STRENGTHEN_GET_SUCCEED,	// 强化栏取回成功
	enum_STR_ERR_PUTON_EQUIP_FIRST,	// 先放入装备

	enum_STR_ERR_MAXLEVEL = -1000,	// 已经达到强化最高等级
	enum_STR_ERR_NOSTONE,			// 没有强化石
	enum_STR_ERR_NOEQUIP,			// 没有装备
	enum_STR_ERR_LEVEL,				// 元丹等级与武器装备不匹配
	enum_STR_ERR_LEVELEX,			// 几率符等级与武器装备不匹配
	enum_STR_ERR_LEVELEX2,			// 保护符等级与武器装备不匹配
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
	enum_INLAY_SUCCESS = 1,				// 镶嵌成功
	enum_INLAY_FAILED,					// 镶嵌失败
	enum_INLAY_NOT_EQUIP,				// 不是装备
	enum_INLAY_NOT_GEM,					// 不是宝石
	enum_INLAY_NOT_POLYCHROME_GEM,		// 不是多彩宝石
	enum_INLAY_NO_EQUIP,				// 请放入装备
	enum_INLAY_NO_GEM,					// 请放入宝石
	enum_INLAY_NO_POLYCHROME_GEM,		// 请放入多彩宝石
	enum_INLAY_GEM_LV_QUALITY_TOO_LOW,	// 宝石等级或者品质低于装备
	enum_INLAY_NO_MONEY,				// 没钱
	enum_INLAY_SLOT_NOT_EXIST,			// 不存在插槽
	enum_INLAY_GEM_EXIST,				// 宝石已经存在

	enum_INLAY_PUT_SUCCEED,				// 镶嵌栏放置成功
	enum_INLAY_PUT_EQUIP_SUCCEED,		// 镶嵌栏放置装备成功
	enum_INLAY_GET_SUCCEED,				// 镶嵌栏取回成功
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
	enum_Fanshion_INLAY_SUCCESS = 1,			// 镶嵌成功
	enum_Fanshion_INLAY_FAILED,					// 镶嵌失败
	enum_Fanshion_INLAY_NOT_EQUIP,				// 不是装备
	enum_Fanshion_INLAY_NOT_GEM,				// 不是宝石
	enum_Fanshion_INLAY_NOT_POLYCHROME_GEM,		// 不是多彩宝石
	enum_Fanshion_INLAY_NO_EQUIP,				// 请放入装备
	enum_Fanshion_INLAY_NO_GEM,					// 请放入宝石
	enum_Fanshion_INLAY_NO_POLYCHROME_GEM,		// 请放入多彩宝石
	enum_Fanshion_INLAY_GEM_LV_QUALITY_TOO_LOW,	// 宝石等级或者品质低于装备
	enum_Fanshion_INLAY_NO_MONEY,				// 没钱
	enum_Fanshion_INLAY_SLOT_NOT_EXIST,			// 不存在插槽
	enum_Fanshion_INLAY_GEM_EXIST,				// 宝石已经存在

	enum_Fanshion_INLAY_PUT_SUCCEED,			// 镶嵌栏放置成功
	enum_Fanshion_INLAY_PUT_EQUIP_SUCCEED,		// 镶嵌栏放置装备成功
	enum_Fanshion_INLAY_GET_SUCCEED,			// 镶嵌栏取回成功
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
	enum_Rabbet_Not_Equip,				// 不是装备
	enum_Rabbet_Not_Fu,					// 不是开槽卷
	enum_Rabbet_NO_Equip,				// 请放入装备
	enum_Rabbet_NO_Fu,					// 请放入开槽卷
	enum_Rabbet_NO_Money,				// 没钱
	enum_Rabbet_Slot_Exist,				// 槽已开

	enum_Rabbet_Put_Succee,				// 开槽栏放置成功
	enum_Rabbet_Get_Succee,				// 开槽栏取回成功
	enum_Rabbet_Cancel_Succee,			// 开槽栏取消成功
};

enum enum_Fanshion_Rabbet_Result_Type
{
	enum_Fanshion_Rabbet_Error = 0,
	enum_Fanshion_Rabbet_Succee,
	enum_Fanshion_Rabbet_Not_Equip,				// 不是装备
	enum_Fanshion_Rabbet_Not_Fu,				// 不是开槽卷
	enum_Fanshion_Rabbet_NO_Equip,				// 请放入装备
	enum_Fanshion_Rabbet_NO_Fu,					// 请放入开槽卷
	enum_Fanshion_Rabbet_NO_Money,				// 没钱
	enum_Fanshion_Rabbet_Slot_Exist,			// 槽已开

	enum_Fanshion_Rabbet_Put_Succee,			// 开槽栏放置成功
	enum_Fanshion_Rabbet_Get_Succee,			// 开槽栏取回成功
	enum_Fanshion_Rabbet_Cancel_Succee,			// 开槽栏取消成功
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
	enum_Break_Not_Equip,				// 不是装备
	enum_Break_Slot_Not_Exist,			// 未开槽
	enum_Break_Not_Fu,					// 不是销槽卷
	enum_Break_NO_Equip,				// 请放入装备
	enum_Break_NO_Fu,					// 请放入销槽卷
	enum_Break_NO_Money,				// 没钱
	enum_Break_Equip_No_Gem,			// 装备不需要分离

	enum_Break_Put_Succee,				// 销槽栏放置成功
	enum_Break_Get_Succee,				// 销槽栏取回成功
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
	enum_Spar_Not_Gem,				// 不是宝石
	enum_Spar_Not_Special,			// 不是几率符
	enum_Spar_Not_Role1,			// 不符合规则1
	enum_Spar_Not_Role2,			// 不符合规则2
	enum_Spar_Not_Role3,			// 不符合规则3
	enum_Spar_Not_Role4,			// 不符合规则4
	enum_Spar_Not_Role5,			// 不符合规则5
	enum_Spar_Not_Role6,			// 不符合规则6
	enum_Spar_NO_Money,				// 没钱

	enum_Spar_Put_Succee,			// 宝石合成栏放置成功
	enum_Spar_Get_Succee,			// 宝石合成栏取回成功
	enum_Spar_Cancel_Succee,
};

enum enum_Compose_Result_Type
{
	enum_Compose_Error = 0,
	enum_Compose_Succee,
	enum_Compose_Short,
	enum_Compose_Put_Succee,		// 装备合成栏放置成功
	enum_Compose_Get_Succee,		// 装备合成栏取回成功
	enum_Compose_Clear_Succee,		// 放回所有物品成功
	enum_Compose_Cancel_Succee,		// 取消合成成功
	enum_Compose_Show_UI,			// 显示UI界面
	enum_Compose_ComposeDo_Error,	// 合成失败
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
	enum_Repair_Pos_Equip = 0,		// 装备
	enum_Repair_Pos_RepairStone,	// 修补石

	enum_Repair_Pos_Count,
};

enum enum_Repair_Result_Type
{
	enum_Repair_Error = 0,
	enum_Repair_Succee,
	enum_Repair_Not_Equip,				// 不是装备
	enum_Repair_Not_RepairStone,		// 不是修补石
	enum_Repair_NO_Equip,				// 请放入装备
	enum_Repair_NO_RepairStone,			// 请放入修补石
	enum_Repair_NO_Money,				// 没钱
	enum_Repair_CANNOT_Repair,			// 不能修理
	enum_Repair_DuraMax,				// 无需修理
	enum_Repair_RepairStone_Lv,			// 修补石等级低于装备
	enum_Repair_RepairStone_Qu,			// 修补石品质不符合装备
	enum_Repair_Equip_IMM,				// 无形装备不能被普通修理

	enum_Repair_Put_Succee,				// 修理栏放置成功
	enum_Repair_Get_Succee,				// 修理栏取回成功
	enum_Repair_Cancel_Succee,			// 修理栏取消成功
};

enum enum_MBStrengthen_Position_Type
{
	enum_MBStrengthen_Pos_Stone = 0,	// 融魂道具
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

	enum_MBStrengthen_Put_Succee,		// 修理栏放置成功
	enum_MBStrengthen_Get_Succee,		// 修理栏取回成功
	enum_MBStrengthen_Cancel_Succee,	// 修理栏取消成功
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
	esBuyItemReq,				// 购买物品申请
	esSellItemReq,				// 出售物品申请
	
	esClientUpdatePageUp = 100,
	esClientUpdatePageDown,
	esClientSellRep,			// 卖物品确认
	esLuaBuyReq,				// 买物品申请
	esLuaSellReq,				// 卖物品申请
	esLuaAutoSellReq,			// 自动卖垃圾 Quality = 1
	esLuaBuyMultiReq,			// 批量买入
	esIsGuildStore,				// 是否是帮派商店
	esIsHonourStore,			// 是否是荣誉商店
	esIsMaterialStore,			// 是否是荣誉商店
	esIsQuestBuyStore,			// 是否是任务只买商店
	esIsQuestSellStore,			// 是否是任务只卖商店
	esIsAffectionStore,			// 是否是恩情值商店
	//跑商之金晶卷轴
	esGetMobileTradeItems,		//金晶卷轴商品列表
};

enum enum_Store_Result
{
	esSuccess			= 1,// 操作成功
	esbFailed			,	// 系统错误，购买失败
	esbNotEnoughMoney	,	// 金钱不足，购买失败
	esbBagFull			,	// 背包空位不足，购买失败
	esbNotEnoughAttribute	,	// 贡献度不足，购买失败
	esbNotEnoughHonor,
	esbNotEnoughSilver,		// 没有足够的银票
	esbInOnlySellStore	,	// 在只卖商店里，不能买
	esbLackOfItem		,	// 限售商品数量不足，购买失败   暂无信息
	essFailed			,	// 卖物品失败
	essNoSold,				// 不可出售
	esbNotEnoughMaterial,	// 没有足够的物品
	essNotEnoughSilverPlace,	// 没有足够的空间放置银票，无法卖出
	essInOnlyBuyStore	,	// 在只买商店里，不能卖
	esbNotEnoughAffection,	// 恩情值不够, 无法购买
	essStoreErrBase = 300,	// 商店买卖错误信息基准编号
};

enum enum_Pick_Item_Result
{
	epSuccess			= 1,// 你拾取了%s
	epFailed			,	// 无法拾取该物品
	epBagFull			,	// 背包已满，无法拾取物品
	epNotYours			,	// 这个物品不属于你
	epTooFar			,	// 距离过远
	epNoItem			,	// 物品不存在
	epOwnerNotOnline	,	// 归属者不在线
	epDeath				,	// 死亡状态
	epStateErr			,	// 当前状态无法拾取
};


enum enum_Delete_Item_Reason
{
	edNoReason			= 0,	// 系统原因
	epTimeUpdate,				// 物品寿命时间到
	epItemTrade,				// 物品交易出去了
	epItemDiceWinner,			// 投掷骰子获胜
};


enum enum_BindType
{
	eBT_NoBind			= 0,	// 不绑定， 可能被商城冲掉规则
	eBT_PickBind		= 1,	// 拾取绑定
	eBT_UseBind			= 2,	// 使用绑定
	eBT_EquipBind		= 3,	// 装备绑定
	eBT_NeverBind		= 4,	// 永远不绑定
	eBT_MBBind			= 5,	// 永远不绑定
};


enum enum_VanishType
{
	eVT_NeverDisappear	= 0,	// 无限次使用
	eVT_Disappear		= 1,	// 使用次数为零时消失
	eVT_NoDisappear		= 2,	// 使用次数为零时不消失	
	
};

//开关性质的各种物品特性描述
enum enum_ItemOperator_ByItemFlags
{
	eItemEvent_DestoryByDead = 1,	//死亡消失
	eItemEvent_DropByDead = 2,		//死亡掉落
	eItemEvent_DestoryByLogin = 4,	//登入时消失
	eItemEvent_DestoryByLoginMoreThen15 =8,	//登入时(离线时间超过15分钟)消失
	eItemEvent_DestoryByLogout =16,	//登出掉落
	eItemEvent_NoDrop = 32,			//不可丢弃
	eItemEvent_NoDestory = 64,		//不可销毁
	eItemEvent_NoSave2Bank = 128,	//不可存放于仓库
	eItemEvent_NoSell = 256,		//不可出售
	eItemEvent_NoTradeWhenCD = 512,	//未冷却时可交易(默认为不可交易)
	eItemEvent_EquipOnlyOne = 1024,	//装备唯一(戒指和配饰)
	eItemEvent_IsQuestItem = 2048,	//是任务道具
	eItemEvent_TempShortcut = 4096,	//显示在临时快捷栏里
	eItemEvent_IBTips		= 8192,	//IB道具提示
	eItemEvent_TalentLearn  = 16384,//天赋学习道具

};

// 物品相关行为会触发任务脚本描述
enum enum_OpenQuestWithItemOperator
{
	eOpenQuest_None = 0x0,					// 无任务触发
	eOpenQuest_ClientClickItem = 0x01,		// 客户端使用时触发
	eOpenQuest_ServerPickUpItem = 0x02,		// 服务器端拾取后触发
	eOpenQuest_ServerUsedItem = 0x04,		// 服务器使用后触发
	eOpenQuest_ServerDestoryItem = 0x08,	// 服务器销毁后触发
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
	enumItemDOPT_All_Item		= 0x00,	// 所有物品
	enumItemDOPT_Equip_Weapon	= 0x01,	// 装备栏武器
	enumItemDOPT_Equip_Helm		= 0x02,	// 装备栏头盔
	enumItemDOPT_Equip_Coat		= 0x04,	// 装备栏衣服
	enumItemDOPT_Equip_Belt		= 0x08,	// 装备栏腰带
	enumItemDOPT_Equip_Cuff		= 0x10,	// 装备栏护腕
	enumItemDOPT_Equip_Boot		= 0x20,	// 装备栏鞋
	enumItemDOPT_Bag_All_Item	= 0x40,	// 背包所有物品
	enumItemDOPT_End			= 0x40,
};

enum enum_ItemDura_OP_Reason
{
	enumItemDOPR_None = 0,
	enumItemDOPR_KillByMonster,		// 被npc杀死
	enumItemDOPR_Normal_Attack,		// 被普通攻击
	enumItemDOPR_Deadlyl_Attack,	// 被致命一击
	enumItemDOPR_Critical_Attack,	// 被会心一击
};


enum enum_Item_Quality_Level
{
	enum_Item_Quality_Invalid,	// 无效
	enum_Item_Quality_Grey,		// 灰色
	enum_Item_Quality_White,	// 白色
	enum_Item_Quality_Green,	// 绿色
	enum_Item_Quality_Blue,		// 蓝色
	enum_Item_Quality_Purple,	// 紫色
	enum_Item_Quality_Golden,	// 金色
};


// 物品操作日志
enum enum_Item_Operator_Type
{
	E_Item_Operator_Unknown = 0,					// 未知操作
	E_Item_Operator_FirstLogin = 1,					// 首次登陆日志
	E_Item_Operator_Init = 2,						// 初始化日志
	E_Item_Operator_Trade = 3,						// 玩家交易
	E_Item_Operator_MovableShop = 4,				// 玩家摆摊
	E_Item_Operator_RealTimeOverEvent = 5,			// 现实时间到期
	E_Item_Operator_GameTimeOverEvent = 6,			// 游戏时间到期
	E_Item_Operator_Pick = 7,						// 拾取
	E_Item_Operator_Destory = 8,					// 销毁
	E_Item_Operator_Quest = 9,						// 任务
	E_Item_Operator_Repair = 10,					// 修理
	E_Item_Operator_Split = 11,						// 拆分
	E_Item_Operator_Billing = 12,					// 商城
	E_Item_Operator_PickAndBind = 13,				// 给予绑定
	E_Item_Operator_MailAttach = 14,				// 邮件系统
	E_Item_Operator_VIPCyberCafe = 15,				// VIP网吧
	E_Item_Operator_CAST_SKILL = 16,				// 使用技能
	E_Item_Operator_MoveItem = 17,					// 移动道具
	E_Item_Operator_UserDelete = 18,				// 用户主动删除(丢弃)
	E_Item_Operator_NestSacrifice = 19,				// 巢穴献祭
	E_Item_Operator_GemStudded = 20,				// 宝石镶嵌
	E_Item_Operator_GemMerge = 21,					// 宝石合成
	E_Item_Operator_GemCarve = 22,					// 宝石雕刻
	E_Item_Operator_GemDismount = 23,				// 宝石拆除
	E_Item_Operator_CatchNpc = 24,					// 抓捕NPC
	E_Item_Operator_ConvertTo_Mail = 25,			// 普通邮件转物品
	E_Item_Operator_GetAttachment = 26,				// 提取附件
	E_Item_Operator_Business = 27,					// 交易
	E_Item_Operator_Store = 28,						// 商店
	E_Item_Operator_ExploreGive = 29,				// 探险之给予奖励
	E_Item_Operator_MakeBlueprint = 30,				// 工坊图纸制作
	E_Item_Operator_Stall = 31,						// 摆摊
	E_Item_Operator_Hatch = 32,						// 孵化
	E_Item_Operator_StallOffline = 33,				// 离线摆摊
	E_Item_Operator_MillDevice = 34,				// 工坊装置物品
	E_Item_Operator_PetBag = 35,					// 宠物背包物品置换
	E_Item_Operator_TribeSystem = 36,				// 部落系统
	E_Item_Operator_BpItem = 37,					// 宠物蛋
	E_Item_Operator_NpcDrop = 38,					// 杀死npc掉落
	E_Item_Operator_License = 39,					// 猎人执照
	E_Item_Operator_PetSkill = 40,					// 宠物技能
	E_Item_Operator_CampQuest = 41,					// 营地任务
	E_Item_Operator_FeedPet = 42,					// 喂养荒兽玩法
	E_Item_Operator_Salesroom = 43,					// 工坊拍卖行
	E_Item_Operator_Exploration = 44,				// 勘探操作
	E_Item_Operator_BpLearnSkill = 45,				// 战宠学习技能
	E_Item_Operator_BpOpenSlotSkill = 46,			// 战宠开技能槽
	E_Item_Operator_MakeFood = 47,					// 料理制作
};


// 金钱操作日志
enum enum_Money_Operator_Type
{
	E_Money_Operator_Unknown = 0,				// 未知操作
	//E_Money_Operator_FirstLogin,				// 首次登陆日志
	//E_Money_Operator_Init,						// 初始化日志
	//E_Money_Operator_ItemStore,					// Npc商店
	//E_Money_Operator_Trade,						// 玩家交易
	//E_Money_Operator_MovableShop,				// 玩家摆摊
	//E_Money_Operator_Strength,					// 强化
	//E_Money_Operator_Inlay,						// 合成
	//E_Money_Operator_Rabbet,					// 开槽
	//E_Money_Operator_Break,						// 清理宝石
	//E_Money_Operator_Spar,						// 镶嵌
	//E_Money_Operator_Relive,					// 复活
	//E_Money_Operator_Quest,						// 任务
	//E_Money_Operator_LearnSkill,				// 学习技能
	//E_Money_Operator_SkillAction,				// 技能
	//E_Money_Operator_GMCmd,						// GM指令
	//E_Money_Operator_Repair,					// 修理
	//E_Money_Operator_Securicar,					// 镖车
	//E_Money_Operator_GuildCreate,				// 公会创建
	//E_Money_Operator_Billing,					// 商城
	//E_Money_Operator_Mail,						// 邮件系统
	//E_Money_Operator_Trans,						// 传送扣除
	//E_Money_Operator_MagicBabyFuse,				// 法宝融合
	//E_Money_Operator_QuizDouble,				// 问答系统(积分加倍功能)
	//E_Money_Operator_ClearTalent,				// 清除天赋点扣除
	//E_Money_Operator_BillTradeCost,				// 交易单扣除成本
	//E_Money_Operator_BillTradeIncome,			// 交易单收入增加
	//E_Money_Operator_NpcTalk,					// NPC对话
	//E_Money_Operator_NationBF,					// 国战副本
	//E_Money_Operator_GuildTalentSpeed,			// 加速帮派天赋升级
	//E_Money_Operator_EatMagicBaby,				// 法宝化灵
	//E_Money_Operator_WorldChat,					// 世界喊话
};

// 料理产物类型
enum enum_Cook_Food_Type
{
	E_Cook_Food_Normal,			// 普通
	E_Cook_Food_Fancy,			// 大餐
};

#define CFG_MAX_VIP_ITEM_BY_LINE	40
