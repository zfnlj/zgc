
#pragma once

#include <System/KType.h>
#include <System/KMacro.h>

#ifdef LOGICWORLD_EXPORTS
#   define _LogicWorldExport __declspec(dllexport)
#else
#   define _LogicWorldExport __declspec(dllimport)
#endif

template <typename F,typename I> class KFacadeImpl
{
public:
	typedef F facade_obj;
	typedef I facade_impl;

public:
	KFacadeImpl()
	{
		m_facadeObj.SetFacadeImpl((facade_impl*)this);
	}

	facade_obj* GetFacadeObject()
	{
		return &m_facadeObj;
	}
	const facade_obj* GetFacadeObject() const
	{
		return &m_facadeObj;
	}

private:
	facade_obj m_facadeObj;
};

template <typename F,typename I> class KFacadeObject
{
public:
	typedef F facade_obj;
	typedef I facade_impl;

public:
	facade_impl* m_pFacadeImpl;

public:
	KFacadeObject():m_pFacadeImpl(NULL)
	{

	}

	facade_impl* GetFacadeImpl()
	{
		return m_pFacadeImpl;
	}
	const facade_impl* GetFacadeImpl() const
	{
		return m_pFacadeImpl;
	}
	void SetFacadeImpl(facade_impl* p)
	{
		m_pFacadeImpl = p;
	}
};
