#pragma once
#include "../Collections/KSDList.h"
#include "../Memory/RingBuffer.h"
#include "KStepObjectPool.h"
#include "../Misc/StreamInterface.h"

namespace piece_buffer_stream
{
	template <size_t pieceSize> struct piece_type : public KPieceBuffer<pieceSize>
	{
		piece_type* m_pNextNode;
	};

	template <typename T,size_t step=64,typename L=JG_S::KNoneLock> class alloc_type
	{
	private:
		typedef System::Memory::KStepObjectPool<T,step,L> pool_type;
		pool_type& pool()
		{
			static typename pool_type m_pool;
			return m_pool;
		}
	public:
		T* Alloc() { return this->pool().Alloc(); }
		void Free(T* p) { this->pool().Free(p); }
	};
}

template <
	size_t pieceSize=2048,
	typename A=piece_buffer_stream::alloc_type< piece_buffer_stream::piece_type<pieceSize>,64,JG_S::KMTLock >
>
class KPieceBufferStream : public StreamInterface
{
public:
	typedef A alloc_type;
	typedef piece_buffer_stream::piece_type<pieceSize> piece_type;
	typedef System::Collections::KSDSelfList<piece_type> queue_type;

public:
	int m_bytes;
	typename queue_type m_queue;
	alloc_type m_alloc;

public:
	KPieceBufferStream():m_bytes(0)
	{

	}
	~KPieceBufferStream()
	{
		this->reset();
	}

public: // from StreamInterface
	int_r ReadData(void* value, size_t length)
	{
		return this->read(value, length);
	}
	int_r WriteData(const void* value, size_t length)
	{
		return this->write(value, length);
	}
	BOOL Seek(size_t pos)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	size_t tell() const
	{
		ASSERT(FALSE);
		return FALSE;
	}
	void Close()
	{

	}
	void flush()
	{

	}
	int ReadString(char* value, size_t bufLength)
	{
		WORD len;
		if(!this->Read(len)) return 0;
		if((int)bufLength < len+1) return 0;
		int bytes = this->read(value, len);
		if(bytes != len) return 0;
		value[len] = '\0';
		return sizeof(len) + len;
	}
	int ReadString8(char* value, size_t bufLength)
	{
		BYTE len;
		if(!this->Read(len)) return 0;
		if((int)bufLength < len+1) return 0;
		int bytes = this->read(value, len);
		if(bytes != len) return 0;
		value[len] = '\0';
		return sizeof(len) + len;
	}
	int ReadString32(char* value, size_t bufLength)
	{
		int len;
		if(!this->Read(len)) return 0;
		if((int)bufLength < len+1) return 0;
		int bytes = this->read(value, len);
		if(bytes != len) return 0;
		value[len] = '\0';
		return sizeof(len) + len;
	}
	BOOL WriteString(const char* value, size_t length=0)
	{
		if(!length) length = strlen(value);
		WORD len = (WORD)length;
		if(!this->Write(len)) return FALSE;
		return this->write(value, len) == len;
	}
	BOOL WriteString8(const char* value, size_t length=0)
	{
		if(!length) length = strlen(value);
		BYTE len = (BYTE)length;
		if(!this->Write(len)) return FALSE;
		return this->write(value, len) == len;
	}
	BOOL WriteString32(const char* value, size_t length=0)
	{
		if(!length) length = strlen(value);
		int len = (int)length;
		if(!this->Write(len)) return FALSE;
		return this->write(value, len) == len;
	}
	template <typename T> BOOL Write(const T& val)
	{
		return this->write(&val, sizeof(T)) == sizeof(T);
	}
	template <typename T> BOOL Read(T& val)
	{
		return this->read(&val, sizeof(T)) == sizeof(T);
	}

public:
	int readSkip(int len)
	{
		int sum = 0;
		char buf[256];
		while(len > 0)
		{
			int n = kmin(len, (int)sizeof(buf));
			int bytes = this->read(buf, n);
			if(bytes > 0) { len -= bytes; sum += bytes; }
			if(bytes != n) return sum;
		}
		return sum;
	}
	int readTo(StreamInterface* so, int len)
	{
		int sum = 0;
		char buf[256];
		while(len > 0)
		{
			int n = kmin(len, (int)sizeof(buf));
			int bytes = this->read(buf, n);
			if(bytes > 0)
			{
				len -= bytes;
				sum += bytes;
				if(so->WriteData(buf, bytes) != bytes)
					return sum;
			}
			if(bytes != n) return sum;
		}
		return sum;
	}
	int peek(void* pData, int len)
	{
		int lenOrg = len;
		char* p = (char*)pData;

		if(m_queue.empty())
			return 0;

		typename piece_type* piece = m_queue.begin();
		while(piece)
		{
			int nBytes = this->_peekPiece(piece, p, len); piece = m_queue.next(piece);
			len -= nBytes; p += nBytes;
			if(len < 1) break;
		}

		return lenOrg - len;
	}
	int read(void* pData, int len)
	{
		int lenOrg = len;
		char* p = (char*)pData;
		while(len > 0)
		{
			if(m_queue.empty()) break;
			int nBytes = this->_read(p, len);
			if(nBytes == 0) break;
			len -= nBytes; p += nBytes;
		}
		return lenOrg - len;
	}
	int write(const void* pData, int len)
	{
		int lenOrg = len;
		const char* p = (const char*)pData;

		while(len > 0)
		{
			int nBytes = this->_currentWriteAvail();
			if(nBytes > 0)
			{
				nBytes = this->_write(p, len);
				p += nBytes; len -= nBytes;
			}
			else
			{
				if(!this->inflate()) break;
			}
		}
		return lenOrg - len;
	}
	int size() const
	{
		return m_bytes;
	}
	void reset()
	{
		while(!m_queue.empty())
		{
			typename piece_type* pPiece = m_queue.pop_front();
			m_alloc.Free(pPiece);
		}
		m_bytes = 0;
	}

private:
	int _peekPiece(piece_type* piece, char* pData, int len)
	{
		int rp = piece->m_rp;
		int nBytes = piece->Read(pData, len);
		piece->m_rp = rp;
		return nBytes;
	}
	int _read(void* pData, int len)
	{
		ASSERT(!m_queue.empty());
		typename piece_type* pPiece = m_queue.begin();
		int nBytes = pPiece->Read(pData, len);
		m_bytes -= nBytes; ASSERT(m_bytes >= 0);
		this->compact();
		return nBytes;
	}
	int _write(const void* pData, int len)
	{
		typename piece_type* pPiece = m_queue.rbegin();
		ASSERT(pPiece);

		int nBytes = pPiece->Write(pData, len);
		m_bytes += nBytes;
		return nBytes;
	}
	int _currentWriteAvail()
	{
		typename piece_type* pPiece = m_queue.rbegin();
		if(!pPiece) return 0;
		return pPiece->WriteAvail();
	}
	BOOL inflate()
	{
		typename piece_type* pPiece = m_alloc.Alloc();
		if(!pPiece) return FALSE;
		pPiece->Reset();
		m_queue.push_back(pPiece);
		return TRUE;
	}
	void compact()
	{
		if(m_queue.empty()) return;

		typename piece_type* pPiece = m_queue.begin();
		if(pPiece->ReadAvail()) return;

		if(m_queue.begin() == m_queue.rbegin())
		{
			pPiece->Reset();
			return;
		}

		if(pPiece->WriteAvail()) return;
		m_queue.pop_front();
		m_alloc.Free(pPiece);

		return;
	}
};