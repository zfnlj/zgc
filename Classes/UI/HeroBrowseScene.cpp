#include "HeroBrowseScene.h"
#include "ccMacros.h"
#include "../GameLogic/KDynamicWorld.h"
#include "GameRoot.h"
#include "../Facade/FBattleGuy.h"
#include "KCardActor.h"
#include "UILayer.h"
#include "cocos-ext.h"
#include "../Facade/FBattleGuy.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../GameLogic/KGameDef.h"
#include "assist/KUIAssist.h"
#include "GameRoot.h"
#include "../KNet/KUserData.h"
#include "../KNet/KSocketFacade.h"
#include "BattleFieldScene.h"
#include "VirtualService.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KQuestFacade.h"
#include "assist/KPopupLayer.h"
#include "../WorldObject/KMainPlayer.h"
#include "../common/KUIMsgDef.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "../StaticTable/StaticData.h"
#include "../WorldObject/KPlayer.h"
#include "assist/KUICardAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* HeroBrowseScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HeroBrowseScene *layer = HeroBrowseScene::create();
    // add layer as a child to scene
    scene->addChild(layer,0,1977);
	
    // return the scene
    return scene;
}

void HeroBrowseScene::update(float dt)
{
	KSceneLayerBase::update(dt);
	m_heroLevUpPanel.update(dt);
}

// on "init" you need to initialize your instance
bool HeroBrowseScene::init()
{

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HeroBrowseScene::update),this,0.1f,false);
    //////////////////////////////
   if (!KSceneLayerBase::init())
	{
		return false;
	}

	addWidget(GetPanel());
	m_heroLevUpPanel.init(this);
	m_depot = KMainPlayer::RealPlayer()->GetCardDepot();
	UpdateUI();
	m_actor.init(m_ui);
    return true;
}

cocos2d::extension::UIWidget* HeroBrowseScene::GetPanel()
{
	if(!m_ui){
		m_ui =  KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/heroBrowse.json");
		
		
		UIWidget* pBut = UIHelper::seekWidgetByName(m_ui,"back_but");
		pBut->addPushDownEvent(this,coco_pushselector(HeroBrowseScene::onClickBack));
		
		pBut = UIHelper::seekWidgetByName(m_ui,"page_down_but");
		pBut->addPushDownEvent(this,coco_pushselector(HeroBrowseScene::onClickPageDown));

		pBut = UIHelper::seekWidgetByName(m_ui,"page_up_but");
		pBut->addPushDownEvent(this,coco_pushselector(HeroBrowseScene::onClickPageUp));

		
		m_pPageInfo = (UILabel*)UIHelper::seekWidgetByName(m_ui,"page_info");

		char sz[32];
		for(int i=0;i<PAGE_CARD_NUM;i++){
			

			sprintf(sz,"Levup_but_%d",i);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);
			pBut->setTag(i);
			pBut->addPushDownEvent(this,coco_pushselector(HeroBrowseScene::onClickHeroLevUp));


			sprintf(sz,"break_but_%d",i);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);
			pBut->setTag(i);
			pBut->addPushDownEvent(this,coco_pushselector(HeroBrowseScene::onClickBreakHero));

		}
		m_radioRace.AddGroupBut("check_race",6,m_ui,this,coco_pushselector(HeroBrowseScene::onClickRace),0);
		
		m_curCardGroup = m_curPage = 0;
		m_depot = NULL;

	}
	return m_ui;
}

void HeroBrowseScene::onClickRace(CCObject* sender)
{
	m_curPage = 0;
	UpdateUI();
}

void HeroBrowseScene::onClickCost(CCObject* sender)
{
	m_curPage = 0;
	UpdateUI();
}

void HeroBrowseScene::onClickBack(CCObject* sender)
{
	m_curPage = 0;
	KUIAssist::_switch2MainMenu();
}

void HeroBrowseScene::UpdateUI()
{
	KCardGroupAssist::ClearSlotElem(m_slotElem,PAGE_CARD_NUM);

	m_radioRace.SetVisible(true);
	ShowHeroCards();
	UpdateHeroLevUpBut();
}

void HeroBrowseScene::UpdatePageInfo(int moreNum)
{
	int totoal = m_curPage*PAGE_CARD_NUM +moreNum;
	int totalPage = totoal/PAGE_CARD_NUM;
	if(totalPage*PAGE_CARD_NUM<totoal) totalPage++;
	m_pPageInfo->setVisible(totoal>PAGE_CARD_NUM);
	char sz[64];
	sprintf(sz,"%d-%d",totalPage,m_curPage+1);
	m_pPageInfo->setText(sz);
	UIWidget* pPageUp = UIHelper::seekWidgetByName(m_ui,"page_up_but");
	KUIAssist::_setButVisible(pPageUp,m_curPage>0);
	UIWidget* pPageDown = UIHelper::seekWidgetByName(m_ui,"page_down_but");
	KUIAssist::_setButVisible(pPageDown, (m_curPage+1 !=totalPage) &&(totalPage>0));
}

void HeroBrowseScene::ShowHeroCards()
{
	int heroNum = m_depot->GetHeroNum();
	KHeroDefList heroLst,tmpLst;
	m_depot->PickAllHero(heroLst);
	
	KCardGroupAssist::FilterHero(heroLst,tmpLst,m_radioRace.GetSelectVal(),m_curPage*PAGE_CARD_NUM);
	UpdatePageInfo(tmpLst.size());

	int curPos = 0;
	for(KHeroDefList::iterator it=tmpLst.begin();it!=tmpLst.end();++it){
		KHeroDef* pHeroDef = *it;
		UIWidget* widget = KUICardAssist::_createHero(*pHeroDef,true,NULL,true);
		if(!widget) continue;

		this->addWidget(widget);
		char sz[64];
		sprintf(sz,"%s_%d","card_slot",curPos);

		UIImageView* widgetPos =(UIImageView*)UIHelper::seekWidgetByName(m_ui,sz);
		widget->setPosition(widgetPos->getPosition());
		widget->setTouchEnable(false);
		KCardGroupAssist::SetSlotElem(&m_slotElem[curPos++],pHeroDef->_id,KCardGroupSlotElem::elem_hero,widget);

	}
}

void HeroBrowseScene::onClickHeroLevUp(CCObject* sender)
{
	UIWidget* widget = (UIWidget*)sender;
	KHeroDef* pHeroDef = m_depot->FindHero(m_slotElem[widget->getTag()]._id);
	
	m_heroLevUpPanel.ShowPanel(pHeroDef);
}

void HeroBrowseScene::UpdateHeroLevUpBut()
{
	for(int i=0;i<PAGE_CARD_NUM;i++){
		UIWidget* pBut = KUIAssist::GetIndexWidget(m_ui,"Levup_but",i);
  		UIWidget* pSlotImage = KUIAssist::GetIndexWidget(m_ui,"card_slot",i);
		UIWidget* pBreakBut = KUIAssist::GetIndexWidget(m_ui,"break_but",i);
		pBut->setTag(i);
		pBreakBut->setTag(i);
		 if(m_slotElem[i]._widget){
			 KUIAssist::ShowButton(pBut,true);
			 KUIAssist::ShowButton(pBreakBut,true);
			 pSlotImage->setVisible(true);
		 }else{
			 KUIAssist::ShowButton(pBut,false);
			 KUIAssist::ShowButton(pBreakBut,false);
			 pSlotImage->setVisible(false);
		 }
	}
}

void HeroBrowseScene::onClickPageDown(CCObject* sender)
{
	m_curPage++;
	UpdateUI();
}

void HeroBrowseScene::onClickPageUp(CCObject* sender)
{
	if(m_curPage>0) m_curPage--;
	UpdateUI();
}

void HeroBrowseScene::DoBreakHero(CCObject* sender)
{
	int stoneNum = m_depot->BreakHero(m_slotElem[KPopupLayer::m_val]._id);
	if(stoneNum>0){
		KPopupLayer::DoModal(UI_NOTIFY_STR,CAPTURE_HERO_CHIP,stoneNum,KPopupLayer::DT_Ok);
	}
	UpdateUI();
}

void HeroBrowseScene::onClickBreakHero(CCObject* sender)
{
	UIWidget* widget = (UIWidget*)sender;
	KPopupLayer::m_val = widget->getTag();
	KPopupLayer::DoModal(UI_WARNING_STR,NOTIFY_DEL_HERO,KPopupLayer::DT_Yes_No,coco_pushselector(HeroBrowseScene::DoBreakHero),this);
}
