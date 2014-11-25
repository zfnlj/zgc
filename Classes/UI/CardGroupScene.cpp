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
#include "assist/KPopupLayer.h"
#include "../WorldObject/KMainPlayer.h"
#include "../common/KUIMsgDef.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "../StaticTable/StaticData.h"
#include "../WorldObject/KPlayer.h"
#include "assist/KUICardAssist.h"

USING_NS_CC;
using namespace cocos2d::extension;

CCScene* CardGroupScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CardGroupScene *layer = CardGroupScene::create();
    // add layer as a child to scene
    scene->addChild(layer,0,1977);
	
    // return the scene
    return scene;
}

void CardGroupScene::update(float dt)
{
	KSceneLayerBase::update(dt);
}

// on "init" you need to initialize your instance
bool CardGroupScene::init()
{

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CardGroupScene::update),this,0.1f,false);
    //////////////////////////////
   if (!KSceneLayerBase::init())
	{
		return false;
	}
	CreateCloseBut();
	addWidget(GetPanel());
	m_pMiniHeroWidget = NULL;
	memset(m_slotElem,0,sizeof(m_slotElem));
	m_depot = KMainPlayer::RealPlayer()->GetCardDepot();
	CreateMiniCardList(m_depot->GetCurDeck());
	ShowMiniHero();
	ShowMiniCardList();
	UpdateUI();
	UpdateCurDeckRadio();
	m_gameRecPanel.init(this);
	m_actor.init(m_ui);
	ShowHelpDrama();
    return true;
}

void CardGroupScene::ShowHelpDrama()
{
	if(STATIC_DATA_INT("cardgroup_help")==0){
		KGameRecordMgr::getSingleton().StartPlay("cardgroup_help",END_CARDGROUP_LESSON_STR);
		STATIC_DATA_SET("cardgroup_help",1);
	}
}

cocos2d::extension::UIWidget* CardGroupScene::GetPanel()
{
	if(!m_ui){
		m_ui =  KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/cardGroup.json");
		
		
		m_radioMain.AddGroupBut("check_main",3,m_ui,this,coco_pushselector(CardGroupScene::onClickMainType),1);

		UIWidget* pBut = UIHelper::seekWidgetByName(m_ui,"back_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickBack));
		
		pBut = UIHelper::seekWidgetByName(m_ui,"page_down_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickPageDown));

		pBut = UIHelper::seekWidgetByName(m_ui,"page_up_but");
		pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickPageUp));

		m_pSaveWidget = UIHelper::seekWidgetByName(m_ui,"but_save");
		m_pSaveWidget->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSaveDeck));

		m_pClearWidget = UIHelper::seekWidgetByName(m_ui,"but_clear");
		m_pClearWidget->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickClearDeck));

		m_pSmartCardGroup = UIHelper::seekWidgetByName(m_ui,"smart_cardgroup_but");
		m_pSmartCardGroup->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSmartCardGroup));

		m_pPageInfo = (UILabel*)UIHelper::seekWidgetByName(m_ui,"page_info");

		char sz[32];
		for(int i=0;i<PAGE_CARD_NUM;i++){
			sprintf(sz,"card_slot_%d",i);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);
			pBut->setTag(i);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlot));

			sprintf(sz,"slot_add_%d",i);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);
			pBut->setTag(i);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlotAdd));

			sprintf(sz,"slot_sub_%d",i);
			pBut = UIHelper::seekWidgetByName(m_ui,sz);
			pBut->setTag(i);
			pBut->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickSlotSub));
		}
		m_radioSelectHero.SetUnSelectAble(false);
		m_radioSelectHero.AddGroupBut("check_hero",8,m_ui,this,coco_pushselector(CardGroupScene::onClickSelectHero),-1);
		m_radioCost.AddGroupBut("check_cost",8,m_ui,this,coco_pushselector(CardGroupScene::onClickCost),0);
		m_radioRace.AddGroupBut("check_race",6,m_ui,this,coco_pushselector(CardGroupScene::onClickRace),0);
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
		m_pMiniHeroWidget = NULL;

	}
	return m_ui;
}

void CardGroupScene::onClickMainType(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	m_curPage = 0;
	UpdateUI();
}

void CardGroupScene::onClickRace(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	m_curPage = 0;
	UpdateUI();
}

void CardGroupScene::onClickCost(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	m_curPage = 0;
	UpdateUI();
}

void CardGroupScene::UpdateCurDeckRadio()
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

void CardGroupScene::onClickBack(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	m_curPage = 0;
	m_radioSelectHero.SetSelected(-1,false);
	if(m_mainType== type_card){
		m_mainType = type_cardgroup;
		CreateMiniCardList(m_depot->GetCurDeck());
		ShowMiniHero();
		ShowMiniCardList();
		UpdateUI();
		UpdateCurDeckRadio();
	}else if(m_mainType==type_cardgroup){
		onCloseCallback(sender);
	}
}

void CardGroupScene::CreateMiniCardList(int index)
{
	if(index<0) return;
	KIntegerList cardList;
	m_depot->GetCardDeck(index,cardList,m_miniHero);
	KCardGroupAssist::ClearMiniCardList(m_miniCardList);
	KCardGroupAssist::SortCardGroup(cardList,m_miniCardList);
}

void CardGroupScene::SetMiniHeroWidget(UIWidget* newHero)
{
	if(m_pMiniHeroWidget){
		KJsonDictMgr::getSingleton().OnMiniCardWidgetDestory(m_pMiniHeroWidget);
		m_pMiniHeroWidget->removeFromParent();
	}
	m_pMiniHeroWidget = newHero;
}

void CardGroupScene::ShowMiniHero()
{
	if(m_miniHero._id==0) return;
	UIWidget* slot = UIHelper::seekWidgetByName(m_ui,"mini_card_pos");
	CCPoint pt = slot->getPosition();
	UIImageView* raceSlot = (UIImageView*)UIHelper::seekWidgetByName(m_ui,"race_slot");
	UIWidget* pWidgetHero = KUICardAssist::_createMiniHero(m_miniHero);

	KUICardAssist::SetRaceIcon(raceSlot, KCardGroupAssist::GetCurDeckRace(m_miniHero,m_miniCardList));
	if(pWidgetHero){
		pWidgetHero->setPosition(pt);
		pWidgetHero->setTouchEnable(true);
		pWidgetHero->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickMiniHero));
		addWidget(pWidgetHero);
	}
	SetMiniHeroWidget(pWidgetHero);
}

void CardGroupScene::ShowMiniCardList()
{
	UIWidget* slot = UIHelper::seekWidgetByName(m_ui,"mini_card_pos");
	CCPoint pt = slot->getPosition();
	pt.y -= 34;

	for(KMiniCardList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KMiniCardWidget& elem = *it;
		UIWidget* pWidget = KUICardAssist::_createMiniCard(elem.GetCardID(),elem.GetCount());
		if(!pWidget) continue;

		elem._pWidget = pWidget;
		pWidget->setPosition(pt);
		pWidget->setTag(elem.GetCardID());
		pWidget->setTouchEnable(true);
		pWidget->addPushDownEvent(this,coco_pushselector(CardGroupScene::onClickMiniCard));
		this->addWidget(pWidget);
		pt.y -= 32;
	}
}

void CardGroupScene::onClickMiniCard(CCObject* sender)
{
	//KGameRecordMgr::getSingleton().onClickWidget(sender);

	if(m_mainType!=type_card) return;
	UIWidget* pWidget = (UIWidget*)sender;

	KCardGroupAssist::AddMiniCard(m_miniCardList,pWidget->getTag(),-1);
	onMiniCardChanged();
}

void CardGroupScene::onClickMiniHero(CCObject* sender)
{
	//KGameRecordMgr::getSingleton().onClickWidget(sender);

	if(m_mainType==type_card){
		m_miniHero.Clear();
		UpdateUI();
		SetMiniHeroWidget(NULL);
		m_radioSelectHero.ClearSelected();
	}
}

void CardGroupScene::UpdateMiniCardNumInfo()
{
	UILabelBMFont* cardNumWidget = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui,"deck_size");
	char sz[64];
	int num = KCardGroupAssist::GetTotalCardNum(m_miniCardList);
	sprintf(sz,"%d/30",num);
	cardNumWidget->setText(sz);
}

void CardGroupScene::onMiniCardChanged()
{
	FreshMiniCardList();
	UpdateDeckCardCount();
	UpdateAddSubBut();
	UpdateSelectHeroBut();
	UpdateMiniCardNumInfo();
	UpdateSmartCardGroupBut();
}

void CardGroupScene::FreshMiniCardList()
{
	UIWidget* slot = UIHelper::seekWidgetByName(m_ui,"mini_card_pos");
	CCPoint pt = slot->getPosition();
	pt.y -= 34;
	for(KMiniCardList::iterator it=m_miniCardList.begin();it!=m_miniCardList.end();++it)
	{
		KMiniCardWidget& elem = *it;
		if(!elem._pWidget){
			elem._pWidget = KUICardAssist::_createMiniCard(elem.GetCardID(),elem.GetCount());
			elem._pWidget->setTag(elem.GetCardID());
			if(!elem._pWidget) continue;
			this->addWidget(elem._pWidget);
		}
		elem._pWidget->setPosition(pt);
		if(pt.y >60) pt.y -= 32;
	}
}


void CardGroupScene::HideNewGroupIcon()
{
	for(int i=0;i<PAGE_CARD_NUM;i++){
		char sz[24];
		sprintf(sz,"card_slot_%d",i);
		UIWidget* pBut = UIHelper::seekWidgetByName(m_ui,sz);
		pBut->setVisible(false);
	}
}

void CardGroupScene::UpdateUI()
{
	HideNewGroupIcon();
	KUIAssist::ShowWidgetArr(this,"slot_txt", PAGE_CARD_NUM,false);
	KCardGroupAssist::ClearSlotElem(m_slotElem,PAGE_CARD_NUM);
	if(m_mainType==type_cardgroup){
		m_radioMain.SetVisible(false);
		m_radioCost.SetVisible(false);
		m_radioRace.SetVisible(false);
		m_radioSelectHero.SetUnSelectAble(false);
		KUIAssist::ShowButton(m_pSaveWidget,false);
		KUIAssist::ShowButton(m_pClearWidget,false);
		ShowCardGroup();
	}else if(m_mainType==type_card){
		m_radioMain.SetVisible(true);
		m_radioCost.SetVisible(true);
		m_radioRace.SetVisible(true);
		m_radioSelectHero.SetUnSelectAble(true);
		KUIAssist::ShowButton(m_pSaveWidget,true);
		KUIAssist::ShowButton(m_pClearWidget,true);
		ShowCardBrowse();
	}
	UpdateAddSubBut();
	UpdateSelectHeroBut();
	UpdateMiniCardNumInfo();
	UpdateSmartCardGroupBut();
}

void CardGroupScene::UpdateSmartCardGroupBut()
{
	if(m_miniHero._id==0|| m_miniCardList.size()>18 ||
		KCardGroupAssist::GetTotalCardNum(m_miniCardList)>=30){
		KUIAssist::_setButVisible(m_pSmartCardGroup,false);
		return;
	}
	UIWidget* slot = UIHelper::seekWidgetByName(m_ui,"mini_card_pos");
	CCPoint pt = slot->getPosition();
	pt.y -= 34;
	pt.y -= m_miniCardList.size()*32;
	m_pSmartCardGroup->setPosition(pt);
	KUIAssist::_setButVisible(m_pSmartCardGroup,true);
}

void CardGroupScene::UpdateDeckCardCount()
{
	for(int i=0;i<PAGE_CARD_NUM;i++){
		char sz[32];
		int deckCardCount =KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,m_slotElem[i]._id);
		sprintf(sz,"%s_%d","slot_txt",i);
		UILabelBMFont* slotTxt = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui,sz);
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
	UIWidget* pPageUp = UIHelper::seekWidgetByName(m_ui,"page_up_but");
	KUIAssist::_setButVisible(pPageUp,m_curPage>0);
	UIWidget* pPageDown = UIHelper::seekWidgetByName(m_ui,"page_down_but");
	KUIAssist::_setButVisible(pPageDown, (m_curPage+1 !=totalPage) &&(totalPage>0));
}

void CardGroupScene::ShowHeroCards()
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

void CardGroupScene::ShowCardBrowse()
{
	if(m_radioMain.GetSelectVal() ==(int)KCardGroupAssist::browse_hero){
		ShowHeroCards();
		return;
	}
	KItemUnitList tmpList,desList;
	m_depot->PickStoreCard(tmpList);
	KCardGroupAssist::SortCardItem(tmpList);
	KCardGroupAssist::FilterCard(tmpList,desList,m_radioMain.GetSelectVal(),m_radioRace.GetSelectVal(),m_miniHero.GetRace(),
									m_radioCost.GetSelectVal(),m_curPage*PAGE_CARD_NUM);
	UpdatePageInfo(desList.size());
	int showPos = 0;
	for(KItemUnitList::iterator it=desList.begin();it!=desList.end();++it){
		if(showPos==PAGE_CARD_NUM) break;
		KDBBagItemUnit& unit = *it;
		KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(unit._id);
		UIWidget* widget = KUICardAssist::_createCardLayout(pST,true);
		char sz[64];
		widget->setTag(unit._id);
		widget->addPushDownEvent(this, coco_pushselector(CardGroupScene::onClickCard));
		sprintf(sz,"%s_%d","card_slot",showPos);

		UIImageView* widgetPos =(UIImageView*)UIHelper::seekWidgetByName(m_ui,sz);
		widget->setPosition(widgetPos->getPosition());

		int deckCardCount = KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,unit._id);
		sprintf(sz,"%s_%d","slot_txt",showPos);
		UILabelBMFont* slotTxt = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui,sz);
		if(deckCardCount>0){
			char str[64];
			sprintf(str,"X %d",deckCardCount);
			slotTxt->setText(str);
			slotTxt->setVisible(true);
		}

		KCardGroupAssist::SetSlotElem(&m_slotElem[showPos++],unit._id,KCardGroupSlotElem::elem_card,widget);
		this->addWidget(widget);
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
		if(tmpList.size()>0 &&hero._cardId>0){
			widget = KUICardAssist::_createHero(hero,true,NULL,true);
		}
		if(widget){
			this->addWidget(widget);
			char sz[64];
			sprintf(sz,"%s_%d","card_slot",curPos);

			UIImageView* widgetPos =(UIImageView*)this->getWidgetByName(sz);
			widget->setPosition(widgetPos->getPosition());
			widget->setTouchEnable(false);
			UIImageView* pCardCenter = (UIImageView*)UIHelper::seekWidgetByName(widget,"card_center");
			pCardCenter->loadTexture("card_fight_txt.png",UI_TEX_TYPE_PLIST);
			if(m_depot->GetCurDeck()==i){
				pCardCenter->setVisible(true);
			}else{
				pCardCenter->setVisible(false);
			}
			pCardCenter->setScale(0.8f);
			KCardGroupAssist::SetSlotElem(&m_slotElem[curPos++],i,KCardGroupSlotElem::elem_card_group,widget);
		}
	}
	if(curPos<MAX_DECK_NUM){
		char sz[64];
		sprintf(sz,"%s_%d","card_slot",curPos++);
		UIImageView* pNewGroup = (UIImageView*)this->getWidgetByName(sz);
		pNewGroup->setVisible(true);
	}
}

void CardGroupScene::onClickCard(CCObject* sender)
{
	int i=0;
	for(;i< PAGE_CARD_NUM;i++){
		if(m_slotElem[i]._widget==sender) break;
	}
	if(m_mainType==type_card && i<PAGE_CARD_NUM){
		UIWidget* pAdd = KUIAssist::GetIndexWidget(this->getRootWidget(),"slot_add",i);
		if(pAdd->isVisible()) onClickSlotAdd(pAdd);
	}
}

void CardGroupScene::onClickSlot(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	UIImageView* pWidgetSlot = (UIImageView*)sender;
	if(m_mainType==type_cardgroup){
		if(pWidgetSlot->isVisible()){
			return onClickNewCardGroup(sender);
		}else if(m_slotElem[pWidgetSlot->getTag()]._widget){
			return onClickCardGroup(pWidgetSlot->getTag());
		}
	}else if(m_mainType==type_card){
		if(m_radioMain.GetSelectVal()==(int)KCardGroupAssist::browse_hero){

			KHeroDef* pHeroDef = m_depot->FindHero(m_slotElem[pWidgetSlot->getTag()]._id);
			if(pHeroDef){
				m_heroLevUpPanel.init(this);
				m_heroLevUpPanel.ShowPanel(pHeroDef);
			}
			/*m_radioSelectHero.SetSelected(pWidgetSlot->getTag(),false);
			
			if(KCardGroupAssist::IsMiniCardListMatch(m_slotElem[pWidgetSlot->getTag()],m_miniHero,m_miniCardList,m_depot)){
				const KHeroDef* pHeroDef = m_depot->FindHero(m_slotElem[pWidgetSlot->getTag()]._id);
				memcpy(&m_miniHero,pHeroDef,sizeof(KHeroDef));
				ShowMiniHero();
			}
			UpdateSmartCardGroupBut();*/
		}else{
			UIWidget* pAdd = KUIAssist::GetIndexWidget(this->getRootWidget(),"slot_add",pWidgetSlot->getTag());
			if(pAdd->isVisible()) onClickSlotAdd(sender);
		}
	}
}

void CardGroupScene::onClickCardGroup(int index)
{

	m_curCardGroup = m_slotElem[index]._id;
	m_curPage = 0;
	m_mainType = type_card;

	CreateMiniCardList(m_curCardGroup);
	onMiniCardChanged();
	ShowMiniHero();
	UpdateUI();
	
	int curPos=0;
	for(int i=0;i<m_depot->GetHeroNum();i++){
		const KHeroDef*  pHeroDef = m_depot->FindHeroOnIndex(i);
		if(!pHeroDef) continue;
		if(pHeroDef->_id==m_miniHero._id) 	m_radioSelectHero.SetSelected(curPos,false);
		curPos++;
	}
}

void CardGroupScene::onClickNewCardGroup(CCObject* sender)
{

	UIImageView* raceSlot = (UIImageView*)this->getWidgetByName("race_slot");
	raceSlot->setVisible(false);
	SetMiniHeroWidget(NULL);
	m_miniHero.Clear();
	KCardGroupAssist::ClearMiniCardList(m_miniCardList);
	m_curCardGroup = 99;
	m_curPage = 0;
	m_mainType = type_card;
	m_radioMain.SetSelected(0,false);
	m_radioSelectHero.SetSelected(-1,false);
	UpdateUI();
}

void CardGroupScene::onClickSlotAdd(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	UIWidget* pWidget = (UIWidget*)sender;
	KCardGroupSlotElem& elem = m_slotElem[pWidget->getTag()];
	KCardGroupAssist::AddMiniCard(m_miniCardList,elem._id,1);
	onMiniCardChanged();
}

void CardGroupScene::onClickSlotSub(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	UIWidget* pWidget = (UIWidget*)sender;
	KCardGroupSlotElem& elem = m_slotElem[pWidget->getTag()];
	KCardGroupAssist::AddMiniCard(m_miniCardList,elem._id,-1);
	onMiniCardChanged();
}

void CardGroupScene::UpdateAddSubBut()
{
	bool bFull = KCardGroupAssist::GetTotalCardNum(m_miniCardList)>= 30;
	for(int i=0;i<PAGE_CARD_NUM;i++){
		 UIWidget* pAdd = KUIAssist::GetIndexWidget(m_ui,"slot_add",i);
		 UIWidget* pSub = KUIAssist::GetIndexWidget(m_ui,"slot_sub",i);

		 int deckCardCount =KCardGroupAssist::GetDeckMiniCardNum(m_miniCardList,m_slotElem[i]._id);

		 bool bMatch = KCardGroupAssist::IsMiniCardListMatch(m_slotElem[i],m_miniHero,m_miniCardList,m_depot);
		 if( !bMatch||
			 m_mainType==type_cardgroup ||
			 (m_mainType==type_card && m_radioMain.GetSelectVal()==(int)KCardGroupAssist::browse_hero)||
			 !m_slotElem[i]._widget){
			 KUIAssist::ShowButton(pAdd,false);
			 KUIAssist::ShowButton(pSub,false);
		 }else if(m_mainType==type_card){
			 if(bFull ||
				 deckCardCount==2||
				 deckCardCount >=m_depot->GetCardNum(m_slotElem[i]._id)){
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
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	m_curPage++;
	UpdateUI();
}

void CardGroupScene::onClickPageUp(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	if(m_curPage>0) m_curPage--;
	UpdateUI();
}

void CardGroupScene::onClickSelectHero(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	int index = -1;
	bool bChecked = false;
	if(!m_radioSelectHero.GetSelectState(index,bChecked)) return;
	if(m_mainType==type_cardgroup){
		m_depot->SetCurDeck(m_slotElem[index]._id);
		CreateMiniCardList(m_depot->GetCurDeck());
		ShowMiniCardList();
	}else if(m_mainType==type_card){
		if(!KCardGroupAssist::IsMiniCardListMatch(m_slotElem[index],m_miniHero,m_miniCardList,m_depot)) return;
		const KHeroDef* pHeroDef = m_depot->FindHero(m_slotElem[index]._id);
		if(bChecked){
			memcpy(&m_miniHero,pHeroDef,sizeof(KHeroDef));
		}else{
			m_miniHero.Clear();
		}
	}

	UpdateUI();
	ShowMiniHero();
}

void CardGroupScene::UpdateSelectHeroBut()
{
	if(m_mainType==type_cardgroup){
		for(int i=0;i<PAGE_CARD_NUM;i++){
			m_radioSelectHero.SetVisible(i,m_slotElem[i]._widget!=NULL&& m_depot->IsDeckReady(i));
		}
	}else if(m_mainType==type_card){
		if(m_radioMain.GetSelectVal()==(int)KCardGroupAssist::browse_hero){
			m_radioSelectHero.SetVisible(true);
			for(int i=0;i<PAGE_CARD_NUM;i++){
				bool bMatch = KCardGroupAssist::IsMiniCardListMatch(m_slotElem[i],m_miniHero,m_miniCardList,m_depot);
				m_radioSelectHero.SetVisible(i,bMatch);
			}
		}else{
			m_radioSelectHero.SetVisible(false);
		}
	}
}

void CardGroupScene::DoClearDeck(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	if(pBut->getTag()==KPopupLayer::RT_YES){
		m_depot->ClearDeck(m_curCardGroup);
		onClickNewCardGroup(NULL);
	}
}

void CardGroupScene::onClickSaveDeck(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	if(m_miniHero._cardId<=0){
		KPopupLayer::DoModal(UI_WARNING_STR,NEED_HERO_CARD_STR,KPopupLayer::DT_Ok);
		return;
	}
	if(KCardGroupAssist::GetTotalCardNum(m_miniCardList)!=30){
		KPopupLayer::DoModal(UI_WARNING_STR,NEED_30_CARD_STR,KPopupLayer::DT_Ok);
		return;
	}
	KCardGroupAssist::SaveCardGroup(m_curCardGroup,m_miniHero,m_miniCardList,m_depot);
	KPopupLayer::DoModal(UI_NOTIFY_STR,SAVE_DECK_OK_STR,KPopupLayer::DT_Ok);
	
}

void CardGroupScene::onClickSmartCardGroup(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);
	KCardGroupAssist::SmartFillCardGroup(m_miniHero,m_miniCardList,m_depot);
	onMiniCardChanged();
}

void CardGroupScene::onClickClearDeck(CCObject* sender)
{
	KGameRecordMgr::getSingleton().onClickWidget(sender);

	KPopupLayer::DoModal(UI_WARNING_STR,DEL_DECK_NOTIFY_STR,KPopupLayer::DT_Yes_No,coco_pushselector(CardGroupScene::DoClearDeck),this);
}

bool CardGroupScene::ccTouchBegan(CCTouch * touch,CCEvent * pevent)
{
	CCPoint touchPoint;
	touchPoint.x = touch->getLocation().x;
	touchPoint.y = touch->getLocation().y;

	if(!KGameRecordMgr::getSingleton().IsClickWidgetValidate(m_ui,touchPoint)) return true;

	KGameRecordMgr::getSingleton().onPlayStepOn();
	if (m_pInputManager) m_pInputManager->onTouchBegan(touch);
	return true;
}

void CardGroupScene::onCloseCallback(CCObject* sender)
{
	KUIAssist::PlayClickButSound();
	if(KGameRecordMgr::getSingleton().IsPlaying()) return;
	KUIAssist::_switch2MainMenu();
}