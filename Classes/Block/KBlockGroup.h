/********************************************************************
created:	2011/8/11
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include "IBlock.h"
#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"
//#include "Base/KLogicMsgParam.h"

namespace KBlockAbout
{
	class KBlockGroupManager
	{
	public:
		KBlockGroupManager();
	public:
		/**
		 * ����һ��IBlock
		 */
		void OnAddBlock(IBlock* pBlock);

		/**
		 * ɾ��һ��IBlock
		 */
		void OnDelBlock(IBlock* pBlock, bool bAnnounceGroupChange = true);

		/**
		 * �¼�����CD�ı�
		 */
		void OnMainPlayerCDChanged(int cdgroup);

		/**
		 * �¼�����Skill�����ı�
		 */
		
		/**
		 * �¼�����CD�ı�
		 */
		void OnCarrierCDChanged(int cdgroup);

		/**
		 * �¼�����Skill�����ı�
		 */
		void OnCarrierSkillChanged(int skillID);

		/**
		 * �¼�����Item�����ı�
		 */
		void OnMainPlayerItemChanged(int itemID);

		/**
		 * �¼�����ĳ����Block���
		 */
		void OnBlockChanged(IBlock* pBlock);

		/**
		 * �¼��������Ƿ��������滻
		 */
		/**
		 * �¼�������ҵȼ������ı�
		 */
 		void OnMainPlayerItemUseChanged();

		void Reset()
		{
			m_groupMap.clear();
		}

	private:
		// ÿ����block������
		typedef System::Collections::DynSortedArray<IBlock*> BLOCK_GROUP;
		// ��idΪ���������б�
		typedef System::Collections::KMapByVector<int, BLOCK_GROUP> BLOCK_GROUP_MAP;
		// ���е���map
		typedef System::Collections::KMapByVector<int, BLOCK_GROUP_MAP> ALL_GROUP;
		ALL_GROUP m_groupMap;

		// ���е���������б�
		enum GROUP_MAP_INDEX
		{
			enumGMI_MainPlayerCDGroup = 0,
			enumGMI_MainPlayerSkillGroup,
			enumGMI_MainPlayerItemGroup,

			enumGMI_CarrierCDGroup,
			enumGMI_CarrierSkillGroup,
			enumGMI_MainPlayerItemUseGroup,		// ��Ʒ�Ƿ����(�жϵȼ���ְҵ�Ƿ����)
		};
		// ������
		void _Insert(int type, int group, IBlock* pToInsert);
		// ������ɾ��
		void _Remove(int type, int group, IBlock* pToRemove);

		// ֪ͨ��������block���
		void _AnnounceGroupChange(int type, int group);
	};
}