#include "KTowerAssist.h"
#include "CommonLogic/KDBDataStruct.h"
#include "../common/KCommonObj.h"

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
	int val = g_rnd.GetRandom(0,100);
	if(val <50){
		record->_gameData._tower._strong ++;
	}else if(val<75){
		record->_gameData._tower._lucky +=5;
		if(record->_gameData._tower._lucky>100) record->_gameData._tower._lucky = 100;
	}else{
		record->_gameData._tower._resLucky +=5;
		if(record->_gameData._tower._resLucky>100) record->_gameData._tower._resLucky = 100;
	}
	record->updateMask(tb_player_record::_CRI);
}

bool _isShow(tb_player_record* record)
{
	return record->_gameData._tower._bShow>0;
}

int _getLayer(tb_player_record* record)
{
	return record->_gameData._tower._lev;
}

void _create(tb_player_record* record)
{
	record->_gameData._tower.Reset();
	record->_gameData._tower._lev = 1 + g_rnd.GetRandom(0,record->_gameData.GetDailyStageLev()+1);
}

bool _active(tb_player_record* record,int curLayer)
{
	if(_isShow(record)) return false;
	if(record->_gameData._tower.GetLev()==0){ //
		_create(record);
	}
	int rnd = g_rnd.GetRandom(0,100);
	if(record->_gameData._tower.GetLev()==curLayer &&
		rnd < 30 + record->_gameData.GetDailyStageLev()*2 ){
		record->_gameData._tower._bShow = true;
		return true;
	}else{
		return false;
	}
}

}