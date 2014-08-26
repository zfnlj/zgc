#ifndef _PLAYERBAGASSIST_H
#define _PLAYERBAGASSIST_H

#include "../WorldObject/KPlayer.h"
namespace KPlayerBagAssist
{
	int GetPlayerItemNum(KWorldObjAbout::KPlayer* player,DWORD itemId);
	bool ConsumeItem(KWorldObjAbout::KPlayer* player,DWORD itemId,int num);
}

#endif // __HELLOWORLD_SCENE_H__
