#ifndef _VIRTUALSERVICE_H
#define _VIRTUALSERVICE_H
#include "System/Singleton.h"

#include "ClientLogic/KZGThread.h"
#include "Quest/KQuestOperation.h"


class VirtualService :  public Singleton<VirtualService> 
{
public :
	static VirtualService& getSingleton(void);
	void init();

	//quest

	bool SubmitQuest(int qid);

	bool AddItem(int itemId,int count);
	bool StartQuestBattle();
private :
	void SendPlayerAllInitPacket();
	void SendMoney();
	void LoadPlayerData();
	void RegEvt();
	KZGThread m_zgThread;

};

#endif