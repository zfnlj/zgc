/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "System/Decimal/KInteger.h"

namespace KMoneyAbout
{

// 超长整数货币
class KMoney
{
public:
	KMoney()
	{
		m_money.ToZero();
	}
	virtual ~KMoney()
	{

	}
public:
	KInteger m_money;	// 钱
public:
	// 增加钱（可以减少）
	inline void Set(const KInteger& val)
	{
		m_money = val;
	}
	inline void Increase(const KInteger& toInc)
	{
		m_money += toInc;
		if (m_money < 0) m_money = 0;
	}
	// 检查是否有足够的钱
	inline bool HasMoney(KInteger toCheck) const
	{
		return m_money >= toCheck;
	}
};

// 个性化的钱表示法1，金和贝壳表示法，以\0结尾。富文本，用图标表示
void GoldShellToRichString(KInteger money, char* buffer, size_t len);
// 个性化的钱表示法1，金和贝壳表示法，以\0结尾。富文本，用文字表示
void GoldShellToString(KInteger money, char* buffer, size_t len);

}
