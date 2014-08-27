#include "KHeroLevUpResultPanel.h"
#include "assist/KJsonDictMgr.h"
#include "../PlayerCard/KPlayerDeck.h"
#include "../StaticTable/KGameStatic.h"

using namespace cocos2d::extension;


KHeroLevUpResultPanel::KHeroLevUpResultPanel():m_Panel(NULL)
{
}

KHeroLevUpResultPanel::~KHeroLevUpResultPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KHeroLevUpResultPanel::init(cocos2d::extension::UILayer* layer)
{
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/LevUpResult.json");
		CC_SAFE_RETAIN(m_Panel);
		//pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		//pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));
	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	// old_strong_val,strong_to,new_strong_val
	// old_lucky_val,lucky_to,new_lucky_val
	// skill_name,

}

void KHeroLevUpResultPanel::update(float dt)
{
}

void KHeroLevUpResultPanel::Show(const KHeroDef& oldHero,const KHeroDef& newHero)
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);

	char sz[64];
	UILabel* labelOldStrong = (UILabel*)m_Panel->getChildByName("old_strong_val");
	sprintf(sz,"%d",oldHero.GetStrong());
	labelOldStrong->setText(sz);
	UIWidget* strongTo = m_Panel->getChildByName("strong_to");
	UILabel* labelNewStrong = (UILabel*)m_Panel->getChildByName("new_strong_val");
	if(oldHero.GetStrong()==newHero.GetStrong()){
		strongTo->setVisible(false);
		labelNewStrong->setVisible(false);
	}else{
		strongTo->setVisible(true);
		labelNewStrong->setVisible(true);
		sprintf(sz,"%d",newHero.GetStrong());
		labelNewStrong->setText(sz);
	}

	UILabel* labelOldLucky = (UILabel*)m_Panel->getChildByName("old_lucky_val");
	sprintf(sz,"%d",oldHero.GetLucky());
	labelOldLucky->setText(sz);
	UIWidget* luckyTo = m_Panel->getChildByName("lucky_to");
	UILabel* labelNewLucky = (UILabel*)m_Panel->getChildByName("new_lucky_val");
	if(oldHero.GetLucky()==newHero.GetLucky()){
		luckyTo->setVisible(false);
		labelNewLucky->setVisible(false);
	}else{
		luckyTo->setVisible(true);
		labelNewLucky->setVisible(true);
		sprintf(sz,"%d",newHero.GetLucky());
		labelNewLucky->setText(sz);
	}
	const KHeroDef::skillDef* newSkill = NULL;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		if(newHero._skill[i]._skillId>0 && 
			oldHero._skill[i]._skillId==0){
				newSkill = &newHero._skill[i];
				break;
		}
	}
	UILabel* lableSkill = (UILabel*)m_Panel->getChildByName("skill_name");
	if(newSkill){
		lableSkill->setVisible(true);
		KHeroSkillStatic* skillST = KGameStaticMgr::getSingleton().GetHeroSkill(newSkill->_skillId);
		lableSkill->setText(skillST->GetName());
	}else{
		lableSkill->setVisible(false);
	}
}