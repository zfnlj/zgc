#pragma once

#include <System/KType.h>
#include <System/Collections/KVector.h>

struct KItemDisplayData
{
	DWORD		s_nItemID;			//这个是原始的物品编号：非0才有意义
	DWORD		s_nDisplayID;		//用于3D显示的物品编号
	WORD		s_nPos;
	DWORD		s_nRecorver[8];		//保留供扩展
	KItemDisplayData()
	{
		memset( this, 0, sizeof(KItemDisplayData) );
	}
};

typedef		System::Collections::KVector<KItemDisplayData>		VEC_ITEM_DISPLAY;