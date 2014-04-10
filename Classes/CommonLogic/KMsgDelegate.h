/********************************************************************
	created:	2011/03/29
	created:	29:3:2011   11:20
	author:		dengkai, Augustine
	
	purpose:	模块间传递消息的代理(delegate)模式，提供对全局函数和对象的成员函数两类需求的处理
	  
*********************************************************************/
#ifndef __KMSGDELEGATE_H__
#define __KMSGDELEGATE_H__

#include "System/KType.h"
#include "System/Collections/DynArray.h"

using namespace System::Collections;

//基础消息虚接口，便于消息声明
class KIMsgDelegate
{
public:
	KIMsgDelegate() {}
	virtual ~KIMsgDelegate(void) {;}

	virtual void Invoke(unsigned long long Param1, unsigned long long Param2) = 0;

};


//提供对全局函数的回调
class KNonTypeDelegate : public KIMsgDelegate
{
public:
	virtual void Invoke(unsigned long long Param1, unsigned long long Param2) 
	{   
		m_pfn(Param1,Param2); 
	}

	KNonTypeDelegate(void (*pfn)(unsigned long long, unsigned long long)):KIMsgDelegate()   {    m_pfn = pfn; }
	virtual ~KNonTypeDelegate(){}
private:
	void (*m_pfn)(unsigned long long, unsigned long long);
};



//利用模板对对象的成员函数的回调
template <typename T>
class KTTypeDelegate : public KIMsgDelegate
{
public:
	virtual void Invoke(unsigned long long Param1, unsigned long long Param2) 
	{   
		(m_pObj->*m_pfn)(Param1, Param2); 
	}

	KTTypeDelegate(T* pt, void (T::*pfn)(unsigned long long, unsigned long long)):KIMsgDelegate()
	{
		m_pObj = pt;
		m_pfn = pfn;
	}
	virtual ~KTTypeDelegate(){}

private:
	T    *m_pObj;
	void (T::*m_pfn)(unsigned long long, unsigned long long);
};

// 消息相同的注册在一起

class KMsgDelegateNode
{
public:
	KMsgDelegateNode(){m_msgID = 0;}
	~KMsgDelegateNode()
	{
		int iSize = m_buf.size();
		int i = 0;
		for(i = 0;i<iSize;i++)
		{
			delete m_buf[i];
		}
		m_buf.clear();
	}
	void init(int iMsgID)
	{
		m_msgID = iMsgID;
	}

	void Invoke(unsigned long long Param1, unsigned long long Param2)
	{
		int iSize = m_buf.size();
		int i = 0;
		for(i = 0;i<iSize;i++)
		{
			m_buf[i]->Invoke(Param1, Param2);
		}
	}
	void RegisterNonType(void (*pfn)(unsigned long long, unsigned long long))
	{
		RegisterMsgDelegate(new KNonTypeDelegate(pfn));
	}

	template <typename T>
	void RegisterType(T* pt, void (T::*pfn)(unsigned long long, unsigned long long))
	{
		RegisterMsgDelegate(new KTTypeDelegate<T>(pt,pfn));
	}

	// KMsgDelegateNode 负责释放pMsgDelegate，不建议直接调用
	void RegisterMsgDelegate(KIMsgDelegate* pMsgDelegate)
	{
		m_buf.insert(m_buf.size(),pMsgDelegate);
	}

	void Clear()
	{
		int iSize = m_buf.size();
		int i = 0;
		for(i = 0;i<iSize;i++)
		{
			delete m_buf[i];
		}
		m_buf.clear();
	}

	INT GetMsgID(){return m_msgID;}
protected:
	INT		m_msgID;
	DynArray<KIMsgDelegate*,1,4> m_buf;
};

/*
	消息映射表
	Msg是连续的一段整数,建议消息用枚举
*/
class KMsgDelegateNodeMap
{
public:
	KMsgDelegateNodeMap()
	{
	}
	
	void init(int iCount, int iStart = 0)
	{
		m_NodeMap = new KMsgDelegateNode[iCount];

		int i = 0;
		for(i = 0;i<iCount;i++)
		{
			m_NodeMap[i].init(i+iStart);
		}

		m_iCount = iCount;
		m_iStart = iStart;
	}


	~KMsgDelegateNodeMap()
	{
		delete[] m_NodeMap;
		m_NodeMap = NULL;
	}

	void Invoke(int iMsg, unsigned long long Param1,unsigned long long Param2)
	{
		m_NodeMap[ iMsg - m_iStart].Invoke(Param1, Param2);
	}

	void RegisterNonType(int iMsg, void (*pfn)(unsigned long long, unsigned long long))
	{
		m_NodeMap[ iMsg - m_iStart].RegisterNonType(pfn);
	}

	template <typename T>
	void RegisterType(int iMsg, T* pt, void (T::*pfn)(unsigned long long, unsigned long long))
	{
		m_NodeMap[ iMsg - m_iStart].RegisterType<T>(pt,pfn);
	}

	void RegisterMsgDelegate(int iMsg,KIMsgDelegate* pMsgDelegate)
	{
		m_NodeMap[ iMsg - m_iStart].RegisterMsgDelegate(pMsgDelegate);
	}

	void ClearMsgDelegate()
	{
		int i = 0;
		for(i = 0;i<m_iCount;i++)
		{
			(m_NodeMap[i]).Clear();
		}
	}

protected:
	KMsgDelegateNode*	m_NodeMap;
	int					m_iCount;
	int					m_iStart;
};






#endif