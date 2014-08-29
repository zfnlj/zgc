#ifndef _KSERIALIZE_H__
#define _KSERIALIZE_H__
#include "System/KType.h"
#include "KCardInst.h"
class KBattleCtrlBase;
class KMemoryStream;
class KBattleDeck;
class KSerialize	 
{
public:
	KSerialize(){}
	~KSerialize(){}
	
	//KBattleCtrlBase*
	static void deserializeDirty(KBattleCtrlBase* ctrl,UINT64 guyId,KMemoryStream* si);
	static void deserializeBattleGuy(KBattleCtrlBase* ctrl,UINT64 guyId,KMemoryStream* si,bool bMainPlayer);
	static size_t serialize(KBattleCtrlBase* ctrl,KMemoryStream* so);
	static bool deserialize(KBattleCtrlBase* ctrl,KMemoryStream* si);
	static size_t serializeAll(KBattleCtrlBase* ctrl,KMemoryStream* so);
	static bool deserializeAll(KBattleCtrlBase* ctrl,KMemoryStream* si);

	//deck 
	static size_t serializeDirty(KBattleDeck* deck,KMemoryStream* so);
	static BOOL deserializeDirty(KBattleDeck*,KMemoryStream* si);
	static size_t serialize(KBattleDeck*,KMemoryStream* so);
	static size_t serializeCardList(KBattleDeck*,KMemoryStream* so,KCardInstList& lst,bool newCard=false);
	static BOOL deserialize(KBattleDeck*,KMemoryStream* si);
	static BOOL deserializeCardList(KBattleDeck*,KMemoryStream* si,KCardInstList& lst);


	//battleGuy
	 static size_t serialize(KBattleGuy*,KMemoryStream* so);
	 static BOOL deserialize(KBattleGuy*,KMemoryStream* si);
	 static size_t serializeDirty(KBattleGuy*,KMemoryStream* so);
	 static BOOL deserializeDirty(KBattleGuy*,KMemoryStream* si);
};

#endif // __HELLOWORLD_SCENE_H__
