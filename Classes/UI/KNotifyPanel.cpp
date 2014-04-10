#include "KNotifyPanel.h"

#include "../GameLogic/KDynamicWorld.h"
#include "../GameLogic/KCardInst.h"
#include "KCardActor.h"
#include "../Facade/FBattleGuy.h"
#include "KUIAssist.h"
#include "GameRoot.h"
#include "UIHelper.h"

using namespace cocos2d::extension;

KNotifyPanel::KNotifyPanel()
{
}

void KNotifyPanel::init(cocos2d::extension::UIWidget* notify)
{
	m_notify = notify;
	m_turnActor.init(m_notify);
}

void KNotifyPanel::breathe(float dt)
{
	m_turnActor.update(dt);
}

void KNotifyPanel::onTurnBegin(bool myTurn)
{
	if(myTurn){
		m_turnActor.GetActionMgr().PlayAction("my_turn_begin");
	}else{
		m_turnActor.GetActionMgr().PlayAction("your_turn_begin");
	}
}