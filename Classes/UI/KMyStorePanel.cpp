#include "KMyStorePanel.h"
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
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"

using namespace cocos2d::extension;
using namespace KStoreAbout;
using namespace KItemAbout;


KMyStorePanel::KMyStorePanel():m_Panel(NULL)
{
}

KMyStorePanel::~KMyStorePanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KMyStorePanel::init(cocos2d::extension::UILayer* layer)
{
	if(!m_Panel){
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/MyStore.json");
		CC_SAFE_RETAIN(m_Panel);
	}
	m_curFilter = view_null;
	m_curFilterParam = 0;
	m_curPage = 0;
	m_layer = layer;
	m_layer->addWidget(m_Panel);
	
	
	UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel, "but_close");
	KMyStorePanel* me = this;
    pBut->addPushDownEvent(me, coco_pushselector(KMyStorePanel::DoClickClose));
	InitCard();
	updateCardPanel();
}

void KMyStorePanel::DoClickClose(CCObject* sender)
{
	m_layer->removeWidget(m_Panel);
}


void KMyStorePanel::InitCard()
{
	for(int i=0;i<MAX_SLOT_NUM;i++){
		m_ShowCard[i].init();
	}
	m_sortStoreCard.clear();
	tb_player_record* record = KMainPlayer::RealPlayer()->GetPlayerRecord();
	void* buf=NULL;
	int len = record->cardStore.Get(buf);
	KDBBagItemUnit* dbCard = (KDBBagItemUnit*)buf;
	int n = len/sizeof(KDBBagItemUnit);
	for(int i=0;i<n;i++){
		m_sortStoreCard.insert_unique(dbCard);
		dbCard++;
	}
	FillFilterCard(view_default);
}

void KMyStorePanel::FillFilterCard(view_filter filter)
{
	if(filter==m_curFilter) return;
	m_curFilter = filter;
	m_filterCardVect.clear();

	int c = m_sortStoreCard.size();
	for(int i=0; i<c; i++)
	{
		if(IsMatchFilter(m_sortStoreCard[i],m_curFilter,m_curFilterParam)) m_filterCardVect.push_back(m_sortStoreCard[i]);
	}

}

bool KMyStorePanel::IsMatchFilter(KDBBagItemUnit* item,view_filter filter,int param)
{
	KCardStatic* pCard = KGameStaticMgr::getSingleton().GetCard(item->_id);
	if(!pCard) return false;
	bool ret = true;
	switch(filter){
	case view_default:
		break;
	case view_cost:
		{
			ret = (pCard->GetCost()==param)?true:false;
		}
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

void KMyStorePanel::pagePrev()
{
	if(m_curPage==0) return;
	m_curPage--;
	updateCardPanel();
}

void KMyStorePanel::pageNext()
{
	int beginPos = m_curPage*MAX_SLOT_NUM;
	if(beginPos>=(int)m_filterCardVect.size()) return;
	m_curPage++;
	updateCardPanel();
}

void KMyStorePanel::updateCardPanel()
{
	int beginPos = m_curPage*MAX_SLOT_NUM;
	int showNum = m_filterCardVect.size() - beginPos;
	if(showNum > MAX_SLOT_NUM) showNum = MAX_SLOT_NUM;

	for(int i=0;i<MAX_SLOT_NUM;i++){
		char slotName[20];
		sprintf(slotName,"slot_%d",i);
		UIImageView* slotView = (UIImageView*)m_layer->getWidgetByName(slotName);
		if(!slotView) continue;

		KCardActor* actor = NULL;
		if(i<showNum){
			m_ShowCard[i].init(0,NULL,KGameStaticMgr::getSingleton().GetCard(m_filterCardVect[beginPos+i]->_id));
			KCardActor* actor = KCardActor::create(&m_ShowCard[i]);
			actor->GetBigCard()->setPosition(slotView->getWorldPosition());
			actor->GetBigCard()->setZOrder(10);
			if(!actor->GetBigCard()->getParent()) m_Panel->addChild(actor->GetBigCard());
			actor->GetBigCard()->setVisible(true);
		}else{
			actor = KCardActor::create(&m_ShowCard[i]);
			if(actor && actor->GetBigCard()) actor->GetBigCard()->setVisible(false);
		}
	}
}

