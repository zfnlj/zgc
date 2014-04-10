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
	 * ����ÿ��Block���ɲ����Ľӿ�
	 */
	class IBlock
	{
	protected:
		int m_boxID;		// Box ID(��������ʹ��)
		int m_posInfo;		// λ��ID
		bool m_dragable;	// �Ƿ���϶�
		int m_status;		// Block״̬����
		bool m_bLock;		// ����״̬

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
		// �õ�ʵ������
		virtual int GetType() const = 0;
		// �õ�ͼ��
		virtual bool GetIcon(void* buffer, int size) const = 0;
		// �õ�CDʱ��
		virtual bool GetCDTime(int& passedTime, int& totalTime) const = 0;
		// �õ�block״̬
		virtual int GetStatus() const { return m_status; }
		virtual void SetStatus(int status) { m_status = status; }
		// �õ��ѵ���
		virtual int GetCount() const { return 0; }
		// �õ�ʹ�ô���
		virtual bool IsFrequency() const { return false; }
		// �õ�ʹ�ô���
		virtual int GetFrequency() const { return 0; }
		// �õ�tooltip
		virtual bool GetTooltip(void* buffer, int size) const = 0;
		// ʹ��(��Ҫʱ��������Ӧ�ڰ����¼��İ��£����Ų�����̧��)
		virtual void Use(float x, float y, bool bOnlyUse, bool bOnlyPrepare) const{}
		virtual void PostUse() const{};
		// �ڴ��ͷ�
		virtual bool Release() = 0;

		// �Ƿ������ҷ
		virtual bool GetDragable() const
		{ 
			return m_dragable; 
		}
		// ������ҷ
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
		// ������ҷ
		virtual void SetLock(bool bLock)
		{
			m_bLock = bLock;
		}
		virtual const char* GetName() const { return ""; }
		virtual int GetCost() const { return 0; }

		virtual int GetQuality() const{ return 0; }

	protected:
		// ����
		virtual void Switch(IBlock* other)
		{
			int temp = m_boxID;
			m_boxID = other->m_boxID;
			other->m_boxID = m_boxID;
			temp = m_posInfo;
			m_posInfo = other->m_posInfo;
			other->m_posInfo = m_posInfo;
		}
		// ����ǰ�Ĵ���
		virtual void OnDestroy() {};
		// ���л�
		virtual int Serilize(void* buffer, int size){return 0;};
		// �����л�
		virtual bool Unserilize(const void* buffer, int size){return false;};

		friend class KBlockBoxManager;
	};

	// ����UI Block�������͵�ö��
	enum BLOCK_TYPE
	{
		enumBT_MainPlayerSkillBlock = 1,		// �������ڣ����Ǽ���block
		enumBT_MainPlayerSkillLinkBlock,		// ������ڣ��������Ǽ���block
		enumBT_CarrierSkillBlock,				// �ؾ߼���block
		enumBT_ItemBlock,						// ��Ʒblock
		enumBT_ItemLinkBlock,					// ��Ʒ����block
		enumBT_ItemSumBlock,					// ������Ʒ����block
		enumBT_ItemAttachBlock,					// ���丽��block
		enumBT_ItemReaderAttachBlock,			// ���丽��block
		enumBT_ItemBusinessBlock,				// ����block
		enumBT_ItemStoreBlock,					// NPC�̵�block
		enumBT_ItemStoreBuyBackBlock,			// NPC�̵�ع�block
		enumBT_SignleItemLinkBlock,				// ���������ڵ���Ʒ����block
		enumBT_OtherStallBlock,					// ��̯��ҵ���Ʒ
		enumBT_StallBlock,						// ��Ұ�̯����Ʒ
		enumBT_BagBlock,						// ��Ʒ����block
		enumBT_CatchItemBlock,					// ץ����Ʒblock
		enumBT_DropGroundItemBlock,				// ��������Ʒblock
	};

	// Block״̬����
	enum BLOCK_STATUS
	{
		enumBS_CanNotUse = -1,	// �޷�ʹ��
		enumBS_Normal = 0,		// ����״̬
		enumBS_AutoFire,		// �Զ�ʩ��
	};

	// UI ������ID(һ������Ӧһ��ID)
	enum BOX_TYPE_ID
	{
		enumBTID_Shoutcut			= 1,	// �����
		enumBTID_Carrier			= 2,	// �ؾ�
		enumBTID_DirectUse			= 3,

		enumBTID_ItemNormalBox		= 100,	// ��Ʒ��ͨ����
		enumBTID_ItemQuestBox		= 102,	// ��Ʒ���񱳰�
		enumBTID_EquipBox			= 150,	// װ����


		enumBTTD_MailAttachment = 401,			// �ʼ�����
		enunBTTD_MailReaderAttach = 402,		// �ʼ�����������
		
		enumBTID_BusinessOtherBox = 500,		// �Է����״���
		enumBTID_BusinessSelfBox = 501,			// �Լ����״���
		enumBTID_StoreBox = 510,				// NPC���״���
		enumBTID_StoreBuyBackBox = 511,			// �ع��̿ڴ���

		enumBTID_MillBagNormalBox = 601,		// ������ͨ�ֿ�
		enumBTID_MillBagMaterialBox = 602,		// �������ϲֿ�
		enumBTID_MillBagEquipmentBox = 603,		// ����װ�òֿ�
		enumBTID_MillExploreBox = 604,			// ����̽��block
		enumBTID_MillSumExploreBox = 605,		// ����̽��ͳ��block
		enumBTID_MillBackExploreBox = 606,		// ����̽�ջ���block
		enumBTID_MillHatchBox = 607,			// ��������Box

		enumBTID_MyselfStallBox = 651,			// ��ҵİ�̯����block
		enumBTID_OtherStallBox = 652,			// ��̯��ұ���block
		enumBTID_CatchBox = 660,				// ץ�����block
		enumBTID_DropGroundBox = 670,			// ���������block
		
		enumBTID_ItemHatchBattlePetBox = 700,	// �������block

		enumBTID_CookMakeBox = 800,				// �����������block
	};

	const static int BOXSIZE_Shoutcut = 12;	
	const static int BOXSIZE_Carrier = 4;
	const static int BOXSIZE_ItemNormalBox = 60;
	const static int BOXSIZE_ItemQuestBox = 60;
	const static int BOXSIZE_EquipBox = 50;


	const static int BOXSIZE_MailAttachBox = 5;							//  �ʼ�����
	const static int BOXSIZE_BusinessBox = BUSINESS_POS_MAX;			// ��������������
	const static int BOXSIZE_StoreBox = STORE_STOREPOS_MAX;				// ��������������
	const static int BOXSIZE_StoreBuyBackBox = STORE_BUYBACKPOS_MAX;	// �ع��̿ڴ��ڸ�������
	
	const static int BOXSIZE_CatchBox = 2;								//  ץ��block��������
	const static int BOXSIZE_DropGroundBox = 9;							//  ���������block��������
	const static int BOXSIZE_DirectUseBox = 2;
}


// ��ı���Ϣ�ṹ��
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
