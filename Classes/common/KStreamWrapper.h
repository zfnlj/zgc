#ifndef _JG_STREAM_WRAPPER_H_
#define _JG_STREAM_WRAPPER_H_

#include <System/KType.h>
#include <System/KMacro.h>
#include <System/Misc/StreamInterface.h>
#include "OgreDataStream.h"

using namespace Ogre;

class KStreamWrapper : public StreamInterface
{
public:
	KStreamWrapper();
	~KStreamWrapper();
	void init(DataStream* ptr);
public:
	operator DataStream* ();
	KStreamWrapper& operator = (DataStream* pStream);
	DataStream* operator -> ();
	const DataStream* operator -> () const;
	BOOL operator == (DataStream* o) const;
	BOOL isNull() const;

public:
	int_r ReadData(void* value, size_t length);
	int_r WriteData(const void* value, size_t length);
	BOOL Seek(size_t pos);
	size_t tell() const;
	void Close();
	void flush() { }

public:
	DataStream* m_streamPtr;
};

#endif