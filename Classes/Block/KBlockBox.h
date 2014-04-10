/********************************************************************
created:	2011/8/3
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "IBlock.h"
#include "System/Collections/KMapByVector.h"
#include "System/Memory/KStepObjectPool.h"

namespace KBlockAbout
{
	class KBlockBox
		: public System::Memory::KPortableMemoryPool<KBlockBox>
	{
	public:
		typedef System::Collections::KMapByVector<int, IBlock*> MAP_TYPE;
		int GetBoxID() const
		{
			return m_boxID;
		}
		void SetBoxID(int boxID)
		{
			m_boxID = boxID;
		}
		// �õ�Ŀ��λ�õ�block��Ϣ
		IBlock* GetBlock(int pos);
		const IBlock* GetBlock(int pos) const;
		// ����һ��block��ָ��λ��
		IBlock* SetBlock(int pos, IBlock* toSet);
		// �ҵ�һ����λ������һ��block
		bool PushBlock(IBlock* toPush);
		// �õ��Ѿ�ʹ�õ�block����
		int GetUsedBlockSize() const;
		// �õ�ʣ���block����
		int GetRemainBlockSize() const;
		// ������������
		void Switch(int posA, int posB);
		// �趨��������
		void SetMaxBlockSize(int maxBlockSize);
		// �õ���������
		int GetMaxBlockSize() const;
		void Clear(bool bAnnounceGroupChange = true);

	public:
		// box����
		MAP_TYPE m_blockMap;

	private:
		// ��������
		int m_maxBlockSize;
		int m_boxID;
	};
}