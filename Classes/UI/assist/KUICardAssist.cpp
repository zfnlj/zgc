#include "KUICardAssist.h"
#include "KUIAssist.h"
#include "cocos2d.h"
#include "../BattleFieldScene.h"
#include "../MainMenuScene.h"
#include "cocos-ext.h"
#include "../../GameLogic/KDynamicWorld.h"
#include "../../GameLogic/KCardInst.h"
#include "../../Facade/FBattleGuy.h"
#include "../KCardActor.h"
#include "GameRoot.h"
#include "KJsonDictMgr.h"
#include "../../GameRecord/KGameRecordMgr.h"
#include "../StageWaitScene.h"
#include "../CardGroupScene.h"
#include "KPopupLayer.h"
#include "../../common/KUIMsgDef.h"
#include "../KShopScene.h"
#include "../../WorldObject/KMainPlayer.h"
#include "../../WorldObject/KPlayer.h"
#include "../../GameLogic/assist/KSkillAssist.h"

#define SHOW_CARD_OFFSET 10
#define MAX_BUF_SLOT_NUM 5

static char* raceIcon[]={"null","gold","tree","water","fire","mud"};
using namespace cocos2d::extension;

void KUICardAssist::SetRaceIcon(UIWidget* widgetRace, int race)
{
	char sz[32];
	UIImageView* imageWidget = (UIImageView*)widgetRace;

	if(race !=(int)KCardStatic::race_null){
		sprintf(sz,"%s_icon.png",raceIcon[race]);
		imageWidget->loadTexture(sz,UI_TEX_TYPE_PLIST);
		imageWidget->setVisible(true);
	}else{
		imageWidget->setVisible(false);
	}
}

UIWidget* KUICardAssist::_createMiniHero(KHeroDef& hero)
{
	KCardStatic* cardST = KGameStaticMgr::getSingleton().GetCard(hero._cardId);
	if(!cardST) return NULL;
	UIWidget* ui = KJsonDictMgr::getSingleton().CreateMiniCardWidget();
	ui->setAnchorPoint(CCPoint(0.5f,0.5f));

	char sz[64];
	sprintf(sz,"pp_%d.jpg",cardST->GetID());
	UILayout* panel = (UILayout*)ui->getChildByName("bk");
	panel->setBackGroundImage(sz,UI_TEX_TYPE_PLIST);

	UIWidget* widgetIcon = ui->getChildByName("cost_icon");
	widgetIcon->setVisible(false);

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost_val");
	labelCost->setVisible(false);

	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	labelNum->setVisible(false);

	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("card_title");
	sprintf(sz,"t_%d.png",cardST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);


	return ui;
}

UIWidget* KUICardAssist::_createMiniCard(int cardId,int num)
{
	KCardStatic* cardST = KGameStaticMgr::getSingleton().GetCard(cardId);
	if(!cardST) return NULL;

	UIWidget* ui = KJsonDictMgr::getSingleton().CreateMiniCardWidget();
	
	ui->setAnchorPoint(CCPoint(0.5f,0.5f));
	char sz[64];
	sprintf(sz,"pp_%d.jpg",cardST->GetID());
	UILayout* panel = (UILayout*)ui->getChildByName("bk");
	panel->setBackGroundImage(sz,UI_TEX_TYPE_PLIST);

	UIWidget* widgetIcon = ui->getChildByName("cost_icon");
	widgetIcon->setVisible(true);

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost_val");
	sprintf(sz,"%d",cardST->GetCost());
	labelCost->setVisible(true);
	labelCost->setStringValue(sz);

	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	sprintf(sz,"%d",num);
	labelNum->setStringValue(sz);
	labelNum->setVisible((num>1)?true:false);

	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("card_title");
	sprintf(sz,"t_%d.png",cardST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);

	return ui;
}




void KUICardAssist::UpdateMiniCardWidgetNum(UIWidget* ui,int num)
{
	UILabelAtlas* labelNum = (UILabelAtlas*)ui->getChildByName("num_val");
	char sz[64];
	sprintf(sz,"%d",num);
	labelNum->setStringValue(sz);
	labelNum->setVisible((num>1)?true:false);
}

void KUICardAssist::_showHeroSkill(UIWidget* widget,const KHeroDef& heroDef)
{
	float starNum = (float)heroDef.GetLev()/3.0f;
	for(int i=0;i<MAX_HERO_SKILL_NUM;i++){
		char sz[64]={0};
		UILabel* pLabel = (UILabel*)KUIAssist::GetIndexWidget(widget,"hero_skill",i);
		KHeroSkillStatic* skill = KGameStaticMgr::getSingleton().GetHeroSkill(heroDef._skill[i]._id);
		if(heroDef.GetSkillLev(i)>0 && skill){
			sprintf(sz,"%s: lev.%d",skill->GetName(),heroDef.GetSkillLev(i));
			pLabel->setText(sz);
			pLabel->setVisible(true);
		}else{
			pLabel->setVisible(false);
		}
	}
}

void KUICardAssist::_showHeroStar(UIWidget* ui,int starNum)
{
	for(int i=0;i<5;i++)
	{
		UIImageView* widgetSlot = (UIImageView*)KUIAssist::GetIndexWidget(ui,"star_slot",i);
		if(i<starNum){
			widgetSlot->loadTexture("card_star.png",UI_TEX_TYPE_PLIST);
		}else{
			widgetSlot->loadTexture("card_star_gray.png",UI_TEX_TYPE_PLIST);
		}
	}
}

UIWidget*  KUICardAssist::_createHero(const KHeroDef& hero,bool bBig,KCardInst* card)
{
	KCardStatic* pST = KGameStaticMgr::getSingleton().GetCard(hero._cardId);
	if(!pST) return NULL;
	UIWidget* widget = NULL; 
	if(card){
		widget = _createHeroLayout(pST,bBig,card);
	}else{
		KCardInst tmpCard;
		tmpCard.init(0,NULL,hero._cardId);
		tmpCard.CurHpSet(pST->GetHp()+hero.GetStrong());
		tmpCard.SetHeroRank(KSkillAssist::_calcHeroRank(hero.GetLucky(),hero.GetStrong()),hero.GetAtkVal());
		widget = _createHeroLayout(pST,bBig,&tmpCard);
	}
	_showHeroSkill(widget,hero);
	_showHeroStar(widget,hero.GetLev());
	UILabel* labelDesc = (UILabel*)widget->getChildByName("detail");
	labelDesc->setVisible(false);
	return widget;
}

UIWidget* KUICardAssist::_createHeroLayout(KCardStatic* pST,bool bBig,KCardInst* card)
{
	int idx = ((int)pST->GetType())*10 + pST->GetRace();
	KCardLayoutStatic* pLayout = KGameStaticMgr::getSingleton().GetCardLayout(idx);
	if(!pLayout) pLayout =  KGameStaticMgr::getSingleton().GetCardLayout(((int)pST->GetType())*10 + 0);
	UIWidget* ui = KJsonDictMgr::getSingleton().CreateHeroWidget();

	char sz[24];
	UIWidget* atkLabel = ui->getChildByName("atk");
	atkLabel->setVisible(false);

	UIImageView* widgetBg =(UIImageView*)ui->getChildByName("background");
	sprintf(sz,"pic_%d.jpg",pST->GetID());
	if(widgetBg){
		widgetBg->loadTexture(sz,UI_TEX_TYPE_PLIST);
	}
	UIImageView* widgetRace = (UIImageView*)ui->getChildByName("race_icon");

	SetRaceIcon(widgetRace, pST->GetRace());

	UIImageView* widgetMask =(UIImageView*)ui->getChildByName("card_mask");
	if(widgetMask) widgetMask->loadTexture(pLayout->GetMask(),UI_TEX_TYPE_PLIST);
	
	
	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("title");
	sprintf(sz,"t_%d.png",pST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);
	widgetTitle->setScale(0.9f);
	
	UIImageView* widgetStone = (UIImageView*)ui->getChildByName("stone_pos");
	int cardRank = pST->GetRank();
	if(card && card->GetHeroRank()>=0) cardRank = card->GetHeroRank();
	if(cardRank>0){
		sprintf(sz,"stone_%d.png",pST->GetRank());
		widgetStone->loadTexture(sz,UI_TEX_TYPE_PLIST);
		widgetStone->setVisible(true);
	}else{
		widgetStone->setVisible(false);
	}

	UILabel* labelDesc = (UILabel*)ui->getChildByName("detail");
	labelDesc->setVisible(true);
	if(bBig){
		if(strlen(pST->GetDetail())>2){
			labelDesc->setText(pST->GetDetail());
		}else{
			labelDesc->setVisible(false);
		}
	}else{
		if(strlen(pST->GetDesc())>2){
			labelDesc->setText(pST->GetDesc());
		}else{
			labelDesc->setVisible(false);
		}
	}

	UILabelAtlas* labelHp = (UILabelAtlas*)ui->getChildByName("hp");
	if(pLayout->IsShowHp()){
		sprintf(sz,"%d",pST->GetHp());
		labelHp->setStringValue(sz);
		labelHp->setVisible(true);
	}else{
		labelHp->setVisible(false);
	}
	
	ui->setAnchorPoint(ccp(0.5,0.5));
	ui->setPosition(ccp(-500.0f,-500.0f));
	if(!bBig) ui->setScale(0.8f);
	return ui;
}

UIWidget* KUICardAssist::_createCardLayout(KCardStatic* pST,bool bBig,KCardInst* card)
{
	int idx = ((int)pST->GetType())*10 + pST->GetRace();
	KCardLayoutStatic* pLayout = KGameStaticMgr::getSingleton().GetCardLayout(idx);
	if(!pLayout) pLayout =  KGameStaticMgr::getSingleton().GetCardLayout(((int)pST->GetType())*10 + 0);
	UIWidget* ui = KJsonDictMgr::getSingleton().CreateCardWidget();

	char sz[24];
	UIImageView* widgetBg =(UIImageView*)ui->getChildByName("background");
	sprintf(sz,"pic_%d.jpg",pST->GetID());
	if(widgetBg){
		widgetBg->loadTexture(sz,UI_TEX_TYPE_PLIST);
	}
	UIImageView* widgetRace = (UIImageView*)ui->getChildByName("race_icon");

	SetRaceIcon(widgetRace, pST->GetRace());

	UIImageView* widgetMask =(UIImageView*)ui->getChildByName("card_mask");
	if(widgetMask) widgetMask->loadTexture(pLayout->GetMask(),UI_TEX_TYPE_PLIST);
	
	
	UIImageView* widgetTitle = (UIImageView*)ui->getChildByName("title");
	sprintf(sz,"t_%d.png",pST->GetID());
	if(widgetTitle) widgetTitle->loadTexture(sz,UI_TEX_TYPE_PLIST);
	widgetTitle->setScale(0.9f);

	UIWidget* widgetIconCost = ui->getChildByName("icon_cost");
	if(widgetIconCost) widgetIconCost->setVisible(pLayout->IsShowCost());

	UIWidget* widgetIconHp = ui->getChildByName("icon_hp");
	if(widgetIconHp) widgetIconHp->setVisible(pLayout->IsShowHp());

	UIWidget* widgetIconAtk = ui->getChildByName("icon_atk");
	if(widgetIconAtk) widgetIconAtk->setVisible(pLayout->IsShowAtk());

	UILabelAtlas* labelCost = (UILabelAtlas*)ui->getChildByName("cost");
 	if(pLayout->IsShowCost()){
		char info[64]={0};
		sprintf(info,"%d",pST->GetCost());
		labelCost->setStringValue(info);
		labelCost->setVisible(true);
	}else{
		labelCost->setVisible(false);
	}
	UIImageView* widgetStone = (UIImageView*)ui->getChildByName("stone_pos");
	int cardRank = pST->GetRank();
	if(card && card->GetHeroRank()>=0) cardRank = card->GetHeroRank();
	if(cardRank>0){
		sprintf(sz,"stone_%d.png",pST->GetRank());
		widgetStone->loadTexture(sz,UI_TEX_TYPE_PLIST);
		widgetStone->setVisible(true);
	}else{
		widgetStone->setVisible(false);
	}

	UILabel* labelDesc = (UILabel*)ui->getChildByName("detail");
	labelDesc->setVisible(true);
	if(bBig){
		if(strlen(pST->GetDetail())>2){
			labelDesc->setText(pST->GetDetail());
		}else{
			labelDesc->setVisible(false);
		}
	}else{
		if(strlen(pST->GetDesc())>2){
			labelDesc->setText(pST->GetDesc());
		}else{
			labelDesc->setVisible(false);
		}
	}

	UILabelAtlas* labelHp = (UILabelAtlas*)ui->getChildByName("hp");
	if(pLayout->IsShowHp()){
		sprintf(sz,"%d",pST->GetHp());
		labelHp->setStringValue(sz);
		labelHp->setVisible(true);
	}else{
		labelHp->setVisible(false);
	}

	UILabelAtlas* labelAtk = (UILabelAtlas*)ui->getChildByName("atk");
	if(pLayout->IsShowAtk()){
		sprintf(sz,"%d",pST->GetAtk());
		labelAtk->setStringValue(sz);
		labelAtk->setVisible(true);
	}else{
		labelAtk->setVisible(false);
	}
	
	ui->setAnchorPoint(ccp(0.5,0.5));
	ui->setPosition(ccp(-500.0f,-500.0f));
	if(!bBig) ui->setScale(0.8f);
	return ui;
}

void KUICardAssist::_hideBufIcon(UIWidget* panel)
{
	for(int i=0;i<MAX_BUF_SLOT_NUM;i++){
		UIImageView* widgetSlot = (UIImageView*)KUIAssist::GetIndexWidget(panel,"buf_slot",i);
		if(widgetSlot) widgetSlot->setVisible(false);
	}
}

void KUICardAssist::_updateBufIcon(UIWidget* panel , KCardInst* card)
{
    //MAX_BUF_SLOT_NUM;
    KCardBufferList& bufList = card->m_attr.m_bufList;
	KCardBufferList iconBufList;
	for(KCardBufferList::iterator it=bufList.begin();it!=bufList.end();++it){
		KAbilityStatic* pAbility = (*it)._pST;
		if(!pAbility->BufIconEmpty()) iconBufList.push_back(*it);
	}
    KCardBufferList::iterator it = iconBufList.begin();
    int pos = 0;
	while(it != iconBufList.end()){
        if(pos==MAX_BUF_SLOT_NUM) break;
		KAbilityStatic* pAbility = (*it)._pST;
        UIImageView* widgetSlot = (UIImageView*)KUIAssist::GetIndexWidget(panel,"buf_slot",pos);
        if(widgetSlot){
            widgetSlot->loadTexture(pAbility->GetBufIcon(),UI_TEX_TYPE_PLIST);
            widgetSlot->setVisible(true);
        }
		it++;
        pos++;
	}
    for(int i=pos;i<MAX_BUF_SLOT_NUM;i++){
         UIImageView* widgetSlot = (UIImageView*)KUIAssist::GetIndexWidget(panel,"buf_slot",i);
        widgetSlot->setVisible(false);
    }
}