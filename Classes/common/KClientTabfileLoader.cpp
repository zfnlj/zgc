#include "KClientTabfileLoader.h"
#include <System/Misc/KCheckSum.h>
#include "System/File/KTabFile.h"
#ifdef _USE_COCOS2DX
#include "platform/CCFileUtils.h"
USING_NS_CC;
#endif
#include "../Inc/KTypeDef.h"

using namespace System::File;
using namespace System::Collections;


KFileStreamBase::KFileStreamBase()
{
	
}


KFileStreamBase::~KFileStreamBase()
{
	this->Close();
}

void KFileStreamBase::init()
{
	m_pBuffer = NULL;
	m_Size = 0;
	m_pos = 0;
}

BOOL KFileStreamBase::Open(const char* filename)
{
	FILE *fp = fopen(filename, "rb");
	if(!fp){
		return false;
	}

	fseek(fp,0,SEEK_END);
    m_Size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    m_pBuffer = new unsigned char[m_Size];
    fread(m_pBuffer,sizeof(unsigned char), m_Size,fp);
	fclose(fp); 
	m_pos = 0;
	return TRUE;
}

int KFileStreamBase::ReadData(void* value, size_t length)
{
	size_t readLen = ((m_Size - m_pos)>length)? length : (m_Size - m_pos);
	if(readLen >0){
		memcpy(value,m_pBuffer+m_pos,readLen);
	}
	m_pos += readLen;
	return readLen;
}

int KFileStreamBase::WriteData(const void* value, size_t length)
{
	return -1;
}

BOOL KFileStreamBase::Seek(size_t pos)
{
	if( pos > m_Size)
		return FALSE;
	m_pos = pos;
	return TRUE;
}

size_t KFileStreamBase::tell() const
{
	return m_pos;
}

void KFileStreamBase::Close()
{
	CC_SAFE_DELETE_ARRAY(m_pBuffer);
	m_Size = m_pos = 0;
	/*if(!m_pDataStream.isNull())
	{
		m_pDataStream->close();
		m_pDataStream.setNull();
	}*/
}

void KFileStreamBase::flush()
{
	
}

// KClientFileStream
KClientFileStream::KClientFileStream()
{

}

KClientFileStream::~KClientFileStream()
{
	this->Close();
}

void KClientFileStream::init()
{
	m_baseStream.init();
}

BOOL KClientFileStream::Open(const char* filename)
{
	if(!m_baseStream.Open(filename)) return FALSE;
	if(!m_encStream.setInputStream(&m_baseStream)) return FALSE;
	return TRUE;
}

int KClientFileStream::ReadData(void* value, size_t length)
{
	return m_encStream.ReadData(value, length);
}

int KClientFileStream::WriteData(const void* value, size_t length)
{
	return 0;
}

BOOL KClientFileStream::Seek(size_t pos)
{
	return m_encStream.Seek(pos);
}

size_t KClientFileStream::tell() const
{
	return m_encStream.tell();
}

void KClientFileStream::Close()
{
	m_encStream.Close();
	m_baseStream.Close();
}

void KClientFileStream::flush()
{
	m_encStream.flush();
}

void KClientTabfileReader::init()
{
	m_fiBase.init();
}

// KClientTabfileReader
void KClientTabfileReader::Close()
{
	m_fileReader.Clear();
	m_fiBase.Close();
	m_fiPatch.Close();
}

void KClientTabfileReader::Release()
{
	this->Close();
	KClientTabfileReader::Free(this);
}

KClientTabfileLoader::KClientTabfileLoader()
{
	
}

void KClientTabfileLoader::init()
{
	KTabfileLoader::SetInstance(*this);
}

KClientTabfileLoader::~KClientTabfileLoader()
{

}


KClientTabfileLoader& KClientTabfileLoader::GetInstance()
{
	static KClientTabfileLoader g_instance;
	g_instance.init();
	return g_instance;
}

BOOL KClientTabfileLoader::SavePatch(const char* fileName, const void* pData, int len)
{
	KPatchFile* pPatch = this->GetPatchFile(fileName);
	if(pPatch)
	{
		KDataChecksum originalSum = KCheckSum::CalcChecksum(pPatch->fileData, pPatch->dataLength);
		KDataChecksum newSum = KCheckSum::CalcChecksum(pData, len);
		if (originalSum.checksum != newSum.checksum)
		{
			pPatch->dataLength = len;
			pPatch->fileData = this->WriteValue(pData, len);
		}
	}
	else
	{
		KPatchFile patchFile;
		patchFile.fileName = this->WriteValue(fileName, strlen(fileName)+1);
		patchFile.dataLength = len;
		patchFile.fileData = this->WriteValue(pData, len);
		m_patchArray.insert_unique(patchFile);
	}
	return TRUE;
}

KTabFile2* KClientTabfileLoader::GetFileReader(const char* filename)
{
	KClientTabfileReader* pFileReader = KClientTabfileReader::Alloc();
	pFileReader->init();
	ASSERT(pFileReader); pFileReader->Close();
    
    

	KTabFile2& m_fileReader = pFileReader->m_fileReader;
	KClientFileStream& m_fiBase = pFileReader->m_fiBase;


	if(!m_fiBase.Open(filename))
	{
		pFileReader->Release();
		return NULL;
	}

	if(!m_fileReader.Open(m_fiBase))
	{
		pFileReader->Release();
		return NULL;
	}
	return &m_fileReader;
}


void KClientTabfileLoader::CloseFileReader(KTabFile2* pReader)
{
	KClientTabfileReader* pFileReader = (KClientTabfileReader*)pReader;
	pFileReader->Release();
}

KTabFile* KClientTabfileLoader::CreateTabfile(const char* filename)
{
	KTabFile2* pReader = this->GetFileReader(filename);
	if(!pReader) return NULL;

	KTabFile* pTabfile = new KTabFile();
	if(!pTabfile->CreateFrom(*pReader))
	{
		this->CloseFileReader(pReader);
		delete pTabfile;
		return NULL;
	}
	
	this->CloseFileReader(pReader);
	return pTabfile;
}

void KClientTabfileLoader::DestroyTabfile(KTabFile* p)
{
	if(!p) return;
	delete p;
}

KPatchFile* KClientTabfileLoader::GetPatchFile(const char* fileName)
{
	KString<256> fn(fileName);

	int c = fn.size();
	for(int i=0; i<c; i++)
	{
		char ch = fn[i];
		if(ch == '\\')
		{
			fn[i] = '/';
		}
	}
	fileName = fn.c_str();

	KPatchFile patchFile;
	patchFile.fileName = (char*)fileName;
	int pos = m_patchArray.find(patchFile);
	if(pos < 0) return NULL;
	return &m_patchArray[pos];
}

char* KClientTabfileLoader::WriteValue(const void* pData, int len)
{
	KBuffer64k* pPiece = NULL;
	if(m_bufferArray.empty())
	{
		pPiece = KBuffer64k::Alloc();
		pPiece->Reset();
		m_bufferArray.push_back(pPiece);
	}
	else
	{
		int idx = m_bufferArray.size() - 1;
		pPiece = m_bufferArray[idx];
		if(pPiece->WriteAvail() < len)
		{
			pPiece = KBuffer64k::Alloc();
			pPiece->Reset();
			m_bufferArray.push_back(pPiece);
		}
	}

	char* pRet = pPiece->GetWriteBuffer();
	pPiece->Write(pData, len);
	return pRet;
}

void KClientTabfileLoader::CloseFileStream(StreamInterface* stream)
{
	if(!stream) return;
	size_t offt = offsetof(KClientTabfileReader, m_fiBase);
	KClientFileStream* p = (KClientFileStream*)stream;
	KClientTabfileReader* pFileReader = (KClientTabfileReader*)((char*)p-offt);
	pFileReader->Release();
}

void KClientTabfileLoader::Clear()
{
	int nCount = m_bufferArray.size();
	for(int i = 0; i < nCount; i++)
	{
		KBuffer64k* pPiece = m_bufferArray[i];
		if(pPiece != NULL)
		{
			KBuffer64k::Free(pPiece);
		}
	}
	m_bufferArray.clear();
}

KString<512> KClientTabfileLoader::GetBaseFilepath( const char* filename )
{
	KString<512> path("../../data/configs/");
	path.append(filename);
	return path;
}

KString<512> KClientTabfileLoader::GetPatchFilepath( const char* filename )
{
	KString<512> path("../../data/Patch/Client/");
	path.append(filename);
	return path;
}

StreamInterface* KClientTabfileLoader::GetFileStream(const char* filename, const char* grp)
{
	KClientTabfileReader* pFileReader = KClientTabfileReader::Alloc();
	ASSERT(pFileReader); pFileReader->Close();

	KClientFileStream& m_fiBase = pFileReader->m_fiBase;
	if(!m_fiBase.Open(filename))
	{
		pFileReader->Release();
		return NULL;
	}
	return &m_fiBase;
}

