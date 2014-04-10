/********************************************************************
	created:	2011/05/18
	created:	18:5:2011   17:40
	filename: 	i:\SVN\PetsWorkShop\V001\AthenaClient\EngineEx\K3DBase\K3DNode.h
	file path:	i:\SVN\PetsWorkShop\V001\AthenaClient\EngineEx\K3DBase
	file base:	K3DNode
	file ext:	h
	author:		
	
	purpose:	命中管理器
*********************************************************************/
#pragma once

#include "System/KType.h"
#include "System/Singleton.h"
#include "../KAction/K3DActionParam.h"

#define MAX_On_HIT_NUM 20
struct K3DActionParam;

class KActionMgr;


class KOnHitMgr : public Singleton<KOnHitMgr>
{
	K3DActionParam m_onHitArr[MAX_On_HIT_NUM];
public:
	KOnHitMgr();
	~KOnHitMgr();
	void init();
	static KOnHitMgr& getSingleton(void);
	bool AddOnHitMissile(K3DActionParam*); //增加子弹命中消息，如不在m_atkList中，则返回false
	void OnMissileOnHit(int id);
};