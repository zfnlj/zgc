#ifndef _KRECORDFILE_H
#define _KRECORDFILE_H
#include <System/Misc/StreamInterface.h>
#include "System/Collections/DynArray.h"

class KInputFileStream;
class KGameRecordDataSaveStream : public StreamInterface
{
public:
	KGameRecordDataSaveStream();
	virtual ~KGameRecordDataSaveStream();
	BOOL Open(const char* filename, const char* mode);
	virtual int_r ReadData(void* value, size_t length);
	virtual int_r WriteData(const void* value, size_t length);
	virtual BOOL Seek(size_t pos);
	virtual size_t tell() const;
	virtual void Close();
	virtual void flush();
	void Reset();
	void* GetTargetDataStream() { return (void*)m_Data.m_vals; }
	unsigned int GetTargetDataStreamSize() { return m_Data.size(); }

private:
	::System::Collections::DynArray<BYTE,1024,1024> m_Data;
	unsigned int m_uPos;
};


class KRecordFile
{
public:
	KRecordFile(){}
	~KRecordFile(){}
	
	bool SaveToFile( const char* szFileName, void* pRawData, unsigned int uRawDataLength );
	bool LoadFile(const char* szFileName, KInputFileStream* inputStream);
};

#endif // __HELLOWORLD_SCENE_H__
