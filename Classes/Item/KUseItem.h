#ifndef _KUSEITEM_H
#define _KUSEITEM_H

#include <System/KPlatform.h>
#include "System/KMacro.h"
#include "Inc/Lua/KLuaWrap.h"
#include <System/Collections/DynArray.h>
#include <System/Collections/KSDList.h>
#include "CommonLogic/Item/KStruct_Item.h"

namespace KItemAbout
{

class KUseItem
	: public System::Memory::KPortableMemoryPool<KUseItem,128>
{
public:
	enum
	{
		lua_Initialize,
		lua_AfterInitialize,
		lua_run,
		lua_Count,
	};

	struct EvtReg
	{
		int evtId;
		int refCount;
	};

	class evtCmp
	{
	public:
		int operator () (const EvtReg& a, const EvtReg& b) const
		{
			return a.evtId - b.evtId;
		}
	};

	typedef System::Collections::KString<128> MethodName;
	struct ItemDesc { DWORD itemId; int count; };
	typedef System::Collections::KDynSortedArray<EvtReg,evtCmp,16,16> EvtRegArray;

public:
	KUseItem();
	~KUseItem();

public:
	// 从Lua中初始化
	bool Initialize(int qid, const char* questTable=NULL);
	bool AfterInitialize(const char* questTable);


public:
	int Breathe(int interval);
	DWORD m_lastBreatheTick;
public:
	KUseItem* Clone();
	void Reset();
	void run(UINT64 playerId);
public:// 事件注册
	bool RegEvent(int evtId, DWORD key, DWORD customData);
	bool RemoveEvent(int evtId, DWORD key);

public:
	// name1=val1;name2=val2; ... ...
	// namei对应到Excel表上面的列名
	// 主要是为 lua 提供接口
	bool SetAttrs(const char* attrStr);

public:
	int GetIntAttr(const char* attrName, int defVal);
	const char* GetStringAttr(const char* attrName, const char* defVal);

public:
	bool SetAttr(int attrId, char* val);
public: // quest flags enum_quest_flag
	bool SetFlag(int which);
	void ClearFlag(int which);
	bool HasFlag(int which) const;
public:
	int   m_qid;

	DWORD m_nameId;
	BYTE  m_type;
	//BYTE  m_view;
	DWORD m_dwFlags;

	DWORD m_dwLuaMethodFlag;
	// 步骤上移过来的
	EvtRegArray m_evtArray;

public:
	static const char* m_luaMethods[lua_Count];
	void InitLuaFuncFlag(const char* tableName);
	bool hasLua(int method);
	MethodName luaMethod(int method);
	void GeneratNormalCard(UINT64 playerId,int count,int rank1Rate,int rank2Rate,int rank3Rate,int heroRate);
	void GenerateCardDeck(UINT64 playerId,int deckId);
public:
	BeginDefLuaClass(KUseItem)
		DefMemberFunc(SetFlag);
		DefMemberFunc(ClearFlag);
		DefMemberFunc(HasFlag);
		DefMemberFunc(SetAttrs);
		DefMemberFunc(GetIntAttr);
		DefMemberFunc(GetStringAttr);
		DefMemberFunc(GeneratNormalCard);
		DefMemberFunc(GenerateCardDeck);
	EndDef
};

DECLARE_SIMPLE_TYPE(KUseItem::EvtReg);
};
#endif

