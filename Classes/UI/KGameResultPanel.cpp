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
		m_Panel = GUIReader::shareReader()->widgetFromJsonFile("GUI/GameResult.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
		pBut->addPushDownEvent(this, coco_pushselector(KGameResultPanel::DoClickClose));
		m_pt =m_Panel->getPosition();
		m_pt =m_Panel->getWorldPosition();

	}
	m_layer = layer;
	m_Panel->setTouchEnable(false);
	pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
	pBut->setTouchEnable(false);
	m_Panel->setVisible(false);
	m_layer->addWidget(m_Panel);
	m_Panel->setZOrder(999);
	updatePanel();
}

void KGameResultPanel::ShowPanel(GameResult& result,panel_type tp)
{
	m_panelType = tp;
	UIWidget* pBut = UIHelper::seekWidgetByName(m_Panel, "bk");
	pBut->setTouchEnable(true);
	
	m_Panel->setVisible(true);
    
    CCPoint pt(m_pt.x,m_pt.y+500);
	KUIAssist::_moveInWidget(m_Panel,pt,m_pt,0.8f);
	KUIAssist::_fadeIn(pBut,0.5);
	m_result = result;
	updatePanel();
}

void KGameResultPanel::DoClickClose(CCObject* sender)
{
    CCPoint pt(m_pt.x,m_pt.y+600);
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
	
}

void KGameResultPanel::updatePanel()
{
	char buf[128];
	UIWidget* pWinWidget = UIHelper::seekWidgetByName(m_Panel,"win");
	UIWidget* pTaskDone = UIHelper::seekWidgetByName(m_Panel,"task_done");
	UILabelAtlas* pMoneyTxt = (UILabelAtlas*) UIHelper::seekWidgetByName(m_Panel,"money_val");
	UILabelAtlas* pExpTxt =  (UILabelAtlas*)UIHelper::seekWidgetByName(m_Panel,"exp_val");
	sprintf(buf,"%d",m_result._money);
	pMoneyTxt->setStringValue(buf);
	sprintf(buf,"%d",m_result._exp);
	pExpTxt->setStringValue(buf);
	switch(m_panelType){
	case enum_panel_task:
		{
			pWinWidget->setVisible(false);
			pTaskDone->setVisible(true);
		}
		break;
	case enum_panel_game:
		{
			pWinWidget->setVisible(true);
			pTaskDone->setVisible(false);
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

void KGameResultPanel::onQuestFinished(int qId)
{
	KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(qId);
	if(!pQuest) return;
	GameResult result;
	result._money = pQuest->m_money;
	result._exp = pQuest->m_exp;
	ShowPanel(result,enum_panel_task);
}

