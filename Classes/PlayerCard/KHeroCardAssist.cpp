#include "KHeroCardAssist.h"
#include "../common/KPlayerBagAssist.h"

namespace KHeroCardAssist
{

bool _LevUp(KHeroDef* hero,KPlayer* player)
{
	if(hero->IsLevMax()){
		return false;
	}
	int needChipNum = hero->GetLevUpStoneNum();
	if(!KPlayerBagAssist::ConsumeItem(player,hero->GetCardId()*10,needChipNum)) return false;
	//m_pHeroDef->LevZero();
	hero->LevUp();
	player->m_cardDepot.SaveHero(hero);
	return true;
}

bool _SkillLevUp(KHeroDef* hero,int index,KPlayer* player)
{
	int needMoney = hero->GetSkillLevUpMoney(index);
	if(!KPlayerBagAssist::ConsumeMoney(player,needMoney)) return false;
	hero->SkillLevUp(index);
	player->m_cardDepot.SaveHero(hero);
}

}