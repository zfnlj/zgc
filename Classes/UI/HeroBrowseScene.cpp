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
	UpdateCurDeckRadio();
	m_gameRecPanel.init(this);
	m_actor.init(m_ui);
    return true;
}

cocos2d::extension::UIWidget* HeroBrowseScene::GetPanel()
{
	if(!m_ui){
		m_ui =  KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/heroBrowse.json");
		
		
		m_radioMain.AddGroupBut("check_main",3,m_ui,this,coco_pushselector(HeroBrowseScene::onClickMainType),1);

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
		m_radioSelectHero.SetUnSelectAble(false);
		m_radioRace.AddGroupBut("check_race",6,m_ui,this,coco_pushselector(HeroBrowseScene::onClickRace),0);
		for(int j=1;j<8;j++){
			sprintf(sz,"check_cost_%d",j);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);

			char val[18];
			sprintf(val,"%d",j);
			CCLabelBMFont* bmFont = CCLabelBMFont::create(val,"GUI/num_1.fnt");
			bmFont->setAnchorPoint(ccp(0.50f,0.50f));
			pBut->addCCNode(bmFont);
		}
		m_mainType = type_cardgroup;
		m_curCardGroup = m_curPage = 0;
		m_miniHero.Clear();
		m_depot = NULL;

	}
	return m_ui;
}

void HeroBrowseScene::onClickMainType(CCObject* sender)
{
	m_curPage = 0;
	UpdateUI();
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

void HeroBrowseScene::UpdateCurDeckRadio()
{
	if(m_mainType==type_cardgroup){
		for(int i=0;i<PAGE_CARD_NUM;i++){
			if(m_slotElem[i]._id==m_depot->GetCurDeck()){
				m_radioSelectHero.SetSelected(i,false);
				break;
			}
		}
	}
}

void HeroBrowseScene::onClickBack(CCObject* sender)
{
	m_curPage = 0;
	m_radioSelectHero.SetSelected(-1,false);
	KUIAssist::_switch2MainMenu();
}

void HeroBrowseScene::UpdateUI()
{
	KUIAssist::ShowWidgetArr(this,"slot_txt", PAGE_CARD_NUM,false);
	KCardGroupAssist::ClearSlotElem(m_slotElem,PAGE_CARD_NUM);

	m_radioMain.SetVisible(true);
	m_radioRace.SetVisible(true);
	m_radioSelectHero.SetUnSelectAble(true);
	ShowAllHero();
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

void HeroBrowseScene::ShowAllHero()
{
	int heroNum = m_depot->GetHeroNum();

	UpdatePageInfo(heroNum);
	int curPos = 0;
	for(int i=0;i<heroNum;i++){
		const KHeroDef*  pHeroDef = m_depot->FindHeroOnIndex(i);
		UIWidget* widget = KUICardAssist::_createHero(*pHeroDef,true,NULL,true);
		if(!widget) continue;

		this->addWidget(widget);
		char sz[64];
		sprintf(sz,"%s_%d","card_slot",curPos);

		UIImageView* widgetPos =(UIImageView*)UIHelper::seekWidgetByName(m_ui,sz);
		widget->setPosition(widgetPos->getPosition());
		widget->setTouchEnable(false);
		//if(pHeroDef->_id==m_miniHero._id) 	m_radioSelectHero.SetSelected(curPos,false);
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

		 if(m_slotElem[i]._widget){
			 KUIAssist::ShowButton(pBut,true);
		 }else{
			 KUIAssist::ShowButton(pBut,false);
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

void HeroBrowseScene::onClickBreakHero(CCObject* sender)
{

}
