#ifndef _KSCENELAYERBASE_H
#define _KSCENELAYERBASE_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KActor.h"
#include "KGameRecordPanel.h"

USING_NS_CC;

class KSceneLayerBase :public cocos2d::extension::UILayer
{
public:
	KSceneLayerBase(){}
	~KSceneLayerBase();
	
	virtual void update(float dt);
	virtual bool init();
	KActor& GetActor(){ return m_actor;}
	KGameRecordPanel& RecordPanel(){ return m_gameRecPanel;}
	virtual void onGenPlayerCard(unsigned long long p1){}
	virtual void onSystemMsg(int id){}
protected:
	cocos2d::extension::UIWidget* m_ui;
	KActor m_actor;
	KGameRecordPanel m_gameRecPanel;
};

#endif // __BattleFieldScene_SCENE_H__
