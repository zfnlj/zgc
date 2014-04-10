#include "KLogAssist.h"
#include "../KBattleGuy.h"
#include "../../Inc/KTypeDef.h"
namespace KLogAssist
{
	void _turnBeginLog(KBattleGuy* guy)
	{
#ifdef _USE_COCOS2DX
		CCLog("TurnBegin");
		std::string str = "Hand Card=";
		KCardInstList* lst = guy->GetDeck().QueryCardSet(KCardInst::enum_slot_hand);
		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
			KCardInst* card = *it;
			str += card->GetST()->GetName();
			str +=",";
		}

		CCLog("%s",str.c_str());

		str = "Fight Card=";
		lst = guy->GetDeck().QueryCardSet(KCardInst::enum_slot_fight);
		for(KCardInstList::iterator it = lst->begin();it!=lst->end();++it){
			KCardInst* card = *it;
			str += card->GetST()->GetName();
			str +=",";
		}

		CCLog("%s",str.c_str());
#endif
	}
}