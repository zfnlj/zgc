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
#include "../PlayerCard/KHeroCardAssist.h"

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
		m_resultPanel.init(layer);

		for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
			pBut = KUIAssist::GetIndexWidget(m_Panel,"skill_levup_but",i);
			pBut->setTag(i);
			pBut->addPushDownEvent(this, coco_pushselector(KHeroLevUpPanel::DoClickSkillLevUp));
		}

	}
	m_layer = layer;
	m_pHeroWidget = NULL;
	m_pHeroDef = NULL;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(900);
	updatePanel();
}

void KHeroLevUpPanel::UpdateMoney()
{
	UILabelBMFont* moneyLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel, "money");
	int kk=0;
	char sz[64];
	sprintf(sz,"%s",KMainPlayer::RealPlayer()->GetMoney());
	moneyLabel->setText(sz);
}

void KHeroLevUpPanel::ShowHero(KHeroDef* hero)
{
	if(m_pHeroWidget){
		KJsonDictMgr::getSingleton().OnHeroWidgetDestory(m_pHeroWidget);
		m_pHeroWidget->removeFromParent();
	}
	m_pHeroDef = hero;
	
	UIWidget* pLevUpBut = UIHelper::seekWidgetByName(m_Panel, "Lev_up_but");
	if(m_pHeroDef->IsLevMax()){
		KUIAssist::_setButVisible(pLevUpBut,false);
	}else{
		KUIAssist::_setButVisible(pLevUpBut,true);
	}
	m_pHeroWidget = KUICardAssist::_createHero(*hero,true);
	UIWidget* slot = UIHelper::seekWidgetByName(m_Panel,"hero_slot");
	m_pHeroWidget->setPosition(slot->getPosition());
	m_pHeroWidget->setZOrder(100);

	
	UpdateSkillInfo();
	UpdateMoney();
	UpdateHeroLevUpInfo();
	m_Panel->addChild(m_pHeroWidget);

	char sz[64];
	UILabel* strongValLabel = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"strong_val");
	sprintf(sz,"%d",m_pHeroDef->GetStrong());
	strongValLabel->setText(sz);

	UILabel* luckyValLabel = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"lucky_val");
	sprintf(sz,"%d",m_pHeroDef->GetLucky());
	luckyValLabel->setText(sz);
}

void KHeroLevUpPanel::ShowPanel(KHeroDef* hero)
{
	ShowHero(hero);
	m_layer->addWidget(m_Panel);
}

void KHeroLevUpPanel::UpdateHeroLevUpInfo()
{
	UILabel* levUpText = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"hero_levup_txt");
	char sz[64];
	int curChipNum = KPlayerBagAssist::GetPlayerItemNum(KMainPlayer::RealPlayer(),m_pHeroDef->GetCardId()*10);
	int needChipNum = m_pHeroDef->GetLevUpStoneNum();
	if(m_pHeroDef->IsLevMax()){
		sprintf(sz,"%s","max");
	}else{
		sprintf(sz,"%d/%d",curChipNum,needChipNum);
	}
	levUpText->setText(sz);
	UILoadingBar* bar = (UILoadingBar*)UIHelper::seekWidgetByName(m_Panel,"levUp_bar");
	if(curChipNum>needChipNum){
		bar->setPercent(
			100.0f);
	}else{
		bar->setPercent((float)curChipNum*100.0f/(float)needChipNum);
	}
	UpdateHeroAtkPower();
	
}

void KHeroLevUpPanel::UpdateHeroAtkPower()
{
	char sz[64];
	UILabelBMFont* heroPowerWidget = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel,"hero_power_val");
	sprintf(sz,"%d",m_pHeroDef->GetAtkVal());
	heroPowerWidget->setText(sz);
}

void KHeroLevUpPanel::UpdateSkillInfo()
{
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		SetLevUpWidgetsVisible(i,m_pHeroDef->_skill[i]._id>0);
	}
}

void KHeroLevUpPanel::SetLevUpWidgetsVisible(int index,bool bVisible)
{
	const KHeroDef::skillDef& skill = m_pHeroDef->_skill[index];
	UILabel* skillTxt = (UILabel*)KUIAssist::GetIndexWidget(m_Panel,"skill_txt",index);
	skillTxt->setVisible(bVisible);
	if(skill._id>0) skillTxt->setText(skill.GetName());

	char sz[64];
	UILabel* skilLevTxt = (UILabel*)KUIAssist::GetIndexWidget(m_Panel,"skill_lev_txt",index);
	sprintf(sz,"lev.%d",skill._lev);
	skilLevTxt->setVisible(bVisible);
	skilLevTxt->setText(sz);

	UIWidget* skilLevUpBut = KUIAssist::GetIndexWidget(m_Panel,"skill_levup_but",index);
	if(bVisible && !skill.IsMaxLev()){
		KUIAssist::ShowButton(skilLevUpBut,true);
	}else{
		KUIAssist::ShowButton(skilLevUpBut,false);
	}

	UIWidget* goldIcon = KUIAssist::GetIndexWidget(m_Panel,"gold_icon",index);
	if(bVisible && !skill.IsMaxLev()){
		goldIcon->setVisible(true);
	}else{
		goldIcon->setVisible(false);
	}

	int needMoney = m_pHeroDef->GetSkillLevUpMoney(index);
	UILabel* moneyTxt = (UILabel*)KUIAssist::GetIndexWidget(m_Panel,"skill_money_txt",index);
	if(bVisible && !skill.IsMaxLev()){
		moneyTxt->setVisible(true);
	}else{
		moneyTxt->setVisible(false);
	}
	sprintf(sz,"%d",needMoney);
	moneyTxt->setText(sz);
}

void KHeroLevUpPanel::DoClickSkillLevUp(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	KHeroDef::skillDef& skill = m_pHeroDef->_skill[pBut->getTag()];
	int needMoney = m_pHeroDef->GetSkillLevUpMoney(pBut->getTag());
	if(!KMainPlayer::RealPlayer()->HasMoney(needMoney)){
		KPopupLayer::DoModal(UI_WARNING_STR,MONEY_LOW_STR,KPopupLayer::DT_Ok);
		return;
	}
	KHeroCardAssist::_SkillLevUp(m_pHeroDef,pBut->getTag(),KMainPlayer::RealPlayer());

	UpdateMoney();
	UpdateSkillInfo();
	UpdateHeroAtkPower();
}

void KHeroLevUpPanel::DoClickHeroLevUp(CCObject* sender)
{
	int curChipNum = KPlayerBagAssist::GetPlayerItemNum(KMainPlayer::RealPlayer(),m_pHeroDef->GetCardId()*10);
	int needChipNum = m_pHeroDef->GetLevUpStoneNum();

	if(curChipNum<needChipNum){
		KPopupLayer::DoModal(UI_WARNING_STR,CHIP_NO_ENOUGH,KPopupLayer::DT_Ok);
		return;
	}
	KHeroDef oldHero;
	memcpy(&oldHero,m_pHeroDef,sizeof(KHeroDef));
	if(!KHeroCardAssist::_LevUp(m_pHeroDef,KMainPlayer::RealPlayer())) return;
	m_resultPanel.Show(oldHero,*m_pHeroDef);

	ShowHero(m_pHeroDef);
}

void KHeroLevUpPanel::DoClickClose(CCObject* sender)
{
	KJsonDictMgr::getSingleton().OnHeroWidgetDestory(m_pHeroWidget);
	m_pHeroWidget->removeFromParent();
	m_pHeroWidget = NULL;
	m_Panel->removeFromParent();
}

void KHeroLevUpPanel::updatePanel()
{
	
}


void KHeroLevUpPanel::update(float dt)
{
	m_resultPanel.update(dt);
}