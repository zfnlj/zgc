#include "KGameSettingPanel.h"
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
#include "../StaticTable/StaticData.h"


using namespace cocos2d::extension;
using namespace KItemAbout;

#define SELECT_ITEM_TAG_BASE 10000
KGameSettingPanel::KGameSettingPanel():m_Panel(NULL)
{
}

KGameSettingPanel::~KGameSettingPanel()
{
	CC_SAFE_RELEASE_NULL(m_Panel);
}
void KGameSettingPanel::init(cocos2d::extension::UILayer* layer)
{
	UIWidget* pBut;
	if(!m_Panel){
		m_Panel =KJsonDictMgr::getSingleton().widgetFromJsonFile("GUI/GameSetting.json");
		CC_SAFE_RETAIN(m_Panel);
		pBut = UIHelper::seekWidgetByName(m_Panel, "but_close");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickClose));


		pBut = UIHelper::seekWidgetByName(m_Panel, "sound_check");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickSoundCheck));

		pBut = UIHelper::seekWidgetByName(m_Panel, "music_check");
		pBut->addPushDownEvent(this, coco_pushselector(KGameSettingPanel::DoClickMusicCheck));


		UISlider* pSlider = (UISlider*)UIHelper::seekWidgetByName(m_Panel, "sound_slider");
		pSlider->addEventListenerSlider(this, sliderpercentchangedselector(KGameSettingPanel::DoClickSoundVol));

		pSlider = (UISlider*)UIHelper::seekWidgetByName(m_Panel, "music_slider");
		pSlider->addEventListenerSlider(this, sliderpercentchangedselector(KGameSettingPanel::DoClickMusicVol));
	}
	m_layer = layer;
	m_Panel->setZOrder(999);
	UpdatePanel();
}

void KGameSettingPanel::UpdatePanel()
{
	int soundOn = STATIC_DATA_INT("Sound On");
	int musicOn = STATIC_DATA_INT("Music On");
	int soundVol = STATIC_DATA_INT("Sound Vol");
	int musicVol = STATIC_DATA_INT("Music Vol");

	UICheckBox* pCheckSound = (UICheckBox*)UIHelper::seekWidgetByName(m_Panel,"sound_check");
	if(pCheckSound) pCheckSound->setSelectedState(soundOn>0);

	UICheckBox* pCheckMusic = (UICheckBox*)UIHelper::seekWidgetByName(m_Panel,"music_check");
	if(pCheckMusic) pCheckMusic->setSelectedState(musicOn>0);

	UISlider* pSoundSlider = (UISlider*)UIHelper::seekWidgetByName(m_Panel,"sound_slider");
	if(pSoundSlider) pSoundSlider->setPercent(soundVol);

	UISlider* pMusicSlider = (UISlider*)UIHelper::seekWidgetByName(m_Panel,"music_slider");
	if(pMusicSlider) pMusicSlider->setPercent(soundVol);
}

void KGameSettingPanel::ShowPanel()
{
	if(!m_Panel->getParent()) m_layer->addWidget(m_Panel);
	m_Panel->setScale(0.1f);
	m_Panel->setAnchorPoint(ccp(0.5,0.5));
	CCActionInterval*  actionBy = CCScaleTo::create(0.3f, 1, 1);
	m_Panel->runAction(actionBy);
	m_Panel->setPosition(KUIAssist::_getScreenCenter());
}


void KGameSettingPanel::DoClickClose(CCObject* sender)
{
	m_Panel->removeFromParent();
}


void KGameSettingPanel::DoClickSoundCheck(CCObject* sender)
{
	UICheckBox* pCheckSound = (UICheckBox*)UIHelper::seekWidgetByName(m_Panel,"sound_check");
	if(pCheckSound) {
		STATIC_DATA_SET("Sound On",!pCheckSound->getSelectedState());
		KUIAssist::PlayClickButSound();
	}
}

void KGameSettingPanel::DoClickMusicCheck(CCObject* sender)
{
	UICheckBox* pCheckMusic = (UICheckBox*)UIHelper::seekWidgetByName(m_Panel,"music_check");
	if(pCheckMusic){
		pCheckMusic->getSelectedState();
		STATIC_DATA_SET("Music On",!pCheckMusic->getSelectedState());
		KUIAssist::PlayBGM();
	}
}

void KGameSettingPanel::DoClickSoundVol(CCObject *pSender, SliderEventType type)
{
	switch (type)
    {
        case cocos2d::extension::SLIDER_PERCENTCHANGED:
			{
				UISlider* pSoundSlider = dynamic_cast<UISlider*>(pSender);
				if(pSoundSlider){
					int val = pSoundSlider->getPercent();
					STATIC_DATA_SET("Sound Vol",val);
					KUIAssist::PlayClickButSound();
				}
			}
			break;
		default:
            break;
    }

}

void KGameSettingPanel::DoClickMusicVol(CCObject *pSender, SliderEventType type)
{
    switch (type)
    {
        case cocos2d::extension::SLIDER_PERCENTCHANGED:
			{
				UISlider* pMusicSlider = dynamic_cast<UISlider*>(pSender);
				if(pMusicSlider){
					int val = pMusicSlider->getPercent();
					STATIC_DATA_SET("Music Vol",val);
				}
				KUIAssist::PlayBGM();
			}
			break;
		 default:
			break;
    }
}

bool KGameSettingPanel::IsShow()
{
	return (m_Panel->getParent()!=NULL);
}