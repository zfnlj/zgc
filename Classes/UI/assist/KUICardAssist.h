#ifndef _KUICARDASSIST_H_
#define _KUICARDASSIST_H_

#include "../BaseClasses/UIWidget.h"
#include "UILayer.h"
#include "../../GameLogic/KCardInst.h"
#include "../../PlayerCard/KPlayerDeck.h"
#include "../../Item/KPlayerTmpBag.h"

#define  ADVICE_TURN_END 3
#define  ADVICE_ENTER_OR_CAST 4
class BattleFieldScene;
class KCardInst;
class KAction;
class KCardActor;
class FBattleGuy;
class KActor;
class KSceneLayerBase;
class KGameRecordPanel;
struct K3DActionParam;
USING_NS_CC;
USING_NS_CC_EXT;
class KUICardAssist
{
public:
	static void SetRaceIcon(UIWidget* widgetRace, int race);
	static UIWidget* _createMiniHero(KHeroDef& hero);
	static UIWidget* _createMiniCard(int cardId,int num);
	static void UpdateMiniCardWidgetNum(UIWidget* ui,int num);
	static void _showHeroSkill(UIWidget* widget,const KHeroDef& heroDef);
	static UIWidget*  _createHero(const KHeroDef& heroDef,bool bBig,KCardInst* card = NULL);
	static UIWidget* _createCardLayout(KCardStatic* pST,bool bBig,KCardInst* card = NULL);
	static void _hideBufIcon(UIWidget* panel);
	static void _updateBufIcon(UIWidget* widget , KCardInst* card);
};

#endif // __HELLOWORLD_SCENE_H__	
