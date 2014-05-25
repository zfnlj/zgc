#include "KGameResultPanel.h"
#include "../GameLogic/KDynamicWorld.h"
#include "KUIAssist.h"
#include "../VirtualService.h"
#include "GameRoot.h"
#include "../KNet/KSocketFacade.h"
#include "../WorldObject/KMainPlayer.h"
#include "../Quest/KPlayerQuestManager.h"
#include "KUIAssist.h"
#include "BattleFieldScene.h"
#include "../Quest/KQuestManager.h"
#include "KQuestFacade.h"

using namespace cocos2d::extension;


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
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/game_result.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_exit");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));

		pBut = UIHelper::seekWidgetByName(m_Panel, "but_continue");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickContinue));

		m_pt =m_Panel->getPosition();
		m_pt =m_Panel->getWorldPosition();

	}
	m_layer = layer;
	//m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	updatePanel();
}

void KGameResultPanel::onGameEnd(unsigned long long Param1)
{
	strGameResult* result = (strGameResult*)Param1;
	memcpy(&m_result,result,sizeof(strGameResult));
	if(result->_winner==GameRoot::getSingleton().BattleCtrl().GetMainPlayer()){
		m_resultType = result_win;
	}else if(result->_winner){
		m_resultType = result_lost;
	}else{
		m_resultType = result_equal;
	}
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	KQuestNew* pQuest = playerQuestManager.GetQuest(result->_questId);
	if(pQuest && pQuest->GetQuestStatus()==KQ_PreStepOver){
		m_result._money = pQuest->m_money;
		m_result._exp = pQuest->m_exp;
		VirtualService::getSingleton().SubmitQuest(pQuest->GetID());
	}
	ShowPanel();
}

void KGameResultPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());
	updatePanel();
}

void KGameResultPanel::DoClickContinue(CCObject* sender)
{
	//m_Panel->removeFromParent();
	KUIAssist::_switch2StageWaitScene();
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
	char buf[128];
	UIWidget* pWinWidget = UIHelper::seekWidgetByName(m_Panel,"win_icon");
	UIWidget* pLostWidget = UIHelper::seekWidgetByName(m_Panel,"lost_icon");
	UIWidget* pEqualWidget = UIHelper::seekWidgetByName(m_Panel,"equal_icon");
	UILabel* pMoneyTxt = (UILabel*) UIHelper::seekWidgetByName(m_Panel,"money_val");
	UILabel* pExpTxt =  (UILabel*)UIHelper::seekWidgetByName(m_Panel,"exp_val");
	sprintf(buf,"%d",m_result._money);
	pMoneyTxt->setText(buf);
	sprintf(buf,"%d",m_result._exp);
	pExpTxt->setText(buf);
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
	UIWidget* pMoneyVal = UIHelper::seekWidgetByName(m_Panel,"money_val");
	if(m_result._money>0){
		pMoneyVal->setVisible(true);
	}else{
		pMoneyVal->setVisible(false);
	}
	UIWidget* pExpVal = UIHelper::seekWidgetByName(m_Panel,"exp_val");
	if(m_result._exp>0){
		pExpVal->setVisible(true);
	}else{
		pExpVal->setVisible(false);
	}
}

