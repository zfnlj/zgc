#include "KTowerAssist.h"
#include "CommonLogic/KDBDataStruct.h"
#include "../common/KCommonObj.h"

namespace KTowerAssist
{

void _reset(tb_player_record* record)
{
	record->_gameData._tower.reset();
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

}