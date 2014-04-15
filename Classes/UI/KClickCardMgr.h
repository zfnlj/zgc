/********************************************************************
	created:	2011/05/18
	created:	18:5:2011   17:40
	filename: 	i:\SVN\PetsWorkShop\V001\AthenaClient\EngineEx\K3DBase\K3DNode.h
	file path:	i:\SVN\PetsWorkShop\V001\AthenaClient\EngineEx\K3DBase
	file base:	K3DNode
	file ext:	h
	author:		
	
	purpose:	√¸÷–π‹¿Ì∆˜
*********************************************************************/
#pragma once

#include "System/KType.h"
#include "System/Singleton.h"

class KCardActor;

class KClickCardMgr : public Singleton<KClickCardMgr>
{
public:
	KClickCardMgr();
	~KClickCardMgr();
	void init();
	static KClickCardMgr& getSingleton(void);
	void onClickCard(KCardActor*);
	void onTouchEnd();
private:
	KCardActor* m_curActor;
};