/********************************************************************
created:	2011/8/4
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "IBlock.h"
#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"
//#include "Base/KLogicMsgParam.h"
#include "KBlockGroup.h"
#include "Inc/Lua/KLuaWrap.h"

namespace KBlockAbout
{
	class KBlockBox;
	class KBlockGroupManager;
	class KBlockBoxManager
		: public System::Patterns::KSingleton<KBlockBoxManager>
	{
	public:
		KBlockBoxManager();
		// ����һ��Box
		bool CreateBlockBox(int boxID, int maxSize);
// 		bool CreateBpMatBlockBox(int nMatBoxID);
		bool DeleteBlockBox(int nBoxID);
		bool ClearBlockBox(int nBoxID);
		// ִ����ק
		bool Drag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// ִ����ק
		bool DragOverNotDstPos(int fromBoxID, int fromPos, int toBoxID);
		// �õ�Block
		const IBlock* GetBlock(int boxID, int pos) const;
		// �õ�Block����const����
		IBlock* GetBlock(int boxID, int pos);
		// ����
		bool Drop(int boxID, int pos);
		// ����
		void Breathe();
		// ���ݵ�ǰ��ҵ�״̬��ʼ��BlockManager
		void Build(const void* shortcutBuffer, int bufferSize);
		// ����һ���µ�block
		void PutBlock(int boxID, int pos, IBlock* pBlock);

		/**
		 * �¼�����CD�ı�
		 */
		void OnMainPlayerCDChanged(int cdgroup);

	

		/**
		 * �¼�����Item�����ı�
		 */
		void OnMainPlayerItemChanged(int itemID);

		/**
		 * �¼�����block�����ı�
		 */
		void OnBlockInfoChanged(int boxID, int pos);

	
		// Block�ǵ���0��װ��1������2
		int GetBlockStandFor(int boxID, int pos);

		// ������Ʒ��λ�øĶ�ʱ��Ҫ������������Ѷ�Ӧ��itemlink���͵�block������λ�������ı�
		void OnItemBlockChanged(int fromBoxID, int toBoxID, int fromPos, int toPos);

		void OnShortcutBlockChanged(int fromBox, int fromPos, int toBox, int toPos);
		/**
		 * �¼�������ҵȼ��ı�
		 */
		void OnPlayerLevelChanged();
		/**
		 * �¼�����block����״̬�����ı�
		 */
		void OnItemLockStatusChanged(int boxID, int pos);

		void Reset();
	
	
		void UseSlot(int boxID, int pos, float x, float y, bool bOnlyUse, bool bOnlyPrepare);
		void PostUseSlot(int boxID, int pos);

		// ��Ʒ��ק
		bool ItemDrag(int fromBoxID, int fromPos, int toBoxID, int toPos, bool bIgnore = false);

	private:
		// ����һ��Block
		IBlock* SetBlock(int boxID, int pos, IBlock* pBlock);
		// ������ק
		bool SwitchDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// ����������ק
		bool ItemSumDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// ��Ʒ������ק
		bool ItemLinkDrag(int fromBoxID, int fromPos, int toBoxID, int toPos);
		// ����һ��������
		IBlock* CopyLink(const IBlock* pB);
		// ���ı���Ϣ
		void PushChangeInfo(int boxID, int pos);
		// ���ı���Ϣ(CD)
		void PushChangeInfo_CD(int boxID, int pos);
		/**
		** ��ҵȼ��ı���Ϣ
		**/
		void PushChangeInfo_PlayerItemUse(int boxID, int pos);
		/**
		** block lock�ı���Ϣ
		**/
		void PushChangeInfo_LockStatus(int boxID, int pos);
		void _OnItemBlockChanged(int fromBoxID, int toBoxID, int checkBoxID, int fromPos, int toPos);

		friend class KBlockGroupManager;
		friend class KMainPlayerSkillBlock;
		friend class KMainPlayerSkillLinkBlock;
		friend class KItemBlock;
		friend class KItemLinkBlock;
		friend class KItemSumBlock;

	public:
		// ɾ��һ��block
		void DestroyBlock(IBlock* pB, bool bAnnounceGroupChange = true);

	private:
		typedef System::Collections::KMapByVector<int, KBlockBox*> MAP_TYPE;
		MAP_TYPE m_boxMap;

		KBlockChangeList m_blockChangeList;
		KBlockChangeList m_blockCDChangeList;
		KBlockChangeList m_blockPlayerItemUseChangeList;
		KBlockChangeList m_blockLockStatusChangeList;

		KBlockGroupManager m_blockGroup;
		
		int m_nDragTmpBagPos;	// ��ק��ʯ���кϳ�/���ʱ���ڼ�¼��ʯ�ڱ���λ��

		// ��¼������ϼ����Ƿ����
		bool m_bShortCutUsable[12];

		const IBlock* m_BreathBlock;
	public:
	/*	BeginDefLuaClass(KBlockBoxManager)
			DefMemberFunc(GetBlockStandFor)
		EndDef*/
	};
}