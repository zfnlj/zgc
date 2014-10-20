#ifndef _KShopScene_H
#define _KShopScene_H

#include "cocos2d.h"
#include "UILayer.h"
#include "KSceneLayerBase.h"
#include "KGenCardPanel.h"

USING_NS_CC;

namespace KStoreAbout{
	class KStoreCreateInfo;
};

class KShopScene :public KSceneLayerBase
{
public:
	KShopScene();
	~KShopScene();

	static cocos2d::CCScene* scene();
	virtual bool init();  

	CREATE_FUNC(KShopScene);
	virtual void update(float dt);
	void DoClickClose(CCObject* sender);
	void UpdateMoney();
	void UpdateNormalBag();
	virtual void onGenPlayerCard(unsigned long long p1);
	virtual void onSystemMsg(int id);
	void DoBuyMoney(CCObject* sender);
	virtual void onCloseCallback();
	virtual void onIAPCallback(const char* productName,int count);
private:
	void InitItem();
	
	void DoClickBuy(CCObject* sender);
	void onClickBuy(CCObject* sender);
	void DoClickUseItem(CCObject* sender);
	void BuyProduct(int productId);
	KStoreAbout::KStoreCreateInfo* m_store;
	int m_buyProduct;
	KGenCardPanel m_genCardPanel;
	
};

#endif // __BattleFieldScene_SCENE_H__
