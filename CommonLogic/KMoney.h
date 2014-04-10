/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "System/Decimal/KInteger.h"

namespace KMoneyAbout
{

// ������������
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
	KInteger m_money;	// Ǯ
public:
	// ����Ǯ�����Լ��٣�
	inline void Set(const KInteger& val)
	{
		m_money = val;
	}
	inline void Increase(const KInteger& toInc)
	{
		m_money += toInc;
		if (m_money < 0) m_money = 0;
	}
	// ����Ƿ����㹻��Ǯ
	inline bool HasMoney(KInteger toCheck) const
	{
		return m_money >= toCheck;
	}
};

// ���Ի���Ǯ��ʾ��1����ͱ��Ǳ�ʾ������\0��β�����ı�����ͼ���ʾ
void GoldShellToRichString(KInteger money, char* buffer, size_t len);
// ���Ի���Ǯ��ʾ��1����ͱ��Ǳ�ʾ������\0��β�����ı��������ֱ�ʾ
void GoldShellToString(KInteger money, char* buffer, size_t len);

}
