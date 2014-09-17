#include "KQuestNew.h"
#include "../WorldObject/KPlayer.h"
#include "KQuestParamParser.h"
#include <System/Memory/KBuffer64k.h>
#include "CommonLogic/StringManager/KStringManager.h"
#include "KQuestManager.h"
#include "CommonLogic/WorldObject/KAttrDefines.h"
#include "KQuestTimer.h"
#include "CommonLogic/Quest/KQuestCommon.h"
#include "KPlayerQuestManager.h"
#include "System/Collections/KString.h"
#include "../WorldObject/KMainPlayer.h"
#include <time.h>
#include "../GameLogic/KDynamicWorld.h"
#include "KClientQuestSession.h"
#include "../common/KPlayerRecordAssist.h"
#include "../Item/KBagManager.h"
#include "../VirtualService.h"
#include "../Item/KUseItemManager.h"
#include "../PlayerCard/KTowerAssist.h"

using namespace KAttributeAbout;
using namespace System::Collections;
using namespace KEventAbout;

static LuaString va(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char buf[512];
	int n = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	return KString<512>(buf, n);
}

const char* KQuestNew::m_luaMethods[lua_Count] = {
	"Initialize",
	"AfterInitialize",
	"GiftDesc",
	"GetTraceText",
	"QName",
	"Synposis",
	"Description",
	"TalkDesc",
	"FinishDesc",
	"NoFinishDesc",
	"CanSee",
	"CanDoThis",
	"UseGift",
	"SelectGiftList"
};

KQuestNew::KQuestNew()
	: m_pSession(NULL)
	, m_pQuestTimer(NULL)
{
	this->Reset();
}

KQuestNew::~KQuestNew()
{
	this->Reset();
}

// 从Lua中初始化
bool KQuestNew::Initialize(int qid, const char* questTable)
{
	m_qid = qid;

	this->InitLuaFuncFlag(questTable);
	if(!this->hasLua(lua_Initialize))
	{
		Log(LOG_ERROR, "error: lua table for quest:%d must has an Initialize method", qid);
		return false;
	}

	MethodName fn = this->luaMethod(lua_Initialize);
	const char* f = fn.c_str();

	if(!LuaWraper.Call<bool>(f, this))
	{
		Log(LOG_FATAL, "quest: qid:%d call lua %s", qid, f);
		return false;
	}
	
	return this->AfterInitialize(questTable);
	return false;
}

bool KQuestNew::AfterInitialize(const char* questTable)
{
    if (this->hasLua(lua_AfterInitialize))
	{
		MethodName fn = this->luaMethod(lua_AfterInitialize);
		const char* f = fn.c_str();
		if(!LuaWraper.Call<bool>(f, this))
		{
			Log(LOG_FATAL, "quest: qid:%d call AfterInitialize:%s", m_qid, f);
			return false;
		}
	}
	return true;
}

int KQuestNew::Breathe(int nInterval)
{
	return 0;
}

void KQuestNew::SetQuestStatus(KQuestStatusCode status)
{
	m_status = status;
	this->SetSession(Session_Status, status);
}

KQuestStatusCode KQuestNew::GetQuestStatus()
{
	static const DWORD key = ELFHash(Session_Status);
	return (KQuestStatusCode)this->GetSession2(key, KQ_None);
}

KQuestNew* KQuestNew::Clone()
{
	KQuestNew* pQuest = KQuestNew::Alloc();
	pQuest->Reset();

	pQuest->m_battleField = m_battleField;
	pQuest->m_extraDescBGPId = m_extraDescBGPId;
	pQuest->m_extraDescStrId = m_extraDescStrId;
	pQuest->m_grade = m_grade;
	pQuest->m_qid = m_qid;
	pQuest->m_nameId = m_nameId;
	pQuest->m_talkId = m_talkId;
	pQuest->m_level = m_level;
	pQuest->m_acceptLevel = m_acceptLevel;
	pQuest->m_acceptTopLimit = m_acceptTopLimit;
	pQuest->m_prevQuest = m_prevQuest;
	pQuest->m_nextQuest = m_nextQuest;
	pQuest->m_type = m_type;
	//pQuest->m_view = m_view;
	pQuest->m_abandon = m_abandon;
	pQuest->m_reuse = m_reuse;
	pQuest->m_share = m_share;

	pQuest->m_profession = m_profession;

	pQuest->m_dwFlags = m_dwFlags;

	pQuest->m_hardDegree = m_hardDegree;
	pQuest->m_money = m_money;
	pQuest->m_exp = m_exp;
	pQuest->m_expBase = m_expBase;
	pQuest->m_descId = m_descId;
	pQuest->m_finishDescId = m_finishDescId;
	pQuest->m_acceptDialogId = m_acceptDialogId;
	pQuest->m_finishDialogId = m_finishDialogId;

	pQuest->m_achieveId = m_achieveId;
	pQuest->m_achievement = m_achievement;

	pQuest->m_timeperiod = m_timeperiod;

	memcpy(pQuest->m_itemGifts, m_itemGifts, sizeof(m_itemGifts));
	pQuest->m_itemGiftCount = m_itemGiftCount;


	pQuest->m_questItemCount = m_questItemCount;
	memcpy(&pQuest->m_questItems, &m_questItems, sizeof(m_questItems));


	pQuest->m_dwLuaMethodFlag = m_dwLuaMethodFlag;

	pQuest->m_pPlayer = m_pPlayer;
	pQuest->m_pSession = m_pSession;

	KTargetGroup* pGrp = m_grpList.begin();
	while(pGrp)
	{
		KTargetGroup* pGrpClone = pGrp->Clone();
		pQuest->AddGroup(pGrpClone);
		if(pGrp == m_pCurrGrp) pQuest->m_pCurrGrp = pGrpClone;
		pGrp = m_grpList.next(pGrp);
	}

	KQuestCustomEvent* pEvt = m_evtList.begin();
	while(pEvt)
	{
		KQuestCustomEvent* pEvtClone = pEvt->Clone();
		pQuest->m_evtList.push_back(pEvtClone);
		pEvt = m_evtList.next(pEvt);
	}

	if(m_pQuestTimer)
	{
		pQuest->m_pQuestTimer = KQuestTimer::CreateInstance(pQuest, *m_pQuestTimer);
	}

	return pQuest;
}

void KQuestNew::Reset()
{
	m_status = KQ_None;
	m_lastBreatheTick = 0;

	m_qid = 0;
	m_extraDescStrId = 0;
	m_extraDescBGPId = 0;
	m_battleField = 0;
	m_nameId = 0;
	m_talkId = 0;
	m_acceptDialogId = 0;	// 接受对话
	m_finishDialogId = 0;	// 完成对话
	m_level = 0;
	m_acceptLevel = 0;
	m_acceptTopLimit = 0;
	m_prevQuest = 0;
	m_nextQuest = 0;
	m_type = 0;
	//m_view = 0;
	m_abandon = 1;
	m_reuse = 0;
	m_share = 0;

	m_profession = 0;

	m_grade = 0;

	// 缺省是可以回退的
	m_dwFlags = 0;

	m_hardDegree = 0;
	m_money = 0;
	m_exp = 0;
	m_expBase = 0;
	m_descId = 0;
	m_finishDescId = 0;

	m_achieveId = 0;
	m_achievement = 0;

	m_timeperiod = 0;

	memset(m_itemGifts, 0, sizeof(m_itemGifts));
	m_itemGiftCount = 0;

	m_questItemCount = 0;
	memset(&m_questItems, 0, sizeof(m_questItems));

	m_evtArray.clear();

	m_dwLuaMethodFlag = 0;

	m_pPlayer = NULL;
	
	if(m_pSession)
	{
		m_pSession->Destroy();
		m_pSession = NULL;
	}
	
	m_pCurrGrp = NULL;

	KTargetGroup* pGrp;
	while(pGrp = m_grpList.pop_front(), pGrp)
	{
		pGrp->Reset();
		KTargetGroup::Free(pGrp);
	}

	KQuestCustomEvent* pEvt;
	while(pEvt = m_evtList.pop_front(), pEvt)
	{
		pEvt->Reset();
		KQuestCustomEvent::Free(pEvt);
	}

	if(m_pQuestTimer)
	{
		m_pQuestTimer->Reset();
		KQuestTimer::Free(m_pQuestTimer);
		m_pQuestTimer = NULL;
	}
}

bool KQuestNew::RegEvent(int evtId, DWORD key, DWORD customData)
{
	return true;
}

bool KQuestNew::RemoveEvent(int evtId, DWORD key)
{
	return true;
}

bool KQuestNew::SetAttrs(const char* attrStr)
{
	KQuestParamParser& parser = KQuestParamParser::GetInstance();
	if(!parser.Parse(attrStr)) return false;
	char* name;
	char* val;
	int c = parser.GetParamCount();
	for(int i=0; i<c; i++)
	{
		parser.GetParameter(i, name, val);
		int attrId = KQuestAttr::GetAttrId(name);
		
		if(attrId < 0)
		{
			Log(LOG_ERROR, "error: unknown quest:%d attr %s=%s", m_qid, name, val);
			continue;
		}

		if(!this->SetAttr(attrId, val))
		{
			//Log(LOG_ERROR, "error: unknown quest:%d attr %s=%s", m_qid, name, val);	// cmh 暂时屏蔽
			continue;
		}
	}
	return true;
}

void KQuestNew::InitLuaFuncFlag(const char* tableName)
{
	char f[256];
	for(int i=0; i<lua_Count; i++)
	{
		sprintf_k(f, sizeof(f), "%s.%s", tableName, m_luaMethods[i]);
		if(LuaWraper.hasFunction(f))
		{
			m_dwLuaMethodFlag |= 1<<i;
		}
	}
}

KTargetGroup* KQuestNew::CreateTargetGroup(const char* rel)
{
	KTargetGroup* pGrp = KTargetGroup::Alloc();
	if (!pGrp->SetTargetRelation(rel))
	{
		pGrp->Reset();
		KTargetGroup::Free(pGrp);
		return NULL;
	}
	this->AddGroup(pGrp);
	return pGrp;
}

void KQuestNew::RemoveSession(const char* name)
{
	if(!m_pSession) return;
	m_pSession->RemoveSession(name);
}

bool KQuestNew::IsTraceMode()
{
	static const DWORD key = ELFHash(Session_TraceTick);
	return this->GetSession2(key, 0) != 0;
}

int KQuestNew::GetIntAttr(const char* attrName, int defVal)
{
	int attrId = KQuestAttr::GetAttrId(attrName);
	if(attrId < 0) return defVal;
	switch(attrId)
	{
	case KQuestAttr::questId:		return m_qid;
	case KQuestAttr::nameId:		return m_nameId;
	case KQuestAttr::extraDescStr:	return m_extraDescStrId;
	case KQuestAttr::extraDescBGP:	return m_extraDescBGPId;
	case KQuestAttr::battleField:	return m_battleField;
	case KQuestAttr::talk:			return m_talkId;
	case KQuestAttr::type:			return m_type;
	case KQuestAttr::level:			return m_level;
	case KQuestAttr::abandon:		return m_abandon;
	case KQuestAttr::reuse:			return m_reuse;
	case KQuestAttr::share:			return m_share;
	case KQuestAttr::acceptLevel:	return m_acceptLevel;
	case KQuestAttr::prevQuest:		return m_prevQuest;
	case KQuestAttr::nextQuest:		return m_nextQuest;
	case KQuestAttr::hardDegree:	return m_hardDegree;
	case KQuestAttr::money:			return m_money;
	case KQuestAttr::exp:			return m_exp;
	case KQuestAttr::desc:			return m_descId;
	case KQuestAttr::finishDesc:	return m_finishDescId;
	case KQuestAttr::achieveId:		return m_achieveId;
	case KQuestAttr::achievement:	return m_achievement;
	case KQuestAttr::timeperiod:	return m_timeperiod;
	case KQuestAttr::grade:			return m_grade;
	case KQuestAttr::acceptTopLimit: return m_acceptTopLimit;
	};
	return defVal;
}

const char* KQuestNew::GetStringAttr(const char* attrName, const char* defVal)
{
	int attrId = KQuestAttr::GetAttrId(attrName);
	if(attrId < 0) return defVal;
	switch (attrId)
	{
	case KQuestAttr::extraDescStr:	return KStringManager::Instance()->GetStringByID(m_extraDescStrId);
	case KQuestAttr::extraDescBGP:	return KStringManager::Instance()->GetStringByID(m_extraDescBGPId);
	default:	break;
	}
	return defVal;
}

bool KQuestNew::CreateUserEvent(int evtId)
{
	KQuestCustomEvent* pEvt = KQuestCustomEvent::Alloc();
	pEvt->Reset();

	pEvt->m_evtId = evtId;
	m_evtList.push_back(pEvt);

	return true;
}

KQuestTimer* KQuestNew::CreateTimer(const char* attrs)
{
	ASSERT(!m_pQuestTimer);

	m_pQuestTimer = KQuestTimer::CreateInstance(attrs);
	ASSERT(m_pQuestTimer);
	
	m_pQuestTimer->SetOwner(this);
	return m_pQuestTimer;
}

KQuestTimer* KQuestNew::GetCurTimer()
{
	if (m_pQuestTimer)
	{
		return m_pQuestTimer;
	}
	return NULL;
}

bool KQuestNew::SetCurrentGroup(int grpIndex)
{
	KTargetGroup* pGrp = this->GetGroup(grpIndex);

	if(!pGrp)
	{
		Log(LOG_FATAL, "quest: quest %d SetCurrentGroup(%d)", m_qid, grpIndex);
		return false;
	}

	m_pCurrGrp = pGrp;

	return true;
}

int KQuestNew::GetCurrentGroup(void)
{
	return this->GetSession(Session_Group, -1) - 1;
}

bool KQuestNew::SetFlag(int which)
{
	if(which < 0 || which >= 32) return false;
	m_dwFlags |= 1 << which;
	return true;
}

void KQuestNew::ClearFlag(int which)
{
	if(which < 0 || which >= 32) return;
	m_dwFlags &= ~(1 << which);
	return;
}

bool KQuestNew::HasFlag(int which) const
{
	if(which < 0 || which >= 32) return false;
	return (m_dwFlags & (1 << which)) != 0;
}

KPlayer* KQuestNew::GetPlayer()
{
	return m_pPlayer;
}

KPlayer* KQuestNew::GetOwner()
{
	return m_pPlayer;
}

int KQuestNew::GetGroupCount() const
{
	return m_grpList.size();
}

KTargetGroup* KQuestNew::GetGroup(int grpIdx)
{
	if(grpIdx < 0 || grpIdx >= this->GetGroupCount())
		return NULL;

	KTargetGroup* pGrp = m_grpList.begin();
	while(grpIdx)
	{
		pGrp = m_grpList.next(pGrp);
		grpIdx -= 1;
	}

	return pGrp;
}

KQuestSession* KQuestNew::GetSessionObj() 
{
	if(m_pSession) return m_pSession;
	m_pSession = KClientQuestSession::Alloc();
	m_pSession->Reset();
	return m_pSession;
}

bool KQuestNew::SetSession(const char* name, int val)
{
	bool ret = GetSessionObj()->SetSession(name, val) == TRUE;
	if(ret) {
		KDynamicWorld::getSingleton().onQuestSetSession(this);
	}
	return ret;
}

int KQuestNew::GetSession2(DWORD key, int defVal)
{
	if(!m_pSession) return defVal;
	return m_pSession->GetSession(key, defVal);
}

int KQuestNew::GetSession(const char* name, int defVal)
{
	if(!m_pSession) return defVal;
	return m_pSession->GetSession(name, defVal);
}

bool KQuestNew::SetSession2(DWORD key, int val)
{
	bool ret = GetSessionObj()->SetSession(key, val) != FALSE;
	if(ret) KDynamicWorld::getSingleton().onQuestSetSession(this);
	return ret;
}

int KQuestNew::IncSession(const char* name, int delta)
{
	if(!m_pSession) return delta;
	return m_pSession->IncSession(name, delta);
}

int KQuestNew::DecSession(const char* name, int delta)
{
	if(!m_pSession) return -delta;
	return m_pSession->IncSession(name, -delta);
}

const char* KQuestNew::_GetTraceText(char* buf, int len)
{
	buf[0] = '\0';

	int grpIdx = this->GetSession(Session_Group, 0) - 1;
	if(grpIdx < 0) return buf;

	KTargetGroup* pCurrGrp = this->GetGroup(grpIdx);
	if(!pCurrGrp) return buf;

	if(m_pQuestTimer && m_pQuestTimer->IsActive())
	{
		int timerlen = m_pQuestTimer->GetTraceText(buf, len);
		if(timerlen)
		{
			buf += timerlen;
			*buf++ = '\n';
			len -= timerlen + 1;
		}
	}


	char* p = buf;
	int n = pCurrGrp->GetTraceText(p, len);
	p += n; len -= n;

	p[0] = '\0';
	return buf;	
}

const char* KQuestNew::GetTraceText(char* buf, int len)
{
	//if(!this->hasLua(lua_GetTraceText))
	//{
	//	return this->_GetTraceText(buf, len);
	//}

	//MethodName fn = this->luaMethod(lua_GetTraceText);
	//const char* f = fn.c_str();
	//const char* traceText = LuaWraper.Call<const char*>(f, this, this->GetPlayer());
	//if(!traceText) return NULL;

	//int textLength = strlen(traceText);
	//if(textLength >= len) textLength = len - 1;
	//memcpy(buf, traceText, textLength);
	//buf[textLength] = '\0';

	//return buf;
	return "";
}

bool KQuestNew::IsOverStatus()
{
	int questStatus = this->GetQuestStatus();
	return	questStatus == KQ_None
			|| questStatus == KQ_QuestOver
			|| questStatus == KQ_QuestFailed;
}

void KQuestNew::SyncStatusFromSession()
{
	m_status = GetQuestStatus();
}

bool KQuestNew::hasLua(int method)
{
	return (m_dwLuaMethodFlag & (1<<method)) != 0;
}

KQuestNew::MethodName KQuestNew::luaMethod(int method)
{
	MethodName name;
	return name.Printf("Quest_%d.%s", m_qid, m_luaMethods[method]);
}

bool KQuestNew::SetAttr(int attrId, char* val)
{
	switch(attrId)
	{
	case KQuestAttr::questId:
		return true;
	case KQuestAttr::nameId:
		m_nameId = str2int(val);
		return true;
	case KQuestAttr::talk:
		m_talkId = str2int(val);
		return true;
	case KQuestAttr::battleField:
		m_battleField = str2int(val);
		return true;
	case KQuestAttr::extraDescBGP:
		m_extraDescBGPId = str2int(val);
		return true;
	case KQuestAttr::extraDescStr:
		m_extraDescStrId = str2int(val);
		return true;
	case KQuestAttr::grade:
		m_grade = str2int(val);	// 任务等阶，决定任务颜色
		return true;
	case KQuestAttr::acceptDialogId:	//接受对话
		m_acceptDialogId = str2int(val);
		return true;
	case KQuestAttr::finishDialogId:	//完成对话
		m_finishDialogId = str2int(val);
		return true;
	case KQuestAttr::type:
		m_type = str2int(val);
		return true;
	case KQuestAttr::level:
		m_level = str2int(val);
		return true;
	case KQuestAttr::abandon:
		m_abandon = str2int(val);
		return true;
	case KQuestAttr::reuse:
		m_reuse = str2int(val);
		return true;
	case KQuestAttr::share:
		m_share = str2int(val);
		return true;
	case KQuestAttr::acceptLevel:
		m_acceptLevel = str2int(val);
		return true;
	case KQuestAttr::acceptTopLimit:
		m_acceptTopLimit = str2int(val);
		return true;
	case KQuestAttr::prevQuest:
		m_prevQuest = str2int(val);
		return true;
	case KQuestAttr::nextQuest:
		m_nextQuest = str2int(val);
		return true;
	case KQuestAttr::hardDegree:
		m_hardDegree = str2int(val);
		return true;
	case KQuestAttr::money:
		m_money = str2int(val);
		return true;
	case KQuestAttr::exp:
		m_exp = str2int(val);
		return true;
	case KQuestAttr::items:
		{
			// 10001:1,10002:1,10003:1
			char* ssItems[5];
			m_itemGiftCount = split(val, ",", ssItems, 5);
			ASSERT_I(m_itemGiftCount < 5);
			m_itemGiftCount = m_itemGiftCount > 4 ? 4 : m_itemGiftCount;

			for (int i=0; i<m_itemGiftCount; i++)
			{
				// 10001:1
				char* ssItemDesc[2];
				split(ssItems[i], ":", ssItemDesc, 2);

				m_itemGifts[i].itemId = str2int(ssItemDesc[0]);
				m_itemGifts[i].count = str2int(ssItemDesc[1]);
			}
		}
		return true;
	case KQuestAttr::questItems:
		{
			char* ssItems[5];
			m_questItemCount = split(val, ",", ssItems, 5);
			ASSERT_I(m_questItemCount < 5);
			m_questItemCount = m_questItemCount > 4 ? 4 : m_questItemCount;

			for (int i=0; i<m_questItemCount; i++)
			{
				// 10001:1
				char* ssItemDesc[2];
				split(ssItems[i], ":", ssItemDesc, 2);
				m_questItems[i].itemId = str2int(ssItemDesc[0]);
				m_questItems[i].count = str2int(ssItemDesc[1]);
			}

			return true;
		}
	case KQuestAttr::desc:
		m_descId = str2int(val);
		return true;
	case KQuestAttr::finishDesc:
		m_finishDescId = str2int(val);
		return true;
	case KQuestAttr::achieveId:
		m_achieveId = str2int(val);
		return true;
	case KQuestAttr::achievement:
		m_achievement = str2int(val);
		return true;
	case KQuestAttr::userData:
		return true;
	case KQuestAttr::timeperiod:
		m_timeperiod = str2int(val);
		return true;
	case KQuestAttr::flag:
		{
			m_dwFlags = 0;

			char* ss[32];
			int n = split(val, ',', ss, 32);

			for(int i=0; i<n; i++)
			{
				char* flagstr = ss[i];
				if(!flagstr[0]) continue;
				
				int whichflag = str2int(flagstr);
				if(whichflag < 0 || whichflag >= 32)
				{
					Log(LOG_ERROR, "error: quest %d, invalid quest flag %d", m_qid, whichflag);
					continue;
				}
				this->SetFlag(whichflag);
			}
			return true;
		}
	};
	return false;
}

const char* KQuestNew::GetStatusDesc(KPlayer* pPlayer, char* buf, int len)
{
	KQuestStatusCode status = this->GetQuestStatus();
	switch(status)
	{
	case KQ_None:
		return this->GetTalkDesc(pPlayer, buf, len);
	case KQ_PreStepOver:
		return this->GetFinishDesc(pPlayer, buf, len);
	case KQ_QuestRuning:
	case KQ_QuestFailed:
	default:
		return this->GetTalkDesc(pPlayer, buf, len);
	}
}

void KQuestNew::GetGiftDesc(KPlayer* pPlayer, KDString<512>& GiftStr)
{

	GiftStr.clear();
	if(!pPlayer) pPlayer = m_pPlayer;
	
	if(this->hasLua(lua_GiftDesc))
	{
		MethodName fn = this->luaMethod(lua_GiftDesc);
		const char* tmp = LuaWraper.Call<const char*>(fn.c_str(), this, pPlayer);
		GiftStr.append(tmp);
		return;
	}

	KDString<32> itemList("");
	if (m_itemGiftCount > 0)
	{
		for (BYTE i=0; i<m_itemGiftCount; i++)
		{
			if(i) itemList.append(";");
			itemList.append(m_itemGifts[i].itemId);
			itemList.append(":");
			itemList.append(m_itemGifts[i].count);
		}
	}

	const char* tmp = LuaWraper.Call<const char* ,const char*, int, int, int>("GetQuestGiftStr", itemList.c_str(), m_exp, m_money, 0);
	GiftStr.append(tmp);
}

void KQuestNew::GetGiftDescWithExtraExp( KPlayer* pPlayer, KDString<512>& GiftStr, int aExp, int aTimes )
{
	//GiftStr.clear();
	//if(!pPlayer) pPlayer = m_pPlayer;

	//if(this->hasLua(lua_GiftDesc))
	//{
	//	MethodName fn = this->luaMethod(lua_GiftDesc);
	//	std::string pc = LuaWraper.Call<const char*>(fn.c_str(), this, pPlayer, aExp, aTimes);
	//	GiftStr.append(pc.c_str());
	//	return;
	//}
	//KDString<32> itemList("");
	//if (m_itemGiftCount > 0)
	//{
	//	for (BYTE i=0; i<m_itemGiftCount; i++)
	//	{
	//		if(i) itemList.append(";");
	//		itemList.append(m_itemGifts[i].itemId);
	//		itemList.append(":");
	//		itemList.append(m_itemGifts[i].count);
	//	}
	//}
	//aExp = (aExp > 0)?aExp:0;
	//aTimes = (aTimes > 1)?aTimes:1;
	//int nExp = (m_exp + aExp)*aTimes;
	//const char* tmp = LuaWraper.Call<const char* ,const char*, int, int, int>("GetQuestGiftStr", itemList.c_str(), nExp, m_money, 0);
	//GiftStr.append(tmp);
}

bool KQuestNew::IsSelectGift()
{
	char buf[128]={0};
	GetSelectGiftList(buf,127);
	return strlen(buf)>0;
}

const char* KQuestNew::GetSelectGiftList(char* buf, int len)
{
	if(this->hasLua(lua_SelectGiftList))
	{
		MethodName fn = this->luaMethod(lua_SelectGiftList);
		const char* f = fn.c_str();
		LuaString str = LuaWraper.Call<LuaString>(f, this);
		strcpy_k(buf, len, str.c_str());
		return buf;
	}
	return "";
}

const char* KQuestNew::GetUseGift(KPlayer* pPlayer,char* buf, int len)
{
	if(this->hasLua(lua_UseGift))
	{
		MethodName fn = this->luaMethod(lua_UseGift);
		const char* f = fn.c_str();
		LuaString str = LuaWraper.Call<LuaString>(f, this,pPlayer);
		strcpy_k(buf, len, str.c_str());
		return buf;
	}
	return "";
}

const char* KQuestNew::GetName(KPlayer* pPlayer, char* buf, int len)
{
	if(this->hasLua(lua_Name))
	{
		MethodName fn = this->luaMethod(lua_Name);
		const char* f = fn.c_str();
		LuaString str = LuaWraper.Call<LuaString>(f, this, pPlayer);
		strcpy_k(buf, len, str.c_str());
		return buf;
	}
	else
	{
		LuaString str = this->_GetName(pPlayer);
		strcpy_k(buf, len, str.c_str());
		return buf;
	}
	return "";
}


const char* KQuestNew::GetDesc(KPlayer* pPlayer, char* buf, int len)
{
	if(this->hasLua(lua_Description))
	{
		MethodName fn = this->luaMethod(lua_Description);
		const char* f = fn.c_str();
		const char* pc = LuaWraper.Call<const char*>(f, this, pPlayer);
		strcpy_k(buf, len, pc);
		strcat(buf, "<br>");
		return buf;
	}
	else
	{
		const char* s = KDynamicWorld::getSingleton().GetStringByID(m_descId);
		if(!s) s = "<NULL>";
		KQuestLuaParser& parser = KQuestManager::GetInstance()->m_luaParser;
		const char* pc = parser.ParseQuestString(pPlayer, this, s);
		if(!pc) pc = "<NULL>";
		strcpy_k(buf, len, pc);
		strcat(buf, "<br>");
		return buf;
	}
	return "";
}

const char* KQuestNew::GetTalkDesc(KPlayer* pPlayer, char* buf, int len)
{
	if(this->hasLua(lua_TalkDesc))
	{
		MethodName fn = this->luaMethod(lua_TalkDesc);
		const char* f = fn.c_str();
		const char* pc = LuaWraper.Call<const char*>(f, this, pPlayer);
		strcpy_k(buf, len, pc);
		return buf;
	}
	else
	{
		const char* s = KDynamicWorld::getSingleton().GetStringByID(m_talkId);
		if(!s) s = "<NULL>";
		KQuestLuaParser& parser = KQuestManager::GetInstance()->m_luaParser;
		const char* pc = parser.ParseQuestString(pPlayer, this, s);
		if(!pc) pc = "<NULL>";
		strcpy_k(buf, len, pc);
		return buf;
	}
	return "";
}

const char* KQuestNew::GetFinishDesc(KPlayer* pPlayer, char* buf, int len)
{
	if(this->hasLua(lua_FinishDesc))
	{
		MethodName fn = this->luaMethod(lua_FinishDesc);
		const char* f = fn.c_str();
		const char* pc = LuaWraper.Call<const char*>(f, this, pPlayer);
		strcpy_k(buf, len, pc);
		return buf;
	}
	else
	{
		const char* s = KDynamicWorld::getSingleton().GetStringByID(m_finishDescId);
		if(!s) s = "<NULL>";
		KQuestLuaParser& parser = KQuestManager::GetInstance()->m_luaParser;
		const char* pc = parser.ParseQuestString(pPlayer, this, s);
		if(!pc) pc = "<NULL>";
		strcpy_k(buf, len, pc);
		return buf;
	}
	return "";
}

bool KQuestNew::IsOver()
{
	int questStatus = this->GetQuestStatus();
	return questStatus == KQ_QuestOver;
}

int KQuestNew::GetStatDescriptionID(KPlayer* iPlayer, DWORD dwQuestState)
{
	if (!iPlayer) return 0;
	switch (dwQuestState)
	{
	case KQ_None:
		return m_talkId;
	case KQ_PreStepOver:
		return m_finishDescId;
	case KQ_QuestRuning:
	case KQ_QuestFailed:
		return 0;
	};
	return 0;
}

void KQuestNew::AddGroup(KTargetGroup* pGrp)
{
	pGrp->m_grpIndex = m_grpList.size();
	pGrp->m_pQuest = this;
	m_grpList.push_back(pGrp);
}

LuaString KQuestNew::_GetName(KPlayer* pPlayer)
{
	LuaString name = this->_GetColorName(pPlayer);
	int status = this->GetQuestStatus();

	KQuestManager* pQuestManager = KQuestManager::GetInstance();
	KQuestLuaParser& parser = pQuestManager->m_luaParser;

	parser.ParseQuestString(pPlayer, this, name.c_str());
	if(!name) name = "<NULL>";

	switch(status)
	{
	case KQ_None:
		break;
	case KQ_PreStepOver:
		name.append(KDynamicWorld::getSingleton().GetStringByID(50001));
		break;
	case KQ_QuestOver:
		break;
	case KQ_QuestRuning:
		break;
	case KQ_QuestFailed:
		name.append(KDynamicWorld::getSingleton().GetStringByID(50002));
		break;
	}

	return name;
}

LuaString KQuestNew::_GetColorName(KPlayer* pPlayer)
{
	/*if (!m_nameId) return LuaString("");
	const char* name = g_DynamicWorld.GetStringByID(m_nameId);
	return LuaString(name);*/
	return LuaString("tbd!");
}

bool KQuestNew::CanSee(KPlayer* pPlayer, DWORD npcId)
{
	//任务中的CanSee和CanDoThis如果存在lua脚本，则不能在客户端判定
	if (this->hasLua(lua_CanSee) || this->hasLua(lua_CanDoThis)) 
		return false;

	if (m_acceptTopLimit && pPlayer->GetAttrValue(ca_level) > m_acceptTopLimit)
	{
		// 超过等级上限检查
		return false;
	}

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;

	if(playerQuestManager.GetQuest(m_qid))
	{
		return true;
	}
	else
	{
		if (playerQuestManager.IsQuestEverDone(m_qid) && (m_reuse == 0))
		{
			return false;
		}

		return true;
	}
}


bool KQuestNew::CanDoThis(KPlayer* pPlayer)
{
	// Level.
	int level = pPlayer->GetLev();
	if (level < m_acceptLevel) return false;
	if(m_acceptTopLimit && level > m_acceptTopLimit) return false;

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	// 是否正在做这个任务
	if (playerQuestManager.GetQuest(m_qid))
		return false;

	if (playerQuestManager.IsQuestEverDone(m_qid) && !m_reuse)
		return false;

	if(m_prevQuest)
	{
		KQuestManager* pQuestManager = KQuestManager::GetInstance();
		KQuestNew* pPrevQuest = pQuestManager->GetQuest(m_prevQuest);
		if(pPrevQuest)
		{
			if(KQuestNew::IsDailyQuest(pPrevQuest->m_type))
			{
				if(!playerQuestManager.IsQuestEverDone(m_prevQuest, 0))
					return false;
			}
			else
			{
				if(!playerQuestManager.IsQuestEverDone(m_prevQuest))
					return false;
			}
		}
	}

	if(KQuestNew::IsDailyQuest(m_type))
	{
		if(playerQuestManager.IsQuestEverDone(m_qid, 0))
			return false;
	}

	if(!this->checkPeriod(pPlayer))
		return false;

	return true;
}

bool KQuestNew::checkPeriod(KPlayer* pPlayer)
{
	int nTimes = 0;
	time_t ttLastTime = 0;

	time_t now = time(NULL);

	if(!m_timeperiod)
		return TRUE;

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	if (m_timeperiod < 24)
	{
		// n小时
		if (playerQuestManager.GetQuestHistory(m_qid, nTimes, ttLastTime))
		{
			if (KQuestCommon::IsSameHours(ttLastTime, now, m_timeperiod) && (nTimes>=m_reuse))
				return FALSE;
		}
		return TRUE;
	}

	if (m_timeperiod == 24)
	{
		// 天
		if (playerQuestManager.GetQuestHistory(m_qid, nTimes, ttLastTime))
		{
			if (KQuestCommon::IsSameDay(ttLastTime, now) && (nTimes>=m_reuse))
				return FALSE;
		}
		return TRUE;
	}

	if (m_timeperiod == 24 * 7)
	{
		// 周
		if (playerQuestManager.GetQuestHistory(m_qid, nTimes, ttLastTime))
		{
			if (KQuestCommon::IsSameWeek(ttLastTime, now) && (nTimes>=m_reuse))
				return FALSE;
		}
		return TRUE;
	}

	if (m_timeperiod == 24 * 7 * 30)
	{
		// 月
		if (playerQuestManager.GetQuestHistory(m_qid, nTimes, ttLastTime))
		{
			if (KQuestCommon::IsSameMonth(ttLastTime, now) && (nTimes>=m_reuse))
				return FALSE;
		}
		return TRUE;
	}
	return TRUE;
}

const char* KQuestNew::GetTypeText( KPlayer* pPlayer, char* buf, int len )
{
	LuaString str("");
	str.append( this->GetTypeText_lua(this->m_type) );
	strcpy_k(buf, len, str.c_str());
	return buf;
}

const char* KQuestNew::GetTypeText_lua( int qtype )
{
	if(qtype == 0)
		qtype = this->m_type;	// 默认为本身type
	switch (qtype)
	{
	case 1:	// 主线
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_1);
	case 2: // 支线
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_2);
	case 3: // 宗门
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_3);
	case 4: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_4);
	case 5: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_5);
	case 6: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_6);
	case 7: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_7);
	case 8: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_8);
	case 9: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_9);
	case 10: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_10);
	case 11: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_11);
	case 12: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_12);
	case 13: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_13);
	case 14: //
		return KDynamicWorld::getSingleton().GetStringByID(QuestType_14);
	default :
		break;
	}
	return "";
}

int KQuestNew::GetBattleField()
{
	if(m_type==enum_daily_quest){
		return m_battleField + g_rnd.GetRandom(0,3);
	}else{
		return m_battleField;
	}
}

bool KQuestNew::IsDailyQuest()
{
	return m_type==enum_daily_quest;
}
bool KQuestNew::IsDailyQuest( BYTE qtype )
{
	if (qtype == enum_daily_quest)
	{
		return true;
	}
	return false;
}

void KQuestNew::GetItemGiftDesc( KPlayer* pPlayer, KDString<512>& GiftStr )
{
	GiftStr.clear();
	if(!pPlayer) pPlayer = m_pPlayer;

	if(this->hasLua(lua_GiftDesc))
	{
		MethodName fn = this->luaMethod(lua_GiftDesc);
		const char* tmp = LuaWraper.Call<const char*>(fn.c_str(), this, pPlayer);
		if (tmp)
		{
			GiftStr.append(tmp);
		}
		else
			GiftStr.append("");
		return;
	}

	KDString<32> itemList("");
	if (m_itemGiftCount > 0)
	{
		for (BYTE i=0; i<m_itemGiftCount; i++)
		{
			if(i) itemList.append(";");
			itemList.append(m_itemGifts[i].itemId);
			itemList.append(":");
			itemList.append(m_itemGifts[i].count);
		}
	}

	const char* tmp = LuaWraper.Call<const char* ,const char*, int, int, int>("GetQuestGiftStr", itemList.c_str(), 0, 0, 0);
	GiftStr.append(tmp);

	return;
}

void KQuestNew::GetLimitCondition( KPlayer* pPlayer, KDString<512>& cStr )
{
	cStr.clear();
	if (LuaWraper.hasFunction("GetQuestLimitCondition"))
	{
		const char* tmp = LuaWraper.Call<const char* , KQuestNew*, KPlayer*>("GetQuestLimitCondition", this, pPlayer);
		cStr.append(tmp);
	}
}

bool KQuestNew::OnEventImp(KEventID id, const KEventData* pData, uint_r customData)
{
	int evtId = (int)id;
	void* p = (void*)pData;
	int len = sizeof(KEventData);

	this->OnGameEvent(evtId, p, len);

	// 这里暂时先设置成false，继续接收处理此类事件。
	return false;
}

void KQuestNew::OnGameEvent(int evtId, void* evtData, int len)
{
	//if(!m_pCurrGrp) return;
	if(!m_pCurrGrp) m_pCurrGrp = m_grpList.begin();

	// 有事件先给成就
	KTargetGroup* pGrp = m_grpList.begin();
	while (pGrp)
	{
		if(pGrp == m_pCurrGrp)
		{
			pGrp->OnGameEvent(evtId, evtData, len);
			if(this->IsOverStatus())
			{
				m_pCurrGrp = NULL;
				return;
			}
			break;
		}
		pGrp = m_grpList.next(pGrp);
	}

	// 当前目标组已经完成，进入到下一个目标组
	while(m_pCurrGrp && m_pCurrGrp->IsOver())
	{
		KTargetGroup* grp = m_pCurrGrp;
		m_pCurrGrp->OnFinish();

		if(this->IsOverStatus())
		{
			m_pCurrGrp = NULL;
			return;
		}

		// 在OnFinish中发生了跳步
		// 需要判断一下
		if(!m_pCurrGrp || !m_pCurrGrp->IsOver()) break;
				
		m_pCurrGrp = m_grpList.next(m_pCurrGrp);
		if(m_pCurrGrp)
		{
			this->SetSession(Session_Group, m_pCurrGrp->m_grpIndex+1);
			//m_pCurrGrp->OnEnter(); //TBD
			if(this->IsOverStatus())
			{
				m_pCurrGrp = NULL;
				return;
			}
		}
	}

	// 没有后续的目标了，任务完成
	if(!m_pCurrGrp)
	{
		this->SetQuestStatus(KQ_PreStepOver);
	}
}

void KQuestNew::SetQuestDone()
{
	SetQuestStatus(KQ_QuestOver);
}

void KQuestNew::OnFinish()
{
	// 播放完成特效

	// 结算任务成就

	// 奖励
	this->OnGift();

	// 推荐活动额外奖励
	// todo:

	// 是否有完成脚本
	/*if (this->hasLua(lua_OnFinish))
	{
		MethodName fn = this->luaMethod(lua_OnFinish);
		const char* f = fn.c_str();
		LuaWraper.Call<bool>(f, this, m_pPlayer);
	}*/


	//再更新下npc任务状态
	this->SetQuestStatus(KQ_QuestOver);

	// 触发完成任务world事件
	//KEventAbout::KEDWorldQuestOver edw;
	//edw.m_pChar = m_pPlayer;
	//edw.m_pQuest = this;
	//edw.m_result = enum_quest_ok;
	//m_pPlayer->GetWorld()->FireEvent(edw);

	// 记录log
}

int KQuestNew::GetAwardFateStone()
{
	if(m_type == enum_tower_quest){
		return KTowerAssist::_calcFateStone(m_pPlayer->GetPlayerRecord());
	}else{
		return 0;
	}
}

int KQuestNew::GetAwardMoney()
{
	if(m_type == enum_daily_quest &&
		KPlayerRecordAssist::RemainDailyQuestNum(&m_pPlayer->m_questRecord)==0){
		return m_money*0.2;
	}else if(m_type == enum_tower_quest){
		return KTowerAssist::_calcMoney(m_pPlayer->GetPlayerRecord());
	}else{
		return m_money;
	}
}
int KQuestNew::GetAwardExp()
{
	if(m_type == enum_daily_quest &&
		KPlayerRecordAssist::RemainDailyQuestNum(&m_pPlayer->m_questRecord)==0){
		return m_exp*0.2;
	}else if(m_type == enum_tower_quest){
		return KTowerAssist::_calcExp(m_pPlayer->GetPlayerRecord());
	}else{
		return m_exp;
	}
}
void KQuestNew::OnGift()
{
	// 奖励
	m_pPlayer->IncreaseCriVal(GetAwardMoney(),GetAwardExp(),GetAwardFateStone());

	if (m_achieveId != 0)
	{
		//m_pPlayer->AddAchievement(m_achieveId, m_achievement);
	}
	char sz[128]={0};
	const char* pUseGift = GetUseGift(m_pPlayer,sz,127);
	int useItem = atoi(pUseGift);
	if(useItem>0) KItemAbout::KUseItemManager::GetInstance()->UseItem(useItem,m_pPlayer->GetID());
	if (m_itemGiftCount > 0)
	{
		for (BYTE i=0; i<m_itemGiftCount; i++)
		{
			VirtualService::getSingleton().AddItem(m_itemGifts[i].itemId,m_itemGifts[i].count);
		}
	}

	// 任务成就的额外奖励
}
int KQuestNew::RemainDailyQuestNum()
{
	return KPlayerRecordAssist::RemainDailyQuestNum(&m_pPlayer->m_questRecord);
}
