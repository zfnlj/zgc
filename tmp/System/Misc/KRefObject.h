#ifndef __KREFOBJECT_H__
#define __KREFOBJECT_H__
#include "../KType.h"
namespace System { 

	class KRefObject
	{
	public:
		KRefObject(){m_uiRefCount = 0;}
		virtual ~KRefObject(){}
		inline void IncRefCount();
		inline void DecRefCount();
		inline unsigned int GetRefCount() const;
	protected:
		virtual void DeleteThis();
	private:
		UINT32 m_uiRefCount;		
	};
	#include "KRefObject.inl"
}


#endif