#ifndef _JG_CRYPT_TOOL_H_
#define _JG_CRYPT_TOOL_H_

#include <System/Collections/KString.h>
#include <System/Collections/KVector.h>
#include <System/Cipher/jgencryptor.h>

typedef System::Collections::KString<512> FilePath;
typedef System::Collections::KVector<FilePath> FilePathArray;

class IFileRefiner
{
public:
	virtual ~IFileRefiner() { }
	virtual BOOL TakeFile(const char* file) = 0;
};

class KEncryptRefiner : public IFileRefiner
{
public:
	BOOL TakeFile(const char* file);
};

class KDecryptRefiner : public IFileRefiner
{
public:
	BOOL TakeFile(const char* file);
};

class JgCryptTool
{
public:
	JgCryptTool();
	~JgCryptTool();

public:
	BOOL EncryptFile(const char* file);
	BOOL EncryptDir(const char* dir);
	BOOL EncryptDirRecursive(const char* dir);

	BOOL DecryptFile(const char* file);
	BOOL DecryptDir(const char* dir);
	BOOL DecryptDirRecursive(const char* dir);

public:
	BOOL refineFile(const char* file, IFileRefiner* refiner);
	BOOL refineDir(const char* dir, IFileRefiner* refiner);
	BOOL refineDirRecursive(const char* dir, IFileRefiner* refiner);

private:
	BOOL _refineFile(const char* file, IFileRefiner* refiner);
	BOOL _refineDir(const char* dir, IFileRefiner* refiner);

private:
	FilePathArray m_pendingDirs;
};

#endif