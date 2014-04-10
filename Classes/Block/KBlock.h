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
	 * ��Ʒblock
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

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ʹ��
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		// �õ�Block����Ʒ�����ñ���Ϣ
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
	
		int GetBagID() const;

		DWORD GetItemID() const
		{
			return m_ItemID;
		}
	

		virtual void SetPos(int boxID, int pos)
		{
			IBlock::SetPos(boxID, pos);

			// ���������ʱ��GetItemCreateInfo()��GetSkillInfo()�϶��ǻ���óɹ��ģ����ᵣ����Ʒ���ڵ����
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
	 * ��Ʒ����block
	 */
	class KItemLinkBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KItemLinkBlock>
	{
	public:
		KItemLinkBlock();

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemLinkBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ʹ��
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;

		// �õ�Block����Ʒ�����ñ���Ϣ
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// �õ�Block�����ļ�����Ϣ
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
	 * ��Ʒ����block
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

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemSumBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const{return 0;}
		// �õ��ѵ���
		virtual int GetCount() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ʹ��
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;

		// �õ�Block����Ʒ�����ñ���Ϣ
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// �õ�Block�����ļ�����Ϣ

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
		int GetLinkPos() const;	// ��������һ��

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
	// purpose	: ���丽��block
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

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemReaderAttachBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		// �õ�Block����Ʒ�����ñ���Ϣ
		const KItemAbout::KCreateInfo_ItemBase* GetItemCreateInfo() const;
		// �õ�Block�����ļ�����Ϣ

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

			// ���������ʱ��GetItemCreateInfo()��GetSkillInfo()�϶��ǻ���óɹ��ģ����ᵣ����Ʒ���ڵ����
			m_ItemID = GetItemCreateInfo()->GetItemID();
		
		}

		virtual bool Release();

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};

	// �����жԷ���ƷBlock
	class KBusinessBlock : public IBlock, public System::Memory::KPortableMemoryPool<KBusinessBlock>
	{
	public:
		KBusinessBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemBusinessBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		// �õ�Block����Ʒ�����ñ���Ϣ
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

			// ���������ʱ��GetItemCreateInfo()��GetSkillInfo()�϶��ǻ���óɹ��ģ����ᵣ����Ʒ���ڵ����
			m_ItemID = GetItemCreateInfo()->GetItemID();
		}

		// �Ƿ������ҷ
		virtual bool GetDragable() const {return false;}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};


	// �̵���ƷBlock
	class KStoreBuyBackBlock : public IBlock, public System::Memory::KPortableMemoryPool<KStoreBuyBackBlock>
	{
	public:
		KStoreBuyBackBlock()
		{
			m_ItemID = 0;
			m_SkillGroup = 0;
		}

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemStoreBuyBackBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		// �õ�Block����Ʒ�����ñ���Ϣ
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

			// ���������ʱ��GetItemCreateInfo()��GetSkillInfo()�϶��ǻ���óɹ��ģ����ᵣ����Ʒ���ڵ����
			m_ItemID = GetItemCreateInfo()->GetItemID();
	
		}

		// �Ƿ������ҷ
		virtual bool GetDragable() const {return false;}
		virtual bool Release();
		virtual bool IsUseStatus() const;
		virtual int GetQuality() const;

	private:
		DWORD m_ItemID;
		int m_SkillGroup;
	};

	// �̵���ƷBlock
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

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_ItemStoreBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		// �õ�Block����Ʒ�����ñ���Ϣ
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

			// ���������ʱ��GetItemCreateInfo()��GetSkillInfo()�϶��ǻ���óɹ��ģ����ᵣ����Ʒ���ڵ����
			m_ItemID = GetItemCreateInfo()->GetItemID();
		}
		
		virtual const char* GetName() const;

		// �Ƿ������ҷ
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
		int m_nLimitSaleCount;				// һ���ܹ���������
	};

	/**
	 * ������������Ʒ����block
	 */
	class KSingleItemLinkBlock
		: public IBlock
		, public System::Memory::KPortableMemoryPool<KSingleItemLinkBlock>
	{
	public:
		KSingleItemLinkBlock();

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_SignleItemLinkBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const;
		// �õ�block״̬
		virtual int GetStatus() const;
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ʹ��
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const;
		// �õ��ѵ���
		virtual int GetCount() const;
		// �Ƿ���ʾʹ�ô����������Ƕѵ���
		virtual bool IsFrequency() const;
		// �õ�ʹ�ô���
		virtual int GetFrequency() const;

		// �õ�Block����Ʒ�����ñ���Ϣ
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
	 * ��Ʒ����block
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

		// �õ�ʵ������
		virtual int GetType() const
		{
			return enumBT_BagBlock;
		}
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const;
		virtual bool GetCDTime(int& passedTime, int& totalTime) const { return false; }
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const;
		// ������֮ǰ����������Ϣ
		virtual void OnDestroy();

		virtual bool GetDragable() const { return m_dragable; }

		// �õ�Block����Ʒ�����ñ���Ϣ
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
