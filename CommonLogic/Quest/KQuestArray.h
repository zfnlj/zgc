#ifndef _K_QUEST_ARRAY_H_
#define _K_QUEST_ARRAY_H_

#include <System/KType.h>
#include <System/KMacro.h>
#include <System/Collections/DynArray.h>
#include <System/Memory/KStepObjectPool.h>

struct quest_array_header
{
	int count;
	int qids[1];
	/////////////////////////////////////////////////////////////////
	quest_array_header():count(0)
	{

	}
	size_t size() const
	{
		return offsetof(quest_array_header,qids) + sizeof(int)*count;
	}
};

template <size_t capacity> struct quest_array_buffer
{
	quest_array_header header;
	int buffer[capacity-1];
};

class KQuestArray
{
public:
	quest_array_header* m_header;
	System::Collections::DynSortedArray<int> m_arr;
	typedef quest_array_buffer<256> buffer_type;
	typedef System::Memory::KStepObjectPool<buffer_type,512> buffer_pool;
private:
	int m_capacity;	// 每个npc最大关联任务个数, 即buffer_type的capacity

protected:
	KQuestArray():m_header(NULL),m_capacity(0)
	{

	}
	KQuestArray(const KQuestArray& o)
	{

	}
	KQuestArray& operator = (const KQuestArray& o)
	{

	}

public:
	KQuestArray(quest_array_header* header):m_header(header),m_capacity(0)
	{
		m_arr.attach(&m_header->qids[0], m_header->count, m_header->count);
	}
	template <size_t capacity> KQuestArray(quest_array_buffer<capacity>& buf):m_header(&buf.header),m_capacity(capacity)
	{
		m_arr.attach(&m_header->qids[0], capacity, m_header->count);
	}
	~KQuestArray()
	{

	}

public:
	int size() const
	{
		return m_arr.size();
	}
	int length() const
	{
		return m_header->size();
	}

	bool add(int qid);
	
	bool remove(int qid)
	{
		if(m_arr.erase_by_value(qid))
		{
			m_header->count -= 1;
			return true;
		}
		return false;
	}
	int at(int idx)
	{
		ASSERT(idx >= 0 && idx < m_arr.size());
		return m_arr.at(idx);
	}
	void clear()
	{
		m_arr.clear();
		m_header->count = 0;
	}
	void* dataPointer()
	{
		return m_header;
	}
};

#endif
