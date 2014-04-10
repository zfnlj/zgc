#include "KBlockBox.h"
#include "KBlockBoxManager.h"

namespace KBlockAbout
{
	IBlock* KBlockBox::GetBlock( int pos )
	{
		MAP_TYPE::iterator it = m_blockMap.find(pos);
		if (it != m_blockMap.end())
		{
			return it->second;
		}
		return NULL;
	}

	const IBlock* KBlockBox::GetBlock( int pos ) const
	{
		MAP_TYPE::const_iterator it = m_blockMap.find(pos);
		if (it != m_blockMap.end())
		{
			return it->second;
		}
		return NULL;
	}

	IBlock* KBlockBox::SetBlock( int pos, IBlock* toSet )
	{
		IBlock* toReturn = NULL;

		if (pos < 0 || pos >= m_maxBlockSize)
		{
			ASSERT_I(false);
			return NULL;
		}

		MAP_TYPE::iterator it = m_blockMap.find(pos);
		if (it != m_blockMap.end())
		{
			toReturn = it->second;
			m_blockMap.erase(it);
		}

		if (toSet)
		{
			toSet->SetPos(m_boxID, pos);
			m_blockMap[pos] = toSet;
		}

		return toReturn;
	}

	bool KBlockBox::PushBlock( IBlock* toPush )
	{
		if (!toPush) return true;
		int pos = 0;
		if (GetRemainBlockSize() <= 0) return false;
		for (MAP_TYPE::iterator it = m_blockMap.begin(); it != m_blockMap.end(); ++it)
		{
			if (it->first > pos) break;
			pos++;
		}
		if (pos < m_maxBlockSize)
		{
			SetBlock(pos, toPush);
			return true;
		}
		ASSERT_I(false);
		return false;
	}

	int KBlockBox::GetUsedBlockSize() const
	{
		return (int)m_blockMap.size();
	}

	int KBlockBox::GetRemainBlockSize() const
	{
		return m_maxBlockSize - (int)m_blockMap.size();
	}

	void KBlockBox::Switch( int posA, int posB )
	{
		IBlock* ba = GetBlock(posA);
		IBlock* bb = GetBlock(posB);
		if (!ba && !bb) return;
		SetBlock(posB, ba);
		SetBlock(posA, bb);
	}

	void KBlockBox::SetMaxBlockSize( int maxBlockSize )
	{
		m_maxBlockSize = maxBlockSize;
	}

	int KBlockBox::GetMaxBlockSize() const
	{
		return m_maxBlockSize;
	}

	void KBlockBox::Clear(bool bAnnounceGroupChange )
	{
		MAP_TYPE::iterator itorTmp = m_blockMap.begin();
		for(;itorTmp!=m_blockMap.end();++itorTmp)
		{
			IBlock* pBlock = itorTmp->second;
			if(pBlock)
			{
				KBlockBoxManager::Instance()->DestroyBlock(pBlock, bAnnounceGroupChange);
			}
		}
		m_blockMap.clear();
	}
}
