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
	m_myResFont = m_yourResFont = NULL;
	m_layer = layer;
	UpdateRes();
}

void KResPanel::UpdateRes()
{
	char msg[64]={0};
	FBattleGuy* pMainPlayer = GameRoot::getSingleton().BattleCtrl().GetMainPlayer();
	if(!pMainPlayer) return;
	pMainPlayer->QueryResInfo(msg);

	if(!m_myResFont){
		m_myResFont = CCLabelBMFont::create(msg,"GUI/num_res.fnt");
		m_myResFont->setAnchorPoint(ccp(0.50f,0.50f));
		UIWidget* resPanel = m_layer->getWidgetByName("my_res");
		m_myResFont->setPosition(ccp(0,-6));
		m_myResFont->setScale(0.8f);
		resPanel->addCCNode(m_myResFont);
	}else{
		m_myResFont->setString(msg);
	}

	FBattleGuy* pOtherPlayer = GameRoot::getSingleton().BattleCtrl().GetOtherPlayer();
	if(!pOtherPlayer) return;
	pOtherPlayer->QueryResInfo(msg);
	if(!m_yourResFont){
		m_yourResFont = CCLabelBMFont::create(msg,"GUI/num_res.fnt");
		m_yourResFont->setAnchorPoint(ccp(0.50f,0.50f));
		UIWidget* resPanel = m_layer->getWidgetByName("your_res");
		m_yourResFont->setPosition(ccp(0,-6));
		m_yourResFont->setScale(0.8f);
		resPanel->addCCNode(m_yourResFont);
	}else{
		m_yourResFont->setString(msg);
	}
}
