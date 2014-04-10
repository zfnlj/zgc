#pragma once

class KBattleMsgHub
{
public:
	KBattleMsgHub();
	virtual ~KBattleMsgHub();

public:
	virtual void BattleInit(unsigned long long Param1, unsigned long long Param2); //牌局开始消息
	virtual void TurnBegin(unsigned long long Param1, unsigned long long Param2);  //回合开始
	virtual void TurnEnd(unsigned long long Param1, unsigned long long Param2);    //回合结束
	virtual void DrawCard(unsigned long long Param1, unsigned long long Param2);   //抓牌
	virtual void CardDuelResult(unsigned long long Param1, unsigned long long Param2); //卡片对打结果
	virtual void UseRes(unsigned long long Param1, unsigned long long Param2); //卡片对打结果
	virtual void CardMove(unsigned long long Param1, unsigned long long Param2); //卡片对打结果
	virtual void UpdateGuyInfo(unsigned long long Param1, unsigned long long Param2);
	virtual void HandCardReady(unsigned long long Param1, unsigned long long Param2);
	virtual void SelectCardOK(unsigned long long Param1, unsigned long long Param2);
	virtual void AbilityResult(unsigned long long Param1, unsigned long long Param2);
	virtual void Card2Tomb(unsigned long long Param1, unsigned long long Param2);
	virtual void Secret2Tomb(unsigned long long Param1, unsigned long long Param2);
	virtual void BattleDirty(unsigned long long Param1, unsigned long long Param2);
	virtual void BattleOpDone(unsigned long long Param1, unsigned long long Param2);
	virtual void SummonCard(unsigned long long Param1, unsigned long long Param2);
	virtual void DeckInit(unsigned long long Param1, unsigned long long Param2);
	virtual void GameEnd(unsigned long long Param1, unsigned long long Param2);
};
