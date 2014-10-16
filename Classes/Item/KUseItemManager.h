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
	static bool m_initFlag;	// questmanager��ʼ����־, ��ö�γ�ʼ��

public:
	// ��ʼ�����������
	bool Initialize(void);
	void Breathe(int interval);

public:
	KUseItem* GetUseItem(int id);	// ͨ������IDȡ������ָ��
	bool ReloadUseItem(int qid);		// ���¼�������ű�
	bool UseItem(int id,UINT64 playerId);
	bool UseItem(const char* itemName,UINT64 playerId);
public:
	UseItemMap m_mapUseItem;
};

};