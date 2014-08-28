#include "KHeroLevUpResultPanel.h"
#include "assist/KJsonDictMgr.h"

#include "../StaticTable/KGameStatic.h"
#include "assist/KUIAssist.h"

using namespace cocos2d::extension;


KHeroLevUpResultPanel::KHeroLevUpResultPanel():m_Panel(NULL),m_elapsed(0.0f),m_pNewSkill(NULL),m_bPlay(false)
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
	m_elapsed = 0.0f;
	m_pNewSkill = NULL;
	m_bPlay = false;
	// old_strong_val,strong_to,new_strong_val
	// old_lucky_val,lucky_to,new_lucky_val
	// skill_name,

}

void KHeroLevUpResultPanel::update(float dt)
{
	m_elapsed += dt;
	if(m_elapsed<4.0f && m_bPlay){
		if(m_elapsed >0.6f){
			UIWidget* strongTo = m_Panel->getChildByName("strong_to");
			if(m_newHero.GetStrong()>m_oldHero.GetStrong()){
				strongTo->setVisible(true);
			}
			UIWidget* luckyTo = m_Panel->getChildByName("lucky_to");
			if(m_newHero.GetLucky()>m_oldHero.GetLucky()){
				luckyTo->setVisible(true);
			}
		}
		if(m_elapsed >1.2f){
			UILabel* labelNewStrong = (UILabel*)m_Panel->getChildByName("new_strong_val");
			if(m_newHero.GetStrong()>m_oldHero.GetStrong()){
				labelNewStrong->setVisible(true);
			}
			UILabel* labelNewLucky = (UILabel*)m_Panel->getChildByName("new_lucky_val");
			if(m_newHero.GetLucky()>m_oldHero.GetLucky()){
				labelNewLucky->setVisible(true);
			}
		}
		if(m_elapsed >1.8f){
			if(m_pNewSkill){
				UILabel* lableSkill = (UILabel*)m_Panel->getChildByName("skill_name");
				lableSkill->setVisible(true);
			}
		}

		int starNum = m_newHero.GetLev();
		float interval = 2.0f / (float)m_newHero.GetLev() ;
		int curStarNum = (m_elapsed>=2.0f)?m_newHero.GetLev(): m_elapsed/interval;
		for(int i=0;i<MAX_HERO_LEV_NUM;i++){
			UIWidget* widgetStar = KUIAssist::GetIndexWidget(m_Panel,"star",i);
			if(i<curStarNum){
				widgetStar->setVisible(true);
			}else{
				widgetStar->setVisible(false);
			}
		}

	}
}

void KHeroLevUpResultPanel::DoClose(float dt)
{
	m_bPlay =false;
	m_Panel->removeFromParent();
}


void KHeroLevUpResultPanel::Show(const KHeroDef& oldHero,const KHeroDef& newHero)
{
	m_bPlay = true;
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_oldHero = oldHero;
	m_newHero = newHero;
	m_elapsed = 0.0f;
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(KHeroLevUpResultPanel::DoClose),this,4,0,0.0f,false);

	char sz[64];
	UILabel* labelOldStrong = (UILabel*)m_Panel->getChildByName("old_strong_val");
	sprintf(sz,"%d",oldHero.GetStrong());
	labelOldStrong->setText(sz);
	UIWidget* strongTo = m_Panel->getChildByName("strong_to");
	UILabel* labelNewStrong = (UILabel*)m_Panel->getChildByName("new_strong_val");
	strongTo->setVisible(false);
	labelNewStrong->setVisible(false);
	sprintf(sz,"%d",newHero.GetStrong());
	labelNewStrong->setText(sz);
	

	UILabel* labelOldLucky = (UILabel*)m_Panel->getChildByName("old_lucky_val");
	sprintf(sz,"%d",oldHero.GetLucky());
	labelOldLucky->setText(sz);
	UIWidget* luckyTo = m_Panel->getChildByName("lucky_to");
	UILabel* labelNewLucky = (UILabel*)m_Panel->getChildByName("new_lucky_val");
	sprintf(sz,"%d",newHero.GetLucky());
	labelNewLucky->setText(sz);
	luckyTo->setVisible(false);
	labelNewLucky->setVisible(false);

	m_pNewSkill = NULL;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		if(newHero._skill[i]._skillId>0 && 
			oldHero._skill[i]._skillId==0){
				m_pNewSkill = &newHero._skill[i];
				break;
		}
	}
	UILabel* lableSkill = (UILabel*)m_Panel->getChildByName("skill_name");
	lableSkill->setVisible(false);
	if(m_pNewSkill){
		lableSkill->setText(m_pNewSkill->GetName());
	}

	for(int i=0;i<MAX_HERO_LEV_NUM;i++){
		UIWidget* widgetStar = KUIAssist::GetIndexWidget(m_Panel,"star",i);
		widgetStar->setVisible(false);
	}
}