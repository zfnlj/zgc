/********************************************************************
created:	2011/8/3
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once

#include "Inc/KBusinessDefine.h"
#include "CommonLogic/Store/KStoreDef.h"
#include "System/KMacro.h"
#include "System/Collections/DynArray.h"
namespace KBlockAbout
{
	/**
	 * 定义每个Block外界可操作的接口
	 */
	class IBlock
	{
	protected:
		int m_boxID;		// Box ID(管理类中使用)
		int m_posInfo;		// 位置ID
		bool m_dragable;	// 是否可拖动
		int m_status;		// Block状态定义
		bool m_bLock;		// 锁定状态

	public:
		IBlock()
		{
			m_dragable = true;
			m_status = 0/*enumBS_Normal*/;
			m_bLock = false;
		}
		virtual ~IBlock(){}
	public:
		virtual void SetPos(int boxID, int pos)
		{
			m_boxID = boxID;
			m_posInfo = pos;
		}
		virtual int GetBoxID() const
		{
			return m_boxID;
		}
		virtual int GetPosInfo() const
		{
			return m_posInfo;
		}
		// 得到实际类型
		virtual int GetType() const = 0;
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const = 0;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const = 0;
		// 得到block状态
		virtual int GetStatus() const { return m_status; }
		virtual void SetStatus(int status) { m_status = status; }
		// 得到堆叠数
		virtual int GetCount() const { return 0; }
		// 得到使用次数
		virtual bool IsFrequency() const { return false; }
		// 得到使用次数
		virtual int GetFrequency() const { return 0; }
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const = 0;
		// 使用(需要时这三个对应于按键事件的按下，按着不动，抬起)
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const{}
		virtual void PostUse() const{};
		// 内存释放
		virtual bool Release() = 0;

		// 是否可以拖曳
		virtual bool GetDragable() const
		{ 
			return m_dragable; 
		}
		// 设置拖曳
		virtual void SetDragable(bool dragable)
		{
			m_dragable = dragable;
		}

		virtual bool Exist() const{ return true;}
		virtual bool IsUseStatus() const {return false;}
		virtual bool GetLock() const
		{ 
			return m_bLock; 
		}
		// 设置拖曳
		virtual void SetLock(bool bLock)
		{
			m_bLock = bLock;
		}
		virtual const char* GetName() const { return ""; }
		virtual int GetCost() const { return 0; }

		virtual int GetQuality() const{ return 0; }

	protected:
		// 交换
		virtual void Switch(IBlock* other)
		{
			int temp = m_boxID;
			m_boxID = other->m_boxID;
			other->m_boxID = m_boxID;
			temp = m_posInfo;
			m_posInfo = other->m_posInfo;
			other->m_posInfo = m_posInfo;
		}
		// 销毁前的处理
		virtual void OnDestroy() {};
		// 序列化
		virtual int Serilize(void* buffer, int size){return 0;};
		// 反序列化
		virtual bool Unserilize(const void* buffer, int size){return false;};

		friend class KBlockBoxManager;
	};

	// 定义UI Block各种类型的枚举
	enum BLOCK_TYPE
	{
		enumBT_MainPlayerSkillBlock = 1,		// 技能书内，主角技能block
		enumBT_MainPlayerSkillLinkBlock,		// 快捷栏内，引用主角技能block
		enumBT_CarrierSkillBlock,				// 载具技能block
		enumBT_ItemBlock,						// 物品block
		enumBT_ItemLinkBlock,					// 物品链接block
		enumBT_ItemSumBlock,					// 背包物品汇总block
		enumBT_ItemAttachBlock,					// 邮箱附件block
		enumBT_ItemReaderAttachBlock,			// 邮箱附件block
		enumBT_ItemBusinessBlock,				// 交易block
		enumBT_ItemStoreBlock,					// NPC商店block
		enumBT_ItemStoreBuyBackBlock,			// NPC商店回购block
		enumBT_SignleItemLinkBlock,				// 单个格子内的物品链接block
		enumBT_OtherStallBlock,					// 摆摊玩家的物品
		enumBT_StallBlock,						// 玩家摆摊的物品
		enumBT_BagBlock,						// 物品背包block
		enumBT_CatchItemBlock,					// 抓捕物品block
		enumBT_DropGroundItemBlock,				// 掉落物物品block
	};

	// Block状态定义
	enum BLOCK_STATUS
	{
		enumBS_CanNotUse = -1,	// 无法使用
		enumBS_Normal = 0,		// 正常状态
		enumBS_AutoFire,		// 自动施放
	};

	// UI 中面板的ID(一个面板对应一个ID)
	enum BOX_TYPE_ID
	{
		enumBTID_Shoutcut			= 1,	// 快捷栏
		enumBTID_Carrier			= 2,	// 载具
		enumBTID_DirectUse			= 3,

		enumBTID_ItemNormalBox		= 100,	// 物品普通背包
		enumBTID_ItemQuestBox		= 102,	// 物品任务背包
		enumBTID_EquipBox			= 150,	// 装备栏


		enumBTTD_MailAttachment = 401,			// 邮件附件
		enunBTTD_MailReaderAttach = 402,		// 邮件附件读窗口
		
		enumBTID_BusinessOtherBox = 500,		// 对方交易窗口
		enumBTID_BusinessSelfBox = 501,			// 自己交易窗口
		enumBTID_StoreBox = 510,				// NPC交易窗口
		enumBTID_StoreBuyBackBox = 511,			// 回购商口窗口

		enumBTID_MillBagNormalBox = 601,		// 工坊普通仓库
		enumBTID_MillBagMaterialBox = 602,		// 工坊材料仓库
		enumBTID_MillBagEquipmentBox = 603,		// 工坊装置仓库
		enumBTID_MillExploreBox = 604,			// 工人探险block
		enumBTID_MillSumExploreBox = 605,		// 工人探险统计block
		enumBTID_MillBackExploreBox = 606,		// 工人探险回收block
		enumBTID_MillHatchBox = 607,			// 工坊孵化Box

		enumBTID_MyselfStallBox = 651,			// 玩家的摆摊背包block
		enumBTID_OtherStallBox = 652,			// 摆摊玩家背包block
		enumBTID_CatchBox = 660,				// 抓捕面板block
		enumBTID_DropGroundBox = 670,			// 掉落物面板block
		
		enumBTID_ItemHatchBattlePetBox = 700,	// 孵化面板block

		enumBTID_CookMakeBox = 800,				// 料理制作面板block
	};

	const static int BOXSIZE_Shoutcut = 12;	
	const static int BOXSIZE_Carrier = 4;
	const static int BOXSIZE_ItemNormalBox = 60;
	const static int BOXSIZE_ItemQuestBox = 60;
	const static int BOXSIZE_EquipBox = 50;


	const static int BOXSIZE_MailAttachBox = 5;							//  邮件附件
	const static int BOXSIZE_BusinessBox = BUSINESS_POS_MAX;			// 交易面板格子总数
	const static int BOXSIZE_StoreBox = STORE_STOREPOS_MAX;				// 交易面板格子总数
	const static int BOXSIZE_StoreBuyBackBox = STORE_BUYBACKPOS_MAX;	// 回购商口窗口格子总数
	
	const static int BOXSIZE_CatchBox = 2;								//  抓捕block格子总数
	const static int BOXSIZE_DropGroundBox = 9;							//  掉落物面板block格子总数
	const static int BOXSIZE_DirectUseBox = 2;
}


// 块改变消息结构体
struct KBlockPosInfo
{
	int boxID;
	int posID;
	bool operator<(const KBlockPosInfo& o) const
	{
		if (boxID < o.boxID) return true;
		else if (boxID == o.boxID) return posID < o.posID;
		return false;
	}
};

DECLARE_SIMPLE_TYPE(KBlockPosInfo);
typedef ::System::Collections::DynSortedArray<KBlockPosInfo> KBlockChangeList;
