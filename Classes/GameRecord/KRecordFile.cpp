#include "KRecordFile.h"
#include "System/Misc/KStream.h"
#include <string>

#ifdef _USE_COCOS2DX
#include "cocos2d.h"
#endif

/////////////////////////////////////////////////////////////////////////
KGameRecordDataSaveStream::KGameRecordDataSaveStream()
: m_uPos( 0 )
{
}

KGameRecordDataSaveStream::~KGameRecordDataSaveStream()
{
	flush();
	Close();
}

BOOL KGameRecordDataSaveStream::Open(const char* filename, const char* mode)
{
	return TRUE;
}

int_r KGameRecordDataSaveStream::ReadData(void* value, size_t length)
{
	unsigned int uEndPos = m_uPos + length;
	if ( uEndPos > (unsigned int)m_Data.size() )
	{
		return 0;
	}
	memcpy( value, &m_Data.get(m_uPos), length );
	m_uPos = uEndPos;
	return length;
}

int_r KGameRecordDataSaveStream::WriteData( const void* value, size_t length )
{
	unsigned int uEndPos = m_uPos + length;
	if ( uEndPos > (unsigned int)m_Data.size() )
	{
		m_Data.resize( uEndPos );
	}
	memcpy( &m_Data.get(m_uPos), value, length );
	m_uPos = uEndPos;
	return length;
}

BOOL KGameRecordDataSaveStream::Seek(size_t pos)
{
	if ( pos >= (unsigned int)m_Data.size() )
	{
		return FALSE;
	}
	m_uPos = pos;
	return TRUE;
}

size_t KGameRecordDataSaveStream::tell() const
{
	return m_uPos;
}

void KGameRecordDataSaveStream::Close()
{
	m_Data.clear();
	m_uPos = 0;
}

void KGameRecordDataSaveStream::flush()
{
}

void KGameRecordDataSaveStream::Reset()
{
	Close();
}

//////////////////////////////////////////////////////////////


bool KRecordFile::SaveToFile( const char* szFileName, void* pRawData, unsigned int uRawDataLength )
{
	char sz[128]={0};
	sprintf(sz,"data/record/%s.rec",szFileName);
	
    std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(sz);
#else
	fullPath = sz;
#endif

	KOutputFileStream fileStream;

	if ( !fileStream.Open( fullPath.c_str(), "wb" ) )
	{
		return false;
	}

	fileStream.WriteData( pRawData, uRawDataLength );
	fileStream.flush();
	return true;
}

bool KRecordFile::LoadFile(const char* szFileName, KInputFileStream* inputStream)
{
	char sz[128]={0};
	sprintf(sz,"data/record/%s.rec",szFileName);
	
    std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(sz);
#else
	fullPath = sz;
#endif
	if (!inputStream->Open( fullPath.c_str(), "rb" ) )
	{
		return false;
	}
	return true;
}