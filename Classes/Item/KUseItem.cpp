#include "KUseItem.h"
#include "../WorldObject/KPlayer.h"
#include <System/Memory/KBuffer64k.h>
#include "CommonLogic/StringManager/KStringManager.h"
#include "CommonLogic/WorldObject/KAttrDefines.h"

#include "System/Collections/KString.h"
#include <time.h>
#include "../GameLogic/KDynamicWorld.h"
#include "KUseItemParamParser.h"
#include "KUseItemAttrs.h"
#include "../StaticTable/KGameStatic.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../common/KPlayerRecordAssist.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Inc/KLogicMsg.h"
#include "../../Inc/PacketGate/c_game_packet.h"
#include "../common/KCommonObj.h"

using namespace KAttributeAbout;
using namespace System::Collections;

static LuaString va(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char buf[512];
	int n = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	return KString<512>(buf, n);
}

namespace KItemAbout
{
const char* KUseItem::m_luaMethods[lua_Count] = {
	"Initialize",
	"AfterInitialize",
	"run",
};

KUseItem::KUseItem()
{
	this->Reset();
}

KUseItem::~KUseItem()
{
	this->Reset();
}

// 从Lua中初始化
bool KUseItem::Initialize(int qid, const char* questTable)
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

bool KUseItem::AfterInitialize(const char* questTable)
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

int KUseItem::Breathe(int nInterval)
{
	return 0;
}

KUseItem* KUseItem::Clone()
{
	KUseItem* pQuest = KUseItem::Alloc();
	pQuest->Reset();

	pQuest->m_qid = m_qid;
	pQuest->m_nameId = m_nameId;
	pQuest->m_type = m_type;
	//pQuest->m_view = m_view;

	pQuest->m_dwFlags = m_dwFlags;

	return pQuest;
}

void KUseItem::Reset()
{
	m_lastBreatheTick = 0;

	m_qid = 0;
	m_nameId = 0;
	m_type = 0;

	m_dwFlags = 0;

	m_evtArray.clear();

}

bool KUseItem::RegEvent(int evtId, DWORD key, DWORD customData)
{
	return true;
}

bool KUseItem::RemoveEvent(int evtId, DWORD key)
{
	return true;
}

bool KUseItem::SetAttrs(const char* attrStr)
{
	KUseItemParamParser& parser = KUseItemParamParser::GetInstance();
	if(!parser.Parse(attrStr)) return false;
	char* name;
	char* val;
	int c = parser.GetParamCount();
	for(int i=0; i<c; i++)
	{
		parser.GetParameter(i, name, val);
		int attrId = KUseItemAttr::GetAttrId(name);
		
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

void KUseItem::InitLuaFuncFlag(const char* tableName)
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


int KUseItem::GetIntAttr(const char* attrName, int defVal)
{
	int attrId = KUseItemAttr::GetAttrId(attrName);
	if(attrId < 0) return defVal;
	switch(attrId)
	{
	case KUseItemAttr::itemId:		return m_qid;
	case KUseItemAttr::type:			return m_type;
	};
	return defVal;
}

const char* KUseItem::GetStringAttr(const char* attrName, const char* defVal)
{
	int attrId = KUseItemAttr::GetAttrId(attrName);
	return defVal;
}

bool KUseItem::SetFlag(int which)
{
	if(which < 0 || which >= 32) return false;
	m_dwFlags |= 1 << which;
	return true;
}

void KUseItem::ClearFlag(int which)
{
	if(which < 0 || which >= 32) return;
	m_dwFlags &= ~(1 << which);
	return;
}

bool KUseItem::HasFlag(int which) const
{
	if(which < 0 || which >= 32) return false;
	return (m_dwFlags & (1 << which)) != 0;
}

bool KUseItem::hasLua(int method)
{
	return (m_dwLuaMethodFlag & (1<<method)) != 0;
}

KUseItem::MethodName KUseItem::luaMethod(int method)
{
	MethodName name;
	return name.Printf("item_%d.%s", m_qid, m_luaMethods[method]);
}

bool KUseItem::SetAttr(int attrId, char* val)
{
	switch(attrId)
	{
	case KUseItemAttr::itemId:
		return true;
	case KUseItemAttr::type:
		m_type = str2int(val);
		return true;

	//case KUseItemAttr::items:
	//	{
	//		// 10001:1,10002:1,10003:1
	//		char* ssItems[5];
	//		m_itemGiftCount = split(val, ",", ssItems, 5);
	//		ASSERT_I(m_itemGiftCount < 5);
	//		m_itemGiftCount = m_itemGiftCount > 4 ? 4 : m_itemGiftCount;

	//		for (int i=0; i<m_itemGiftCount; i++)
	//		{
	//			// 10001:1
	//			char* ssItemDesc[2];
	//			split(ssItems[i], ":", ssItemDesc, 2);

	//			m_itemGifts[i].itemId = str2int(ssItemDesc[0]);
	//			m_itemGifts[i].count = str2int(ssItemDesc[1]);
	//		}
	//	}
	//	return true;
	};
	return false;
}

void KUseItem::run(UINT64 playerId)
{
	MethodName fn = this->luaMethod(lua_run);
	const char* f = fn.c_str();


	if(!LuaWraper.Call<bool>(f, this,playerId))
	{
		Log(LOG_FATAL, "KUseItem: qid:%d call lua %s", m_qid, f);
		return ;
	}

}

void KUseItem::GeneratNormalCard(UINT64 playerId,int count,int rank1Rate,int rank2Rate,int rank3Rate,int heroRate)
{
	SC_GenPlayerCard genCard;
	KIntegerList tmpLst;
	KGameStaticMgr::getSingleton().RndGetNormalCard(count,rank1Rate,rank2Rate,rank3Rate,heroRate,tmpLst);
	KWorldObjAbout::KPlayer* pPlayer = KDynamicWorld::getSingleton().GetPlayer(playerId);
	for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();it++){

		bool bNew = true;
		KCardStatic* pCardStatic = KGameStaticMgr::getSingleton().GetCard(*it);
		if(pCardStatic->GetType()==KCardStatic::card_hero){
			KHeroDef hero;
			hero.rndGenerate(*it);
			KPlayerRecordAssist::addHero(pPlayer->GetPlayerRecord(),&hero);
		}else{
			KPlayerRecordAssist::addStoreCard(pPlayer->GetPlayerRecord(),*it,1,bNew);
		}
		genCard.AddCard(*it,bNew);
	}
	KDynamicWorld::getSingleton().SendWorldMsg(LOGIC_WORLD_GEN_PLAYERCARD,(unsigned long long)&genCard,0);
}

void KUseItem::GenerateCardDeck(UINT64 playerId,int deckId)
{
	KWorldObjAbout::KPlayer* pPlayer = KDynamicWorld::getSingleton().GetPlayer(playerId);
	pPlayer->m_cardDepot.CreateOnDeckDef(deckId);
}

void KUseItem::GenerateHero(UINT64 playerId,int heroId)
{
	KWorldObjAbout::KPlayer* pPlayer = KDynamicWorld::getSingleton().GetPlayer(playerId);
	int id = pPlayer->m_cardDepot.CreateHero(heroId);
	pPlayer->GetResultBag()->Add(1,id,1);
}

int KUseItem::RndVal()
{
	return _RndNormal(1,100);
}

};