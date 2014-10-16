#pragma once

#include <System/Collections/KMapByVector.h>
#include <System/Collections/KTree.h>
#include <System/Memory/KAlloc.h>
#include "Inc/Lua/KLuaWrap.h"

namespace KItemAbout
{
class KUseItem;

class KUseItemManager
{
public:
	typedef System::Collections::KMapByVector<int,KUseItem*> UseItemMap;
public:
	KUseItemManager(void);
	~KUseItemManager(void);

public:
	static KUseItemManager* GetInstance();

private:
	static bool m_initFlag;	// questmanager初始化标志, 免得多次初始化

public:
	// 初始化任务管理器
	bool Initialize(void);
	void Breathe(int interval);

public:
	KUseItem* GetUseItem(int id);	// 通过任务ID取得任务指针
	bool ReloadUseItem(int qid);		// 重新加载任务脚本
	bool UseItem(int id,UINT64 playerId);
	bool UseItem(const char* itemName,UINT64 playerId);
public:
	UseItemMap m_mapUseItem;
};

};