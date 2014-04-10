/********************************************************************
//	2011-07-07 韩玉信定义物品相关的结构体和枚举
*********************************************************************/
#pragma once


#include "System/KType.h"
#include "System/KMacro.h"

#include "Inc/KCommonDefine.h"
#include "../StringManager/KStringManager.h"
#include "KItemDef.h"

namespace KItemAbout
{
	const int MAX_WORLD_DROP_COUNT				= 5;

	const int CFG_MAX_BAG_EQUIP_ITEM_COUNT		= 20;		// 背包中装备道具数量
	const int CFG_MAX_BAG_NORMAL_ITEM_COUNT		= 60;		// 背包中普通道具数量
	const int CFG_MAX_BAG_MATERIAL_ITEM_COUNT	= 60;		// 背包中材料道具数量
	const int CFG_MAX_BAG_QUEST_ITEM_COUNT		= 60;		// 背包中任务道具数量
	const int CFG_MAX_BANK_NORMAL_ITEM_COUNT	= 100;		// 仓库中普通道具数量
	const int CFG_MAX_BANK_MATERIAL_ITEM_COUNT	= 49;		// 仓库中材料道具数量
	const int CFG_MAX_BPMAT_CELL_COUNT			= 60;		// 战兽材料背包格子数量
	const int BagReaderAttachCount = 5;						// 邮箱附件阅读包格子数量

#pragma warning( disable: 4200 )	//warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma pack(push,1)

	// 背包ID(背包类型)
	enum ENUM_ITEM_BAG_TYPE
	{
		enum_item_Bag_Start = 0,
		enum_item_BagEquip = enum_item_Bag_Start,	// 装备背包(0)
		enum_item_BagNormal,						// 普通背包(1)
		enum_item_BagMat,							// 战兽材料背包(2)
		enum_item_BagQuest,							// 任务背包(3)
		enum_item_BagBank_Normal,	// 普通仓库(目前未使用)
		enum_item_BagBank_Mat,		// 材料仓库(目前未使用)

		enum_item_MillBagNormalBox,					// 工坊普通仓库(6)
		enum_item_MillBagMaterialBox,				// 工坊材料仓库(7)
		enum_item_MillBagEquipmentBox,				// 工坊装置仓库(8)

		enum_item_BagCount,			// 背包数量

		enum_item_BagReaderAttach = 16,	// 收件箱附件
	};

	// 物品类型
	enum ENUM_ITEM_TYPE
	{
		enum_item_normal = 0,	// 普通物品
		enum_item_card,			// 材料
		enum_item_quest,		// 任务物品
		enum_item_gem,			// 宝石
		enum_item_hammer,		// 榔头
		enum_item_Blueprint,	// 图纸
		enum_item_PetBag,		// 宠物背包
		enum_item_PetEgg,		// 宠物蛋
		enum_item_SkillBook,	// 宠物技能书
		enum_item_SkillStar,	// 宠物技能星
		enum_item_cookMeat,		// 料理食材
		enum_item_cookIngredient,// 料理配料
		enum_item_cookFood,		// 料理产物
	};

	const int EQUIPMENT_FIRST_ITEM = 1;

	// 装备位置
	enum enum_Equipment_Posision
	{
		enum_EQUIPMENT_NORMAL = 0,			// 普通？
		enum_EQUIPMENT_WEAPON = EQUIPMENT_FIRST_ITEM,				//武器
		enum_EQUIPMENT_BODY,				// 身体
		enum_EQUIPMENT_HEAD,				// 头部
		enum_EQUIPMENT_HAND,				// 手
		enum_EQUIPMENT_FEET,				// 脚
		enum_EQUIPMENT_AMULET,				// 颈部（护身符）
		enum_EQUIPMENT_RINGA,				// 戒指左
		enum_EQUIPMENT_RINGB,				// 戒指右

		enum_EQUIPMENT_COUNT,				//20.数量

		// 如下是所有历史遗留的
		enum_EQUIPMENT_HEADGEAR,			// 头饰
		enum_EQUIPMENT_MANTLE,				// 04.背部（披风）
		enum_EQUIPMENT_BELT,				// 07.腰部
		enum_EQUIPMENT_CUFF,				// 08.腕部
		enum_EQUIPMENT_SHOES,				// 09.足部：鞋子
		enum_EQUIPMENT_ADORNMENTA, 			// 10.佩饰A
		enum_EQUIPMENT_ADORNMENTB,			// 11.佩饰B
		enum_EQUIPMENT_TRUMP,				// 14.法宝
		enum_EQUIPMENT_ADORNMENT,			// 15.佩饰（全）
		enum_EQUIPMENT_RING,				// 16.戒指（全）

		enum_EQUIPMENT_FASHION_HEAD,		// 17.时装头饰
		enum_EQUIPMENT_FASHION_CLOTHING,	// 18.时装衣服
		enum_EQUIPMENT_FASHION_SHOES,		// 19.时装鞋子
	};
	
	struct ItemDesc { DWORD itemId; int count; };

	struct KDBItemBaseFlags
	{
	public:
		DWORD GetValue()
		{
			DWORD dwValue = 0;
			memcpy(&dwValue, this, sizeof(DWORD));
			return dwValue;
		}

		KDBItemBaseFlags()
		{
			memset( this, 0, sizeof(KDBItemBaseFlags) );
		}

		KDBItemBaseFlags(DWORD dwVal)
		{
			*this = dwVal;
		}

		KDBItemBaseFlags& operator=(DWORD val)
		{
			memcpy(this, &val, sizeof(DWORD));
			return *this;
		}

		DWORD mFlag_Immateriality : 1;	// 无形(不可修复)
		DWORD mFlag_Invariable : 1;		// 永不磨损
		DWORD mFlag_Preserving14 : 1;
		DWORD mFlag_Preserving13 : 1;
		DWORD mFlag_Preserving12 : 1;
		DWORD mFlag_Preserving11 : 1;
		DWORD mFlag_Preserving10 : 1;
		DWORD mFlag_Preserving9 : 1;
		DWORD mFlag_Preserving8 : 1;
		DWORD mFlag_Preserving7 : 1;
		DWORD mFlag_Preserving6 : 1;
		DWORD mFlag_Preserving5 : 1;
		DWORD mFlag_Preserving4 : 1;
		DWORD mFlag_Preserving3 : 1;
		DWORD mFlag_Preserving2 : 1;
		DWORD mFlag_Preserving1 : 1;
		DWORD m_CoalescentFlags	: 16;
	};

// 	enum ENUM_ITEM_CLASS_TYPE
// 	{
// 		enum_item_class_Base	= 0,	// 基础类
// 		enum_item_class_Simple,			// 简单物品
// 		enum_item_class_CanUse,			// 可以使用的物品
// 		enum_item_class_CanEquip,		// 可以装备的物品
// 	};

#pragma pack( pop )

	struct KAliveTimeItemInfo
	{
		ENUM_ITEM_BAG_TYPE	nBagID;
		BYTE				nAliveType;
		int nBagPos;
	};

	/**
	** 物品ID范围
	**/
	enum ENUM_ITEM_ID_RANGE
	{
		e_id_range_item			= 0,		// 普通物品的ID起始
		e_id_range_card			= 10000,	// 卡牌ID起始
		e_id_range_gem			= 200001,	// 宝石ID起始(十进制200001开始)
		e_id_range_hammer		= 220000,	// 榔头ID起始
		e_id_range_petegg		= 300001,	// 宠物蛋ID起始
		e_id_range_skillbook	= 400001,	// 宠物技能书ID起始
		e_id_range_skillstar	= 500001,	// 宠物技能星ID起始
		e_id_range_blueprint	= 2000001,	// 工坊图纸ID起始
		e_id_range_device		= 2100000,	// 工坊装置ID起始
		e_id_range_petbag		= 2200000,	// 宠物背包ID起始
		e_id_range_meat			= 2300000,	// 料理食材ID起始
		e_id_range_ingredient	= 2400000,	// 料理配料ID起始
		e_id_range_food			= 2500000,	// 料理产物ID起始

		// 在这里添加记得修改KCreateInfo_ItemBase里的IsxxxItem/IsxxxItemStatic接口
	};

	/**
	** 物品基类
	**/
	struct KCreateInfo_ItemBase
	{
		KCreateInfo_ItemBase()
		{
			Init();
		}
		virtual ~KCreateInfo_ItemBase() {}

		void Init()
		{
			memset((void*)this, 0, sizeof(KCreateInfo_ItemBase));
			s_nMaxStacks = 1;
		}

		bool IsGMItem() const
		{
			return (ItemFlags & (1 << 19)) != 0;
		}

		INT		ItemFlags;			// 物品特殊性标志 //此变量根据bit位用来判断多种情况
		DWORD	s_nItemID;			// 物品ID
		INT		s_nNameID;			// 物品名字ID 对应StringManager表
		DWORD	s_nDesID;			// 物品描述
		WORD	s_nLevel;			// 物品级别
		BYTE	s_byQuality;		// 物品品质
		DWORD	s_nDisplayID;		// 3D显示编号？

		BYTE	s_byItemType;		// 物品分类（普通道具、材料、装备、任务道具等）
		DWORD	s_nPrice;			// 物品卖出价格：买入价格放到商店表里配置
		BYTE	s_byBindType;		// 物品绑定类型
		BYTE	s_byAliveType;		// 存在方式	"0.不消失 1.游戏时间 2.现实时间"
		DWORD	s_dwAliveTime;		// 存在时间
		BYTE	s_nWGroup[MAX_WORLD_DROP_COUNT];	// 怪物掉落组
		WORD	s_nWProb[MAX_WORLD_DROP_COUNT];		// 怪物掉落率

		WORD	s_nMaxStacks;		// 物品最大堆叠数量
		BYTE	s_byVanishType;		// 物品消失规则

		BYTE	InventoryType;		// 背包类型
		int		s_nCostType;		// 消耗类型：0,不变 1,扣一次次数 2,扣一个物品
		char	s_icon[64];			// 图标
		DWORD	s_dwFlags;			// 标记

		WORD	s_nMaxKeep;			// 物品最多可携带数量

		bool IsUsableItem() const
		{
			return (s_nItemID < e_id_range_card);
		}
		static bool IsUsableItemStatic(DWORD dwItemID)
		{
			return dwItemID < e_id_range_card;
		}
		static bool IsCardItem(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_card && dwItemID < e_id_range_gem;
		}
		bool IsCardItem() const
		{
			return s_nItemID >= e_id_range_card && s_nItemID < e_id_range_gem;
		}
		bool IsGemableItem() const
		{
			return s_nItemID >= e_id_range_gem && s_nItemID < e_id_range_hammer;
		}
		static bool IsGemableItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_gem && dwItemID < e_id_range_hammer;
		}

		bool IsHammerableItem() const
		{
			return s_nItemID >= e_id_range_hammer && s_nItemID < e_id_range_petegg;
		}
		static bool IsHammerableItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_hammer && dwItemID < e_id_range_petegg;
		}

		bool IsPetEggItem() const
		{
			return s_nItemID >= e_id_range_petegg && s_nItemID < e_id_range_skillbook;
		}
		static bool IsPetEggItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_petegg && dwItemID < e_id_range_skillbook;
		}
		bool IsSkillBookItem() const
		{
			return s_nItemID >= e_id_range_skillbook && s_nItemID < e_id_range_skillstar;
		}
		static bool IsSkillBookItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_skillbook && dwItemID < e_id_range_skillstar;
		}
		bool IsSkillStarItem() const
		{
			return s_nItemID >= e_id_range_skillstar && s_nItemID < e_id_range_blueprint;
		}
		static bool IsSkillStarItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_skillstar && dwItemID < e_id_range_blueprint;
		}

		bool IsBlueprintItem() const
		{
			return s_nItemID >= e_id_range_blueprint && s_nItemID < e_id_range_device;
		}
		static bool IsBlueprintItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_blueprint && dwItemID < e_id_range_device;
		}

		// 验证是否是工坊装置
		bool IsMillDeviceItem() const
		{
			return s_nItemID >= e_id_range_device && s_nItemID < e_id_range_petbag;
		}
		static bool IsMillDeviceItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_device && dwItemID < e_id_range_petbag;
		}
		bool IsPetBagItem() const
		{
			return s_nItemID >= e_id_range_petbag && s_nItemID < e_id_range_meat;
		}
		static bool IsPetBagItemStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_petbag && dwItemID < e_id_range_meat;
		}

		// 食材
		bool IsCookMeat() const
		{
			return s_nItemID >= e_id_range_meat && s_nItemID < e_id_range_ingredient;
		}
		static bool IsCookMeatStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_meat && dwItemID < e_id_range_ingredient;
		}

		// 配料
		bool IsCookIngredient() const
		{
			return s_nItemID >= e_id_range_ingredient && s_nItemID < e_id_range_food;
		}
		static bool IsCookIngredientStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_ingredient && dwItemID < e_id_range_food;
		}

		// 料理产物
		bool IsCookFood() const
		{
			return s_nItemID >= e_id_range_food;
		}
		static bool IsCookFoodStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_food;
		}
		// 如果添加新类型，上面要写最大值

		int GetItemID() const	{	return (int)s_nItemID;	}
		int GetLevel() const	{	return (int)s_nLevel;	}
		int GetQuality() const	{	return (int)s_byQuality;}
		int GetItemType() const	{	return (int)s_byItemType;}
		int GetPrice() const	{	return (int)s_nPrice;	}
		int GetBindType() const	{	return (int)s_byBindType;	}
		int GetAliveType() const{	return (int)s_byAliveType;	}
		int GetAliveTime() const{	return (int)s_dwAliveTime;	}
		int GetMaxStacks() const{	return (int)s_nMaxStacks;	}
		int GetVanishType() const{	return (int)s_byVanishType;	}
		int GetInventoryType() const{	return (int)InventoryType;	}
		int GetMaxKeep() const	{	return (int)s_nMaxKeep;	}

		const char* GetIcon() const	{	return s_icon;	}
		const char* GetName() const	{	return KStringManager::Instance()->GetStringByID(s_nNameID);	}
		const char* GetDescription() const	{	return KStringManager::Instance()->GetStringByID(s_nDesID);	}

		bool IsNotTrade() const { return _IsFlagTrue(F_NotTrade); }
		bool IsNotDrop() const {return _IsFlagTrue(F_NotDrop); }
		bool IsNotSell() const {return _IsFlagTrue(F_NotSell); }

		bool IsNotHatchFight() const {return _IsFlagTrue(F_NotHatchFight); }
		bool IsNotHatchWork() const {return _IsFlagTrue(F_NotHatchWork); }
		bool IsNotHatchRide() const {return _IsFlagTrue(F_NotHatchRide); }
		
	protected:
		enum enumFlags
		{
			F_NotTrade=0,	// 不可交易
			F_NotDrop,		// 不可丢弃
			F_NotSell,		// 不可出售
			
			F_NotHatchFight,	// 不能孵化成战宠
			F_NotHatchWork,		// 不能孵化成工宠
			F_NotHatchRide,		// 不能孵化成骑宠
		};

		bool _IsFlagTrue(int nBitPos) const
		{
			return ((s_dwFlags & (1 << (int)nBitPos)) ? true : false);
		}
	public:
		BeginDefLuaClass(KCreateInfo_ItemBase)
			DefMemberVar(ItemFlags);
			DefMemberVar(s_nItemID);
			DefMemberVar(s_nNameID);
	
		EndDef
	};
	
	const int MAX_EFFECT_PARAM_COUNT = 3;		// 修改技能的参数个数
	/**
	** 普通物品
	**/
	struct KCreateInfo_Item : public KCreateInfo_ItemBase
	{
		KCreateInfo_Item() : KCreateInfo_ItemBase()
		{
			Init();
		}
		void Init()
		{
            s_nSkillID = 0;
            s_nConditionID = s_nCParam = 0;
            s_wMaxDura = s_wInitDura = 0;
            s_byMaxUseTime = s_byInitUseTime = 0;
            ItemMaterial = 0;
            s_nItemWeight = 0;
		}

		int		s_nSkillID;			// 物品绑定技能ID
		DWORD	s_nConditionID;		// 条件ID	//物品使用检查条件
		DWORD	s_nCParam;			// 条件参数1

		WORD	s_wMaxDura;			// 最大耐久度
		WORD	s_wInitDura;		// 初始化耐久度

		BYTE	s_byMaxUseTime;		// 最大使用次数
		BYTE	s_byInitUseTime;	// 初始化使用次数

		WORD	ItemMaterial;		// 物品材料
		int		s_nItemWeight;		// 重量（仅材料有）

		int GetConditionID() const	{	return (int)s_nConditionID;	}
		int GetCParam() const	{	return (int)s_nCParam;	}
		int GetMaxDura() const	{	return (int)s_wMaxDura;	}
		int GetInitDura() const	{	return (int)s_wInitDura;	}
		int GetMaxUseTime() const	{	return (int)s_byMaxUseTime;	}
		int GetInitUseTime() const	{	return (int)s_byInitUseTime;	}
		int GetItemMaterial() const	{	return (int)ItemMaterial;	}

	
		BeginDefLuaClass(KCreateInfo_Item)
			DefMemberFunc(GetDescription);
		EndDef
	};

	const int MAX_EQUIP_ARRT_COUNT = 4;	// 属性数量
	const int MAX_EQUIP_TRIGGER_COUNT = 1;	// 触发器数量
	/**
	** 装备/武器
	**/
	
	const int MAX_GEM_ARRT_COUNT = 4;	// 属性数量

};

DECLARE_SIMPLE_TYPE(KItemAbout::KAliveTimeItemInfo)
