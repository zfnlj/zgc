#pragma once
#include "IAttrSet.h"

// 仅仅是作为缺省模板参数，满足语法要求
// 也许有一定的开销，
// 但是对于使用KAttrSetCombo的人带来了便利
class _KEmptyAttrSet
{
public:
	_KEmptyAttrSet(IAttrSet& owner) { }
	void clearAttrs() { return; }
	bool hasAttr(int attrId) const { return false; }
	int  getAttrValue(int attrId, int defVal) { return defVal; }
	void setAttrValue(int attrId, int val,bool first) { return; }
	int setAttrParam(int attrId, int paramId, int val,bool first) { return 0; }
	int getAttrParam(int attrId, int paramId, int defVal) { return defVal; }
	void _setAttrValue(int attrId, int val) { return; }
	int  _setAttrParam(int attrId, int paramId, int val) { return 0; }
	void setAttrObserver(int attrId, IAttrObserver* obv) { }
};

template <
	  typename Set1
	, typename Set2
	, typename Set3=_KEmptyAttrSet
	, typename Set4=_KEmptyAttrSet
>
class KAttrSetCombo
{
public:
	Set1 m_set1;
	Set2 m_set2;
	Set3 m_set3;
	Set4 m_set4;

public:
	KAttrSetCombo(IAttrSet& owner)
		: m_set1(owner)
		, m_set2(owner)
		, m_set3(owner)
		, m_set4(owner)
	{
	}
	void clearAttrs()
	{
		m_set1.clearAttrs();
		m_set2.clearAttrs();
		m_set3.clearAttrs();
		m_set4.clearAttrs();
	}
	bool hasAttr(int attrId) const
	{
		return m_set1.hasAttr(attrId)
			|| m_set2.hasAttr(attrId)
			|| m_set3.hasAttr(attrId)
			|| m_set4.hasAttr(attrId);
	}
	int  getAttrValue(int attrId, int defVal=0)
	{
		if(m_set1.hasAttr(attrId))
			return m_set1.getAttrValue(attrId, defVal);
		else if(m_set2.hasAttr(attrId))
			return m_set2.getAttrValue(attrId, defVal);
		else if(m_set3.hasAttr(attrId))
			return m_set3.getAttrValue(attrId, defVal);
		else if(m_set4.hasAttr(attrId))
			return m_set4.getAttrValue(attrId, defVal);
		return defVal;
	}
	void setAttrValue(int attrId, int val,bool first)
	{
		if(m_set1.hasAttr(attrId))
		{
			m_set1.setAttrValue(attrId, val,first);
		}
		else if(m_set2.hasAttr(attrId))
		{
			m_set2.setAttrValue(attrId, val,first);
		}
		else if(m_set3.hasAttr(attrId))
		{
			m_set3.setAttrValue(attrId, val,first);
		}
		else if(m_set4.hasAttr(attrId))
		{
			m_set4.setAttrValue(attrId, val,first);
		}
	}
	int setAttrParam(int attrId, int paramId, int val,bool first)
	{
		if(m_set1.hasAttr(attrId))
			return m_set1.setAttrParam(attrId, paramId, val,first);
		if(m_set2.hasAttr(attrId))
			return m_set2.setAttrParam(attrId, paramId, val,first);
		if(m_set3.hasAttr(attrId))
			return m_set3.setAttrParam(attrId, paramId, val,first);
		if(m_set4.hasAttr(attrId))
			return m_set4.setAttrParam(attrId, paramId, val,first);
		return this->getAttrValue(attrId, 0);
	}
	int getAttrParam(int attrId, int paramId, int defVal)
	{
		if(m_set1.hasAttr(attrId))
			return m_set1.getAttrParam(attrId, paramId, defVal);
		else if(m_set2.hasAttr(attrId))
			return m_set2.getAttrParam(attrId, paramId, defVal);
		else if(m_set3.hasAttr(attrId))
			return m_set3.getAttrParam(attrId, paramId, defVal);
		else if(m_set4.hasAttr(attrId))
			return m_set4.getAttrParam(attrId, paramId, defVal);
		return defVal;
	}
	//void _setAttrValue(int attrId, int val,bool first)
	//{
	//	if(m_set1.hasAttr(attrId))
	//	{
	//		m_set1.setAttrValue(attrId, val,first);
	//	}
	//	else if(m_set2.hasAttr(attrId))
	//	{
	//		m_set2.setAttrValue(attrId, val,first);
	//	}
	//	else if(m_set3.hasAttr(attrId))
	//	{
	//		m_set3.setAttrValue(attrId, val,first);
	//	}
	//	else if(m_set4.hasAttr(attrId))
	//	{
	//		m_set4.setAttrValue(attrId, val,first);
	//	}
	//}
	//int  _setAttrParam(int attrId, int paramId, int val)
	//{
	//	if(m_set1.hasAttr(attrId))
	//		return m_set1._setAttrParam(attrId, paramId, val);
	//	if(m_set2.hasAttr(attrId))
	//		return m_set2._setAttrParam(attrId, paramId, val);
	//	if(m_set3.hasAttr(attrId))
	//		return m_set3._setAttrParam(attrId, paramId, val);
	//	if(m_set4.hasAttr(attrId))
	//		return m_set4._setAttrParam(attrId, paramId, val);
	//	return this->getAttrValue(attrId, 0);
	//}
	void setAttrObserver(int attrId, IAttrObserver* obv)
	{
		if(m_set1.hasAttr(attrId))
			m_set1.setAttrObserver(attrId, obv);
		else if(m_set2.hasAttr(attrId))
			m_set2.setAttrObserver(attrId, obv);
		else if(m_set3.hasAttr(attrId))
			m_set3.setAttrObserver(attrId, obv);
		else if(m_set4.hasAttr(attrId))
			m_set4.setAttrObserver(attrId, obv);
	}
};
