﻿#ifndef _K_STREAM_SESSION_H_
#define _K_STREAM_SESSION_H_

#include "stddef.h"
#include <System/KPlatform.h>
#include <System/Collections/DynArray.h>

struct StreamSessionUnit
{
	DWORD key;
	BYTE  len;
	char  val[3];
	//--------------------------
	int size() const
	{
		return offsetof(StreamSessionUnit,val)+this->len;
	}
	static int size(int len)
	{
		return offsetof(StreamSessionUnit,val)+len;
	}
};

class KStreamSession
{
public:
	class cmp
	{
	public:
		typedef StreamSessionUnit* ptr;
		int operator () (const ptr& a, const ptr& b) const
		{
			if(a->key > b->key) return 1;
			if(a->key < b->key) return -1;
			return 0;
		}
	};
	typedef JG_C::KDynSortedArray<StreamSessionUnit*,cmp,128,64> SessionArray;

public:
	KStreamSession();
	~KStreamSession();

public:
	bool attach(void* pData, int capacity, int dataSize);
	void detach();

public:
	// copy from other object
	bool copy(const KStreamSession& o);
	bool concat(const KStreamSession& o);

	// data size of bytes
	int size() const;

public:
	template <typename T> T* getSession(DWORD key) const
	{
		const StreamSessionUnit* psu = this->_getSession(key);
		if(!psu) return NULL;
		ASSERT(psu->len == sizeof(T));
		return (T*)&psu->val;
	}
	template <typename T> bool setSession(DWORD key, const T* val)
	{
		return this->setSession(key, (const void*)val, sizeof(T));
	}

public:
	bool hasSession(DWORD key) const;
	int getSession(DWORD key, void** ppData);
	bool setSession(DWORD key, const void* val, int len);
	void removeSession(DWORD key);
	void clearSessions();

public:
	bool  setInteger(DWORD key, int val);
	bool  setFloat(DWORD key, float val);
	bool  setInt64(DWORD key, INT64 val);
	int	  getInteger(DWORD key, int defVal) const;
	float getFloat(DWORD key, float defVal) const;
	INT64 getInt64(DWORD key, INT64 defVal) const;

public:
	void _buildIndex(); // 创建索引
	StreamSessionUnit* _getSession(DWORD key);
	const StreamSessionUnit* _getSession(DWORD key) const;
	int _getSessionPosition(StreamSessionUnit* psu);
	void* getData(){ return m_pData; }

public:
	char* m_pData;
	int m_dataSize;
	int m_capacity;
	SessionArray m_sessionArray;

public:
	//BeginDefLuaClassNoCon(KStreamSession);
	//	DefMemberFunc(size);
	//	DefMemberFunc(attach);
	//	DefMemberFunc(detach);
	//	DefMemberFunc(hasSession);
	//	DefMemberFunc2(getSession_lua,"getSession");
	//	DefMemberFunc2(setSession_lua,"setSession");
	//	DefMemberFunc(removeSession);
	//	DefMemberFunc(clearSessions);
	//	DefMemberFunc(setInteger);
	//	DefMemberFunc(setFloat);
	//	DefMemberFunc(setInt64)
	//	DefMemberFunc(getInteger);
	//	DefMemberFunc(getFloat);
	//	DefMemberFunc(getInt64);
	//	DefMemberFunc(getData);
	//EndDef;
};

//--------------------------------------------------
// 自带缓冲区的StreamSession
//
template <size_t capacity>
class KStreamSessionTmpl : public KStreamSession
{
public:
	enum { idxCount = capacity/sizeof(StreamSessionUnit*) };
	char m_buffer[capacity];
	StreamSessionUnit* m_idxBuf[idxCount];
	KStreamSessionTmpl()
	{
		m_sessionArray.attach(m_idxBuf, idxCount, 0);
		this->attach(&m_buffer, sizeof(m_buffer), 0);
	}
};


#endif