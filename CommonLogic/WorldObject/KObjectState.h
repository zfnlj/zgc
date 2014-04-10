#pragma once

#include <System/KType.h>
#include <System/KMacro.h>

enum
{
	cs_none,		// ��Ч״̬
	cs_stand,		// վ��״̬
	cs_move,		// �ƶ�״̬���ߡ��ܣ�
	cs_death,		// ����״̬
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
	virtual void onMessage(int cmd, const void* data, int len); //����������Ϣ
	virtual void reset();
	virtual void destroy();
};
