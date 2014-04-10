#ifndef _K_QUEST_PERSIST_H_
#define _K_QUEST_PERSIST_H_

#include <System/KPlatform.h>
#include <System/Misc/KStream.h>
#include "Inc/Lua/KLuaWrap.h"

struct KPersistQuestAttr;
struct KPersistTargetAttr;

#pragma pack(push, 1)
struct KPersistAttr
{
	DWORD flag  : 1;
	DWORD other : 31;
	
	KPersistQuestAttr* toQuestAttr();
	KPersistTargetAttr* toTargetAttr();
	KPersistAttr* next();
	int size();
};

struct KPersistQuestAttr
{
	DWORD flag   : 1;
	DWORD attrId : 7;
	DWORD length : 8;
	DWORD val    : 16;

	void hold();
	BOOL isQuestAttr() const;
	void setAttr(int attrId, const LuaString& val);
	int  getAttr(LuaString& val);
	int  size() const;

	char* _valPtr();
};

struct KPersistTargetAttr
{
	DWORD flag   : 1;
	DWORD group  : 7;
	DWORD target : 8;
	DWORD nlen   : 8;
	DWORD vlen   : 8;
	char  data[4];

	void hold();
	BOOL isTargetAttr(int group, int target, const char* name);
	void setAttr(int group, int target, const LuaString& name, const LuaString& val);
	LuaString getAttr(LuaString& val);
	int size() const;

	char* _namePtr();
	char* _valPtr();
};
#pragma pack(pop)

struct KPersistQuestBlock
{
	DWORD qid;
	WORD  length;
	char  data[2];

	int size() const;
	KPersistAttr* beginAttr();
	KPersistAttr* nextAttr(KPersistAttr* pAttr);
	KPersistQuestBlock* next();
	BOOL validate();
};

class IPersistObserver
{
public:
	virtual ~IPersistObserver() { }
	virtual void OnQuestAttr(int qid, int attrId, const LuaString& val) = 0;
	virtual void OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val) = 0;
};

class KDumpPersistObserver : public IPersistObserver
{
public:
	KDumpPersistObserver(::System::Collections::KDString<512>& str);
	~KDumpPersistObserver();

public:
	void OnQuestAttr(int qid, int attrId, const LuaString& val);
	void OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val);


public:
	int m_qid;
	::System::Collections::KDString<512>& m_str;
};

class KQuestPersist
{
public:
	KQuestPersist();
	~KQuestPersist();

public:
	void attach(const void* buf, int len, int capacity);
	::System::Collections::KDString<512> dumpString();
	int getPersistData(const char** ppData);
	void goThrough(IPersistObserver& oberver);
	int getQuests(int* qids, int count);
	BOOL setQuestAttr(int qid, int attrId, const char* val, int len=0);
	LuaString getQuestAttr(int qid, int attrId);
	BOOL setTargetAttr(int qid, int group, int target, const char* name, const char* val);
	void removeQuest(int qid);
	int size() const;

protected:
	virtual void beginUpdate();
	virtual void endUpdate();

public:
	KPersistQuestBlock* findQuest(int qid);
	BOOL validate();

private:
	KPersistQuestBlock* writeQuest(int qid);
	int getPos(const void* p);

public:
	KMemoryStream m_stream;
};

#endif
