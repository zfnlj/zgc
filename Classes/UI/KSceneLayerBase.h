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
	enum Scene_Def{
		scene_null,
		scene_battle,
		scene_cardGroup,
		scene_heroBrowse,
		scene_shop,
		scene_mainMenu,
		scene_stageSelect,
	};
	KSceneLayerBase(){}
	~KSceneLayerBase();
	
	virtual void update(float dt);
	virtual bool init();
	KActor& GetActor(){ return m_actor;}
	KGameRecordPanel& RecordPanel(){ return m_gameRecPanel;}
	virtual void onGenPlayerCard(unsigned long long p1){}
	virtual void onSystemMsg(int id);
	virtual void onIAPCallback(const char* productName,int count);
	void CreateCloseBut();
	void menuCloseCallback(CCObject* pSender);
	virtual void onCloseCallback(CCObject* sender)=0;
protected:
	cocos2d::extension::UIWidget* m_ui;
	KActor m_actor;
	KGameRecordPanel m_gameRecPanel;
	Scene_Def m_sceneDef;
};

#endif // __BattleFieldScene_SCENE_H__
