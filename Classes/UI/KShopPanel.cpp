#include "KShopPanel.h"

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
#include "assist/KUIMsgDef.h"

using namespace cocos2d::extension;
using namespace KStoreAbout;
using namespace KItemAbout;
#define MAX_SHOP_ITEM 8
#define MAX_BAG_ITEM 10

KShopPanel::KShopPanel():m_Panel(NULL),m_store(NULL)
{
}

KShopPanel::~KShopPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KShopPanel::init(cocos2d::extension::UILayer* layer)
{
	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/shop.json");
		CC_SAFE_RETAIN(m_Panel);

		UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel, "but_close");
		pBut->setTouchEnable(true);
		KShopPanel* me = this;
		pBut->addPushDownEvent(me, coco_pushselector(KShopPanel::DoClickClose));
	}
	m_buyProduct = 0;
	m_layer = layer;
	m_layer->addWidget(m_Panel);
	InitItem();
	UpdateNormalBag();
	UpdateMoney();
}

void KShopPanel::UpdateMoney()
{
	UILabelBMFont* pLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel, "money_txt");
	pLabel->setText(KMainPlayer::RealPlayer()->GetMoney());
}

void KShopPanel::DoClickClose(CCObject* sender)
{
	m_layer->removeWidget(m_Panel);
}

void KShopPanel::onClickBuy(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	m_buyProduct = pBut->getTag();

	const KItemAbout::KCreateInfo_ItemBase* item = KStoreManager::GetInstancePtr()->GetItemCreateInfo(m_buyProduct);
	char sz[128];
	KHelpStringStatic* willBuyString = KGameStaticMgr::getSingleton().GetHelpString(WILL_BUY_STR);

	sprintf(sz,"%s [ %s ]?",willBuyString->GetString(),item->GetName());

	KHelpStringStatic* titleString = KGameStaticMgr::getSingleton().GetHelpString(UI_NOTIFY_STR);

	KPopupLayer::DoModal(titleString->GetString(),sz,KPopupLayer::DT_Yes_No,coco_pushselector(KShopPanel::DoClickBuy),this);
}

void KShopPanel::DoClickBuy(CCObject* sender)
{
	UIWidget* pBut = (UIWidget*)sender;
	if(pBut->getTag()!=KPopupLayer::RT_YES) return;

	if(!KStoreManager::GetInstancePtr()->TryBuy(m_buyProduct,1)) return;

	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	if(!pBagMgr) return;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return;


	
}

void KShopPanel::DoClickUseItem(CCObject* sender)
{
	UIWidget* pWidget = (UIWidget*)sender;
	KItemAbout::KBagManager* pBagMgr = KMainPlayer::Instance()->GetBagManagerPtr();
	if(!pBagMgr) return;
	KBagNormal* pBag = pBagMgr->FindNormalBag();
	if(!pBag) return;

	if(GameRoot::getSingleton().BattleCtrl().IsServerSide()){
		pBagMgr->TryToUse(pBag,pWidget->getTag(),0);
	}else{
		//TBD
	}
}

void KShopPanel::InitItem()
{
	m_store =KStoreCreateInfoManager::GetInstancePtr()->GetStoreCreateInfo(1);
	if(!m_store) return;

	for(int i=0;i<MAX_SHOP_ITEM;i++){
		char slot_name[64];
		sprintf(slot_name,"item_%d",i);
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_Panel, slot_name);
		KUIAssist::_setButVisible(slotWidget,false);
		slotWidget->addPushDownEvent(this, coco_pushselector(KShopPanel::onClickBuy));

		sprintf(slot_name,"cost_%d",i);
		UILabelBMFont* pPriceLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel, slot_name);
		pPriceLabel->setVisible(false);
	}

	int index = 0;
	MAP_STORE_PRODUCT::iterator itorTmp = m_store->m_mapStoreProduct.begin();
	for(;itorTmp!=m_store->m_mapStoreProduct.end();++itorTmp)
	{
		char slot_name[64];
		sprintf(slot_name,"item_%d",index);
		UIImageView* slotWidget = (UIImageView*)UIHelper::seekWidgetByName(m_Panel, slot_name);
		KUIAssist::_setButVisible(slotWidget,true);
		
		slotWidget->setTag(index+1);

		KStoreProduct* pStoreProduct = itorTmp->second;
		if(!pStoreProduct) continue;
		
		KCreateInfo_Item* item = KItemCreate::GetInstancePtr()->GetCreateInfo_Item(pStoreProduct->m_dwItemID);
		std::string fullPath;
		fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(item->GetIcon());
		slotWidget->loadTexture(fullPath.c_str());
		
	
		sprintf(slot_name,"cost_%d",index);
		UILabelBMFont* pPriceLabel = (UILabelBMFont*)UIHelper::seekWidgetByName(m_Panel, slot_name);
		char buf[10];
		sprintf(buf,"%d",item->GetPrice());
		pPriceLabel->setText(buf);
		pPriceLabel->setVisible(true);

		index++;
	}
}

void KShopPanel::UpdateNormalBag()
{
	for(int i=1;i<=MAX_BAG_ITEM;i++){
		char slot_name[64];
		sprintf(slot_name,"my_slot%d",i);
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_Panel, slot_name);
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
		UIWidget* slotWidget = UIHelper::seekWidgetByName(m_Panel, slot_name);
		if(!slotWidget) continue;
		slotWidget->setVisible(true);
		slotWidget->setTag(itorTmp->first);
		slotWidget->addPushDownEvent(this, coco_pushselector(KShopPanel::DoClickUseItem));

		
		
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

