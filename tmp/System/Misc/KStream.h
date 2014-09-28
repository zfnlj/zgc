#ifndef _K_STREAM_H_
#define _K_STREAM_H_

#include "../KType.h"
#include <stdio.h>
#include "../KPlatform.h"
#include "./StreamInterface.h"

class KMemoryStream : public StreamInterface
{
public:
	KMemoryStream();
	virtual ~KMemoryStream() {}

public:
	void bind(void* p, size_t len);
	void clear() { m_pos = 0; }

public:
	virtual int_r ReadData(void* value, size_t length);
	virtual int_r WriteData(const void* value, size_t length);
	virtual BOOL CheckBuffer(size_t wantedBytes);

public:
	BOOL Insert(size_t pos, const void* pData, size_t len);
	BOOL Remove(size_t pos, size_t len);

public:
	BOOL Seek(size_t pos);
	size_t tell() const { return m_pos; }
	void Close();
	void flush();

public:
	const void* data() const { return m_pBuf; }
	size_t size() const { return m_pos; }

public:
	BOOL ReadInt(int& value);
	BOOL ReadShort(short& value);
	BOOL ReadByte(BYTE& value);
	BOOL ReadWord(WORD& value);
	BOOL ReadDword(DWORD& value);
	BOOL ReadFloat(float& value);
	BOOL ReadDouble(double& value);
	BOOL ReadInt64(INT64& value);
	BOOL ReadUint64(UINT64& value);
	BOOL ReadByteArray(void* value, size_t length);
	BOOL ReadString(char* value, size_t bufLength);
	BOOL ReadString8(char* value, size_t bufLength);
	BOOL ReadString32(char* value, size_t bufLength);
	template<typename T>
	BOOL Read(T& value)
	{
		return ReadByteArray(&value, sizeof(T));
	}

public:
	BOOL WriteInt(int value);
	BOOL WriteShort(short value);
	BOOL WriteByte(BYTE value);
	BOOL WriteWord(WORD value);
	BOOL WriteDword(DWORD value);
	BOOL WriteFloat(float value);
	BOOL WriteDouble(double value);
	BOOL WriteInt64(INT64 value);
	BOOL WriteUint64(UINT64 value);
	BOOL WriteByteArray(const void* value, size_t length);
	BOOL WriteString(const char* value, size_t length=0);
	BOOL WriteString8(const char* value, size_t length=0);
	BOOL WriteString32(const char* value, size_t length=0);

	template<typename T> BOOL Write(const T& value)
	{
		return WriteByteArray(&value, sizeof(T));
	}
	// 增加个简单类型的>> by Allen, 20121107
	template<typename T> KMemoryStream& operator>>(T& o)
	{
		Read(o);
		return *this;
	}
	template<typename T> KMemoryStream& operator<<(const T& o)
	{
		Write(o);
		return *this;
	}

public:
	size_t m_pos;
	char* m_pBuf;
	size_t m_bufLength;
};

class KMsgInputStream : public KMemoryStream
{
public:
	KMsgInputStream() {}
	KMsgInputStream(const void* buf, size_t length)
	{
		m_pBuf = (char*)buf;
		m_bufLength = length;
		m_pos = 0;
	}
	void Attach(const void* buf, size_t length)
	{
		m_pBuf = (char*)buf;
		m_bufLength = length;
		m_pos = 0;
	}
	const void* GetCurrentHead() const
	{
		return &m_pBuf[m_pos];
	}
	size_t GetDataSizeLeft() const
	{
		if (m_pos >= m_bufLength) return 0;
		return m_bufLength - m_pos;
	}
};

class KMsgOutputStream : public KMemoryStream
{
public:
	KMsgOutputStream();
	~KMsgOutputStream();
	void Close();
	BOOL CheckBuffer(size_t wantedBytes);
	BOOL Resize(size_t length);

protected:
	BOOL SetLength(size_t length);
};

class KInputFileStream : public StreamInterface
{
public:
	KInputFileStream();
	KInputFileStream(FILE* f, bool own=false);
	KInputFileStream(const char* filename);
	virtual ~KInputFileStream();

public:
	bool attach(FILE* filep) { ASSERT(filep); m_pfile = filep; return true; }
	void detach() { m_pfile = NULL; }
	
public:
	void Close();
	BOOL Open(const char* filename);
	BOOL Open(const char* filename, const char* mode);
	BOOL Good() const;
	int_r ReadData(void* value, size_t length);
	int_r WriteData(const void* value, size_t length);
	BOOL Seek(size_t pos);
	size_t tell() const;
	void flush();

public:
	bool m_own;
	FILE* m_pfile;
};

class KOutputFileStream : public StreamInterface
{
public:
	KOutputFileStream();
	KOutputFileStream(FILE* f, bool own=false);
	KOutputFileStream(const char* filename, const char* mode);
	virtual ~KOutputFileStream();

public:
	bool attach(FILE* filep) { ASSERT(filep); m_pfile = filep; return true; }
	void detach() { m_pfile = NULL; }

public:
	void Close();
	BOOL Open(const char* filename, const char* mode);
	BOOL OpenForce(const char* filename, const char* mode);
	BOOL Good() const;
	int_r ReadData(void* value, size_t length);
	int_r WriteData(const void* value, size_t length);
	BOOL Seek(size_t pos);
	size_t tell() const;
	void flush();

public:
	bool m_own;
	FILE* m_pfile;
};

class KDataOutputStream : public KMemoryStream
{
public:
	KDataOutputStream(void* data, size_t len)
	{
		m_pBuf = (char*)data;
		m_pos = 0;
		m_bufLength = len;
	}
};

template <size_t capacity> class KBufferOutputStream : public KMemoryStream
{
public:
	char m_buf[capacity];
	KBufferOutputStream()
	{
		m_pos = 0;
		m_pBuf = (char*)&m_buf;
		m_bufLength = capacity;
	}
};

#endif