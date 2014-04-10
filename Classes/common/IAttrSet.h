#pragma once

#include <System/KType.h>
#include <System/KMacro.h>
#include <string.h>

class IAttrSet;
#define ATTR_PARAM_SELF (0)

/**
 * �������Եı仯������ʱ��:�仯ǰ�ͱ仯��
 * �仯ǰ�¼��������false����ֹ�仯����
 */
class IAttrObserver
{
public:
	virtual ~IAttrObserver() { } // paramId Ϊ ATTR_PARAM_SELF ʱ����ʾ����ֵ����
	virtual bool beforeAttrChange(bool first,IAttrSet& attrSet, int attrId, int paramId, int val) { return true; }
	virtual void afterAttrChange(bool first,IAttrSet& attrSet, int attrId, int paramId, int val)  { }
};

class IAttrSet
{
public:
	virtual ~IAttrSet() { }

	virtual void clearAttrs()										{ return; }
	virtual bool hasAttr(int attrId) const							{ return false; }
	virtual int  getAttrValue(int attrId, int defVal)				{ return defVal; }
	virtual void setAttrValue(int attrId, int val)					{ return; }

	// ����ֵΪ������ֵ
	virtual int setAttrParam(int attrId, int paramId, int val)		{ return 0; }
	virtual int getAttrParam(int attrId, int paramId, int defVal)	{ return defVal; }

	// ���������Ա仯�¼�
	virtual void _setAttrValue(int attrId, int val)					{ return; }
	virtual int  _setAttrParam(int attrId, int paramId, int val)	{ return 0; }

	virtual void setAttrObserver(int attrId, IAttrObserver* obv)	{ }

	// ���Լ�����Ҳ���յ��¼�֪ͨ������ͬ��֮�����������������
	// param == ATTR_PARAM_SELF ��ʾ���Ա���
	virtual void onAttrChanged(int attrId, int paramId, int val)	{ }
};

template <int startAttr,int endAttr> class KAttrSet_1
{
public:
	struct Attr_1 { int val; IAttrObserver* obv; };
	Attr_1 m_attrs[endAttr-startAttr];
	IAttrSet& m_owner;

public:
	KAttrSet_1(IAttrSet& owner):m_owner(owner)
	{
		memset(&m_attrs, 0, sizeof(m_attrs));
	}
	~KAttrSet_1()
	{

	}

public:
	void clearAttrs()
	{	// �������Լ���
		for(int i=0; i<endAttr-startAttr; i++)
			m_attrs[i].val = 0;
	}
	bool hasAttr(int attrId) const
	{
		return attrId >= startAttr && attrId < endAttr;
	}
	int getAttrValue(int attrId, int defVal)
	{
		if(!this->hasAttr(attrId)) return defVal;
		return m_attrs[attrId-startAttr].val;
	}
	void setAttrValue(int attrId, int val,bool first)
	{
		if(!this->hasAttr(attrId)) return;
		Attr_1& attr = m_attrs[attrId-startAttr];
		if(attr.obv)
		{
			if(!attr.obv->beforeAttrChange(first,m_owner, attrId, ATTR_PARAM_SELF, val))
				return;
		}
		m_attrs[attrId-startAttr].val = val;
		if(attr.obv)
			attr.obv->afterAttrChange(first,m_owner, attrId, ATTR_PARAM_SELF, val);
		m_owner.onAttrChanged(attrId, ATTR_PARAM_SELF, val);
	}
	int setAttrParam(int attrId, int paramId, int val,bool first)
	{
		if(paramId >= 0) return val; // paramId == ATTR_PARAM_SELF ��ʾ�Լ�
		this->setAttrValue(attrId, val,first);
		return val;
	}
	int getAttrParam(int attrId, int paramId, int defVal)
	{	// paramId == ATTR_PARAM_SELF ��ʾ�Լ�
		if(paramId < 0) return this->getAttrValue(attrId, defVal);
		return defVal; // 1 ������û�в���
	}
	void _setAttrValue(int attrId, int val)
	{
		if(!this->hasAttr(attrId)) return;
		m_attrs[attrId-startAttr].val = val;
	}
	int _setAttrParam(int attrId, int paramId, int val)
	{
		if(paramId != ATTR_PARAM_SELF) return 0;
		this->_setAttrValue(attrId, val);
		return val;
	}
	void setAttrObserver(int attrId, IAttrObserver* obv)
	{
		if(!this->hasAttr(attrId)) return;
		m_attrs[attrId-startAttr].obv = obv;
	}
};

template <int startAttr,int endAttr> class KAttrSet_2
{
public:
	struct Attr_2
	{
		int baseVal;
		int p1;
		int p2;
		int val;
		IAttrObserver* obv;
	};
	Attr_2 m_attrs[endAttr-startAttr];
	IAttrSet& m_owner;

public:
	KAttrSet_2(IAttrSet& owner):m_owner(owner)
	{
		memset(&m_attrs, 0, sizeof(m_attrs));
	}
	~KAttrSet_2()
	{

	}

public:
	void clearAttrs()
	{	// �������Լ���
		for(int i=0; i<endAttr-startAttr; i++)
		{
			Attr_2& attr = m_attrs[i];
			IAttrObserver* obv = attr.obv;
			memset(&attr, 0, sizeof(attr));
			attr.obv = obv;
		}
	}
	bool hasAttr(int attrId) const
	{
		return attrId >= startAttr && attrId < endAttr;
	}
	int getAttrValue(int attrId, int defVal)
	{
		if(!this->hasAttr(attrId)) return defVal;
		return m_attrs[attrId-startAttr].val;
	}
	void setAttrValue(int attrId, int val)
	{
		if(!this->hasAttr(attrId)) return;
		Attr_2& attr = m_attrs[attrId-startAttr];
		if(attr.obv)
		{
			if(!attr.obv->beforeAttrChange(m_owner, attrId, ATTR_PARAM_SELF, val))
				return;
		}
		
		attr.val = val;

		if(attr.obv)
			attr.obv->afterAttrChange(m_owner, attrId, ATTR_PARAM_SELF, val);
		m_owner.onAttrChanged(attrId, ATTR_PARAM_SELF, val);
	}
	int setAttrParam(int attrId, int paramId, int val)
	{
		if(!this->hasAttr(attrId)) return 0;
		Attr_2& attr = m_attrs[attrId-startAttr];
		
		if(attr.obv)
		{
			if(!attr.obv->beforeAttrChange(m_owner, attrId, paramId, val))
				return this->getAttrValue(attrId, 0);
		}
		
		switch(paramId)
		{
		case ATTR_PARAM_SELF:
			attr.val = val; break;
		case 1:
			attr.baseVal = val; break;
		case 2:
			attr.p1 = val; break;
		case 3:
			attr.p2 = val; break;
		default: return 0;
		}
		
		int attrVal = paramId == ATTR_PARAM_SELF ? val : this->_calcAttrValue(attr);
		
		if(attr.obv)
			attr.obv->afterAttrChange(m_owner, attrId, paramId, val);
		m_owner.onAttrChanged(attrId, paramId, val);

		return attrVal;
	}
	int getAttrParam(int attrId, int paramId, int defVal)
	{
		if(!this->hasAttr(attrId)) return 0;
		Attr_2& attr = m_attrs[attrId-startAttr];

		switch(paramId)
		{
		case ATTR_PARAM_SELF:
			return attr.val;
		case 1:
			return attr.baseVal;
		case 2:
			return attr.p1;
		case 3:
			return attr.p2;
		default:
			return defVal;
		}
	}
	// ���������Ա仯�¼�
	void _setAttrValue(int attrId, int val)
	{
		if(!this->hasAttr(attrId)) return;
		m_attrs[attrId-startAttr].val = val;
	}
	int _setAttrParam(int attrId, int paramId, int val)
	{
		if(!this->hasAttr(attrId)) return 0;
		Attr_2& attr = m_attrs[attrId-startAttr];
		switch(paramId)
		{
		case ATTR_PARAM_SELF:
			attr.val = val; break;
		case 1:
			attr.baseVal = val; break;
		case 2:
			attr.p1 = val; break;
		case 3:
			attr.p2 = val; break;
		default: return 0;
		}
		return paramId == ATTR_PARAM_SELF ? val : this->_calcAttrValue(attr);
	}
	void setAttrObserver(int attrId, IAttrObserver* obv)
	{
		if(!this->hasAttr(attrId)) return;
		m_attrs[attrId-startAttr].obv = obv;
	}

private:
	int _calcAttrValue(Attr_2& attr)
	{
		// Result = (Base + P1) * (1 + P2), ʹ��int64��ֹ���
		attr.val = (int)(((INT64)attr.baseVal + (INT64)attr.p1) * (1000 + (INT64)attr.p2) / 1000);
		return attr.val;
	}
};
