#pragma once

#include <System/KType.h>
#include <System/KMacro.h>

enum
{
	cs_none,		// 无效状态
	cs_stand,		// 站立状态
	cs_move,		// 移动状态（走、跑）
	cs_death,		// 死亡状态
	cs_count,
};

namespace KWorldObjAbout
{
	class KWorldObj;
}

class KObjectState
{
public:
	int m_sid; // state id
	KWorldObjAbout::KWorldObj* m_object; // owner object

public:
	KObjectState();
	KObjectState(int stateID);
	virtual ~KObjectState();

public:
	virtual void onEnterState();
	virtual void breathe(int interval);
	virtual void onLeaveState();
	virtual void onMessage(int cmd, const void* data, int len); //接收网络消息
	virtual void reset();
	virtual void destroy();
};
