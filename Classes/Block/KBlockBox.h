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
		// 得到目标位置的block信息
		IBlock* GetBlock(int pos);
		const IBlock* GetBlock(int pos) const;
		// 设置一个block到指定位置
		IBlock* SetBlock(int pos, IBlock* toSet);
		// 找第一个空位置塞入一个block
		bool PushBlock(IBlock* toPush);
		// 得到已经使用的block个数
		int GetUsedBlockSize() const;
		// 得到剩余的block个数
		int GetRemainBlockSize() const;
		// 交换两个东西
		void Switch(int posA, int posB);
		// 设定最大格子数
		void SetMaxBlockSize(int maxBlockSize);
		// 得到最大格子数
		int GetMaxBlockSize() const;
		void Clear(bool bAnnounceGroupChange = true);

	public:
		// box内容
		MAP_TYPE m_blockMap;

	private:
		// 最大格子数
		int m_maxBlockSize;
		int m_boxID;
	};
}