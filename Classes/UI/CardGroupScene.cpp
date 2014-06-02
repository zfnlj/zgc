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

		m_radioMain.AddGroupBut("check_main",3,m_ui,this,coco_pushselector(CardGroupScene::onClickMainType),1);

		UIWidget* pBut = m_ui->getWidgetByName("back_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickBack));
		
		pBut = m_ui->getWidgetByName("page_down_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickPageDown));

		pBut = m_ui->getWidgetByName("page_up_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickPageUp));

		m_pPageInfo = (UILabel*)m_ui->getWidgetByName("page_info");

		char sz[32];
		for(int i=0;i<PAGE_CARD_NUM;i++){
			sprintf(sz,"card_slot_%d",i);
			pBut = m_ui->getWidgetByName(sz);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlot));


			sprintf(sz,"slot_add_%d",i);
			pBut = m_ui->getWidgetByName(sz);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlotAdd));

			sprintf(sz,"slot_sub_%d",i);
			pBut = m_ui->getWidgetByName(sz);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlotSub));
		}


		m_radioCost.AddGroupBut("check_cost",8,m_ui,this,coco_pushselector(CardGroupScene::onClickCost),0);
		for(int j=1;j<8;j++){
			sprintf(sz,"check_cost_%d",j);
			pBut = m_ui->getWidgetByName(sz);

			char val[18];
			sprintf(val,"%d",j);
			CCLabelBMFont* bmFont = CCLabelBMFont::create(val,"GUI/num_1.fnt");
			bmFont->setAnchorPoint(ccp(0.50f,0.50f));
			pBut->addCCNode(bmFont);
		}
		m_mainType = type_cardgroup;
		m_curPage = 0;
	}
	return m_ui;
}

void CardGroupScene::onClickMainType(CCObject* sender)
{
	m_curPage = 0;
	UpdateUI();
}

void CardGroupScene::onClickCost(CCObject* sender)
{
	m_curPage = 0;
	UpdateUI();
}

void CardGroupScene::onClickBack(CCObject* sender)
{
	m_curPage = 0;
	if(m_mainType== type_card){
		m_mainType = type_cardgroup;
		UpdateUI();
	}else if(m_mainType==type_cardgroup){
		KUIAssist::_switch2MainMenu();
	}
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
	for(KMiniCardList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KMiniCardWidget& elem = *it;
		UIWidget* pWidget = KUIAssist::_createMiniCard(elem._id,elem._count);
		if(!pWidget) continue;

		elem._pWidget = pWidget;
		pWidget->setPosition(pt);
		pWidget->setTag(elem._id);
		pWidget->setTouchEnable(true);
		pWidget->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickMiniCard));
		m_ui->addWidget(pWidget);
		pt.y -= 32;
	}
}

void CardGroupScene::onClickMiniCard(CCObject* obj)
{
	if(m_mainType!=type_card) return;
	UIWidget* pWidget = (UIWidget*)obj;

	KCardGroupAssist::AddMiniCard(m_miniCardList,pWidget->getTag(),-1);
	onMiniCardChanged();
}

void CardGroupScene::onMiniCardChanged()
{
	FreshMiniCardList();
	UpdateDeckCardCount();
	UpdateAddSubBut();
}

void CardGroupScene::FreshMiniCardList()
{
	UIWidget* slot = m_ui->getWidgetByName("mini_card_pos");
	CCPoint pt = slot->getPosition();
	for(KMiniCardList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KMiniCardWidget& elem = *it;
		if(!elem._pWidget){
			elem._pWidget = KUIAssist::_createMiniCard(elem._id,elem._count);
			if(!elem._pWidget) continue;
			m_ui->addWidget(elem._pWidget);
		}
		elem._pWidget->setPosition(pt);
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
		m_radioCost.SetVisible(false);
		ShowCardGroup();
	}else if(m_mainType==type_card){
		m_radioMain.SetVisible(true);
		m_radioCost.SetVisible(true);
		ShowCardBrowse();
	}
	UpdateAddSubBut();
}

void CardGroupScene::UpdateDeckCardCount()
{
	int showPos = 0;
	for(int i=0;i<PAGE_CARD_NUM;i++){
		char sz[32];
		int deckCardCount =KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,m_slotElem[i]._id);
		sprintf(sz,"%s_%d","slot_txt",i);
		UILabelBMFont* slotTxt = (UILabelBMFont*)m_ui->getWidgetByName(sz);
		if(deckCardCount>0){
			char str[64];
			sprintf(str,"X %d",deckCardCount);
			slotTxt->setText(str);
			slotTxt->setVisible(true);
		}else{
			slotTxt->setVisible(false);
		}
	}
}

void CardGroupScene::UpdatePageInfo(int moreNum)
{
	int totoal = m_curPage*PAGE_CARD_NUM +moreNum;
	int totalPage = totoal/PAGE_CARD_NUM;
	if(totalPage*PAGE_CARD_NUM<totoal) totalPage++;
	m_pPageInfo->setVisible(totoal>PAGE_CARD_NUM);
	char sz[64];
	sprintf(sz,"%d-%d",totalPage,m_curPage+1);
	m_pPageInfo->setText(sz);
	UIWidget* pPageUp = m_ui->getWidgetByName("page_up_but");
	pPageUp->setVisible(m_curPage>0);
	UIWidget* pPageDown = m_ui->getWidgetByName("page_down_but");
	pPageDown->setVisible(m_curPage+1 !=totalPage);
}

void CardGroupScene::ShowCardBrowse()
{

	KItemUnitList tmpList,desList;
	m_depot->PickStoreCard(tmpList);

	KCardGroupAssist::FilterCard(tmpList,desList,m_radioMain.GetSelectVal(),m_radioCost.GetSelectVal(),m_curPage*PAGE_CARD_NUM);
	UpdatePageInfo(desList.size());
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

		int deckCardCount = KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,unit._id);
		sprintf(sz,"%s_%d","slot_txt",showPos);
		UILabelBMFont* slotTxt = (UILabelBMFont*)m_ui->getWidgetByName(sz);
		if(deckCardCount>0){
			char str[64];
			sprintf(str,"X %d",deckCardCount);
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
	UpdatePageInfo(MAX_DECK_NUM);
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
	UpdateUI();
}

void CardGroupScene::onClickNewCardGroup(CCObject* sender)
{

}

void CardGroupScene::onClickSlotAdd(CCObject* sender)
{

	UIWidget* pWidget = (UIWidget*)sender;
	KCardGroupSlotElem& elem = m_slotElem[pWidget->getTag()];
	KCardGroupAssist::AddMiniCard(m_miniCardList,elem._id,1);
	onMiniCardChanged();
}

void CardGroupScene::onClickSlotSub(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	KCardGroupSlotElem& elem = m_slotElem[pWidget->getTag()];
	KCardGroupAssist::AddMiniCard(m_miniCardList,elem._id,-1);
	onMiniCardChanged();
}

void CardGroupScene::UpdateAddSubBut()
{
	bool bFull = KCardGroupAssist::GetTotalCardNum(m_miniCardList)>= 30;
	for(int i=0;i<PAGE_CARD_NUM;i++){
		 UIWidget* pAdd = KUIAssist::GetIndexWidget(m_ui->getRootWidget(),"slot_add",i);
		 UIWidget* pSub = KUIAssist::GetIndexWidget(m_ui->getRootWidget(),"slot_sub",i);

		 int deckCardCount =KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,m_slotElem[i]._id);

		 if(m_mainType==type_cardgroup ||
			 !m_slotElem[i]._widget){
			 KUIAssist::ShowButton(pAdd,false);
			 KUIAssist::ShowButton(pSub,false);
		 }else if(m_mainType==type_card){
			 if(bFull ||
				 deckCardCount==2){
				 KUIAssist::ShowButton(pAdd,false);
			 }else{
				 KUIAssist::ShowButton(pAdd,true);
			 }
			 if(deckCardCount>0){
				 KUIAssist::ShowButton(pSub,true);
			 }else{
				 KUIAssist::ShowButton(pSub,false);
			 }
		 }
	}
}

void CardGroupScene::onClickPageDown(CCObject* sender)
{
	m_curPage++;
	UpdateUI();
}

void CardGroupScene::onClickPageUp(CCObject* sender)
{
	if(m_curPage>0) m_curPage--;
	UpdateUI();
}