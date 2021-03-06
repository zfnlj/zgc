#include "KGameResultPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "assist/KUIAssist.h"
#include "../VirtualService.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "assist/KUIAssist.h"
#include "BattleFieldScene.h"
#include "../Quest/KQuestManager.h"
#include "assist/KQuestFacade.h"
#include "../Item/KItemCreate.h"
#include "../WorldObject/KPlayer.h"
#include "assist/KJsonDictMgr.h"
#include "assist/KAudioAssist.h"

using namespace cocos2d::extension;
using namespace KItemAbout;

#define SELECT_ITEM_TAG_BASE 10000
KGameResultPanel::KGameResultPanel():m_Panel(NULL)
{
}

KGameResultPanel::~KGameResultPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGameResultPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/GameResult.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_exit");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_continue");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickContinue));

		for(int i=0; i<3; i++)
		{
			UIImageView* pSlot = (UIImageView*)KUIAssist::GetIndexWidget(m_Panel,"slot",i);
			pSlot->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickSlot));
			pSlot->setTag(i);
		}

	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	m_bSelectGift = false;
	m_expBar.init(NULL,0,0,0.0f);

	updatePanel();
}

void KGameResultPanel::ResetSlotInfo()
{
	for(int i=0; i<3; i++)
	{
		UIWidget* pItem = m_Panel->getChildByTag(SELECT_ITEM_TAG_BASE+i);
		if(pItem) pItem->removeFromParent();
		UIImageView* pSlot = (UIImageView*)KUIAssist::GetIndexWidget(m_Panel,"slot",i);
		KUIAssist::ShowButton(pSlot,false);
		UIWidget* pSelect = KUIAssist::GetIndexWidget(m_Panel,"select",i);
		pSelect->setVisible(false);
	}
}

void KGameResultPanel::onGameEnd(unsigned long long Param1)
{
	strGameResult* result = (strGameResult*)Param1;
	memcpy(&m_result,result,sizeof(strGameResult));
	if(result->_winner==GameRoot::getSingleton().BattleCtrl()->GetMainPlayer()){
		m_resultType = result_win;
		KAudioAssist::playSound("audio/scene/battle_win.wav");
	}else if(result->_winner){
		m_resultType = result_lost;
		KAudioAssist::playSound("audio/scene/battle_lost.wav");
	}else{
		m_resultType = result_equal;
	}
	ShowPanel();
}

void KGameResultPanel::onQuestPreOver(KQuestNew* pQuest)
{
	m_result._questId = pQuest->GetID();
	m_result._winner = GameRoot::getSingleton().BattleCtrl()->GetMainPlayer();
	m_resultType = result_win;
	ShowPanel();
}


void KGameResultPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1f);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3f, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());
	updatePanel();
}

void KGameResultPanel::DoClickSlot(CCObject* sender)
{
	int selectId = ((UIWidget*)sender)->getTag();
	for(int i=0; i<3; i++)
	{
		UIWidget* pSelect = KUIAssist::GetIndexWidget(m_Panel,"select",i);
		pSelect->setVisible(i==selectId);
	}
	KMainPlayer::RealPlayer()->UpdateAttrValue(88,selectId);
	m_bSelectGift = false;
}

void KGameResultPanel::DoClickContinue(CCObject* sender)
{
	if(m_bSelectGift) return;
	//m_Panel->removeFromParent();
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.GetQuest(m_result._questId);
	if(pQuest&& pQuest->GetQuestStatus()==KQ_PreStepOver){
		VirtualService::getSingleton().SubmitQuest(pQuest->GetID());
	}
	if(m_result._type==scene_daily){
		KUIAssist::_switch2StageSelectScene();
	}else{
		KUIAssist::_switch2StageWaitScene();
	}
}


void KGameResultPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();
	
	KUIAssist::_switch2MainMenu();

	/*   CCPoint pt(m_pt.x,m_pt.y+600);
	switch(m_panelType){
	case enum_panel_task:
	{

	KUIAssist::_moveOutWidget(m_Panel,m_pt,pt,0.8f);
	}
	break;
	case enum_panel_game:
	{
	m_Panel->setVisible(false);
	KUIAssist::_switch2MainMenu();
	}
	break;
	default:
	break;
	}
	*/
}

void KGameResultPanel::updatePanel()
{
	ResetSlotInfo();
	char buf[128];
	UIWidget* pWinWidget = UIHelper::seekWidgetByName(m_Panel,"win_icon");
	UIWidget* pLostWidget = UIHelper::seekWidgetByName(m_Panel,"lost_icon");
	UIWidget* pEqualWidget = UIHelper::seekWidgetByName(m_Panel,"equal_icon");
	UILabel* pMoneyTxt = (UILabel*) UIHelper::seekWidgetByName(m_Panel,"money_val");
	UILabel* pStoneTxt =  (UILabel*)UIHelper::seekWidgetByName(m_Panel,"stone_val");

	for(int i=0; i<3; i++)
	{
		UIWidget* pSelect = KUIAssist::GetIndexWidget(m_Panel,"select",i);
		pSelect->setVisible(false);
	}
	switch(m_resultType){
	case result_win:
		{
			pWinWidget->setVisible(true);
			pLostWidget->setVisible(false);
			pEqualWidget->setVisible(false);
		}
		break;
	case result_lost:
		{
			pWinWidget->setVisible(false);
			pLostWidget->setVisible(true);
			pEqualWidget->setVisible(false);
		}
		break;
	case result_equal:
		{
			pWinWidget->setVisible(false);
			pLostWidget->setVisible(false);
			pEqualWidget->setVisible(true);
		}
		break;
    default:
        break;
	}

	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	
	if(m_result._type==scene_tower){
		KPlayerTmpBag* pBag = KMainPlayer::RealPlayer()->GetResultBag();
		m_result._money = pBag->GetMoney();
		m_result._exp = pBag->GetExp();
		m_result._fateStone = pBag->GetFateStone();
	}else{
		KQuestNew* pQuest = playerQuestManager.GetQuest(m_result._questId);
		if(pQuest && pQuest->GetQuestStatus()==KQ_PreStepOver){
			m_result._money = pQuest->GetAwardMoney();
			m_result._exp = pQuest->GetAwardExp();
			m_result._fateStone = pQuest->GetAwardFateStone();

			if(ShowSelectGift(pQuest)) return;
			VirtualService::getSingleton().SubmitQuest(pQuest->GetID());
		}
	}

	sprintf(buf,"+%d",m_result._money);
	pMoneyTxt->setText(buf);
	sprintf(buf,"+%d",m_result._fateStone);
	pStoneTxt->setText(buf);

	
	UIWidget* pSelectAward = UIHelper::seekWidgetByName(m_Panel,"select_award_txt");
	pSelectAward->setVisible(false);

	ShowObtainGift();
	ShowGuyLevAndExp();
}

void KGameResultPanel::ShowObtainGift()
{
	KPlayerTmpBag* pBag = KMainPlayer::RealPlayer()->GetResultBag();
	if(pBag->m_itemList.empty()) return;
	KPlayerTmpBag::ItemDefList::iterator it = pBag->m_itemList.begin();
	
	int index = 0;
	for(it;it!=pBag->m_itemList.end();++it,index++){
		UIWidget* widget = KUIAssist::_createBagItemWidget(*it);
		UIImageView* pSlot = (UIImageView*)KUIAssist::GetIndexWidget(m_Panel,"slot",index);
		widget->setPosition(pSlot->getPosition());
		widget->setTag(SELECT_ITEM_TAG_BASE+index);
		m_Panel->addChild(widget);
	}
	
	//UIWidget* pMoneyVal = UIHelper::seekWidgetByName(m_Panel,"money_val");
	//pMoneyVal->setVisible(false);
	//UIWidget* pExpVal = UIHelper::seekWidgetByName(m_Panel,"exp_val");
	//pExpVal->setVisible(false);


	//UIWidget* pMoneyIcon = UIHelper::seekWidgetByName(m_Panel,"money_icon");
	//pMoneyIcon->setVisible(false);

	//UIWidget* pExpIcon = UIHelper::seekWidgetByName(m_Panel,"exp_icon");
	//pExpIcon->setVisible(false);

}

bool KGameResultPanel::ShowSelectGift(KQuestNew* pQuest)
{
	char buf[64]={0};
	pQuest->GetSelectGiftList(buf,63);
	m_bSelectGift = (strlen(buf)>0)? true:false;
	if(!m_bSelectGift) return false;


	char* ss[32];
	int n = split(buf, ';', ss, 32);
	for(int i=0; i<n; i++)
	{
		int itemId = atoi(ss[i]);
		const KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(itemId);
		UIImageView* pSlot = (UIImageView*)KUIAssist::GetIndexWidget(m_Panel,"slot",i);
		pSlot->loadTexture(pCIIB->s_icon,UI_TEX_TYPE_PLIST);
		KUIAssist::ShowButton(pSlot,true);
	}

	/*UIWidget* pMoneyVal = UIHelper::seekWidgetByName(m_Panel,"money_val");
	pMoneyVal->setVisible(false);
	UIWidget* pExpVal = UIHelper::seekWidgetByName(m_Panel,"exp_val");
	pExpVal->setVisible(false);*/

	UIWidget* pSelectAward = UIHelper::seekWidgetByName(m_Panel,"select_award_txt");
	pSelectAward->setVisible(true);

	/*UIWidget* pMoneyIcon = UIHelper::seekWidgetByName(m_Panel,"money_icon");
	pMoneyIcon->setVisible(false);

	UIWidget* pExpIcon = UIHelper::seekWidgetByName(m_Panel,"exp_icon");
	pExpIcon->setVisible(false);*/
	return true;
}

void KGameResultPanel::ShowGuyLevAndExp()
{
	KPlayer* pPlayer = KMainPlayer::RealPlayer();
	int oldExp = pPlayer->m_playerRecord.GetExp()-m_result._exp;
	int oldLev = KGameStaticMgr::getSingleton().PlayerExpToLevel(oldExp);
	float rate1 =KGameStaticMgr::getSingleton().GetLevRate(oldExp);
	float rate2 =KGameStaticMgr::getSingleton().GetLevRate(pPlayer->m_playerRecord.GetExp());
	char sz[64];
	sprintf(sz,"Lev%d",oldLev);
	UILabel* pLevWidget = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"Lev_txt");
	pLevWidget->setText(sz);
	UILoadingBar* pBar = (UILoadingBar*)UIHelper::seekWidgetByName(m_Panel,"exp_bar");
	if(m_result._exp>0.0f) m_expBar.init(pBar,rate1,rate2,1.6f,0.0f);
}

void KGameResultPanel::update(float dt)
{
	bool bLevUpdate = m_expBar.update(dt);
	if(bLevUpdate){
		int lev = KGameStaticMgr::getSingleton().PlayerExpToLevel(KMainPlayer::RealPlayer()->m_playerRecord.GetExp());
		char sz[64];
		sprintf(sz,"Lev%d",lev);
		UILabel* pLevWidget = (UILabel*)UIHelper::seekWidgetByName(m_Panel,"Lev_txt");
		pLevWidget->setText(sz);
	}
}