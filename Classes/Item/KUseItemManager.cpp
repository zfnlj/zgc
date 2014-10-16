#include "KUseItemManager.h"
#include <System/File/KTabfileLoader.h>
#include "KGlobalFunction.h"
//#include "../KGameServerApp.h"
#include "../WorldObject/KPlayer.h"
//#include <Attribute/KAttrDefine.h>
#include "KUseItem.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../Inc/KLogicMsg.h"

#ifdef _USE_COCOS2DX
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#endif
#include <string>
using namespace KAttributeAbout;
using namespace System::File;

namespace KItemAbout
{
bool KUseItemManager::m_initFlag = false;	// 初始化标志
KUseItemManager::KUseItemManager(void)
{
}

KUseItemManager::~KUseItemManager(void)
{
}

KUseItemManager* KUseItemManager::GetInstance()
{
	static KUseItemManager* p = new KUseItemManager();
	return p;
}

bool KUseItemManager::Initialize(void)
{
	return true;
}

void KUseItemManager::Breathe(int interval)
{
	// 
}

KUseItem* KUseItemManager::GetUseItem(int id)
{
	// 先看是否是普通任务
	UseItemMap::iterator it = m_mapUseItem.find(id);
	if(it != m_mapUseItem.end() )
	{
		return it->second;
	}
	if(!this->ReloadUseItem(id))
	{
		return NULL;
	}

	return m_mapUseItem[id];
}

bool KUseItemManager::ReloadUseItem(int qid)
{
    KUseItem* pUseItem = m_mapUseItem[qid];
	if(pUseItem)
	{
		pUseItem->Reset();
		KUseItem::Free(pUseItem);

		m_mapUseItem[qid] = NULL;
		pUseItem = NULL;
	}

	static char achQuestFileName[256];
	sprintf(achQuestFileName, "item/Script/item_%d.lua", qid);

	//sprintf(achQuestFileName, "../../data/configs/Quest/QuestScripts/Q_%d.lua", qid);
	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(achQuestFileName);
#else
	fullPath = achQuestFileName;
#endif

	StreamInterface* pStream = KTabfileLoader::GetInstance().GetFileStream(fullPath.c_str(), "");
	if(!pStream || LuaWraper.doStream(*pStream) == FALSE)
	{
		Log(LOG_ERROR, "error: use item %s", achQuestFileName);
		if (pUseItem)
		{
			pUseItem->Reset();
			KUseItem::Free(pUseItem);
			pUseItem = NULL;
		}
		if(pStream) KTabfileLoader::GetInstance().CloseFileStream(pStream);
		return false;
	}
	KTabfileLoader::GetInstance().CloseFileStream(pStream);

	char tableName[256];
	sprintf(tableName, "item_%d", qid);
	pUseItem = KUseItem::Alloc(); pUseItem->Reset();
	if(!pUseItem->Initialize(qid, tableName))
	{
		pUseItem->Reset();
		KUseItem::Free(pUseItem);
		return false;
	}

	ASSERT(!m_mapUseItem[qid]);
	m_mapUseItem[qid] = pUseItem;

	return true;
}

bool KUseItemManager::UseItem(const char* itemName,UINT64 playerId)
{
	char f[256];
	sprintf_k(f, sizeof(f), "UseItemUtil:%s", itemName);
	if(!LuaWraper.hasFunction(f)) return false;

	LuaWraper.Call<int>(f);
	return true;
}

bool KUseItemManager::UseItem(int id,UINT64 playerId)
{
	KUseItem* useItem = GetUseItem(id);
	if(!useItem) return false;
	useItem->run(playerId);
	return true;
}
};