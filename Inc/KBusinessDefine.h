/********************************************************************
created:	2012/09/03
manager:	
copyright:	Aurora Interactive, 2011

*********************************************************************/


#pragma once

#include "System/KType.h"

#pragma pack(push,1)

struct BusinessKey
{
	BusinessKey() { Reset(); }
	BusinessKey(UINT64 nID1, UINT64 nID2) : nKey1(nID1),nKey2(nID2) {}
	
	void Reset() { nKey2 = 0; nKey1 = 0; }
	
	bool IsValid() const
	{
		if(0 == nKey1 || 0 == nKey2) return false;
		return true;
	}

	bool IsPlayer(UINT64 nPlayer1, UINT64 nPlayer2) const
	{
		if((nKey1 == nPlayer1 || nKey1 == nPlayer2) && (nKey2 == nPlayer1 || nKey2 == nPlayer2))
		{
			return true;
		}

		return false;
	}

	UINT64 GetOtherPlayer(UINT64 nID) const
	{
		if(nID == nKey1) return nKey2;
		else if(nID == nKey2) return nKey1;
		else return 0;
	}

	UINT64 GetKey1 () const
	{
		return nKey1;
	}

	UINT64 GetKey2 () const
	{
		return nKey2;
	}

	bool operator < (const BusinessKey& a) const
	{
		if(nKey1 < a.nKey1) return true;
		else if(nKey1 > a.nKey1) return false;
		return (nKey2 < a.nKey2);
	};

	UINT64 nKey1;
	UINT64 nKey2;
};

#pragma pack(pop)

namespace KBusinessAbout
{
	// 交易过程标记
	enum BUSINESS_FLAG
	{
		enumRB_None=0,		// 初始化
		enumRB_Try,			// 请求交易
		enumRB_Confirm,		// 同意可以交易
		enumRB_Lock,		// 锁定交易
		enumRB_Start,		// 开始交易

		enumRB_Count
	};
	
// #define BUSINESS_POSID_MIN		(0)		// 交易面板最小位置编号
// #define BUSINESS_POSID_MAX		(15)	// 交易面板最大位置编号
#define BUSINESS_POS_MAX			(16)
#define BUSINESS_WAIT_MAXTIME		(30)	// 30秒
#define BUSINESS_PUTMONEY_MAX		(999999999)	// 金钱最大数

}