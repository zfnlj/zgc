/********************************************************************
created:	2011/4/19
manager:	zaki
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once

#include <System/KType.h>
#include <System/Collections/KString.h>
#include "KCommonDefine.h"

namespace KWorldObjAbout
{
	class KCharacter;
};

namespace CharacterState
{
	template<class T>
	struct IState
	{
	public:
		typedef T KSelfType;

	public:
		virtual ~IState() {};

	public:
		virtual INT OnEnter(KSelfType * pSelf, DWORD param = 0) = 0;
		virtual INT OnIdle(KSelfType * pSelf, DWORD param = 0) = 0;
		virtual INT OnExit(KSelfType * pSelf, DWORD param = 0) = 0;
	};
};