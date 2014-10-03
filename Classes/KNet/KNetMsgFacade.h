#pragma once
#include <System/KType.h>

#include "../GameLogic/KCardInst.h"
class KNetMsgFacade
{
public:
	static void onBattleInit();
	static void onTurnBegin(const void* pData);
	static void onSelectHandCardOK(UINT64 playId);
	static void onCardMove(int realId);
	static void onCardDuelResult(int atk,int def,int val1,int val2);
	static void onUseAbilityResult(int atk,int def,int abilityId,int val);
	static void onBattleDrawCard(KCardInstList* list);
	static void onCard2Tomb(int realId);
	static void onBattleSummonCard(int src,int des);
	static void onStoreAddProduct(const void* pData,int len);
	static void onBagOperation(const void* pData,int len);
	static void onInitOneBag(const void* pData,int len);
	static void onChangeMoney(const void* pData, int len);
	static void syncAvailQuests(const void* pData, int len);
	static void onAddMoney(int val);
	//quest
	static void onQuestFinished(int qId);
	static void onCancleQuest(int qId);
	static bool onAcceptQuest(int qId);
};
