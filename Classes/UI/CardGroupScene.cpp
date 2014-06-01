#include "CardGroupScene.h"
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
#include "../WorldObject/KMainPlayer.h"

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* CardGroupScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardGroupScene *layer = CardGroupScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

void CardGroupScene::update(float dt)
{
}

// on "init" you need to initialize your instance
bool CardGroupScene::init()
{
	//CCDirector::sharedDirector()->getRunningScene
	m_ui = NULL;

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CardGroupScene::update),this,0.1f,false);
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


	this->addChild(GetUILayer(), 1);

	memset(m_slotElem,0,sizeof(m_slotElem));
	m_depot = KMainPlayer::RealPlayer()->GetCardDepot();
	CreateMiniCardList(m_depot->GetCurDeck());
	ShowMiniCardList();
	UpdateUI();
    return true;
}


cocos2d::extension::UILayer* CardGroupScene::GetUILayer()
{
	if(!m_ui){
		m_ui =cocos2d::extension::UILayer::create();
		m_ui->addWidget(KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/cardGroup.json"));
		//m_ui->addWidget(GUIReader::shareReader()->widgetFromJsonFile("GUI/MainMenu.json"));
		m_radioMain.AddBut((UICheckBox*)m_ui->getWidgetByName("check_hero"),
						this,coco_pushselector(CardGroupScene::onClickCheckHero));

		m_radioMain.AddBut((UICheckBox*)m_ui->getWidgetByName("check_soldier"),
						this,coco_pushselector(CardGroupScene::onClickCheckSoldier));

		m_radioMain.AddBut((UICheckBox*)m_ui->getWidgetByName("check_skill"),
						this,coco_pushselector(CardGroupScene::onClickCheckSkill));

		UIWidget* pBut = m_ui->getWidgetByName("back_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickBack));
		
		char sz[32];
		for(int i=0;i<PAGE_CARD_NUM;i++){
			sprintf(sz,"card_slot_%d",i);
			pBut = m_ui->getWidgetByName(sz);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlot));
		}
		m_mainType = type_cardgroup;
		m_curPage = 0;
	}
	return m_ui;
}

void CardGroupScene::onClickBack(CCObject* sender)
{
	if(m_mainType== type_card){
		m_mainType = type_cardgroup;
		UpdateUI();
	}else if(m_mainType==type_cardgroup){
		KUIAssist::_switch2MainMenu();
	}
}

void CardGroupScene::onClickCheckHero(CCObject* sender)
{
	int kk=0;
}

void CardGroupScene::onClickCheckSoldier(CCObject* sender)
{
	if(m_browseType==KCardGroupAssist::browse_soldier) return;
	m_browseType = KCardGroupAssist::browse_soldier;
	UpdateUI();
}

void CardGroupScene::onClickCheckSkill(CCObject* sender)
{
	if(m_browseType==KCardGroupAssist::browse_skill) return;
	m_browseType = KCardGroupAssist::browse_skill;
	UpdateUI();
}

void CardGroupScene::CreateMiniCardList(int index)
{
	if(index<0) return;
	KIntegerList cardList;
	KHeroDef heroDef;
	m_depot->GetCardDeck(index,cardList,heroDef);
	m_miniCardList.clear();
	KCardGroupAssist::SortCardGroup(cardList,m_miniCardList);
}

void CardGroupScene::ShowMiniCardList()
{
	UIWidget* slot = m_ui->getWidgetByName("mini_card_pos");
	CCPoint pt = slot->getPosition();
	for(KItemUnitList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KDBBagItemUnit& elem = *it;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(elem._id);
		if(!pST) continue;
		UIWidget* pWidget = KUIAssist::_createMiniCard(pST,elem._count);
		pWidget->setPosition(pt);
		m_ui->addWidget(pWidget);
		pt.y -= 32;
	}
}

void CardGroupScene::HideNewGroupIcon()
{
	for(int i=0;i<PAGE_CARD_NUM;i++){
		char sz[24];
		sprintf(sz,"card_slot_%d",i);
		UIWidget* pBut = m_ui->getWidgetByName(sz);
		pBut->setVisible(false);
	}
}

void CardGroupScene::UpdateUI()
{
	HideNewGroupIcon();
	KUIAssist::ShowWidgetArr(m_ui,"slot_txt", PAGE_CARD_NUM,false);
	KCardGroupAssist::ClearSlotElem(m_slotElem,PAGE_CARD_NUM);
	if(m_mainType==type_cardgroup){
		m_radioMain.SetVisible(false);
		ShowCardGroup();
	}else if(m_mainType==type_card){
		m_radioMain.SetVisible(true);
		ShowCardBrowse();
	}
}

void CardGroupScene::ShowCardBrowse()
{

	KItemUnitList tmpList,desList;
	m_depot->PickStoreCard(tmpList);

	KCardGroupAssist::FilterCard(tmpList,desList,m_browseType,m_curPage*PAGE_CARD_NUM);
	int showPos = 0;
	for(KItemUnitList::iterator it=desList.begin();it!=desList.end();++it){
		if(showPos==PAGE_CARD_NUM) break;
		KDBBagItemUnit& unit = *it;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(unit._id);
		UIWidget* widget = KUIAssist::_createCardLayout(pST,true);
		char sz[64];
		sprintf(sz,"%s_%d","card_slot",showPos);

		UIImageView* widgetPos =(UIImageView*)m_ui->getWidgetByName(sz);
		widget->setPosition(widgetPos->getPosition());

		int deckCardCount = m_depot->GetDeckCardNum(m_curCardGroup,unit._id);
		sprintf(sz,"%s_%d","slot_txt",showPos);
		UILabelBMFont* slotTxt = (UILabelBMFont*)m_ui->getWidgetByName(sz);
		if(deckCardCount>0){
			char str[64];
			sprintf(str,"X  %d",deckCardCount);
			slotTxt->setText(str);
			slotTxt->setVisible(true);
		}

		KCardGroupAssist::SetSlotElem(&m_slotElem[showPos++],unit._id,KCardGroupSlotElem::elem_card,widget);
		m_ui->addWidget(widget);
	}
}

void CardGroupScene::ShowCardGroup()
{
	int curPos = 0;
	for(int i=0;i<MAX_DECK_NUM;i++){
		KIntegerList tmpList;
		KHeroDef hero;
		m_depot->GetCardDeck(i,tmpList,hero);
		UIWidget* widget = NULL;
		if(tmpList.size()>0 &&hero._heroId>0){
			KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(hero._heroId);
			widget = KUIAssist::_createCardLayout(pST,true);
			KUIAssist::_showHeroSkill(widget,hero);
		}
		if(widget){
			m_ui->addWidget(widget);
			char sz[64];
			sprintf(sz,"%s_%d","card_slot",curPos);

			UIImageView* widgetPos =(UIImageView*)m_ui->getWidgetByName(sz);
			widget->setPosition(widgetPos->getPosition());
			widget->setTouchEnable(false);
			UIImageView* pCardCenter = (UIImageView*)UIHelper::seekWidgetByName(widget,"card_center");
			pCardCenter->loadTexture("card_edit_txt.png",UI_TEX_TYPE_PLIST);
			pCardCenter->setScale(0.8f);
			pCardCenter->setVisible(true);
			KCardGroupAssist::SetSlotElem(&m_slotElem[curPos++],i,KCardGroupSlotElem::elem_card_group,widget);
		}
	}
	if(curPos<MAX_DECK_NUM){
		char sz[64];
		sprintf(sz,"%s_%d","card_slot",curPos++);
		UIImageView* pNewGroup = (UIImageView*)m_ui->getWidgetByName(sz);
		pNewGroup->setVisible(true);
	}
}

void CardGroupScene::onClickSlot(CCObject* sender)
{
	UIImageView* pWidgetSlot = (UIImageView*)sender;
	if(m_mainType==type_cardgroup){
		if(pWidgetSlot->isVisible()){
			return onClickNewCardGroup(sender);
		}else if(m_slotElem[pWidgetSlot->getTag()]._widget){
			return onClickCardGroup(pWidgetSlot->getTag());
		}
	}
}

void CardGroupScene::onClickCardGroup(int index)
{
	m_curCardGroup = m_slotElem[index]._id;
	m_curPage = 0;
	m_mainType = type_card;
	m_browseType = KCardGroupAssist::browse_all;
	UpdateUI();
}

void CardGroupScene::onClickNewCardGroup(CCObject* sender)
{

}