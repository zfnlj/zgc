#include "KGameRecordPanel.h"
#include "../GameRecord/KGameRecordMgr.h"

using namespace cocos2d::extension;

KGameRecordPanel::KGameRecordPanel():m_layer(NULL)
{
}

KGameRecordPanel::~KGameRecordPanel()
{
}

void KGameRecordPanel::init(cocos2d::extension::UILayer* layer)
{
	m_layer = layer;

	layer->getWidgetByName("but_rec_play")->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickPlay));
	layer->getWidgetByName("but_rec_rec")->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickRecord));

	UIWidget* pBut = layer->getWidgetByName("but_rec_stop");
	pBut->addPushDownEvent(this, coco_pushselector(KGameRecordPanel::DoClickStop));
	pBut->setVisible(false);
}

void KGameRecordPanel::DoClickPlay(CCObject* sender)
{
	m_layer->getWidgetByName("but_rec_play")->setVisible(false);
	m_layer->getWidgetByName("but_rec_rec")->setVisible(false);
	m_layer->getWidgetByName("but_rec_stop")->setVisible(true);
	KGameRecordMgr::getSingleton().StartPlay("abc");
}

void KGameRecordPanel::DoClickRecord(CCObject* sender)
{
	m_layer->getWidgetByName("but_rec_play")->setVisible(false);
	m_layer->getWidgetByName("but_rec_rec")->setVisible(false);
	m_layer->getWidgetByName("but_rec_stop")->setVisible(true);
	KGameRecordMgr::getSingleton().StartRecord(1);
}

void KGameRecordPanel::DoClickStop(CCObject* sender)
{
	m_layer->getWidgetByName("but_rec_play")->setVisible(true);
	m_layer->getWidgetByName("but_rec_rec")->setVisible(true);
	m_layer->getWidgetByName("but_rec_stop")->setVisible(false);
	KGameRecordMgr::getSingleton().Stop();
}