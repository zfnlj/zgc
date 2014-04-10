#ifndef _KSHOPPANEL_H
#define _KSHOPPANEL_H

#include "cocos2d.h"
#include "UILayer.h"

USING_NS_CC;

namespace KStoreAbout{
	class KStoreCreateInfo;
};

class KShopPanel :public CCObject
{
public:
	KShopPanel();
	~KShopPanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
	void UpdateMoney();
	void UpdateNormalBag();
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;

	void InitItem();
	
	void DoClickBuy(CCObject* sender);
	void DoClickUseItem(CCObject* sender);
	KStoreAbout::KStoreCreateInfo* m_store;
};

#endif // __BattleFieldScene_SCENE_H__
