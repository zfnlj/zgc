#pragma once

#include <System/Collections/KMapByVector.h>
#include <System/Collections/KTree.h>
#include <System/Memory/KAlloc.h>
#include "Inc/Lua/KLuaWrap.h"

// 未接任务相关
struct AvailQuestDesc
{
	int m_nQuestId;
	int m_nLevel;
	DWORD m_dwNpcID;
	int m_nNationality;
	int m_nMinLevelLimit;
	int m_nMaxLevelLimit;
};
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
public:
	UseItemMap m_mapUseItem;
};

};