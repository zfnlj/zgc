#include "KShopScene.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "assist/KUIAssist.h"
#include "cocos-ext.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../Store/KStoreManager.h"
#include "../Store/KStoreCreate.h"
#include "../Item/KItemCreate.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "assist/KPopupLayer.h"
#include "../common/KUIMsgDef.h"
#include "platform/KIOSiAP_Bridge.h"
#include "assist/KJsonDictMgr.h"

using namespace cocos2d::extension;
using namespace KStoreAbout;
using namespace KItemAbout;
#define MAX_SHOP_ITEM 8
#define MAX_BAG_ITEM 10

CCScene* KShopScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    KShopScene *layer = KShopScene::create();
    // add layer as a child to scene
    scene->addChild(layer,0,1977);
	
    // return the scene
    return scene;
}

KShopScene::KShopScene():m_store(NULL)
{
}

KShopScene::~KShopScene()
{
}

void KShopScene::update(float dt)
{
	KSceneLayerBase::update(dt);
}

bool KShopScene::init()
{
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(KShopScene::update),this,0.033f,false);
    //////////////////////////////
    // 1. super init first
    if ( !KSceneLayerBase::init() )
    {
        return false;
    }
  
    // add "BattleFieldScene" splash screen"

	if(!m_ui){
		m_ui = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/shop.json"); 
		CC_SAFE_RETAIN(m_ui);
		addWidget(m_ui);

		UIWidget* pBut = UIHelper::seekWidgetByName(m_ui, "but_close");
		pBut->setTouchEnable(true);
		KShopScene* me = this;
		pBut->addPushDownEvent(me, coco_pushselector(KShopScene::DoClickClose));
		m_actor.init(m_ui);
	}

	m_buyProduct = 0;
	InitItem();
	UpdateNormalBag();
	UpdateMoney();
	
    return true;
}

void KShopScene::UpdateMoney()
{
	UILabelBMFont* pLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui, "money_txt");
	pLabel->setText(KMainPlayer::RealPlayer()->GetMoney());
}

void KShopScene::DoClickClose(CCObject* sender)
{
	KUIAssist::_switch2MainMenu();
}

void KShopScene::onClickBuy(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	m_buyProduct = pBut->getTag();

	const KItemAbout::KCreateInfo_ItemBase* item = KStoreManager::GetInstancePtr()->GetItemCreateInfo(m_buyProduct);

	KStoreAbout::KStoreProduct* pProduct = m_store->GetStoreProduct(m_buyProduct);
	if(!pProduct) return;
	if(pProduct->GetMoneyType()==0){ //╫П╠р
		char sz[128];
		KHelpStringStatic* willBuyString = KGameStaticMgr::getSingleton().GetHelpString(WILL_BUY_STR);
		sprintf(sz,"%s [ %s ]?",willBuyString->GetString(),item->GetName());
		KHelpStringStatic* titleString = KGameStaticMgr::getSingleton().GetHelpString(UI_NOTIFY_STR);
		KPopupLayer::DoModal(titleString->GetString(),sz,KPopupLayer::DT_Yes_No,coco_pushselector(KShopScene::DoClickBuy),this);
	}else{ //ож╫П
		BuyProduct(m_buyProduct);
	}

}

void KShopScene::BuyProduct(int productId)
{
	if(!KStoreManager::GetInstancePtr()->TryBuy(productId,1)) return;

	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	if(!pBagMgr) return;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return;

	const KItemAbout::KCreateInfo_ItemBase* item = KStoreManager::GetInstancePtr()->GetItemCreateInfo(productId);
	if(!item) return;
	int pos = pBag->GetFirstPos(item->s_nItemID);
	if(pos<0) return;

	if(GameRoot::getSingleton().BattleCtrl()->IsServerSide()){
		pBagMgr->TryToUse(pBag,pos,0);
	}else{
		//TBD
	}
	UpdateMoney();
}

void KShopScene::DoClickBuy(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	if(pBut->getTag()!=KPopupLayer::RT_YES) return;
	BuyProduct(m_buyProduct);
}

void KShopScene::DoClickUseItem(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	if(!pBagMgr) return;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return;

	if(GameRoot::getSingleton().BattleCtrl()->IsServerSide()){
		pBagMgr->TryToUse(pBag,pWidget->getTag(),0);
	}else{
		//TBD
	}
}

void KShopScene::InitItem()
{
	m_store =KStoreCreateInfoManager::GetInstancePtr()->GetStoreCreateInfo(1);
	if(!m_store) return;

	for(int i=0;i<MAX_SHOP_ITEM;i++){
		char slot_name[64];
		sprintf(slot_name,"item_%d",i);
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		KUIAssist::_setButVisible(slotWidget,false);
		slotWidget->addPushDownEvent(this, coco_pushselector(KShopScene::onClickBuy));

		sprintf(slot_name,"cost_%d",i);
		UILabelBMFont* pPriceLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui, slot_name);
		pPriceLabel->setVisible(false);

		sprintf(slot_name,"gold_icon_%d",i);
		UIWidget* pWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		pWidget->setVisible(false);

		sprintf(slot_name,"pile_%d",i);
		pWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		pWidget->setVisible(false);
	}

	int index = 0;
	MAP_STORE_PRODUCT::iterator itorTmp = m_store->m_mapStoreProduct.begin();
	for(;itorTmp!=m_store->m_mapStoreProduct.end();++itorTmp)
	{
		char slot_name[64];
		sprintf(slot_name,"item_%d",index);
		UIImageView* slotWidget = (UIImageView*)UIHelper::seekWidgetByName(m_ui, slot_name);
		KUIAssist::_setButVisible(slotWidget,true);
		
		slotWidget->setTag(index+1);

		KStoreProduct* pStoreProduct = itorTmp->second;
		if(!pStoreProduct) continue;
		
		KCreateInfo_Item* item = KItemCreate::GetInstancePtr()->GetCreateInfo_Item(pStoreProduct->m_dwItemID);
		std::string fullPath;
		fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(item->GetIcon());
		slotWidget->loadTexture(fullPath.c_str());
		
	
		sprintf(slot_name,"cost_%d",index);
		UILabelBMFont* pPriceLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui, slot_name);
		char buf[10];
		sprintf(buf,"%d",item->GetPrice());
		pPriceLabel->setText(buf);
		pPriceLabel->setVisible(true);

		if(pStoreProduct->m_nPileCount>1){
			sprintf(slot_name,"pile_%d",index);
			UILabelBMFont* pPileLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_ui, slot_name);
			sprintf(buf,"X%d",pStoreProduct->m_nPileCount);
			pPileLabel->setText(buf);
			pPileLabel->setVisible(true);
		}

		sprintf(slot_name,"gold_icon_%d",index);
		UIWidget* pWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		if(pStoreProduct->GetMoneyType()==0) pWidget->setVisible(true);

		index++;
	}
}

void KShopScene::UpdateNormalBag()
{
	for(int i=1;i<=MAX_BAG_ITEM;i++){
		char slot_name[64];
		sprintf(slot_name,"my_slot%d",i);
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		if(slotWidget) slotWidget->setVisible(false);
	}

	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	if(!pBagMgr) return;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return;

	int index = 1;
	KBag::CellMapVec::const_iterator itorTmp = pBag->m_ListItem.begin();
	for(;itorTmp!=pBag->m_ListItem.end();itorTmp++)
	{
		const KCellBag* pCell = (const KCellBag*)(itorTmp->second);
		KCreateInfo_Item* item = KItemCreate::GetInstancePtr()->GetCreateInfo_Item(pCell->GetItem().GetID());

		char slot_name[64];
		sprintf(slot_name,"my_slot%d",index);
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_ui, slot_name);
		if(!slotWidget) continue;
		slotWidget->setVisible(true);
		slotWidget->setTag(itorTmp->first);
		slotWidget->addPushDownEvent(this, coco_pushselector(KShopScene::DoClickUseItem));

		
		
		UIImageView* iconWidget = (UIImageView*)UIHelper::seekWidgetByName(slotWidget, "icon_ImageView");
		std::string fullPath;
		fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(item->GetIcon());
		iconWidget->loadTexture(fullPath.c_str());
		
		UILabel* pLabel = (UILabel*)UIHelper::seekWidgetByName(slotWidget, "item_name");
		pLabel->setText(item->GetName());
		
		UILabelBMFont* pNumLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(slotWidget, "num_text");
		char buf[10];
		sprintf(buf,"%d",pCell->GetItem().GetStackNumber());
		pNumLabel->setText(buf);


		index++;
	}
}

void KShopScene::onGenPlayerCard(unsigned long long p1)
{
	m_genCardPanel.init(this,p1);
}

void KShopScene::onSystemMsg(int id)
{
	if(id==MONEY_LOW_STR){
		KPopupLayer::DoModal(UI_NOTIFY_STR,MONEY_LOW_STR,KPopupLayer::DT_Yes_No,coco_pushselector(KShopScene::DoBuyMoney),this);
	}
}

void KShopScene::DoBuyMoney(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	if(pBut->getTag()==KPopupLayer::RT_YES){
		//TBD
	}
}

void TestIAP()
{
	IOSiAP_Bridge iap;
	iap.requestProducts();
}