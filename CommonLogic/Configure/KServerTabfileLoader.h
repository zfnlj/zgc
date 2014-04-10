#pragma once

#include "System/File/KTabfileLoader.h"
#include <System/Collections/KString.h>
#include <System/File/KTabFile.h>
#include <System/Memory/KStepObjectPool.h>

class KServerTabFileReader : public JG_M::KPortableStepPool<KServerTabFileReader,4>
{
public:
	JG_F::KTabFile2 m_fileReader;
	KInputFileStream m_istream;

public:
	KServerTabFileReader();
	void Close();
	void Release();
};

class KServerTabfileLoader : public KTabfileLoader
{
public:
	KServerTabfileLoader();
	~KServerTabfileLoader();
	static KServerTabfileLoader& GetInstance();

public:
	// filename
	// 服务器端，不包含 Settings 以及上层的路径
	// 客户端，不包含 data/configs 以及上层的路径
	JG_F::KTabFile2* GetFileReader(const char* filename);
	void CloseFileReader(JG_F::KTabFile2* pReader);

	JG_F::KTabFile* CreateTabfile(const char* filename);
	void DestroyTabfile(JG_F::KTabFile* p);

	StreamInterface* GetFileStream(const char* filename, const char* grp);
	void CloseFileStream(StreamInterface* stream);

private:
	JG_C::KString<512> GetBaseFilepath(const char* filename);
	JG_C::KString<512> GetPatchFilepath(const char* filename);
};
