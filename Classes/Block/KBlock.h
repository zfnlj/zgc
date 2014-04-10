/********************************************************************
created:	2011/8/3
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "IBlock.h"
#include "System/Memory/KStepObjectPool.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "CommonLogic/KMoney.h"


namespace KItemAbout
{
	struct KCreateInfo_ItemBase;
}

namespace KWorldObjAbout
{
	class KBattlePetCreateInfo;
}

namespace KBlockAbout
{

	/**
	 * 物品block
	 */
	class KItemBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KItemBlock>
	{
	public:
		KItemBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 使用
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
	
		int GetBagID() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
	

		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
			m_ItemID = GetItemCreateInfo()->GetItemID();
			
		}

		virtual bool IsUseStatus() const;
		virtual bool Release();
		virtual bool GetDragable() const;
		virtual const char* GetName() const;

		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};

	/**
	 * 物品链接block
	 */
	class KItemLinkBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KItemLinkBlock>
	{
	public:
		KItemLinkBlock();

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemLinkBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 使用
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// 得到Block关联的技能信息
		void SetItemBlock(int BoxID, int Pos);

		int GetLinkBoxID() const;
		int GetLinkPos() const;
		void SetLinkPos(int Pos);

		virtual bool Exist()const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual const char* GetName() const;

	protected:
		int m_LinkBoxID;
		int m_LinkPos;

		DWORD m_ItemID;
		int m_SkillGroup;
	};

	/**
	 * 物品汇总block
	 */
	class KItemSumBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KItemSumBlock>
	{
	public:
		KItemSumBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemSumBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const{return 0;}
		// 得到堆叠数
		virtual int GetCount() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 使用
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// 得到Block关联的技能信息

		void SetItemID(int ItemID);
		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}

		void SetBagType(int nBoxID);
		int GetBagID() const { return m_BagType;} 
		
		int GetLinkBoxID() const;
		int GetLinkPos() const;	// 返回其中一个

		virtual bool Exist()const;
		virtual bool Release();
		virtual const char* GetName() const;

	protected:
		DWORD m_ItemID;
		KItemAbout::ENUM_ITEM_BAG_TYPE m_BagType;

		int m_SkillGroup;
	};

	/**************************************************************** 
	// Class 	: KAttachBlock
	// Author	: ShiLei
	// purpose	: 邮箱附件block
	// Create	: 2012-8-2 10:03
	****************************************************************/ 
	class KAttachBlock : public IBlock, public System::Memory::KPortableMemoryPool<KAttachBlock>
	{
	public:
		KAttachBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemReaderAttachBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// 得到Block关联的技能信息

		int GetBagID() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}
		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
			m_ItemID = GetItemCreateInfo()->GetItemID();
		
		}

		virtual bool Release();

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};

	// 交易中对方物品Block
	class KBusinessBlock : public IBlock, public System::Memory::KPortableMemoryPool<KBusinessBlock>
	{
	public:
		KBusinessBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemBusinessBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}
		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
			m_ItemID = GetItemCreateInfo()->GetItemID();
		}

		// 是否可以拖曳
		virtual bool GetDragable() const {return false;}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};


	// 商店物品Block
	class KStoreBuyBackBlock : public IBlock, public System::Memory::KPortableMemoryPool<KStoreBuyBackBlock>
	{
	public:
		KStoreBuyBackBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemStoreBuyBackBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}

		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
			m_ItemID = GetItemCreateInfo()->GetItemID();
	
		}

		// 是否可以拖曳
		virtual bool GetDragable() const {return false;}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};

	// 商店物品Block
	class KStoreBlock : public IBlock, public System::Memory::KPortableMemoryPool<KStoreBlock>
	{
	public:
		KStoreBlock()
		{
			m_ItemID = 0;
			m_nLableID = -1;
			m_nProductID = -1;
			m_SkillGroup = 0;
			m_moneyPrice.m_money = 0;
			m_nLimitSaleCount = 0;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_ItemStoreBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;

		int GetBagID() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}
		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// 调用这个的时候，GetItemCreateInfo()和GetSkillInfo()肯定是会调用成功的，不会担心物品不在的情况
			m_ItemID = GetItemCreateInfo()->GetItemID();
		}
		
		virtual const char* GetName() const;

		// 是否可以拖曳
		virtual bool GetDragable() const {return true;}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetCost() const;

		void SetProductItem(int nLableID, int nProductID, int nPrice, int nLimitSaleCount);
		int GetLableID() const;
		int GetProductID() const;
		int GetLimitSaleCount() const;
		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_nLableID;
		int m_nProductID;
		int m_SkillGroup;
		KMoneyAbout::KMoney m_moneyPrice;
		int m_nLimitSaleCount;				// 一次能购买的最大数
	};

	/**
	 * 单个格子中物品链接block
	 */
	class KSingleItemLinkBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KSingleItemLinkBlock>
	{
	public:
		KSingleItemLinkBlock();

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_SignleItemLinkBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		// 得到CD时间
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// 得到block状态
		virtual int GetStatus() const;
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 使用
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// 得到堆叠数
		virtual int GetCount() const;
		// 是否显示使用次数，而不是堆叠数
		virtual bool IsFrequency() const;
		// 得到使用次数
		virtual int GetFrequency() const;

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;

		void SetItemBlock(int BoxID, int Pos);

		int GetLinkBoxID() const;
		int GetLinkPos() const;
		void SetLinkPos(int Pos);

		virtual bool Exist()const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
		int GetSkillGroup()
		{
			return m_SkillGroup;
		}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetQuality() const;

	protected:
		int m_LinkBoxID;
		int m_LinkPos;

		DWORD m_ItemID;
		int m_SkillGroup;
	};
	/**
	 * 物品背包block
	 */
	class KBagBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KBagBlock>
	{
	public:
		KBagBlock()
		{
			m_ItemID = 0;
			m_dragable = false;
		}

		// 得到实际类型
		virtual int GetType() const
		{
			return enumBT_BagBlock;
		}
		// 得到图标
		virtual bool GetIcon(void* buffer, int size) const;
		virtual bool GetCDTime(int& passedTime, int& totalTime) const { return false; }
		// 得到tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// 被销毁之前，处理丢弃消息
		virtual void OnDestroy();

		virtual bool GetDragable() const { return m_dragable; }

		// 得到Block中物品的配置表信息
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}

		virtual void SetPos(int boxID, int pos);

		virtual bool Release();

	private:
		DWORD	m_ItemID;
	};

}
