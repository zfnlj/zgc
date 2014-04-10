/********************************************************************
//	2011-07-07 �����Ŷ�����Ʒ��صĽṹ���ö��
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

	const int CFG_MAX_BAG_EQUIP_ITEM_COUNT		= 20;		// ������װ����������
	const int CFG_MAX_BAG_NORMAL_ITEM_COUNT		= 60;		// ��������ͨ��������
	const int CFG_MAX_BAG_MATERIAL_ITEM_COUNT	= 60;		// �����в��ϵ�������
	const int CFG_MAX_BAG_QUEST_ITEM_COUNT		= 60;		// �����������������
	const int CFG_MAX_BANK_NORMAL_ITEM_COUNT	= 100;		// �ֿ�����ͨ��������
	const int CFG_MAX_BANK_MATERIAL_ITEM_COUNT	= 49;		// �ֿ��в��ϵ�������
	const int CFG_MAX_BPMAT_CELL_COUNT			= 60;		// ս�޲��ϱ�����������
	const int BagReaderAttachCount = 5;						// ���丽���Ķ�����������

#pragma warning( disable: 4200 )	//warning C4200: nonstandard extension used : zero-sized array in struct/union
#pragma pack(push,1)

	// ����ID(��������)
	enum ENUM_ITEM_BAG_TYPE
	{
		enum_item_Bag_Start = 0,
		enum_item_BagEquip = enum_item_Bag_Start,	// װ������(0)
		enum_item_BagNormal,						// ��ͨ����(1)
		enum_item_BagMat,							// ս�޲��ϱ���(2)
		enum_item_BagQuest,							// ���񱳰�(3)
		enum_item_BagBank_Normal,	// ��ͨ�ֿ�(Ŀǰδʹ��)
		enum_item_BagBank_Mat,		// ���ϲֿ�(Ŀǰδʹ��)

		enum_item_MillBagNormalBox,					// ������ͨ�ֿ�(6)
		enum_item_MillBagMaterialBox,				// �������ϲֿ�(7)
		enum_item_MillBagEquipmentBox,				// ����װ�òֿ�(8)

		enum_item_BagCount,			// ��������

		enum_item_BagReaderAttach = 16,	// �ռ��丽��
	};

	// ��Ʒ����
	enum ENUM_ITEM_TYPE
	{
		enum_item_normal = 0,	// ��ͨ��Ʒ
		enum_item_card,			// ����
		enum_item_quest,		// ������Ʒ
		enum_item_gem,			// ��ʯ
		enum_item_hammer,		// ��ͷ
		enum_item_Blueprint,	// ͼֽ
		enum_item_PetBag,		// ���ﱳ��
		enum_item_PetEgg,		// ���ﵰ
		enum_item_SkillBook,	// ���＼����
		enum_item_SkillStar,	// ���＼����
		enum_item_cookMeat,		// ����ʳ��
		enum_item_cookIngredient,// ��������
		enum_item_cookFood,		// �������
	};

	const int EQUIPMENT_FIRST_ITEM = 1;

	// װ��λ��
	enum enum_Equipment_Posision
	{
		enum_EQUIPMENT_NORMAL = 0,			// ��ͨ��
		enum_EQUIPMENT_WEAPON = EQUIPMENT_FIRST_ITEM,				//����
		enum_EQUIPMENT_BODY,				// ����
		enum_EQUIPMENT_HEAD,				// ͷ��
		enum_EQUIPMENT_HAND,				// ��
		enum_EQUIPMENT_FEET,				// ��
		enum_EQUIPMENT_AMULET,				// �������������
		enum_EQUIPMENT_RINGA,				// ��ָ��
		enum_EQUIPMENT_RINGB,				// ��ָ��

		enum_EQUIPMENT_COUNT,				//20.����

		// ������������ʷ������
		enum_EQUIPMENT_HEADGEAR,			// ͷ��
		enum_EQUIPMENT_MANTLE,				// 04.���������磩
		enum_EQUIPMENT_BELT,				// 07.����
		enum_EQUIPMENT_CUFF,				// 08.��
		enum_EQUIPMENT_SHOES,				// 09.�㲿��Ь��
		enum_EQUIPMENT_ADORNMENTA, 			// 10.����A
		enum_EQUIPMENT_ADORNMENTB,			// 11.����B
		enum_EQUIPMENT_TRUMP,				// 14.����
		enum_EQUIPMENT_ADORNMENT,			// 15.���Σ�ȫ��
		enum_EQUIPMENT_RING,				// 16.��ָ��ȫ��

		enum_EQUIPMENT_FASHION_HEAD,		// 17.ʱװͷ��
		enum_EQUIPMENT_FASHION_CLOTHING,	// 18.ʱװ�·�
		enum_EQUIPMENT_FASHION_SHOES,		// 19.ʱװЬ��
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

		DWORD mFlag_Immateriality : 1;	// ����(�����޸�)
		DWORD mFlag_Invariable : 1;		// ����ĥ��
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
// 		enum_item_class_Base	= 0,	// ������
// 		enum_item_class_Simple,			// ����Ʒ
// 		enum_item_class_CanUse,			// ����ʹ�õ���Ʒ
// 		enum_item_class_CanEquip,		// ����װ������Ʒ
// 	};

#pragma pack( pop )

	struct KAliveTimeItemInfo
	{
		ENUM_ITEM_BAG_TYPE	nBagID;
		BYTE				nAliveType;
		int nBagPos;
	};

	/**
	** ��ƷID��Χ
	**/
	enum ENUM_ITEM_ID_RANGE
	{
		e_id_range_item			= 0,		// ��ͨ��Ʒ��ID��ʼ
		e_id_range_card			= 10000,	// ����ID��ʼ
		e_id_range_gem			= 200001,	// ��ʯID��ʼ(ʮ����200001��ʼ)
		e_id_range_hammer		= 220000,	// ��ͷID��ʼ
		e_id_range_petegg		= 300001,	// ���ﵰID��ʼ
		e_id_range_skillbook	= 400001,	// ���＼����ID��ʼ
		e_id_range_skillstar	= 500001,	// ���＼����ID��ʼ
		e_id_range_blueprint	= 2000001,	// ����ͼֽID��ʼ
		e_id_range_device		= 2100000,	// ����װ��ID��ʼ
		e_id_range_petbag		= 2200000,	// ���ﱳ��ID��ʼ
		e_id_range_meat			= 2300000,	// ����ʳ��ID��ʼ
		e_id_range_ingredient	= 2400000,	// ��������ID��ʼ
		e_id_range_food			= 2500000,	// �������ID��ʼ

		// ��������Ӽǵ��޸�KCreateInfo_ItemBase���IsxxxItem/IsxxxItemStatic�ӿ�
	};

	/**
	** ��Ʒ����
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

		INT		ItemFlags;			// ��Ʒ�����Ա�־ //�˱�������bitλ�����ж϶������
		DWORD	s_nItemID;			// ��ƷID
		INT		s_nNameID;			// ��Ʒ����ID ��ӦStringManager��
		DWORD	s_nDesID;			// ��Ʒ����
		WORD	s_nLevel;			// ��Ʒ����
		BYTE	s_byQuality;		// ��ƷƷ��
		DWORD	s_nDisplayID;		// 3D��ʾ��ţ�

		BYTE	s_byItemType;		// ��Ʒ���ࣨ��ͨ���ߡ����ϡ�װ����������ߵȣ�
		DWORD	s_nPrice;			// ��Ʒ�����۸�����۸�ŵ��̵��������
		BYTE	s_byBindType;		// ��Ʒ������
		BYTE	s_byAliveType;		// ���ڷ�ʽ	"0.����ʧ 1.��Ϸʱ�� 2.��ʵʱ��"
		DWORD	s_dwAliveTime;		// ����ʱ��
		BYTE	s_nWGroup[MAX_WORLD_DROP_COUNT];	// ���������
		WORD	s_nWProb[MAX_WORLD_DROP_COUNT];		// ���������

		WORD	s_nMaxStacks;		// ��Ʒ���ѵ�����
		BYTE	s_byVanishType;		// ��Ʒ��ʧ����

		BYTE	InventoryType;		// ��������
		int		s_nCostType;		// �������ͣ�0,���� 1,��һ�δ��� 2,��һ����Ʒ
		char	s_icon[64];			// ͼ��
		DWORD	s_dwFlags;			// ���

		WORD	s_nMaxKeep;			// ��Ʒ����Я������

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

		// ��֤�Ƿ��ǹ���װ��
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

		// ʳ��
		bool IsCookMeat() const
		{
			return s_nItemID >= e_id_range_meat && s_nItemID < e_id_range_ingredient;
		}
		static bool IsCookMeatStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_meat && dwItemID < e_id_range_ingredient;
		}

		// ����
		bool IsCookIngredient() const
		{
			return s_nItemID >= e_id_range_ingredient && s_nItemID < e_id_range_food;
		}
		static bool IsCookIngredientStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_ingredient && dwItemID < e_id_range_food;
		}

		// �������
		bool IsCookFood() const
		{
			return s_nItemID >= e_id_range_food;
		}
		static bool IsCookFoodStatic(DWORD dwItemID)
		{
			return dwItemID >= e_id_range_food;
		}
		// �����������ͣ�����Ҫд���ֵ

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
			F_NotTrade=0,	// ���ɽ���
			F_NotDrop,		// ���ɶ���
			F_NotSell,		// ���ɳ���
			
			F_NotHatchFight,	// ���ܷ�����ս��
			F_NotHatchWork,		// ���ܷ����ɹ���
			F_NotHatchRide,		// ���ܷ��������
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
	
	const int MAX_EFFECT_PARAM_COUNT = 3;		// �޸ļ��ܵĲ�������
	/**
	** ��ͨ��Ʒ
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

		int		s_nSkillID;			// ��Ʒ�󶨼���ID
		DWORD	s_nConditionID;		// ����ID	//��Ʒʹ�ü������
		DWORD	s_nCParam;			// ��������1

		WORD	s_wMaxDura;			// ����;ö�
		WORD	s_wInitDura;		// ��ʼ���;ö�

		BYTE	s_byMaxUseTime;		// ���ʹ�ô���
		BYTE	s_byInitUseTime;	// ��ʼ��ʹ�ô���

		WORD	ItemMaterial;		// ��Ʒ����
		int		s_nItemWeight;		// �������������У�

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

	const int MAX_EQUIP_ARRT_COUNT = 4;	// ��������
	const int MAX_EQUIP_TRIGGER_COUNT = 1;	// ����������
	/**
	** װ��/����
	**/
	
	const int MAX_GEM_ARRT_COUNT = 4;	// ��������

};

DECLARE_SIMPLE_TYPE(KItemAbout::KAliveTimeItemInfo)
