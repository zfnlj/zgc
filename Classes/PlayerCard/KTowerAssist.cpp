#include "KTowerAssist.h"
#include "CommonLogic/KDBDataStruct.h"
#include "../common/KCommonObj.h"
#include "../StaticTable/KGameStatic.h"
#include "../WorldObject/KMainPlayer.h"
#include "../WorldObject/KPlayer.h"
#include "../Quest/KQuestManager.h"
namespace KTowerAssist
{

void _reset(tb_player_record* record)
{
	record->_gameData._tower.Reset();
	record->updateMask(tb_player_record::_CRI);
}

void _stepOn(tb_player_record* record)
{
	record->_gameData._tower._lev++;
	record->_gameData._tower._strong++;
	record->_gameData._tower._strong += g_rnd.GetRandom(0,2);
	record->_gameData._tower._fate += g_rnd.GetRandom(0,5);
	record->_gameData._tower._lucky +=g_rnd.GetRandom(0,4);
	if(record->_gameData._tower._lucky>100) record->_gameData._tower._lucky = 100;
	record->_gameData._tower._resLucky +=g_rnd.GetRandom(0,6);
	if(record->_gameData._tower._resLucky>100) record->_gameData._tower._resLucky = 100;
	record->updateMask(tb_player_record::_CRI);
}

bool _isShow(tb_player_record* record)
{
	return record->_gameData._tower._bShow>0;
}

int _getPos(tb_player_record* record)
{
	return record->_gameData._tower._pos;
}

int _getLayer()
{
	tb_player_record* record = KMainPlayer::RealPlayer()->GetPlayerRecord();
	return record->_gameData._tower._lev;
}

void _create(tb_player_record* record)
{
	record->_gameData._tower.Reset();
	record->_gameData._tower._pos = 1 + g_rnd.GetRandom(0,record->_gameData.GetDailyStageLev());
	record->_gameData._tower._lev = 1; 
	record->_gameData._tower._strong = 2;
	record->updateMask(tb_player_record::_CRI);
}

bool _active(tb_player_record* record,int curLayer)
{
	if(_isShow(record)) return false;
	if(record->_gameData._tower.GetLev()==0){ //
		_create(record);
		
	}
	int rnd = g_rnd.GetRandom(0,100);
	if(record->_gameData._tower.GetPos()==curLayer &&
		rnd < 25 + record->_gameData.GetDailyStageLev()*4 ){
		record->_gameData._tower._bShow = true;
		record->updateMask(tb_player_record::_CRI);
		return true;
	}else{
		return false;
	}
}

void _deactive(tb_player_record* record)
{
	record->_gameData._tower.Reset();
	record->updateMask(tb_player_record::_CRI);
}

int _calcMoney(tb_player_record* record)
{
	int towerLev = record->_gameData._tower.GetLev();
	float money = KGameStaticMgr::getSingleton().GetTowerLevMoney(towerLev);
	money = money *(1.0f + (float)record->_gameData._tower._pos *0.05f);
	return (int)money;
}

int _calcFateStone(tb_player_record* record)
{
	int towerLev = record->_gameData._tower.GetLev();
	float stone = KGameStaticMgr::getSingleton().GetTowerLevFateStone(towerLev);
	stone = stone *(1.0f + (float)record->_gameData._tower._pos *0.05f);
	return (int)stone;
}

int _calcExp(tb_player_record* record)
{
	return (float)_calcMoney(record)*0.5f;
}

KDeckDefStatic* _createDeckDef(KDeckDefStatic* pDeck)
{
	tb_player_record* record = KMainPlayer::RealPlayer()->GetPlayerRecord();
	static KDeckDefStatic towerDeckDef;
	towerDeckDef.clone(pDeck);
	towerDeckDef.AddHeroStrong(record->_gameData._tower._strong);
	towerDeckDef.AddHeroLucky(record->_gameData._tower._lucky);
	towerDeckDef.AddResLucky(record->_gameData._tower._resLucky);
	towerDeckDef.AddFate(record->_gameData._tower._fate);
	return &towerDeckDef;
}

void _win(int questId)
{
	tb_player_record* record = KMainPlayer::RealPlayer()->GetPlayerRecord();
	KQuestNew* pTowerQuest = KQuestManager::GetInstance()->GetQuest(questId);
	if(!pTowerQuest) return;
	KPlayerTmpBag* pTmpBag = KMainPlayer::RealPlayer()->GetResultBag();
	pTmpBag->Reset();
	int money = _calcMoney(record);
	int exp = _calcExp(record);
	int fateStone = _calcFateStone(record);
	pTmpBag->AddMoney(money);
	pTmpBag->AddExp(exp);
	pTmpBag->AddFateStone(fateStone);
	record->_money += money;
	record->_exp += exp;
	record->_fateStone += fateStone;
	_stepOn(record);
}

}