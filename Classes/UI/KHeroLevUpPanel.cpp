#include "KHeroLevUpPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "assist/KUIAssist.h"
#include "../VirtualService.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "../Quest/KQuestManager.h"
#include "assist/KQuestFacade.h"
#include "../Item/KItemCreate.h"
#include "../WorldObject/KPlayer.h"
#include "assist/KJsonDictMgr.h"
#include "../common/KPlayerBagAssist.h"
#include "assist/KUICardAssist.h"
#include "assist/KPopupLayer.h"
#include "../common/KUIMsgDef.h"

using namespace cocos2d::extension;
using namespace KItemAbout;

KHeroLevUpPanel::KHeroLevUpPanel():m_Panel(NULL),m_pHeroDef(NULL),m_pHeroWidget(NULL)
{
}

KHeroLevUpPanel::~KHeroLevUpPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KHeroLevUpPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/HeroLevUp.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "close_but");
		pBut->addPushDownEvent(this, coco_pushselector(KHeroLevUpPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "Lev_up_but");
		pBut->addPushDownEvent(this, coco_pushselector(KHeroLevUpPanel::DoClickHeroLevUp));
		

	}
	m_layer = layer;
	m_pHeroWidget = NULL;
	m_pHeroDef = NULL;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	updatePanel();
}

void KHeroLevUpPanel::ShowPanel(const KHeroDef* hero)
{
	m_pHeroDef = hero;
	m_layer->addWidget(m_Panel);

	m_pHeroWidget = KUICardAssist::_createHero(*hero,true);
	UIWidget* slot = UIHelper::seekWidgetByName(m_Panel,"hero_slot");
	m_pHeroWidget->setPosition(slot->getPosition());
	m_pHeroWidget->setZOrder(100);
	m_Panel->addChild(m_pHeroWidget);

	for(int i=0;i<MAX_HERO_LEV_INDEX;i++){
		SetLevUpWidgetsVisible(i,i<m_pHeroDef->GetLev());
	}
	UpdateHeroLevUpInfo();

}

void KHeroLevUpPanel::UpdateHeroLevUpInfo()
{
	UILabel* levUpText = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"hero_levup_txt");
	char sz[64];
	int curChipNum = KPlayerBagAssist::GetPlayerItemNum(KMainPlayer::RealPlayer(),m_pHeroDef->GetCardId()*10);
	int needChipNum = m_pHeroDef->GetLevUpStoneNum();
	sprintf(sz,"%d/%d",curChipNum,needChipNum);
	levUpText->setText(sz);
	UILoadingBar* bar = (UILoadingBar*)UIHelper::seekWidgetByName(m_Panel,"levUp_bar");
	bar->setPercent((float)curChipNum*100.0f/(float)needChipNum);
}

void KHeroLevUpPanel::SetLevUpWidgetsVisible(int index,bool bVisible)
{
	UIWidget* skilTxt = KUIAssist::GetIndexWidget(m_Panel,"skill_txt",index);
	skilTxt->setVisible(bVisible);

	UIWidget* skilLevTxt = KUIAssist::GetIndexWidget(m_Panel,"skill_lev_txt",index);
	skilLevTxt->setVisible(bVisible);

	UIWidget* skilLevUpBut = KUIAssist::GetIndexWidget(m_Panel,"skill_levup_but",index);
	KUIAssist::ShowButton(skilLevUpBut,bVisible);

	UIWidget* goldIcon = KUIAssist::GetIndexWidget(m_Panel,"gold_icon",index);
	goldIcon->setVisible(bVisible);

	UIWidget* moneyTxt = KUIAssist::GetIndexWidget(m_Panel,"skill_money_txt",index);
	moneyTxt->setVisible(bVisible);
}

void KHeroLevUpPanel::DoClickHeroLevUp(CCObject* sender)
{
	int curChipNum = KPlayerBagAssist::GetPlayerItemNum(KMainPlayer::RealPlayer(),m_pHeroDef->GetCardId()*10);
	int needChipNum = m_pHeroDef->GetLevUpStoneNum();

	if(curChipNum<needChipNum){
		KPopupLayer::DoModal(UI_WARNING_STR,CHIP_NO_ENOUGH,KPopupLayer::DT_Ok);
		return;
	}
	if(m_pHeroDef->GetLev()==4) return;

}

void KHeroLevUpPanel::DoClickClose(CCObject* sender)
{
	m_pHeroWidget->removeFromParent();
	m_Panel->removeFromParent();
	
	//KUIAssist::_switch2MainMenu();

	/*   CCPoint pt(m_pt.x,m_pt.y+600);
	switch(m_panelType){
	case enum_panel_task:
	{

	KUIAssist::_moveOutWidget(m_Panel,m_pt,pt,0.8f);
	}
	break;
	case enum_panel_game:
	{
	m_Panel->setVisible(false);
	KUIAssist::_switch2MainMenu();
	}
	break;
	default:
	break;
	}
	*/
}

void KHeroLevUpPanel::updatePanel()
{
	
}


void KHeroLevUpPanel::update(float dt)
{
}