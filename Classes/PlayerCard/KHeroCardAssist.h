#ifndef _KHEROCARDASSIST_H
#define _KHEROCARDASSIST_H

#include "KPlayerDeck.h"
#include "../WorldObject/KPlayer.h"
namespace KHeroCardAssist
{
	bool _LevUp(KHeroDef* hero,KPlayer* player);
	bool _SkillLevUp(KHeroDef* hero,int index,KPlayer* player);
	bool _LevUpFate(KHeroDef* hero,KPlayer* player);
}

#endif // 
