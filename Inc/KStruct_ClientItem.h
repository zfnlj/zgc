#pragma once

#include <System/KType.h>
#include <System/Collections/KVector.h>

struct KItemDisplayData
{
	DWORD		s_nItemID;			//�����ԭʼ����Ʒ��ţ���0��������
	DWORD		s_nDisplayID;		//����3D��ʾ����Ʒ���
	WORD		s_nPos;
	DWORD		s_nRecorver[8];		//��������չ
	KItemDisplayData()
	{
		memset( this, 0, sizeof(KItemDisplayData) );
	}
};

typedef		System::Collections::KVector<KItemDisplayData>		VEC_ITEM_DISPLAY;