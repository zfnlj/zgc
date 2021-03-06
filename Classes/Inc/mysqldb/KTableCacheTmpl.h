#ifndef _K_TABLE_CACHE_TMPL_H_
#define _K_TABLE_CACHE_TMPL_H_

#include "KTableCache.h"
#include "KCacheObject.h"
#include "KVarBinary.h"

class ITableCacheCreator
{
public:
	virtual ~ITableCacheCreator() { }
	virtual KTableCache* Create(KSQLTableSchema& schema, KVarBinaryMemory* vbm) = 0;
};

class KDefaultTableCacheCreator : public ITableCacheCreator
{
public:
	KTableCache* Create(KSQLTableSchema& schema, KVarBinaryMemory* vbm);
};

/**
 * 作为 Cache Table 的主键的数据类型 PKEY 需要满足几个条件
 *
 * 1. public inner type (1) key_type (2) key_cmp
 * 2. BOOL SetPrimaryKey(KCacheObject& cache, const key_type& val)
 * 3. BOOL GetPrimaryKey(KCacheObject& cache, key_type& val) const
 * 4. BOOL SetSQLParam(KSQLCommand& cmd, int paramBaseIdx, KCacheObject& cache)
 * 5. KString<512> ToString(const key_type& val) const
 *
 * 这里假定表的主键一定是从第一个字段开始并且字段之间是连续的
 *
 * 一些缺省的类型已经在 KCachePkey.h 中定义
 * 如果需要未提供的主键类型，需要写一个PKEY，
 * 并且继承KDefaultTableCacheCreator，生成自己需要的KTableCache数据类型
 */

template <typename PKEY>
class KTableCacheTmpl : public KTableCache
{
public:
	typedef System::Collections::KMapNode<typename PKEY::key_type,HANDLE> node_type;
	typedef System::Memory::KStepBlockAlloc<node_type,1024> alloc_type;
	typedef System::Collections::KMapByVector2<typename PKEY::key_type,HANDLE,typename PKEY::key_cmp,alloc_type> RecordMap;

public:
	KTableCacheTmpl()
	{

	}
	KTableCacheTmpl(KVarBinaryMemory* vbm):KTableCache(vbm)
	{

	}
	~KTableCacheTmpl()
	{

	}

public:
	BOOL SetPrimaryKey(KCacheObject& cache, const void* pKey)
	{
		const typename PKEY::key_type& key = *(typename PKEY::key_type*)pKey;
		return m_keyCls.SetPrimaryKey(cache, key);
	}

	BOOL SetSQLParam(KSQLCommand& cmd, int paramBaseIdx, KCacheObject& cache)
	{
		return m_keyCls.SetSQLParam(cmd, paramBaseIdx, cache);
	}

	System::Collections::KString<512> Key2String(const void* pKey) const
	{
		const typename PKEY::key_type& key = *(typename PKEY::key_type*)pKey;
		return m_keyCls.ToString(key);
	}

	System::Collections::KString<512> GetKeyString(HANDLE hRec) const
	{
		KCacheObject cacheObj;
		ASSERT(cacheObj.Attach((KTableCache*)this, hRec));
		
		typename PKEY::key_type key;
		if(!((KTableCache*)this)->GetPrimaryKey(cacheObj, &key))
		{
			return System::Collections::KString<512>("<error:pkey>");
		}

		return this->Key2String(&key);
	}

	BOOL GetPrimaryKey(KCacheObject& cache, void* pKey)
	{
		typename PKEY::key_type& key = *(typename PKEY::key_type*)pKey;
		return m_keyCls.GetPrimaryKey(cache, key);
	}

	BOOL IsPrimaryKey(KCacheObject& cache, const void* pKey)
	{
		const typename PKEY::key_type& key_1 = *(typename PKEY::key_type*)pKey;

		typename PKEY::key_type key_2;
		if(!m_keyCls.GetPrimaryKey(cache, key_2))
			return FALSE;

		typename PKEY::key_cmp cmp;
		return cmp(key_1, key_2) == 0;
	}

	BOOL SetAutoIncreaseKey(KCacheObject& cache, UINT64 key)
	{
		return m_keyCls.SetAutoIncreaseKey(cache, key);
	}

	HANDLE _FindRecord(const void* pKey)
	{
		const typename PKEY::key_type& key = *(typename PKEY::key_type*)pKey;
		typename RecordMap::iterator it = m_recMap.find(key);
		if(it == m_recMap.end()) return INVALID_HREC;
		return it->second;
	}

protected: // 由模板类实现的方法

	// 加载了一条新的记录，添加到内部的MAP中
	BOOL OnRecordLoaded(HANDLE hRec)
	{
		KCacheObject cacheObj;
		ASSERT(cacheObj.Attach(this, hRec));

		typename PKEY::key_type key;
		if(!m_keyCls.GetPrimaryKey(cacheObj, key))
			return FALSE;

		m_recMap.insert_unique(key, hRec);
		return TRUE;
	}

	// 卸载了一条记录，从MAP中删除
	BOOL OnRecordUnload(HANDLE hRec)
	{
		KCacheObject cacheObj;
		ASSERT(cacheObj.Attach(this, hRec));

		typename PKEY::key_type key;
		if(!m_keyCls.GetPrimaryKey(cacheObj, key))
			return FALSE;
		
		typename RecordMap::iterator it = m_recMap.find(key);
		if(it == m_recMap.end())
			return FALSE;

		m_recMap.erase(it);
		return TRUE;
	}

	BOOL _checkKeyLength(int len) const
	{
		return len == sizeof(typename PKEY::key_type);
	}

public:
	PKEY m_keyCls;
	RecordMap m_recMap;
};

#endif
