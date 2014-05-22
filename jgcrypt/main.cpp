#include "JgCryptTool.h"
#include <System/File/KFile.h>
#include <System/Log/log.h>

using namespace System::File;

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("usage: %s encrypt|decrypt file|dir\n", argv[0]);
		return 1;
	}

	enum { encrypt_mode = 1, decrypt_mode = 2 };

	DirectFileLogger logger("./jgcrypt.log");
	logger.SetLogLevel(LOG_ERROR|LOG_FATAL);
	GetGlobalLogger().SetLogger(&logger);

	const char* smode = argv[1];
	const char* target = argv[2];

	int mode = encrypt_mode;
	if(stricmp(smode, "encrypt") == 0)
	{
		mode = encrypt_mode;
	}
	else if(stricmp(smode, "decrypt") == 0)
	{
		mode = decrypt_mode;
	}
	else
	{
		printf("usage: %s encrypt|decrypt file|dir\n", argv[0]);
		return 1;
	}

	if(!KFileUtil::IsFileExists(target))
	{
		printf("file %s not found\n", target);
		return 1;
	}

	JgCryptTool tool;

	if(KFileUtil::IsDir(target))
	{
		if(mode == encrypt_mode)
			tool.EncryptDirRecursive(target);
		else
			tool.DecryptDirRecursive(target);
	}
	else
	{
		if(mode == encrypt_mode)
			tool.EncryptFile(target);
		else
			tool.DecryptFile(target);
	}

	return 0;
}