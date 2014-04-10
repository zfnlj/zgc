#include "KStreamWrapper.h"
#include "OgreDataStream.h"

KStreamWrapper::KStreamWrapper()
{

}

void KStreamWrapper::init(DataStream* ptr)
{
	m_streamPtr = ptr;
}

KStreamWrapper::~KStreamWrapper()
{
}

KStreamWrapper::operator DataStream* ()
{
	return m_streamPtr;
}

KStreamWrapper& KStreamWrapper::operator = (DataStream* pStream)
{
	m_streamPtr = pStream;
	return *this;
}

DataStream* KStreamWrapper::operator -> ()
{
	return m_streamPtr;
}

const DataStream* KStreamWrapper::operator -> () const
{
	return m_streamPtr;
}

BOOL KStreamWrapper::operator == (DataStream* o) const
{
	return m_streamPtr == o;
}

BOOL KStreamWrapper::isNull() const
{
	return m_streamPtr==NULL;
}

int_r KStreamWrapper::ReadData(void* value, size_t length)
{
	return m_streamPtr->read(value, length);
}

int_r KStreamWrapper::WriteData(const void* value, size_t length)
{
	return 0;
}

BOOL KStreamWrapper::Seek(size_t pos)
{
	m_streamPtr->seek(pos);
	return TRUE;
}

size_t KStreamWrapper::tell() const
{
	return m_streamPtr->tell();
}

void KStreamWrapper::Close()
{
	if(m_streamPtr)
	{
		m_streamPtr->close();
		m_streamPtr = NULL;
	}
}