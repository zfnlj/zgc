#include "KGameRecordPanel.h"
#include "../GameRecord/KGameRecordMgr.h"
#include "assist/KJsonDictMgr.h"
#include "../GameRoot.h"

using namespace cocos2d::extension;

KGameRecordPanel::KGameRecordPanel():m_panel(NULL)
{
}

KGameRecordPanel::~KGameRecordPanel()
{
}

void KGameRecordPanel::init(cocos2d::extension::UILayer* layer)
{
	m_panel = KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/record_panel.json");

	
	UIHelper::seekWidgetByName(m_panel,"but_rec_play")->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickPlay));
	UIHelper::seekWidgetByName(m_panel,"but_rec_rec")->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickRecord));

	UIWidget* pBut = UIHelper::seekWidgetByName(m_panel,"but_rec_stop");
	pBut->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickStop));
	pBut->setVisible(false);

	CCPoint pt = layer->getWidgetByName("rec_panel_pos")->getPosition();
	m_panel->setPosition(pt);
	m_panel->setAnchorPoint(CCPoint(0.5f,0.5f));
	if(GameRoot::getSingleton().m_bRecMode) layer->addWidget(m_panel);
}

void KGameRecordPanel::DoClickPlay(CCObject* sender)
{
	if(!GameRoot::getSingleton().m_bRecMode) return;

	UIHelper::seekWidgetByName(m_panel,"but_rec_play")->setVisible(false);
	UIHelper::seekWidgetByName(m_panel,"but_rec_rec")->setVisible(false);
	UIHelper::seekWidgetByName(m_panel,"but_rec_stop")->setVisible(true);
	KGameRecordMgr::getSingleton().StartPlay("abc");
}

void KGameRecordPanel::DoClickRecord(CCObject* sender)
{
	if(!GameRoot::getSingleton().m_bRecMode) return;

	UIHelper::seekWidgetByName(m_panel,"but_rec_play")->setVisible(false);
	UIHelper::seekWidgetByName(m_panel,"but_rec_rec")->setVisible(false);
	UIHelper::seekWidgetByName(m_panel,"but_rec_stop")->setVisible(true);
	KGameRecordMgr::getSingleton().StartRecord();
}

void KGameRecordPanel::DoClickStop(CCObject* sender)
{
	if(!GameRoot::getSingleton().m_bRecMode) return;

	UIHelper::seekWidgetByName(m_panel,"but_rec_play")->setVisible(true);
	UIHelper::seekWidgetByName(m_panel,"but_rec_rec")->setVisible(true);
	UIHelper::seekWidgetByName(m_panel,"but_rec_stop")->setVisible(false);
	KGameRecordMgr::getSingleton().Stop();
}