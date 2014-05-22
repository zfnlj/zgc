#include "JgCryptTool.h"
#include <System/Log/log.h>
#include <System/File/KFile.h>

using namespace System::File;

BOOL KEncryptRefiner::TakeFile(const char* file)
{
	FilePath filepath(file);
	FilePath copypath(file); copypath.append(".copy");

	KInputFileStream fi;
	if(!fi.Open(filepath.c_str()))
	{
		Log(LOG_ERROR, "encrypt fail open, %s", filepath.c_str());
		return FALSE;
	}

	JgEncStream esi;
	if(!esi.setInputStream(&fi))
	{
		Log(LOG_ERROR, "encrypt fail input, %s", filepath.c_str());
		return FALSE;
	}

	if(esi.m_encMode)
	{
		Log(LOG_ERROR, "not changed %s", filepath.c_str());
		return TRUE;
	}

	KOutputFileStream fo;
	if(!fo.Open(copypath.c_str(), "w+b"))
	{
		Log(LOG_ERROR, "encrypt open new, %s", copypath.c_str());
		return FALSE;
	}

	JgEncStream eso;
	eso.setOutputStream(&fo, 4);

	char buf[1024];
	while(TRUE)
	{
		int len = esi.ReadData(buf, sizeof(buf));
		if(len < 1) break;
		eso.WriteData(buf, len);
	}

	fi.Close();
	fo.Close();

	if(!DeleteFileA(filepath.c_str()))
	{
		Log(LOG_ERROR, "error: encrypt delete %s", filepath.c_str());
		return FALSE;
	}

	if(!MoveFileA(copypath.c_str(), filepath.c_str()))
	{
		Log(LOG_ERROR, "error: encrypt move %s -> %s", copypath.c_str(), filepath.c_str());
		return FALSE;
	}

	Log(LOG_ERROR, "encrypt %s", filepath.c_str());
	return TRUE;
}

BOOL KDecryptRefiner::TakeFile(const char* file)
{
	FilePath filepath(file);
	FilePath copypath(file); copypath.append(".copy");

	KInputFileStream fi;
	if(!fi.Open(filepath.c_str()))
	{
		Log(LOG_ERROR, "decrypt fail open, %s", filepath.c_str());
		return FALSE;
	}

	JgEncStream esi;
	if(!esi.setInputStream(&fi))
	{
		Log(LOG_ERROR, "decrypt fail input, %s", filepath.c_str());
		return FALSE;
	}

	if(!esi.m_encMode)
	{
		Log(LOG_ERROR, "not changed %s", filepath.c_str());
		return TRUE;
	}

	KOutputFileStream fo;
	if(!fo.Open(copypath.c_str(), "w+b"))
	{
		Log(LOG_ERROR, "decrypt open new, %s", copypath.c_str());
		return FALSE;
	}

	JgEncStream eso;
	eso.setOutputStream(&fo, 0);

	char buf[1024];
	while(TRUE)
	{
		int len = esi.ReadData(buf, sizeof(buf));
		if(len < 1) break;
		eso.WriteData(buf, len);
	}

	fi.Close();
	fo.Close();

	if(!DeleteFileA(filepath.c_str()))
	{
		Log(LOG_ERROR, "error: decrypt delete %s", filepath.c_str());
		return FALSE;
	}

	if(!MoveFileA(copypath.c_str(), filepath.c_str()))
	{
		Log(LOG_ERROR, "error: decrypt move %s -> %s", copypath.c_str(), filepath.c_str());
		return FALSE;
	}

	Log(LOG_ERROR, "decrypt %s", filepath.c_str());
	return TRUE;
}

JgCryptTool::JgCryptTool()
{

}

JgCryptTool::~JgCryptTool()
{

}

BOOL JgCryptTool::EncryptFile(const char* file)
{
	KEncryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start encrypt %s", file);
	BOOL bRet = this->refineFile(file, &refiner);
	Log(LOG_ERROR, ">>>> end encrypt %s", file);
	return bRet;
}

BOOL JgCryptTool::EncryptDir(const char* dir)
{
	KEncryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start encrypt %s", dir);
	BOOL bRet = this->refineDir(dir, &refiner);
	Log(LOG_ERROR, ">>>> end encrypt %s", dir);
	return bRet;
}

BOOL JgCryptTool::EncryptDirRecursive(const char* dir)
{
	KEncryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start recursive encrypt %s", dir);
	BOOL bRet = this->refineDirRecursive(dir, &refiner);
	Log(LOG_ERROR, ">>>> end recursive encrypt %s", dir);
	return bRet;
}

BOOL JgCryptTool::DecryptFile(const char* file)
{
	KDecryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start decrypt %s", file);
	BOOL bRet = this->refineFile(file, &refiner);
	Log(LOG_ERROR, ">>>> end decrypt %s", file);
	return bRet;
}

BOOL JgCryptTool::DecryptDir(const char* dir)
{
	KDecryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start decrypt %s", dir);
	BOOL bRet = this->refineDir(dir, &refiner);
	Log(LOG_ERROR, ">>>> end decrypt %s", dir);
	return bRet;
}

BOOL JgCryptTool::DecryptDirRecursive(const char* dir)
{
	KDecryptRefiner refiner;
	Log(LOG_ERROR, ">>>> start recursive decrypt %s", dir);
	BOOL bRet = this->refineDirRecursive(dir, &refiner);
	Log(LOG_ERROR, ">>>> end recursive decrypt %s", dir);
	return bRet;
}

BOOL JgCryptTool::refineFile(const char* file, IFileRefiner* refiner)
{
	m_pendingDirs.clear();
	return refiner->TakeFile(file);
}

BOOL JgCryptTool::refineDir(const char* dir, IFileRefiner* refiner)
{
	m_pendingDirs.clear();
	return this->_refineDir(dir, refiner);
}

BOOL JgCryptTool::refineDirRecursive(const char* dir, IFileRefiner* refiner)
{
	m_pendingDirs.clear();

	System::Collections::KString<512> firstDir(dir);
	m_pendingDirs.push_back(firstDir);

	do 
	{
		System::Collections::KString<512> folder = m_pendingDirs[0];
		m_pendingDirs.erase(0);

		this->_refineDir(folder.c_str(), refiner);
	}
	while (m_pendingDirs.size());

	return TRUE;
}

BOOL JgCryptTool::_refineFile(const char* file, IFileRefiner* refiner)
{
	return refiner->TakeFile(file);
}

BOOL JgCryptTool::_refineDir(const char* dir, IFileRefiner* refiner)
{
	KDirectory folder;
	if(!folder.Open(dir))
	{
		Log(LOG_ERROR, "error: crypt tool open dir %s", dir);
		return FALSE;
	}

	KDirectory::FileList flist;
	KFileSuffixFilter emptyFilter("");
	KFileSuffixMultiFilter filter2(".txt|.cfg|.xml|.lua");

	if(!folder.ListFile(flist, &emptyFilter))
	{
		Log(LOG_ERROR, "error: crypt tool, list files, %s", dir);
		return FALSE;
	}

	int c = flist.size();
	System::Collections::KString<512> fullpath;

	for(int i=0; i<c; i++)
	{
		KFileData& fdata = flist[i];

		if(fdata.m_shortName[0] == '.')
			continue;

		fullpath = fdata.m_dirName;
		fullpath.append('/');
		fullpath.append(fdata.m_shortName);

		if(KFileUtil::IsDir(fullpath.c_str()))
		{
			m_pendingDirs.push_back(fullpath);
			continue;
		}
		if(filter2.Accept(fdata))
		{
			this->_refineFile(fullpath.c_str(), refiner);
		}
	}

	return TRUE;
}