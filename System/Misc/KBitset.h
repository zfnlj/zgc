#ifndef _K_BITSET_H_
#define _K_BITSET_H_

#include "../KType.h"
#include "../KPlatform.h"

class KBitset
{
public:
	KBitset(int bitCount);
	KBitset(void* pData, int bitCount);
	~KBitset();

public:
	static int GetByteCount(int bitCount);

public:
	int  GetBitCount() const;
	void SetAll();
	void ClearAll();
	BOOL SetBit(int bitno);
	BOOL ClearBit(int bitno);
	BOOL GetBit(int bitno) const;

private:
	int m_bitCount;
	int m_byteCount;
	DWORD* m_pData;
	DWORD m_bits;    // 当需要的BIT数量小于32时，使用这个DWORD来存储
	BOOL m_memOwner;
};

#endif