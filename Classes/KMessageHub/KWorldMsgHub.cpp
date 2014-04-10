#include "KWorldMsgHub.h"
#include "../../Inc/PacketGate/c_game_packet.h"
#include "../GameRoot.h"
#include "../UI/BattleFieldScene.h"
#include "../UI/MainMenuScene.h"

KWorldMsgHub::KWorldMsgHub()
{
	
}

KWorldMsgHub::~KWorldMsgHub()
{

}

void KWorldMsgHub::UseItem(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX

#else
#endif
}

void KWorldMsgHub::GenPlayerCard(unsigned long long Param1, unsigned long long Param2)
{
#ifdef _USE_COCOS2DX
	GameRoot::getSingleton().getMainMenuScene()->onGenPlayerCard(Param1);
#else
	
#endif
}
