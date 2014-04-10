#ifndef _K_CACHE_OBJECT_H_
#define _K_CACHE_OBJECT_H_

#include <System/KType.h>

/**
 * 共享内存中一条记录的内存映像的格式
 *
 * 1. 边界是与BYTE对齐的 pack(1)
 * 2. 字符串类型的字段采用的前面两个字节的长度，后面跟没有结束符的字符串内容
 * 3. 二进制的字段采用前面两个字节的长度，后面跟内容
 * 4. 字符串类型需要注意数据库表使用的字符集，如GB2312的一个字符是两个BYTE
 * 
 * 如果想把共享内存中的记录直接映射为应用程序中的结构，
 * 必须遵从上面的上个原则，否则后果不可预料
 */

#pragma pack(push,1)
template <size_t size> struct KDBString // size 可容纳的数据长度
{
	WORD actualLength;  // 实际数据长度
	char strData[size]; // 不包含结束字符
	BOOL Set(const char* str, size_t len=0)
	{
		if(!len) len = strlen(str);
		if(len < 0 || len > size) return FALSE;
		memcpy(strData, str, len);
		actualLength = (WORD)len;
		return TRUE;
	}
	const char* Get(char* buf) const
	{
		memcpy(buf, strData, actualLength);
		buf[actualLength] = '\0';
		return buf;
	}
	const char* Get(char* buf, int bufferLen) const
	{
		ASSERT_RETURN(bufferLen >= actualLength + 1, "");
		memcpy(buf, strData, actualLength);
		buf[actualLength] = '\0';
		return buf;
	}
};
struct KDBBaseBinary
{
	WORD actualLength;  // 实际数据长度
	char binData[1];
};
template <int size> struct KDBBinary : public KDBBaseBinary// size 可容纳的数据长度
{

	char _binData[size-1];
	BOOL Set(void* pData, int len)
	{
		if(len < 0 || len > size) return FALSE;
		memcpy(binData, pData, len);
		actualLength = len;
		return TRUE;
	}
	BOOL Append(void* pData, int len)
	{
		if(len < 0 || (len+actualLength) > size) return FALSE;
		memcpy(binData+actualLength, pData, len);
		actualLength += len;
		return TRUE;
	}

	int Get(void*& buf)
	{
		buf = &binData[0];
		return actualLength;
	}
	int GetSize() const
	{
		return size;
	}
};


struct KDbBinaryHead
{
	WORD length;
	char data[2];
};

#pragma pack(pop)

#endif