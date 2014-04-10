#ifndef _K_CLIENT_TABFILE_LOADER_H_
#define _K_CLIENT_TABFILE_LOADER_H_

#include <System/File/KTabfileLoader.h>
#include <System/Cipher/jgencryptor.h>


class KFileStreamBase : public StreamInterface
{
public:
	KFileStreamBase();
	~KFileStreamBase();

public:
	void init();
	BOOL Open(const char* filename);
	int  ReadData(void* value, size_t length);
	int  WriteData(const void* value, size_t length);
	BOOL Seek(size_t pos);
	size_t tell() const;
	void Close();
	void flush();

private:
	unsigned char* m_pBuffer;
	size_t m_Size;
	int m_pos;
};

class KClientFileStream : public StreamInterface
{
public:
	KClientFileStream();
	~KClientFileStream();

public:
	void init();
	BOOL Open(const char* filename);
	int  ReadData(void* value, size_t length);
	int  WriteData(const void* value, size_t length);
	BOOL Seek(size_t pos);
	size_t tell() const;
	void Close();
	void flush();

public:
	KFileStreamBase m_baseStream;
};

struct KPatchFile
{
	char* fileName;
	int dataLength;
	char* fileData;
};

DECLARE_SIMPLE_TYPE(KPatchFile);

class KClientTabfileReader : public System::Memory::KPortableStepPool<KClientTabfileReader,4>
{
public:
	System::File::KTabFile2 m_fileReader;
	KClientFileStream m_fiBase;
	KMsgInputStream m_fiPatch;

	void init();
public:
	void Close();
	void Release();
};

class KClientTabfileLoader : public KTabfileLoader
{
public:
	class KPatchCompare
	{
	public:
		BOOL operator () (const KPatchFile& a, const KPatchFile& b) const
		{
			return stricmp(a.fileName, b.fileName) < 0;
		}
	};
	
	typedef System::Collections::DynArray<KBuffer64k*> KBufferArray;
	typedef System::Collections::DynSortedArray<KPatchFile,KPatchCompare> KPatchArray;

public:
	KClientTabfileLoader();
	~KClientTabfileLoader();
	static KClientTabfileLoader& GetInstance();
	void init();
public:
	BOOL SavePatch(const char* fileName, const void* pData, int len);

public:
	System::File::KTabFile2* GetFileReader(const char* filename);

	void CloseFileReader(System::File::KTabFile2* pReader);
	System::File::KTabFile* CreateTabfile(const char* filename);
	void DestroyTabfile(System::File::KTabFile* p);

	void CloseFileStream(StreamInterface* stream);

	void Clear();
	StreamInterface* GetFileStream(const char* filename, const char* grp);

private:
	System::Collections::KString<512> GetBaseFilepath(const char* filename);
	System::Collections::KString<512> GetPatchFilepath(const char* filename);

public:
	KPatchFile* GetPatchFile(const char* fileName);
	char* WriteValue(const void* pData, int len);

public:
	KPatchArray m_patchArray;
	KBufferArray m_bufferArray;
};

#endif

