#include "KGameTipPanel.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "assist/KJsonDictMgr.h"
#include "../StaticTable/StaticData.h"
#include "StageWaitScene.h"
#include "assist/KQuestFacade.h"

using namespace cocos2d::extension;

#define SELECT_ITEM_TAG_BASE 10000
KGameTipPanel::KGameTipPanel():m_Panel(NULL)
{
}

KGameTipPanel::~KGameTipPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGameTipPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/tipPanel.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "but_ok");
		pBut->addPushDownEvent(this, coco_pushselector(KGameTipPanel::DoClickClose));
		
	}
	m_layer = layer;
	m_Panel->setZOrder(999);
	UpdatePanel();
}

void KGameTipPanel::UpdatePanel()
{
}

void KGameTipPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1f);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3f, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());

	UILabel* pLabelTip = (UILabel*)UIHelper::seekWidgetByName(m_Panel, "Label_info");
	pLabelTip->setVisible(false);
	pLabelTip = (UILabel*)UIHelper::seekWidgetByName(m_Panel, "Label_info2");
	pLabelTip->setVisible(false);
	
	char icon_name[256];
	sprintf(icon_name,"icon//lesson_%d.jpg",StageWaitScene::m_qId);
	UIImageView* widgetTitle = (UIImageView*)UIHelper::seekWidgetByName(m_Panel,"lesson_icon");
	if(widgetTitle&& CCFileUtils::sharedFileUtils()->isFileExist(icon_name)) widgetTitle->loadTexture(icon_name);


	pLabelTip = (UILabel*)UIHelper::seekWidgetByName(m_Panel, (CCFileUtils::sharedFileUtils()->isFileExist(icon_name))?"Label_info":"Label_info2");
	KHelpStringStatic* pHelpString = KGameStaticMgr::getSingleton().GetLessonTip(StageWaitScene::m_qId);
	if(pHelpString){
		pLabelTip->setVisible(true);
		pLabelTip->setText(pHelpString->GetString());
	}

}


void KGameTipPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();


	if(StageWaitScene::m_sceneType==scene_battle){
		KQuestFacade::_startBattle();
	}else if(StageWaitScene::m_sceneType==scene_daily){
		KQuestFacade::_startDaily(StageWaitScene::m_qId);
	}else if(StageWaitScene::m_sceneType==scene_tower){
		KQuestFacade::_startTower(StageWaitScene::m_qId);
	}
}

