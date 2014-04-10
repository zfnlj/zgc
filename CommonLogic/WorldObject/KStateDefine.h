/********************************************************************
created:	2011/3/22
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "KObjectState.h"

namespace KAttributeAbout
{
	/**
	 * 角色状态的枚举
	 */
	enum CHARACTER_STATE
	{
		enumCS_Stand = cs_stand,	// 站立
		enumCS_Move  = cs_move,		// 移动
		enumCS_Death = cs_death,		// 死亡
	};
}
