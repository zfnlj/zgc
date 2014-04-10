#include "KQuestTarget.h"
#include "KQuestParamParser.h"
#include "KQuestNew.h"
//#include <PacketGate/KQuestPacket.h>
#include "KQuestManager.h"
#include "../WorldObject/KPlayer.h"
#include "KQuestTimer.h"
#include "System/Collections/KString.h"
#include "../Item/KBagManager.h"
#include "../EventSystem/KEventDefine.h"

using namespace System::Sync;
using namespace System::Collections;

char* KQuestTarget::m_buffer = new char[256];

#if !defined(max)
#	define max(a,b) (((a)<(b))?(b):(a))
#endif
#if !defined(min)
#	define min(a,b) (((a)<(b))?(a):(b))
#endif

static LuaString va(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char buf[512];
	int n = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	return KString<512>(buf, n);
}

template <typename T> KQuestTarget* CreateTarget(void) { return T::Alloc(); }

KQuestTarget* KQuestTarget::Create(const char* typeName)
{
	struct KTargetCreator{ const char* szName; KQuestTarget* (*Create)(); };
	static KTargetCreator creators[] =
	{
		{ Target_NpcDialog,			&CreateTarget<KTarget_NpcDialog>			},
		{ Target_QuestOver,			&CreateTarget<KTarget_QuestOver>			},
		{ Target_LevelUp,			&CreateTarget<KTarget_LevelUp>				},
		{ Target_GetItem,			&CreateTarget<KTarget_GetItem>				},
		{ Target_Dialog,			&CreateTarget<KTarget_Dialog>				},
		{ Target_UseItem,			&CreateTarget<KTarget_UseItem>				},
		{ Target_PlayerOperate,		&CreateTarget<KTarget_PlayerOperate>		},
		{ Target_KillMonster,		&CreateTarget<KTarget_KillMonster>			},
		{ NULL,},
	};

	for (KTargetCreator* p = &creators[0]; p->szName; p++)
	{
		if (0 == stricmp(typeName, p->szName))
		{
			KQuestTarget* pTarget = p->Create();
			if(!pTarget) return NULL;
			pTarget->Reset();
			return pTarget;
		}
	}

	return NULL;
}

KQuestTimer* KQuestTarget::CreateTimer(const char* attrs)
{
	ASSERT(!m_pTargetTimer);
	
	m_pTargetTimer = KQuestTimer::CreateInstance(attrs);
	ASSERT(m_pTargetTimer);

	m_pTargetTimer->SetOwner(this);
	return m_pTargetTimer;
}

KQuestTarget::KQuestTarget()
{
	m_targetIdx = -1;
	m_traceId = 0;
	m_pGroup = NULL;
	m_pTargetTimer = NULL;
	m_location = 0;	// 寻路信息
}

KQuestTarget::~KQuestTarget(void)
{
	m_pGroup = NULL;
}

bool KQuestTarget::Initialize(const char* paramStr)
{
	char* name;
	char* val;
	
	KQuestParamParser& parser = KQuestParamParser::GetInstance();
	if(!parser.Parse(paramStr)) return false;

	int n = parser.GetParamCount();
	for(int i=0; i<n; i++)
	{	
		parser.GetParameter(i, name, val);
		if(!this->SetAttr(name, val))
		{
			//Log(LOG_WARN, "warn: quest target-%s attr: %s=%s", GetName(), name, val);
			continue;
		}
	}

	return true;
}

void KQuestTarget::OnFinish()
{
	//if(this->target_HasLua(lua_OnFinish)) 
	//{
	//	MethodName fn = this->target_LuaMethod(lua_OnFinish);
	//	const char* f = fn.c_str();
	//	return LuaWraper.Call<void>(f, this);
	//}
	//else
		return this->_OnFinish();
}

void KQuestTarget::_OnFinish()
{
	return;
}

int KQuestTarget::_GetTraceText(char* buf, int len)
{
	return 0;
}

bool KQuestTarget::ReadParams(KQuestParamParser& parser)
{
	return true;
}

void KQuestTarget::CopyTo(KQuestTarget* pTarget)
{
	pTarget->m_traceId = m_traceId;
	pTarget->m_location = m_location;
	if(m_pTargetTimer)
	{
		pTarget->m_pTargetTimer = KQuestTimer::CreateInstance(pTarget, *m_pTargetTimer);
	}
}

bool KQuestTarget::IsOver(void)
{
	return false;
}

void KQuestTarget::Reset(void)
{
	m_traceId = 0;
	m_location = 0;
	m_pGroup = NULL;
	if(m_pTargetTimer)
	{
		m_pTargetTimer->Reset();
		KQuestTimer::Free(m_pTargetTimer);
		m_pTargetTimer = NULL;
	}
	return;
}

void KQuestTarget::Destroy(void)
{
	this->Destroy_Delete();
}

bool KQuestTarget::IsKindOf(const char* targetName)
{
	return stricmp(targetName, this->GetName()) == 0;
}

KTargetGroup* KQuestTarget::GetGroup()
{
	return m_pGroup;
}

const char* KQuestTarget::Topname()
{
	//char* buf = KQuestTarget::m_buffer;
	//buf[0] = '\0';

	//int npcId = this->GetIntAttr("npc", -1);
	//if(npcId == -1) return "NULL";

	//return KQuestManager::GetInstance()->GetNpcPositionHref(npcId);
	return "";
}

const char* KQuestTarget::Tonpc()
{
	// 临时实现
	return Topname();
}

LuaString KQuestTarget::ToPosName(const char* name)
{
	char buf[512];
	int len=0;
	if (!m_location)
	{
		return LuaString(name);
	}
	
	const char* posInfo = KStringManager::GetInstancePtr()->GetStringByID(m_location);
	if(!posInfo || !(len = strlen(posInfo))) return LuaString(name);	//没有对应位置信息


	//int len = sprintf_k(buf, sizeof(buf), "<RB=Topname,%s,%d,%d,%d>",
	//	name,
	//	m_location,
	//	m_location,
	//	m_location);
		//GetNpcX(m_location),
		//GetNpcY(m_location),
		//GetNpcMapId(m_location));
	if(len>=sizeof(buf)) return LuaString(name);
	memcpy(buf,posInfo,len);
	buf[len] = '\0';
	return LuaString(buf, len);	
}

LuaString KQuestTarget::ToNpcName(DWORD npcTypeId)
{
	// 返回带有寻路超链接的Npc名字
	char buf[256];
	//strcpy_k(buf, sizeof(buf), KQuestManager::GetInstance()->GetNpcPositionHref(npcTypeId));
	return LuaString(buf, strlen(buf));
}

bool KQuestTarget::SetAttr(const char* name, const char* val)
{
	if(stricmp(name, "trace") == 0)
	{
		m_traceId = str2int(val);
		return true;
	}
	if (stricmp(name, "shareFlag") == 0)
	{
		return true;
	}
	if(stricmp(name, "location") == 0)
	{
		if(!isnumber3(val)) return false;
		m_location = str2int(val);
		return true;
	}
	
	// 派生类处理其他的属性
	return this->_SetAttr(name, val);
}

int KQuestTarget::GetIntAttr(const char* name, int defVal)
{
	if(stricmp(name, "location") == 0)
	{
		return defVal;
	}
	return this->_GetIntAttr(name, defVal);
}

const char* KQuestTarget::GetStringAttr(const char* name, const char* defVal)
{
	if(stricmp(name, "location") == 0)
	{
		return defVal;
	}
	return this->_GetStringAttr(name, defVal);
}

KQuestNew* KQuestTarget::GetQuest()
{
	if(m_pGroup) 
		return m_pGroup->GetQuest();
	return _GetQuest();
}

KPlayer* KQuestTarget::GetPlayer()
{
	if(!m_pGroup) return NULL;
	KQuestNew* pQuest = m_pGroup->GetQuest();
	if(!pQuest) return NULL;
	return pQuest->GetOwner();
}

void KQuestTarget::SetOver(void)
{

}

void KQuestTarget::OnAddToGroup(KTargetGroup* pGrp)
{
	m_pGroup = pGrp;
	m_targetIdx = m_pGroup->m_targetList.size();
}

void KQuestTarget::Destroy_Delete(void)
{
	delete this;
}

int KQuestTarget::GetTraceText(char* buf, int len)
{
	return this->_GetTraceText(buf, len);
}

LuaString KQuestTarget::GetTraceText_lua()
{
	char buf[512];
	int len = this->GetTraceText(buf, 512);
	return LuaString(buf, len);
}

int KQuestTarget::GetQuestType()
{
	KQuestNew* pQuest = this->GetQuest();
	if(!pQuest) return 0;
	return pQuest->m_type;
}


LuaString KQuestTarget::GetTargetCounterName()
{
	// 派生类处理
	return this->GetCounterName();
}

bool KQuestTarget::OnGameEvent( int evtId, void* evtData, int len )
{
	if (IsOver())
	{
		return false;	// target已完成直接返回
	}
	return this->_OnGameEvent(evtId, evtData, len);
}

bool KQuestTarget::_OnGameEvent(int evtId, void* evtData, int len)
{
	return false;
}


///////// KTarget_NpcDialog /////////////////////////////////////////////////

KTarget_NpcDialog::KTarget_NpcDialog(void)
{
	m_npcId = 0;
}

KTarget_NpcDialog::~KTarget_NpcDialog(void)
{
}

const char* KTarget_NpcDialog::GetName(void) const
{
	return Target_NpcDialog;
}

KQuestTarget* KTarget_NpcDialog::Clone()
{
	KTarget_NpcDialog* pTarget = KTarget_NpcDialog::Alloc();
	KQuestTarget::CopyTo(pTarget);
	pTarget->m_npcId = m_npcId;
	return pTarget;
}

bool KTarget_NpcDialog::_SetAttr(const char* name, const char* val)
{
	if(stricmp(name, "npc") == 0)
	{
		if(!isnumber3(val)) return false;
		m_npcId = str2int(val);
		if(!m_npcId) return false;
		return true;
	}
	return false;
}

int KTarget_NpcDialog::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "npc") == 0)
	{
		return m_npcId;
	}
	else if (stricmp(name, "count") == 0)
	{
		char sn[64];
		sprintf_k(sn, sizeof(sn), "nd_%u", m_npcId);

		KQuestNew* pQuest = m_pGroup->GetOwner();
		return pQuest->GetSession(sn, defVal) > 0 ? 1 : 0;
	}
	else if (stricmp(name, "total") == 0)
	{
		return 1;
	}
	return defVal;
}

bool KTarget_NpcDialog::IsOver(void)
{
	char sn[64];
	sprintf_k(sn, sizeof(sn), "nd_%u", m_npcId);

	KQuestNew* pQuest = m_pGroup->GetOwner();
	int n = pQuest->GetSession(sn, 0);

	return n > 0;
}

void KTarget_NpcDialog::Reset(void)
{
	KQuestTarget::Reset();
	m_npcId = 0;
}

void KTarget_NpcDialog::Destroy(void)
{
	this->Reset();
	KTarget_NpcDialog::Free(this);
}

///////////////////////////////////////////////////////////

KTarget_QuestOver::KTarget_QuestOver()
{

}

KTarget_QuestOver::~KTarget_QuestOver()
{

}

const char* KTarget_QuestOver::GetName() const
{
	return Target_QuestOver;
}

bool KTarget_QuestOver::_SetAttr(const char* name, const char* val)
{
	if(stricmp(name, "quest") == 0)
	{
		if(!isnumber3(val)) return false;
		m_qid = str2int(val);
		return true;
	}
	if(stricmp(name, "questType") == 0)
	{
		if(!isnumber3(val)) return false;
		m_qType = str2int(val);
		return true;
	}
	if(stricmp(name, "reason") == 0)
	{
		if(!isnumber3(val)) return false;
		m_reason = str2int(val);
		if(m_reason < enum_quest_ok && m_reason > enum_quest_cancel)
			return false;

		return true;
	}
	if (stricmp(name, "count") == 0)
	{
		if(!isnumber3(val)) return false;
		m_total = str2int(val);
		return true;
	}

	return false;
}

int KTarget_QuestOver::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "quest") == 0)
	{
		return m_qid;
	}
	else if (stricmp(name, "questType") == 0)
	{
		return m_qType;
	}
	else if(stricmp(name, "reason") == 0)
	{
		return m_reason;
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		int n = GetQuest()->GetSession(sn.c_str(), defVal);
		n = (n > 0)?n:0;
		return  min(n,m_total);
	}
	else if (stricmp(name, "total") == 0)
	{
		return m_total;
	}

	return defVal;
}

bool KTarget_QuestOver::IsOver(void)
{
	LuaString sn = this->GetCounterName();
	return this->GetQuest()->GetSession(sn.c_str(), 0) >= m_total;
}

KQuestTarget* KTarget_QuestOver::Clone()
{
	KTarget_QuestOver* pClone = KTarget_QuestOver::Alloc();
	pClone->Reset();

	this->CopyTo(pClone);
	pClone->m_qid = m_qid;
	pClone->m_qType = m_qType;
	pClone->m_reason = m_reason;
	pClone->m_total = m_total;
	return pClone;
}

void KTarget_QuestOver::Reset(void)
{
	KQuestTarget::Reset();
	m_qid = 0;
	m_qType = 0;
	m_reason = enum_quest_ok;
	m_total = 0;
}

void KTarget_QuestOver::Destroy(void)
{
	this->Reset();
	KTarget_QuestOver::Free(this);
}

LuaString KTarget_QuestOver::GetCounterName()
{
	LuaString sn("qo");
	sn.append('_');
	sn.append(m_qid);
	sn.append('_');
	sn.append(m_qType);
	sn.append('_');
	sn.append(m_reason);
	return sn;
}

//////////////////////////////////////////////////////////////////////////

KTarget_GetItem::KTarget_GetItem(void)
{
	m_total = 0;
	m_itemCount = 0;
	memset(m_itemIds, 0, sizeof(m_itemIds));
}

KTarget_GetItem::~KTarget_GetItem(void)
{
}

const char* KTarget_GetItem::GetName(void) const
{
	return Target_GetItem;
}

KQuestTarget* KTarget_GetItem::Clone()
{
	KTarget_GetItem* pTarget = KTarget_GetItem::Alloc();
	KQuestTarget::CopyTo(pTarget);
	pTarget->m_total = m_total;
	memcpy(pTarget->m_itemIds, m_itemIds, sizeof(m_itemIds));
	pTarget->m_itemCount = m_itemCount;
	return pTarget;
}

bool KTarget_GetItem::_SetAttr(const char* name, const char* val)
{
	char buf[256];
	strcpy_k(buf, sizeof(buf), val);

	if(stricmp(name, "item") == 0)
	{
		char* ssItem[32];
		m_itemCount = split(buf, ",", ssItem, 32);
		if (!m_itemCount || m_itemCount > 10) return false;

		for(int i=0; i<m_itemCount; i++)
		{
			const char* str = ssItem[i];
			if(!isnumber3(str)) return false;

			m_itemIds[i] = str2int(str);
			if(!m_itemIds[i]) return false;
		}
		return true;
	}
	if(stricmp(name, "count") == 0)
	{
		if(!isnumber3(val)) return false;
		m_total = str2int(val);
		if(!m_total) return false;
		return true;
	}
	return true;
}

int KTarget_GetItem::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "item") == 0)
	{
		ASSERT(m_itemCount > 0);
		return m_itemIds[0];
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		KQuestNew* pQuest = m_pGroup->GetOwner();
		int count = pQuest->GetSession(sn.c_str(), defVal);
		return min(count, m_total);
	}
	else if (stricmp(name, "total") == 0)
	{
		return m_total;
	}
	return defVal;
}

bool KTarget_GetItem::IsOver(void)
{
	LuaString sn = this->GetCounterName();

	KQuestNew* pQuest = m_pGroup->GetOwner();
	int n = pQuest->GetSession(sn.c_str(), 0);
	return n >= m_total;
}

void KTarget_GetItem::Reset(void)
{
	KQuestTarget::Reset();
	m_total = 0;
	m_itemCount = 0;
	memset(m_itemIds, 0, sizeof(m_itemIds));
}

void KTarget_GetItem::Destroy(void)
{
	this->Reset();
	KTarget_GetItem::Free(this);
}

bool KTarget_GetItem::IsCared(DWORD itemId)
{
	for (int i=0; i<m_itemCount; i++)
	{
		if (m_itemIds[i] == itemId)
			return true;
	}
	return false;
}

LuaString KTarget_GetItem::GetCounterName()
{
	LuaString sn("gi");
	for(int i=0; i<m_itemCount; i++)
	{
		sn.append('_');
		sn.append((int)m_itemIds[i]);
	}
	return sn;
}
//////////////////////////////////////////////////////////////////////////

KTarget_LevelUp::KTarget_LevelUp(void)
{
	m_level = 0;
}

KTarget_LevelUp::~KTarget_LevelUp(void)
{
}

const char* KTarget_LevelUp::GetName(void) const
{
	return Target_LevelUp;
}

KQuestTarget* KTarget_LevelUp::Clone()
{
	KTarget_LevelUp* pTarget = KTarget_LevelUp::Alloc();
	KQuestTarget::CopyTo(pTarget);
	pTarget->m_level = m_level;
	return pTarget;
}

bool KTarget_LevelUp::_SetAttr(const char* name, const char* val)
{
	if(stricmp(name, "level") == 0)
	{
		if(!isnumber3(val)) return false;
		m_level = str2int(val);
		if(m_level < 0 || m_level >= 1000) return false;
		return true;
	}
	return false;
}

int KTarget_LevelUp::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "level") == 0)
	{
		return m_level;
	}
	else if (stricmp(name, "count") == 0)
	{
		char sn[64];
		sprintf_k(sn, 64, "lu_%u", m_level);
		return GetQuest()->GetSession(sn, defVal) > 0 ? 1 : 0;
	}
	else if (stricmp(name, "total") == 0)
	{
		return 1;
	}

	return defVal;
}

bool KTarget_LevelUp::IsOver(void)
{
	KQuestNew* pQuest = m_pGroup->GetOwner();

	char sn[64];
	sprintf_k(sn, sizeof(sn), "lu_%u", m_level);
	int n = pQuest->GetSession(sn, 0);
	return n > 0;
}

void KTarget_LevelUp::Reset(void)
{
	KQuestTarget::Reset();
	m_level = 0;
}

void KTarget_LevelUp::Destroy(void)
{
	this->Reset();
	KTarget_LevelUp::Free(this);
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

KTarget_Dialog::KTarget_Dialog(void)
{
	Reset();
}

KTarget_Dialog::~KTarget_Dialog(void)
{
}

const char* KTarget_Dialog::GetName(void) const
{
	return Target_Dialog;
}

KQuestTarget* KTarget_Dialog::Clone()
{
	KTarget_Dialog* pTarget = KTarget_Dialog::Alloc();
	pTarget->Reset();
	KQuestTarget::CopyTo(pTarget);

	memcpy(&pTarget->m_npcIds, m_npcIds, sizeof(m_npcIds));
	pTarget->m_npcCount = m_npcCount;
	pTarget->m_dialogIdx = m_dialogIdx;
	return pTarget;
}

bool KTarget_Dialog::_SetAttr(const char* name, const char* val)
{
	char buf[256];
	strcpy_k(buf, sizeof(buf), val);

	if(stricmp(name, "npc") == 0)
	{
		char* ssNpc[32];
		m_npcCount = split(buf, ",", ssNpc, 32);
		if (!m_npcCount || m_npcCount > 10) return false;

		for(int i=0; i<m_npcCount; i++)
		{
			const char* str = ssNpc[i];
			if(!isnumber3(str)) return false;

			m_npcIds[i] = str2int(str);
			if(!m_npcIds[i]) return false;
		}
		return true;
	}
	if(stricmp(name, "dialog") == 0)
	{
		if(!isnumber3(val)) return false;
		m_dialogIdx = str2int(val);
		if(m_dialogIdx < 0) return false;
		return true;
	}
	return false;
}

int KTarget_Dialog::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "npc") == 0)
	{
		ASSERT(m_npcCount > 0);
		return m_npcIds[0];
	}
	else if (stricmp(name, "dialog") == 0)
	{
		return m_dialogIdx;
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		KQuestNew* pQuest = m_pGroup->GetOwner();
		return pQuest->GetSession(sn.c_str(), defVal) > 0 ? 1 : 0;
	}
	else if (stricmp(name, "total") == 0)
	{
		return 1;
	}
	return defVal;
}

bool KTarget_Dialog::IsOver(void)
{
	LuaString sn = this->GetCounterName();
	KQuestNew* pQuest = m_pGroup->GetOwner();
	return pQuest->GetSession(sn.c_str(), 0) > 0;
}

void KTarget_Dialog::Reset(void)
{
	KQuestTarget::Reset();
	memset(&m_npcIds, 0, sizeof(m_npcIds));
	m_npcCount = 0;
	m_dialogIdx = 0;
}

void KTarget_Dialog::Destroy(void)
{
	this->Reset();
	KTarget_Dialog::Free(this);
}

bool KTarget_Dialog::IsCared(DWORD npcId)
{
	for (int i=0; i<m_npcCount; i++)
	{
		if (m_npcIds[i] == npcId)
			return true;
	}
	return false;
}

LuaString KTarget_Dialog::GetCounterName(void)
{
	LuaString sn("d");
	for(int i=0; i<m_npcCount; i++)
	{
		sn.append('_');
		sn.append((int)m_npcIds[i]);
	}
	sn.append('_');
	sn.append(m_dialogIdx);
	return sn;
}


KTarget_UseItem::KTarget_UseItem( void )
{
	this->Reset();
}

KTarget_UseItem::~KTarget_UseItem( void )
{
	this->Reset();
}

const char* KTarget_UseItem::GetName( void ) const
{
	return Target_UseItem;
}

KQuestTarget* KTarget_UseItem::Clone()
{
	KTarget_UseItem* pTarget = KTarget_UseItem::Alloc();
	pTarget->Reset();
	KQuestTarget::CopyTo(pTarget);

	pTarget->m_itemId = m_itemId;
	pTarget->m_objId = m_objId;
	pTarget->m_objType = m_objType;
	pTarget->m_npcId = m_npcId;
	pTarget->m_adr = m_adr;
	pTarget->m_x = m_x;
	pTarget->m_y = m_y;
	pTarget->m_total = m_total;

	return pTarget;
}

bool KTarget_UseItem::_SetAttr( const char* name, const char* val )
{
	if(stricmp(name, "item") == 0)
	{
		if(!isnumber3(val)) return false;
		m_itemId = str2int(val);
		if(!m_itemId) return false;
		return true;
	}
	else if(stricmp(name, "npc") == 0)
	{
		if(!isnumber3(val)) return false;
		m_npcId = str2int(val);
		if(!m_npcId) return false;
		return true;
	}
	else if(stricmp(name, "obj") == 0)
	{
		if(!isnumber3(val)) return false;
		m_objId = str2int(val);
		if(!m_objId) return false;
		return true;
	}
	else if(stricmp(name, "type") == 0)
	{
		if(!isnumber3(val)) return false;
		m_objType = str2int(val);
		if(!m_objType) return false;
		return true;
	}
	else if(stricmp(name, "adr") == 0)
	{
		if(!isnumber3(val)) return false;
		m_adr = str2int(val);
		if(!m_adr) return false;
		return true;
	}
	else if(stricmp(name, "x") == 0)
	{
		if(!isnumber3(val)) return false;
		m_x = str2int(val);
		if(!m_x) return false;
		return true;
	}
	else if(stricmp(name, "y") == 0)
	{
		if(!isnumber3(val)) return false;
		m_y = str2int(val);
		if(!m_y) return false;
		return true;
	}
	else if(stricmp(name, "count") == 0)
	{
		if(!isnumber3(val)) return false;
		m_total = str2int(val);
		if(!m_total) return false;
		return true;
	}

	return false;
}

int KTarget_UseItem::_GetIntAttr( const char* name, int defVal )
{
	if (stricmp(name, "obj") == 0)
	{
		return m_objId;
	}
	else if (stricmp(name, "type") == 0)
	{
		return m_objType;
	}
	else if (stricmp(name, "npc") == 0)
	{
		return m_npcId;
	}
	else if (stricmp(name, "adr") == 0)
	{
		return m_adr;
	}
	else if (stricmp(name, "x") == 0)
	{
		return m_x;
	}
	else if (stricmp(name, "y") == 0)
	{
		return m_y;
	}
	else if (stricmp(name, "item") == 0)
	{
		return m_itemId;
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		KQuestNew* pQuest = m_pGroup->GetOwner();
		int n = pQuest->GetSession(sn.c_str(), defVal);
		return min(n, m_total);
	}
	else if (stricmp(name, "total") == 0)
	{
		return m_total;
	}
	return defVal;
}

bool KTarget_UseItem::IsOver( void )
{
	LuaString sn = this->GetCounterName();

	KQuestNew* pQuest = m_pGroup->GetOwner();
	int n = pQuest->GetSession(sn.c_str(), 0);
	return n >= m_total;
}

void KTarget_UseItem::Reset( void )
{
	KQuestTarget::Reset();
	m_adr = 0;
	m_itemId = 0;
	m_npcId = 0;
	m_objId = 0;
	m_objType = 0;
	m_x = 0;
	m_y = 0;
	m_total = 1;
}

void KTarget_UseItem::Destroy( void )
{
	this->Reset();
	KTarget_UseItem::Free(this);
}

bool KTarget_UseItem::IsCared( DWORD itemId )
{
	return this->m_itemId == itemId;
}

LuaString KTarget_UseItem::GetCounterName()
{
	LuaString sn("ui");
	sn.append('_');
	sn.append( (int)m_itemId );
	sn.append('_');
	sn.append( m_objId );
	sn.append('_');
	sn.append(m_adr);
	return sn;
}

KTarget_PlayerOperate::KTarget_PlayerOperate( void )
{
	this->Reset();
}

KTarget_PlayerOperate::~KTarget_PlayerOperate( void )
{
	this->Reset();
}

const char* KTarget_PlayerOperate::GetName( void ) const
{
	return Target_PlayerOperate;
}

KQuestTarget* KTarget_PlayerOperate::Clone()
{
	KTarget_PlayerOperate* pTarget = KTarget_PlayerOperate::Alloc();
	pTarget->Reset();
	KQuestTarget::CopyTo(pTarget);

	pTarget->m_opType = m_opType;
	pTarget->m_opParam = m_opParam;

	return pTarget;
}

bool KTarget_PlayerOperate::_SetAttr( const char* name, const char* val )
{
	if (stricmp(name, "type") == 0)
	{
		if(!isnumber3(val)) return false;
		m_opType = str2int(val);
		if(!m_opType) return false;
		return true;
	}
	else if (stricmp(name, "param") == 0)
	{
		if(!isnumber3(val)) return false;
		m_opParam = str2int(val);
		if(!m_opParam) return false;
		return true;
	}
	return false;
}

int KTarget_PlayerOperate::_GetIntAttr( const char* name, int defVal )
{
	if (stricmp(name, "type") == 0)
	{
		return m_opType;
	}
	else if (stricmp(name, "param") == 0)
	{
		return m_opParam;
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		KQuestNew* pQuest = this->GetQuest();
		int count = pQuest->GetSession(sn.c_str(), defVal);
		return min(count, 1);
	}
	else if (stricmp(name, "total") == 0)
	{
		return 1;
	}
	return defVal;
}

bool KTarget_PlayerOperate::IsOver( void )
{
	LuaString sn = this->GetCounterName();
	KQuestNew* pQuest = m_pGroup->GetOwner();
	return pQuest->GetSession(sn.c_str(), 0) > 0;
}

void KTarget_PlayerOperate::Reset( void )
{
	KQuestTarget::Reset();
	m_opType = 0;
	m_opParam = 0;
}

void KTarget_PlayerOperate::Destroy( void )
{
	Reset();
	KTarget_PlayerOperate::Free(this);
}

LuaString KTarget_PlayerOperate::GetCounterName()
{
	LuaString sn("pop");
	sn.append('_');
	sn.append( m_opType );
	sn.append('_');
	sn.append( m_opParam );
	return sn;
}



KTarget_KillMonster::KTarget_KillMonster(void)
{
	m_total = 0;
	m_npcCount = 0;
	memset(m_npcIds, 0, sizeof(m_npcIds));
}

KTarget_KillMonster::~KTarget_KillMonster(void)
{
}

const char* KTarget_KillMonster::GetName(void) const
{
	return Target_KillMonster;
}

KQuestTarget* KTarget_KillMonster::Clone()
{
	KTarget_KillMonster* pTarget = KTarget_KillMonster::Alloc();
	KQuestTarget::CopyTo(pTarget);
	pTarget->m_total = m_total;
	memcpy(&pTarget->m_npcIds, &m_npcIds, sizeof(m_npcIds));
	pTarget->m_npcCount = m_npcCount;
	return pTarget;
}

bool KTarget_KillMonster::_SetAttr(const char* name, const char* val)
{
	char buf[256];
	strcpy_k(buf, sizeof(buf), val);

	if(stricmp(name, "npc") == 0)
	{
		char* ssNpc[32];
		m_npcCount = split(buf, ",", ssNpc, 32);
		if (!m_npcCount || m_npcCount > 10) return false;

		for(int i=0; i<m_npcCount; i++)
		{
			const char* str = ssNpc[i];
			if(!isnumber3(str)) return false;

			m_npcIds[i] = str2int(str);
			if(!m_npcIds[i]) return false;
		}
		return true;
	}
	if(stricmp(name, "count") == 0)
	{
		if(!isnumber3(val)) return false;
		m_total = str2int(val);
		if(!m_total) return false;
		return true;
	}
	return false;
}

int KTarget_KillMonster::_GetIntAttr(const char* name, int defVal)
{
	if (stricmp(name, "npc") == 0)
	{
		ASSERT(m_npcCount > 0);
		return m_npcIds[0];
	}
	else if (stricmp(name, "count") == 0)
	{
		LuaString sn = this->GetCounterName();
		KQuestNew* pQuest = m_pGroup->GetOwner();
		int count = pQuest->GetSession(sn.c_str(), defVal);
		return min(count, m_total);
	}
	else if (stricmp(name, "total") == 0)
	{
		return m_total;
	}
	return defVal;
}

bool KTarget_KillMonster::IsOver(void)
{
	LuaString sn = this->GetCounterName();
	KQuestNew* pQuest = m_pGroup->GetOwner();
	return pQuest->GetSession(sn.c_str(), 0) >= m_total;
}

void KTarget_KillMonster::Reset(void)
{
	KQuestTarget::Reset();

	m_total = 0;
	m_npcCount = 0;
	memset(m_npcIds, 0, sizeof(m_npcIds));
}

void KTarget_KillMonster::Destroy(void)
{
	this->Reset();
	KTarget_KillMonster::Free(this);
}

bool KTarget_KillMonster::IsCared(DWORD npcId)
{
	for(int i=0; i<m_npcCount; i++)
	{
		if(m_npcIds[i] == -1)
			return true;

		if(m_npcIds[i] == npcId)
			return true;
	}
	return false;
}

LuaString KTarget_KillMonster::GetCounterName()
{
	LuaString sn("km");
	for(int i=0; i<m_npcCount; i++)
	{
		sn.append('_');
		sn.append((int)m_npcIds[i]);
	}
	return sn;
}

bool KTarget_KillMonster::_OnGameEvent(int evtId, void* evtData, int len)
{
	if (evtId == KEventAbout::enumWorldEvent_KillMonster)
	{
		KEventAbout::KEDKillMonster& evt = *(KEventAbout::KEDKillMonster*)evtData;
		if(!this->IsCared(evt.m_nMonsterID)) return false;
	}else{
		return false;
	}

	KQuestNew* pQuest = m_pGroup->GetOwner();

	LuaString sn = this->GetCounterName();
	int n = pQuest->GetSession(sn.c_str(), 0);
	n++;
	pQuest->SetSession(sn.c_str(), n);

	if(n > m_total) return false;
	return true;
}

void KTarget_KillMonster::_OnFinish()
{
}