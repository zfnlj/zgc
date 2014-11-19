#include "KPlayerGCAssist.h"
#include "../StaticTable/KGameStatic.h"
#include "../../platform/IOSFunc_Bridge.h"

namespace KPlayerGCAssist
{

void updateCardAchieve(tb_player_record* record)
{
	void* buf=NULL;
	int len = record->_cardStore.Get(buf);
	KDBBagItemUnit* dbCard = (KDBBagItemUnit*)buf;
	int n = len/sizeof(KDBBagItemUnit);
	bool bFound = false;
	int myRareCardNum = 0;
	int myGoldCardNum = 0;
	int myWaterCardNum = 0;
	int myFireCardNum = 0;
	for(int i=0;i<n;i++){
		KCardStatic* pCard = KGameStaticMgr::getSingleton().GetCard(dbCard->_id);
		dbCard++;
		if(!pCard) continue;
		if(pCard->GetRank()==4) myRareCardNum++;
		switch(pCard->GetRace()){
		case KCardStatic::race_gold:
			myGoldCardNum++;
			break;
		case KCardStatic::race_fire:
			myFireCardNum++;
			break;
		case KCardStatic::race_water:
			myWaterCardNum++;
			break;
		}
	}
	int totalRareCardNum = KGameStaticMgr::getSingleton().GetRankCardNum(4);
	if(totalRareCardNum==myRareCardNum){
		IOSFunc_Bridge::_ReportAchievement("card_col_rare",100.0f,true);
	}else{
		IOSFunc_Bridge::_ReportAchievement("card_col_rare",(double)myRareCardNum*100/(double)totalRareCardNum,true);
	}

	int totalGoldNum = KGameStaticMgr::getSingleton().GetCardNum(KCardStatic::race_gold);
	if(totalGoldNum==myGoldCardNum){
		IOSFunc_Bridge::_ReportAchievement("card_col_gold",100.0f,true);
	}else{
		IOSFunc_Bridge::_ReportAchievement("card_col_gold",(double)myGoldCardNum*100/(double)totalGoldNum,true);
	}

	int totalFireNum = KGameStaticMgr::getSingleton().GetCardNum(KCardStatic::race_fire);
	if(totalFireNum==myFireCardNum){
		IOSFunc_Bridge::_ReportAchievement("card_col_fire",100.0f,true);
	}else{
		IOSFunc_Bridge::_ReportAchievement("card_col_fire",(double)myFireCardNum*100/(double)totalFireNum,true);
	}

	int totalWaterNum = KGameStaticMgr::getSingleton().GetCardNum(KCardStatic::race_water);
	if(totalWaterNum==myWaterCardNum){
		IOSFunc_Bridge::_ReportAchievement("card_col_water",100.0f,true);
	}else{
		IOSFunc_Bridge::_ReportAchievement("card_col_water",(double)myWaterCardNum*100/(double)totalWaterNum,true);
	}
	int totalCardNum = KGameStaticMgr::getSingleton().GetCardNum(KCardStatic::race_all);
	if(totalCardNum==n){
		IOSFunc_Bridge::_ReportAchievement("card_col_all",100.0f,true);
	}else{
		IOSFunc_Bridge::_ReportAchievement("card_col_all",(double)n*100/(double)totalCardNum,true);
	}
}

}