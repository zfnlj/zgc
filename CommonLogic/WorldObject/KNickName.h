#pragma once
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include <System/KPlatform.h>

class KNickName
{
public:
	char m_nickName[MAX_NAME_DEF];

public:
	KNickName()
	{
		m_nickName[0] = '\0';
	}
	KNickName(const KNickName& o)
	{
		strcpy_k(m_nickName, sizeof(m_nickName), o.m_nickName);
	}
	KNickName(const char* nickName)
	{
		strcpy_k(m_nickName, sizeof(m_nickName), nickName);
	}
	KNickName& operator = (const KNickName& o)
	{
		if(this == &o) return *this;
		strcpy_k(m_nickName, sizeof(m_nickName), o.m_nickName);
		return *this;
	}
	KNickName& operator = (const char* nickName)
	{
		strcpy_k(m_nickName, sizeof(m_nickName), nickName);
		return *this;
	}
	operator char* ()
	{
		return &m_nickName[0];
	}
	operator const char* () const
	{
		return &m_nickName[0];
	}
	void clear()
	{
		m_nickName[0] = '\0';
	}
	size_t size() const
	{
		return strlen(m_nickName);
	}
	const char* c_str() const
	{
		return m_nickName;
	}
	bool empty() const
	{
		return m_nickName[0] == '\0';
	}
};

typedef KNickName KNick;

class nickname_cmp
{
public:
	int operator () (const KNickName& a, const KNickName&b) const
	{
		return strcmp(a, b);
	}
};
