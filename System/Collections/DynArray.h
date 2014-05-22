#ifndef _DYN_ARRAY_H_
#define _DYN_ARRAY_H_

#include "../KType.h"
#include "../KMacro.h"
#include "../Memory/KMemory.h"
#include <string.h>
#include <stdlib.h>

namespace System { namespace Collections {

// using namespace ::System::Collections;

/**
 * 可以扩展的数组，有一个初始容量
 * 对可以放到容器中的数据结构是有要求的:
 *   1. 元素必须可以通过memcpy来复制
 *   2. 析构时没有额外的操作
 *
 * 适用于简单的数据类型，可以和栈里面的数组绑定
 */
template <typename T, int initCapacity=32, int inflateStep=16> class DynArray
{
public:
	typedef T value_type;

public:
	BOOL m_ownMem;
	int m_size;
	int m_capacity;
	value_type* m_vals;

public:
	DynArray()
		: m_ownMem(TRUE)
		, m_size(0)
		, m_capacity(0)
		, m_vals(NULL)
	{
		ASSERT(_is_simple_type_<T>::value);
	}
	DynArray(value_type* buffer, int capacity, int size)
		: m_ownMem(FALSE)
		, m_vals(buffer)
		, m_capacity(capacity)
		, m_size(size)
	{
		ASSERT(_is_simple_type_<T>::value);
	}
	DynArray(const DynArray& o)
		:m_ownMem(TRUE)
		,m_size(0)
		,m_capacity(0)
		,m_vals(NULL)
	{
		ASSERT(_is_simple_type_<T>::value);
		if(!o.m_size) return;
		this->inflate(o.m_size);
		memcpy(m_vals, o.m_vals, o.m_size*sizeof(value_type));
		m_size = o.m_size;
	}
	~DynArray()
	{
		this->destroy();
	}
	DynArray& operator = (const DynArray& o)
	{
		if(this == &o) return *this;

		this->clear();
		if(!o.m_size) return *this;
		
		if(m_capacity < o.m_size) this->inflate(o.m_size);
		memcpy(m_vals, o.m_vals, o.m_size*sizeof(value_type));
		m_size = o.m_size;
		
		return *this;
	}
	int size() const
	{
		return m_size;
	}
	void clear()
	{
		m_size = 0;
	}
	BOOL empty() const
	{
		return m_size < 1;
	}
	DynArray& attach(value_type* vals, int capacity, int size)
	{
		this->destroy();
		m_ownMem = FALSE;
		m_size = size;
		m_capacity = capacity;
		m_vals = vals;
		return *this;
	}
	void detach()
	{
		if(m_ownMem) return;
		m_ownMem = TRUE;
		m_size = 0;
		m_capacity = 0;
		m_vals = NULL;
	}
	void resize( int newSize )
	{
		ASSERT( newSize >= 0 );
		if(m_capacity < newSize) this->inflate(newSize);
		if(m_size < newSize)
		{
			if ( _type_default_value_<value_type>::needFillDefaultValue )
			{
				this->fillDefault(m_size, newSize-m_size);
			}
		}
		m_size = newSize;
	}
	value_type& at(int idx)
	{
		ASSERT(idx >= 0);
		int size = idx + 1;
		if(m_capacity < size) this->inflate(size);
		if(m_size < size)
		{
			if ( _type_default_value_<value_type>::needFillDefaultValue )
			{
				this->fillDefault(m_size, size-m_size);
			}
			m_size = size;
		}
		return m_vals[idx];
	}
	const value_type& at(int idx) const
	{
		ASSERT(idx >= 0 && idx < m_size);
		return m_vals[idx];
	}
	value_type& get(int idx)
	{
		ASSERT(idx >= 0 && idx < m_size);
		return m_vals[idx];
	}
	value_type& operator [] (int idx)
	{
		return this->at(idx);
	}
	const value_type& operator [] (int idx) const
	{
		return this->at(idx);
	}
	DynArray& push_back(const value_type& val)
	{
		this->at(m_size) = val;
		return *this;
	}
	value_type pop_back()
	{
		ASSERT(m_size);
		m_size -= 1;
		return m_vals[m_size];
	}
	value_type& back()
	{
		ASSERT(m_size);
		return m_vals[m_size-1];
	}
	const value_type& back() const
	{
		ASSERT(m_size);
		return m_vals[m_size-1];
	}
	value_type& front()
	{
		ASSERT(m_size);
		return m_vals[0];
	}
	const value_type& front() const
	{
		ASSERT(m_size);
		return m_vals[0];
	}
	DynArray& erase(int idx)
	{
		if(idx < 0 || idx >= m_size) return *this;
		int len = (m_size-idx-1) * sizeof(value_type);
		if(len > 0)
		{
			void* pSrc = &m_vals[idx+1];
			void* pDst = &m_vals[idx];
			::memmove(pDst, pSrc, len);
		}
		m_size -= 1;
		return *this;
	}
	DynArray& insert(int idx, const value_type& val)
	{
		if(idx >= m_size)
		{
			this->at(idx) = val;
			return *this;
		}

		int size = m_size + 1;
		if(m_capacity < size) this->inflate(size);

		int len = (m_size-idx)*sizeof(value_type);
		if(len > 0)
		{
			void* pSrc = &m_vals[idx];
			void* pDst = &m_vals[idx+1];
			memmove(pDst, pSrc, len);
		}
		m_vals[idx] = val;
		m_size += 1;
		return *this;
	}

protected:
	// 在当前的capacity小于size时被调用
	BOOL inflate(int size)
	{
		ASSERT(m_ownMem);
		if(!m_vals)
		{
			int capacity = initCapacity;
			while(capacity < size) capacity += inflateStep;
			m_vals = (value_type*)malloc_k(capacity*sizeof(value_type));
			m_capacity = capacity;
		}
		else
		{
			int capacity = m_capacity;
			while(capacity < size) capacity += inflateStep;
			m_vals = (value_type*)realloc_k(m_vals, capacity*sizeof(value_type));
			m_capacity = capacity;
		}
		return TRUE;
	}
	void destroy()
	{
		this->clear();
		if(m_ownMem && m_vals)
		{
			free_k(m_vals);
			m_vals = NULL;
			m_capacity = 0;
		}
	}
	void fillDefault(int from, int count)
	{
		const value_type defVal = _type_default_value_<value_type>::get();
		while(count > 0)
		{
			m_vals[from++] = defVal;
			count -= 1;
		}
	}
};

template <typename T,typename C=KLessCompare< T,KLess<T> >,int initCapacity=32, int inflateStep=16>
class KDynSortedArray : private DynArray<T, initCapacity, inflateStep>
{
public:
	typedef T value_type;
	typedef DynArray<T,initCapacity,inflateStep> base_type;
	typedef C cmp_type;

public:
	cmp_type m_cmp;

public:
	KDynSortedArray()
	{

	}
	KDynSortedArray(value_type* vals, int capacity):base_type(vals,capacity,0)
	{

	}
	KDynSortedArray(const KDynSortedArray& o):base_type(o)
	{

	}
	~KDynSortedArray()
	{

	}
	KDynSortedArray& operator = (const KDynSortedArray& o)
	{
		if(this == &o) return *this;
		base_type::operator = (o);
		return *this;
	}
	KDynSortedArray& attach(value_type* vals, int capacity)
	{
		base_type::attach(vals, capacity, 0);
		return *this;
	}
	// 调用者必须保证已有的数据是排好序的
	KDynSortedArray& attach(value_type* vals, int capacity, int size)
	{
		base_type::attach(vals, capacity, size);
		return *this;
	}
	KDynSortedArray& detach()
	{
		base_type::detach();
		return *this;
	}
	int size() const
	{
		return base_type::m_size;
	}
	int empty() const
	{
		return base_type::m_size < 1;
	}
	void clear()
	{
		base_type::clear();
	}
	value_type& at(int idx)
	{
		return base_type::at(idx);
	}
	const value_type& at(int idx) const
	{
		return base_type::at(idx);
	}
	value_type& operator [] (int idx)
	{
		return base_type::at(idx);
	}
	const value_type& operator [] (int idx) const
	{
		return base_type::at(idx);
	}
	value_type& get(int idx)
	{
		return base_type::get(idx);
	}
	const value_type& get(int idx) const
	{
		return base_type::get(idx);
	}
	int find(const value_type& val) const
	{
		BOOL found;
		int pos = this->bsearch(val, found);
		if(!found) return -1;
		return pos;
	}
	int insert(const value_type& val)
	{
		BOOL found;
		int pos = this->bsearch(val, found);
		base_type::insert(pos, val);
		return pos;
	}
	int insert_unique(const value_type& val)
	{
		BOOL found;
		int pos = this->bsearch(val, found);
		if(found) return -1;
		base_type::insert(pos, val);
		return pos;
	}
	value_type pop_back()
	{
		return base_type::pop_back();
	}
	value_type& back()
	{
		return base_type::back();
	}
	const value_type& back() const
	{
		return base_type::back();
	}
	value_type& front()
	{
		return base_type::front();
	}
	const value_type& front() const
	{
		return base_type::front();
	}
	KDynSortedArray& erase(int idx)
	{
		base_type::erase(idx);
		return *this;
	}
	KDynSortedArray& erase_by_index(int idx)
	{
		base_type::erase(idx);
		return *this;
	}
	BOOL erase(const value_type& val)
	{
		int pos = this->find(val);
		if(pos < 0) return FALSE;
		this->erase(pos);
		return TRUE;
	}
	BOOL erase_by_value(const value_type& val)
	{
		int pos = this->find(val);
		if(pos < 0) return FALSE;
		this->erase_by_index(pos);
		return TRUE;
	}

	/**
	* 实现对半查找，返回值是元素应当插入的位置
	*/
	int bsearch(const value_type& val, BOOL& found) const
	{
		found = FALSE;
		if(base_type::m_size < 1) return 0;

		char* orig = (char*)base_type::m_vals;
		register char* lo = orig;
		register char* hi = lo + (base_type::m_size-1) * sizeof(value_type);

		register int num = base_type::m_size;
		register int unitlen = sizeof(value_type);

		char* mid;
		int half;
		int_r result;

		while (lo <= hi)
		{
            half = num / 2;
			if (half)
			{	// num >= 2
				mid = lo + (num & 1 ? half : (half - 1)) * unitlen;
				if (!(result = m_cmp(val, *(const value_type*)mid)))
				{
					found = TRUE;
					return (int)((mid-orig) / unitlen);
				}
				else if (result < 0)
				{	// val < mid
					hi = mid - unitlen;
					num = num & 1 ? half : half-1;

					if(lo > hi)
					{
						return (int)((mid-orig) / unitlen);
					}
				}
				else
				{	// val > mid
					lo = mid + unitlen;
					num = half;

					if(lo > hi)
					{
						return (int)((mid-orig) / unitlen + 1);
					}
				}
			}
			else if (num)
			{	//只有一个
				int_r n = m_cmp(val, *(const value_type*)lo);
				if(!n)
				{
					found = TRUE;
					return (int)((lo-orig)/unitlen);
				}
				else if(n < 0)
				{
					return (int)((lo-orig)/unitlen);
				}
				else
				{
					return (int)((lo-orig)/unitlen + 1);
				}
			}
			else
			{	//没有
				return (int)((lo-orig)/unitlen);
			}
		}
		//不可达
		return -1;
	}
};

template <typename T, typename C=KLess<T>, int initCapacity=32, int inflateStep=16>
class DynSortedArray : public KDynSortedArray<T,KLessCompare<T,C>,initCapacity,inflateStep>
{
public:
	typedef KDynSortedArray<T,KLessCompare<T,C>,initCapacity,inflateStep> base_type;
	typedef T value_type;

public:
	DynSortedArray()
	{

	}
	DynSortedArray(value_type* vals, int capacity):base_type(vals,capacity)
	{

	}
	DynSortedArray(const DynSortedArray& o):base_type(o)
	{

	}
	~DynSortedArray()
	{

	}
	DynSortedArray& operator = (const DynSortedArray& o)
	{
		if(this == &o) return *this;
		base_type::operator = (o);
		return *this;
	}
};

} }

#endif