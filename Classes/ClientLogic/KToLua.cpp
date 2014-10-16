#include "KToLua.h"
//#include "KGlobalParameter.h"
#include "CommonLogic/WorldObject/ICharacter.h"

#include "System/Random/KRandomGroup.h"
#include "System/Algorithm/AngleComputer.h"

#include "../WorldObject/KCharacter.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"

#include "CommonLogic/Item/KStruct_Item.h"
#include "CommonLogic/Store/KStoreProduct.h"

#include "../Item/KItem.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "../Item/KItemCreate.h"
#include "../Item/KUseItem.h"

#include "CommonLogic/StringManager/KStringManager.h"
#include "../Quest/KQuestManager.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KClientBattleCtrl.h"
#include "../GameLogic/assist/KAIAssist.h"
#include "../WorldObject/KMainPlayer.h"

using namespace KAttributeAbout;

static void InitScriptDirectory()
{
//	ASSERT(lwp_success == LuaWraper.doDirScripts("../../data/lua/Logic/"));
	const static char* list[] = {
		"Assist.lua",
		"Global.lua",
		"testlua.lua",
		"HeadText.lua",
	};
	for (int i = 0; i < sizeof(list) / sizeof(list[0]); ++i)
	{
		LuaWraper.doStream(*KTabfileLoader::GetInstance().GetFileStream(list[i], "General"));
	}
}

namespace KScriptAbout
{
	void CharToLuaType(const char* name, const KWorldObjAbout::ICharacter* pChar)
	{
		ASSERT_RETURN_VOID(name);
		if (!pChar)
		{
			return;
		}

		if (pChar->IsKindOf(wokPlayer))
		{
			ObjToLua(name, (const KWorldObjAbout::KPlayer*)pChar);
		}
		else
		{
			ObjToLua(name, pChar);
		}
	}
	
	void WorldObjToLuaType(const char* name, const KWorldObjAbout::KWorldObj* pObj)
	{
		ASSERT_RETURN_VOID(name);
		if (!pObj)
		{
			return;
		}

		if (pObj->IsKindOf(wokGroundItem))
		{
			ObjToLua(name, (const KWorldObjAbout::KGroundItem*)pObj);
		}
		else if (pObj->IsKindOf(wokCharacter))
		{
			CharToLuaType(name, (const KWorldObjAbout::ICharacter*)pObj);
		}
		else
		{
			ObjToLua(name, pObj);
		}
	}

	/**
	 * 在LUA中使用，把数值常数转换成INT64或者UINT64
	 * 在LUA中不要直接使用数值常量作为对象ID的参数，
	 *
	 * 如
	 *     dynmicWorld.GetPlayer(9999)
	 *
	 * 应改成
	 *     dynmicWorld.GetPlayer(UINT64(9999))
	 */
	INT64 LuaInt64(double id)
	{
		return (INT64)id;
	}

	UINT64 LuaUint64(double id)
	{
		return (UINT64)id;
	}

	int LuaInt64ToInt(lua_State* L)
	{
		UINT64 v = 0;

		int n = lua_gettop(L);

		if (n >= 1) v = LMREG::read<UINT64>(L, 1);

		lua_pushnumber(L, (int)v);
		return 1;
	}

	int LuaGetFaceAngle(lua_State* L)
	{
		float fromx = 0.0f;
		float fromy = 0.0f;
		float tox = 0.0f;
		float toy = 0.0f;
		float n = lua_gettop(L);
		if (n >= 1) fromx = LMREG::read<float>(L, 1);
		if (n >= 2) fromy = LMREG::read<float>(L, 2);
		if (n >= 3) tox = LMREG::read<float>(L, 3);
		if (n >= 4) toy = LMREG::read<float>(L, 4);

		float angle = System::Algorithm::AngleComputer::GetFaceAngle(fromx, fromy, tox, toy);
		lua_pushnumber(L, (float)angle);
		return 1;
	}

	KPlayer * CharToPlayer(KCharacter * pChar)
	{
		return NULL == pChar ? NULL : (!pChar->IsKindOf(wokPlayer) ? NULL : static_cast<KPlayer *>(pChar));
	}

	
	bool IsUsableItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsUsableItemStatic(dwItemID);
	}
	

	bool IsPetEggItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsPetEggItemStatic(dwItemID);
	}

	bool IsSkillBookItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsSkillBookItemStatic(dwItemID);
	}

	bool IsSkillStarItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsSkillStarItemStatic(dwItemID);
	}

	bool IsHammerableItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsHammerableItemStatic(dwItemID);
	}

	bool IsBlueprintItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsBlueprintItemStatic(dwItemID);
	}

	bool IsMillDeviceItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsMillDeviceItemStatic(dwItemID);
	}

	bool IsPetBagItem(DWORD dwItemID)
	{
		return KItemAbout::KCreateInfo_ItemBase::IsPetBagItemStatic(dwItemID);
	}

	DWORD GetNow()
	{
		return 0;//NOW;
	}

	const char* STRING(int id)
	{
		return KStringManager::Instance()->GetStringByID(id);
	}

	int AND(int a, int b)
	{
		return a & b;
	}
	int OR(int a, int b)
	{
		return a | b;
	}
	int NOT(int a)
	{
		return ~a;
	}
	int XOR(int a, int b)
	{
		return a ^ b;
	}

	UINT64 StringToID(const char* str)
	{
		ASSERT_RETURN(str, 0);
		UINT64 result;
		sscanf(str, "%llu", &result);
		return result;
	}

	const char* IDToString(UINT64 id)
	{
		static char buffer[32];
		sprintf_k(buffer, sizeof(buffer), "%llu", id);
		return buffer;
	}

	static const char* IDToStringWithQuotes(UINT64 id)
	{
		static char buffer[32];
		sprintf_k(buffer, sizeof(buffer), "'%llu'", id);
		return buffer;
	}
	
	/**
	 * 让服务器执行脚本
	 */
	void ScriptToServer(const char* script)
	{
		ASSERT_RETURN_VOID(script);
		//KDynamicWorld::getSingleton().SendToGatewayServer(s_nCS_ServerScript, (void*)script, strlen(script) + 1);
	}

	/**
	 * 让AIServer执行脚本
	 */
	void ScriptToAIServer(const char* script)
	{
		ASSERT_RETURN_VOID(script);
		//KDynamicWorld::getSingleton().SendToGatewayServer(s_nCS_AIServerScript, (void*)script, strlen(script) + 1);
	}

	KToLua::KToLua()
	{
		// 来个随机数种子
		char buffer[128];
		sprintf_k(buffer, sizeof(buffer), "math.randomseed(%d)", GetTickCount());
		LuaWraper.doString(buffer);

		//InitScriptDirectory();
		// 初始化技能用以强化计算的相关数据
		// 创建全局导出表
		LuaWraper.doString("CPP={}");
		LuaWraper.doString("CPP.enum={}");

		ToLuaEnum();
		ToLuaClass();
		ToLuaClassDirectly();
		ToLuaCPPTable();
		ToLuaExtra();
	}

	void KToLua::ToLuaCPPTable()
	{
		LuaWraper.RegFunc("INT64", LuaInt64);
		LuaWraper.RegFunc("UINT64", LuaUint64);
		LuaWraper.RegFunc("int64", LuaInt64);
		LuaWraper.RegFunc("uint64", LuaUint64);
		
		LuaWraper.RegFunc("CharToPlayer", CharToPlayer);
			
		LuaWraper.RegFunc("StringToID", StringToID);
		LuaWraper.RegFunc("IDToString", IDToString);
		LuaWraper.RegFunc("IDToStringWithQuotes", IDToStringWithQuotes);
		LuaWraper.RegFunc("GetNow", GetNow);
		LuaWraper.RegFunc("STRING", STRING);
		LuaWraper.RegFunc("AND", AND);
		LuaWraper.RegFunc("OR", OR);
		LuaWraper.RegFunc("NOT", NOT);
		LuaWraper.RegFunc("XOR", XOR);
		
		
		LuaWraper.RegFunc("IsUsableItem", IsUsableItem);
		LuaWraper.RegFunc("IsPetEggItem", IsPetEggItem);
		LuaWraper.RegFunc("IsSkillBookItem", IsSkillBookItem);
		LuaWraper.RegFunc("IsSkillStarItem", IsSkillStarItem);
		LuaWraper.RegFunc("IsHammerableItem", IsHammerableItem);
		LuaWraper.RegFunc("IsBlueprintItem", IsBlueprintItem);
		LuaWraper.RegFunc("IsMillDeviceItem", IsMillDeviceItem);
		LuaWraper.RegFunc("IsPetBagItem", IsPetBagItem);
		
		LuaWraper.RegFunc("ScriptToServer", ScriptToServer);
		LuaWraper.RegFunc("ScriptToAIServer", ScriptToAIServer);

		// GameServer，AIServer，客户端都有的------------------------------------------------------
		//ObjToLua("CPP.worldMgr", &KDynamicWorld::getSingleton());
	
		ObjToLua("CPP.itemCreate", KItemAbout::KItemCreate::Instance());
		ObjToLua("CPP.stringMgr", KStringManager::Instance());
		ObjToLua("CPP.questMgr", KQuestManager::GetInstance());

		ObjToLua("CPP.battleCtrl", KClientBattleCtrl::getInstance());
		ObjToLua("CPP.AIAssist", KAIAssist::getInstance());
		ObjToLua("CPP.MainPlayer", KMainPlayer::Instance());
	
		// 独有的----------------------------------------------------------------------------------
		//ObjToLua("CPP.mainPlayer", KMainPlayer::Instance());
	}

	void KToLua::ToLuaClass()
	{
		// GameServer，AIServer，客户端都有的----------------------------------------------------
		// CommonLogic来的
		REGISTER_LUA_CLASS(KWorldObjAbout::ICharacter);
		REGISTER_LUA_CLASS(KStringManager);
		REGISTER_LUA_CLASS(KQuestManager);

		// 当作角色操作
		REGISTER_LUA_CLASS(KWorldObjAbout::KCharacter);
		REGISTER_LUA_CLASS(KWorldObjAbout::KPlayer);
		REGISTER_LUA_CLASS(KMainPlayer);



		REGISTER_LUA_CLASS(KItemAbout::KItemCreate);
		REGISTER_LUA_CLASS(KItemAbout::KCreateInfo_ItemBase);
		REGISTER_LUA_CLASS(KItemAbout::KCreateInfo_Item);


		REGISTER_LUA_CLASS(KItemAbout::KItem);
		REGISTER_LUA_CLASS(KItemAbout::KCellBag);
		REGISTER_LUA_CLASS(KItemAbout::KUseItem);
		
	
		//任务
		REGISTER_LUA_CLASS(KQuestNew);
		REGISTER_LUA_CLASS(KTargetGroup);
		REGISTER_LUA_CLASS(KQuestTarget);

		//card
		REGISTER_LUA_CLASS(KClientBattleCtrl);
		REGISTER_LUA_CLASS(KAIAssist);
		REGISTER_LUA_CLASS(KCardInst);
	}

	struct KItemListInfo 
	{
	};
	void KToLua::ToLuaClassDirectly()
	{
		BeginExportLuaClassDirectly(System::Random::KRandomGroup, KRandomGroup, LuaWraper.GetMainState())
			DefMemberFunc(InsertData);
			DefMemberFunc(InsertRange);
			DefMemberFunc(Clear);
			DefMemberFunc(IsEmpty);
			DefMemberFunc(GetRandomVal);
			DefMemberFunc(TakeRandomVal);
		EndDef;


		BeginExportLuaClassDirectly(KItemListInfo, KItemListInfo, LuaWraper.GetMainState())
		EndDef;
	}

	void KToLua::ToLuaExtra()
	{
		// 导出全局的sleep
		lua_register(LuaWraper.GetMainState(), "int64ToInt", LuaInt64ToInt);
		lua_register(LuaWraper.GetMainState(), "getFaceAngle", LuaGetFaceAngle);
	}

	void KToLua::ToLuaEnum()
	{
		// GameServer，AIServer，客户端都有的------------------------------------------------------
		ObjToLua("CPP.enum.wokPlayer", wokPlayer);
		ObjToLua("CPP.enum.wokNpc", wokNpc);
		ObjToLua("CPP.enum.wokCharacter", wokCharacter);
		ObjToLua("CPP.enum.wokGroundItem", wokGroundItem);


		// 属性
		LuaWraper.doString("CPP.enum.attr={}");
#define EXPORT_ATTR(id) \
	ObjToLua("CPP.enum.attr."#id, id);

		//EXPORT_ATTR(ca_SharpEyed);

#undef EXPORT_ATTR

		// 商店购买类型枚举
		LuaWraper.doString("CPP.enum.storepricetype={}");
#define EXPORT_STOREPRICE_TYPE(id) \
	ObjToLua("CPP.enum.storepricetype."#id, (int)KStoreAbout::id);

		EXPORT_STOREPRICE_TYPE(enumPT_none);
		EXPORT_STOREPRICE_TYPE(enumPT_money);
		EXPORT_STOREPRICE_TYPE(enumPT_cash);
		EXPORT_STOREPRICE_TYPE(enumPT_bluePower);
		EXPORT_STOREPRICE_TYPE(enumPT_item);
		EXPORT_STOREPRICE_TYPE(enumPT_count);

#undef EXPORT_STOREPRICE_TYPE

		LuaWraper.doString("CPP.enum.transPos={}");


		// 任务状态枚举
		LuaWraper.doString("CPP.enum.queststate={}");
#define EXPORT_QUEST_STATE(id) \
	ObjToLua("CPP.enum.queststate."#id, id);

		EXPORT_QUEST_STATE(KQ_None);
		EXPORT_QUEST_STATE(KQ_PreStepOver);
		EXPORT_QUEST_STATE(KQ_QuestOver);
		EXPORT_QUEST_STATE(KQ_QuestRuning);
		EXPORT_QUEST_STATE(KQ_QuestFailed);
		EXPORT_QUEST_STATE(KQ_QuestCancel);

#undef EXPORT_QUEST_STATE

		LuaWraper.doString("CPP.enum.charOwner={}");

	}

	void PushCharToLuaStateByType(lua_State* l, const KWorldObjAbout::ICharacter* pChar)
	{
#define PUSH_CHAR_TO_LUA_STACK(TYPE)	\
		else if (pChar->IsKindOf(wok##TYPE))	\
		{	\
			LMREG::push(l, (const K##TYPE*)pChar);	\
		}

		if (!pChar)
		{
			lua_pushnil(l);
		}
		PUSH_CHAR_TO_LUA_STACK(Player)
		else
		{
			LMREG::push(l, pChar);
		}

#undef PUSH_CHAR_TO_LUA_STACK

	}
}
