#include "KResPanel.h"

#include "cocos-ext.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "KUIAssist.h"
#include "GameRoot.h"

using namespace cocos2d::extension;

KResPanel::KResPanel()
{
}

void KResPanel::init(cocos2d::extension::UILayer* layer)
{
	m_layer = layer;
	UpdateRes();
}

void KResPanel::UpdateRes()
{
	char msg[64]={0};
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	pMainPlayer->QueryResInfo(msg);

	cocos2d::extension::UILabelAtlas* label = (cocos2d::extension::UILabelAtlas*)m_layer->getWidgetByName("my_res");
	label->setVisible(true);
	label->setStringValue(msg);

	FBattleGuy* pOtherPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();
	pOtherPlayer->QueryResInfo(msg);
	label = (cocos2d::extension::UILabelAtlas*)m_layer->getWidgetByName("your_res");
	label->setVisible(true);
	label->setStringValue(msg);
}
