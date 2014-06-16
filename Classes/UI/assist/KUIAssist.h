#ifndef _KUIASSIST_H_
#define _KUIASSIST_H_

#include "../BaseClasses/UIWidget.h"
#include "UILayer.h"
#include "../GameLogic/KCardInst.h"
#include "../../PlayerCard/KPlayerDeck.h"

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
class KUIAssist
{
public:
	static cocos2d::CCPoint _queryCardPos(KCardInstList* lst,KCardInst* card);
	static const char* _getBasePosName(KCardInst* card);
	static void _showCardSet(KCardInstList* lst);
	static void _removeCardSet(KCardInstList* lst);
	static void _moveCardSet(KCardInstList* lst,const char* actionname);
	static void _showCard(KCardInst* card);
	static void _showHeroSkill(UIWidget* widget,const KHeroDef& heroDef);
	static void _updateCard(KCardInst* card);
	static UIWidget* GetIndexWidget(UIWidget* panel,const char* baseName,int index);
	static UIWidget* _setVisible(UIWidget* panel,const char* baseName,int pos ,bool);
	static void _moveInWidget(UIWidget* panel,CCPoint& src,CCPoint& des,float elapsed);
	static void _moveOutWidget(UIWidget* panel,CCPoint& src,CCPoint& des,float elapsed);
	static CCAction* _fadeIn(UIWidget* widget,float val);
	static void _setButVisible(UIWidget* but,bool flag);
	static void _loadAnimationData(const char* plistName,const char* animationName,int totalFrame,float delay);
	static CCSprite* CreateAnimationSprite(const char* animationName,bool bLoop=true);
	static void _switch2BattleScene();
	static void _switch2MainMenu();
	static void _switch2StageWaitScene();
	static void _switch2ShopScene();
	static void _switch2CardGroupScene();
	static UIWidget* _createCardLayout(KCardStatic* pST,bool bBig);
	static UIWidget*  _createHero(const KHeroDef& heroDef,bool bBig);
	static void _createAffectAction(int actorId,const char* action,K3DActionParam* param,KAction* parent,int key);
	static CCSize _getRealSize(UIWidget* widget);
	static CCAction* _createAtkMove(UIWidget* widgetSrc,int des,float val);
	static CCParticleSystem* _createParticle(const char* name);
	static KCardActor* _getCardActor(int realId);
	static CCPoint _getScreenCenter();
	static void _soldierShow(KCardInst* card);
    static void _updateBufIcon(UIWidget* widget , KCardInst* card);
	static void _updateSecretIcon(bool bMy,KCardInstList* lst);
	static void _updateCardListBuf(KCardInstList* lst);
	static bool _queryScreenPos(const char* name,cocos2d::CCPoint& pt);
	static cocos2d::CCPoint _querySecretPos(KCardInst* card);
	static cocos2d::CCPoint _querySecretShowPos(KCardInst* card);
	static void _resortHandCardSet(FBattleGuy* guy);
	static void _setActionParamSlot(K3DActionParam* param);
	static cocos2d::CCPoint _queryFighterPos(KCardInst*);
	static bool _IsPlayCardAble();
	static void _resortCardSet(FBattleGuy* guy,int slot);
	static UIWidget* _createMiniCard(int cardId,int num);
	static UIWidget* _createMiniHero(KHeroDef& hero);
	static void UpdateMiniCardWidgetNum(UIWidget* ui,int num);
	static void ShowWidgetArr(UILayer* layer,const char* name,int num,bool flag);
	static void ShowButton(UIWidget* pBut,bool flag);
	static void SetRaceIcon(UIWidget* widgetRace, int race);
	//record 
	static bool _IsValidateSrcCard(KCardInst* card);
	static bool _IsValidateDesCard(KCardInst* card);
	static void _playClickCardAction(KCardInst* card);
	static void _playClickSlotAction(int slot);
	static void _playClickFightArea();
	static void _playClickTurnEnd();
	static void _playClickWidget(const char* widgetName);
	static void _stopClickAction();
	static void _stopAdviceMsg();
	static void _playAdviceMsg(int id);
	static void _playLessonMsg(int id);
	static KSceneLayerBase* _activeSceneLayer();
	static KActor& _activeSceneActor();
	static void _hideBufIcon(UIWidget* panel);
	static void _popNotifyMsg(int dlgId);
	//shader
	static CCGLProgram* _createGrayProgram();
	static KGameRecordPanel& _recordPanel();
	static KSceneLayerBase* _activeScene;
};

#endif // __HELLOWORLD_SCENE_H__	
