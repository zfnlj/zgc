#pragma once
#include "../KPlatform.h"
#include "../Collections/KSDList.h"
#include "../Collections/KHashTable.h"
#include "../Memory/RingBuffer.h"
#include "StreamInterface.h"
#include "../File/KTabfileLoader.h"

class KDiffFileReader;
class KStringtable
{
public:
	typedef ::System::Collections::KString<2048> _KString;
	struct Buffer : public KPieceBuffer<10240> { Buffer* m_pNextNode; };
	typedef ::System::Collections::KSDSelfList<Buffer> BufferList;
	typedef ::System::Collections::KHashTable<int,ccstr,KIntCompare<int>,KHasher<int>,JG_S::KNoneLock,2048,8192> Map;

public:
	class IVisitor
	{
	public:
		virtual ~IVisitor() { }
		virtual bool visit(int id, const char* str) = 0;
	};

public:
	Map m_map;
	Buffer* m_buffer;
	BufferList m_bufferList;
	
private:
	KStringtable(const KStringtable&);
	KStringtable& operator = (const KStringtable&);

public:
	KStringtable();
	~KStringtable();

public:
	bool load(StreamInterface& si);
	void reset();

public:
	const char* getString(int id) const;
	const char* getString2(int id, const char* defVal) const;
	bool foreach(IVisitor& v) const;

public:
	bool patch(KDiffFileReader& diff);
	bool setString(int id, const char* val);

protected:
	const char* allocString(const char* str, size_t len);
	/////////////////////////////////////////////////////
	// 去转义，认为和源码一个级别
	// 对StringManager表中读取出来的字符串进行转换
	// \r	->	'\r'
	// \n	->	'\n'
	// \t	->	'\t'
	// \b	->	'\b'
	// #\n, #\t 不做转换，留给UI处理
	const char* translateSourceString(const char* s, _KString& str);
};

/// KStringtableManager

class KStringtableManager
{
public:
	KStringtableManager();
	~KStringtableManager();

public:
	const KStringtable* getByLang(int lang) const;

public:
	KStringtable* _getByLang(int lang);
	KStringtable* _getByLangName(const char* langName);

public:
	KStringtable* m_tables[KLanguage::lang_count];
};