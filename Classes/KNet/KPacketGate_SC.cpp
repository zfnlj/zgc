#include "KPacketGate_SC.h"
#include "KClientGateListener.h"
#include <System/Log/log.h>
#include <System/Misc/KStream.h>
#include "cocos2d.h"
#include "../GameRoot.h"
#include "KUserData.h"
#include "KNetMsgFacade.h"
#include "../UI/BattleFieldScene.h"
#include "../GameLogic/KSerialize.h"
USING_NS_CC;
KPacketGate_SC::KPacketGate_SC()
{
	memset(&m_methods, 0 , sizeof(m_methods));
	this->Register(s_nSC_Connected,		&KPacketGate_SC::Process_SC_Connected);
	this->Register(s_nSC_ConnClosed,	&KPacketGate_SC::Process_SC_ConnClosed);
	this->Register(s_nSC_ConnError,		&KPacketGate_SC::Process_SC_ConnError);
	this->Register(s_nSC_PingAck,		&KPacketGate_SC::Process_SC_PingAck);
	this->Register(s_nSC_PlayerLogin_Rsp, &KPacketGate_SC::Process_SC_PlayerLogin_Rsp);
	this->Register(s_nSC_ChangeScene, &KPacketGate_SC::Process_SC_ChangeScene);
	

	//卡牌战斗
	this->Register(s_nSC_BattleInit, &KPacketGate_SC::Process_SC_BattleInit);
	this->Register(s_nSC_BattleCtrlInfo, &KPacketGate_SC::Process_SC_BattleCtrlInfo);
	this->Register(s_nSC_Battle_TurnBegin,&KPacketGate_SC::Process_SC_BattleTurnBegin);
	this->Register(s_nSC_Battle_SelectHandCardOK,&KPacketGate_SC::Process_SC_BattleSelectHandCardOK);
	this->Register(s_nSC_Battle_WorldUpdate,&KPacketGate_SC::Process_SC_BattleWorldUpdate);
	this->Register(s_nSC_Battle_CardMove,&KPacketGate_SC::Process_SC_BattleCardMove);
	this->Register(s_nSC_CardDuelResult,&KPacketGate_SC::Process_SC_BattleCardDuelResult);
	this->Register(s_nSC_UseAbilityResult,&KPacketGate_SC::Process_SC_BattleUseAbilityResult);
	this->Register(s_nSC_BattleDrawCard,&KPacketGate_SC::Process_SC_BattleDrawCard);
	this->Register(s_nSC_Battle_Card2Tomb,&KPacketGate_SC::Process_SC_BattleCard2Tomb);
	this->Register(s_nSC_BattleSummonCard,&KPacketGate_SC::Process_SC_BattleSummonCard);
	
	//quest
	this->Register(s_nSC_SyncAvailQuests,&KPacketGate_SC::Process_SC_SyncAvailQuests);

	//store
	this->Register(s_nSC_BagOperation,&KPacketGate_SC::Process_SC_BagOperation);

	//////////////////////////////////////////////////////////////////////
	this->Register(s_nSC_MessageBox, &KPacketGate_SC::Process_SC_MessageBox);
	this->Register(s_nSC_PlayerAppear, &KPacketGate_SC::Process_SC_PlayerAppear);
	this->Register(s_nSC_PlayerDisappear, &KPacketGate_SC::Process_SC_PlayerDisappear);
}

KPacketGate_SC::~KPacketGate_SC()
{

}

BOOL KPacketGate_SC::Register(int cmd, ProcMethod method)
{
	if(cmd < CommandBegin) return FALSE;
	if(cmd >= CommandEnd)  return FALSE;
	m_methods[cmd-CommandBegin] = method;
	return TRUE;
}

void KPacketGate_SC::Process(KGameSocket* pSock, const void* pPacket)
{
	const JgPacketHead* pHead = (const JgPacketHead*)pPacket;

	const void* pData = pHead + 1;
	int command = pHead->Command();
	int dataLength = pHead->Length();

	if(command < CommandBegin || command >= CommandEnd)
		return;

	ProcMethod method = m_methods[command - CommandBegin];
	if(!method ) return;

	(this->*method)(pSock, pData, dataLength);
}

void KPacketGate_SC::Process_SC_Connected(KGameSocket* pSock, const void* pData, int len)
{
	CCLog("SC_Connected %s", pSock->ToString().c_str());

	CS_Ping packet;
	packet.localSendTime = GetTickCount();
	pSock->Send(s_nCS_Ping, &packet, sizeof(packet));

	CS_ClientReady clientReady;
	memset(&clientReady.mac, 0, sizeof(clientReady.mac));
	pSock->Send(s_nCS_ClientReady, &clientReady, sizeof(clientReady));
}

void KPacketGate_SC::Process_SC_ConnError(KGameSocket* pSock, const void* pData, int len)
{
	CCLog("SC_ConnError %s", pSock->ToString().c_str());
}

void KPacketGate_SC::Process_SC_ConnClosed(KGameSocket* pSock, const void* pData, int len)
{
	CCLog("SC_ConnClosed %s", pSock->ToString().c_str());
	if(!pSock->IsReady()) pSock->m_shouldRemove = TRUE;
	pSock->m_clientRelease = FALSE;
}

void KPacketGate_SC::Process_SC_PingAck(KGameSocket* pSock, const void* pData, int len)
{
	DWORD now = GetTickCount();
	SC_PingAck* pingAck = (SC_PingAck*)pData;
}

void KPacketGate_SC::Process_SC_PlayerLogin_Rsp(KGameSocket* pSock, const void* pData, int len)
{
	SC_PlayerLogin_Rsp& packet = *(SC_PlayerLogin_Rsp*)pData;
	CCLog("SC_PlayerLogin_Rsp result:%d", packet.result);
	if(packet.m_mapId!=2) KClientGateListener::getSingleton().EnterGameLobby();
}

void KPacketGate_SC::Process_SC_ChangeScene(KGameSocket* pSock, const void* pData, int len)
{
	SC_ChangeScene& rsp = *(SC_ChangeScene*)pData;
}

void KPacketGate_SC::Process_SC_MessageBox(KGameSocket* pSock, const void* pData, int len)
{
}

void KPacketGate_SC::Process_SC_PlayerAppear(KGameSocket* pSock, const void* pData, int len)
{
}

void KPacketGate_SC::Process_SC_PlayerDisappear(KGameSocket* pSock, const void* pData, int len)
{
}

void KPacketGate_SC::Process_SC_BattleCtrlInfo(KGameSocket* pSock, const void* pData, int len)
{
	KMsgInputStream si(pData,len);
	KSerialize::deserialize(GameRoot::getSingleton().BattleCtrl(),&si);
}

void KPacketGate_SC::Process_SC_BattleInit(KGameSocket* pSock, const void* pData, int len)
{

	KMsgInputStream si(pData,len);
	int playerNum;
	if(!si.ReadInt(playerNum))
		return;
	for(int i=0;i<playerNum;i++){
		UINT64 id;
		if(!si.ReadUint64(id))
			return;
		PlayerData* player = KUserData::GetInstancePtr()->get(0);
		bool bMainPlayer =false;
		if(player&& player->playerID==id) bMainPlayer = true;
		KSerialize::deserializeBattleGuy(GameRoot::getSingleton().BattleCtrl(),id,&si,bMainPlayer);
	}
	KNetMsgFacade::onBattleInit();
}

void KPacketGate_SC::Process_SC_BattleTurnBegin(KGameSocket* pSock, const void* pData, int len)
{
	KNetMsgFacade::onTurnBegin(pData);
}

void KPacketGate_SC::Process_SC_BattleSelectHandCardOK(KGameSocket* pSock, const void* pData, int len)
{
	SC_BattleSelectHandCardOK* rsp = (SC_BattleSelectHandCardOK*)pData;

	KNetMsgFacade::onSelectHandCardOK(rsp->playerID);
}

void KPacketGate_SC::Process_SC_BattleWorldUpdate(KGameSocket* pSock, const void* pData, int len)
{
	KMsgInputStream si(pData,len);
	int playerNum;
	if(!si.ReadInt(playerNum))
		return;
	for(int i=0;i<playerNum;i++){
		UINT64 id;
		if(!si.ReadUint64(id))
			return;
		KSerialize::deserializeDirty(GameRoot::getSingleton().BattleCtrl(),id,&si);
	}
	GameRoot::getSingleton().getBattleScene()->onUseRes();
}

void KPacketGate_SC::Process_SC_BattleCardMove(KGameSocket* pSock, const void* pData, int len)
{
	SC_Battle_CardMove* packet = (SC_Battle_CardMove*)pData;
	KNetMsgFacade::onCardMove(packet->_realId);
}	

void KPacketGate_SC::Process_SC_BattleCard2Tomb(KGameSocket* pSock, const void* pData, int len)
{
	SC_Battle_Card2Tomb* packet = (SC_Battle_Card2Tomb*)pData;
	KNetMsgFacade::onCard2Tomb(packet->_realId);
}

void KPacketGate_SC::Process_SC_BattleCardDuelResult(KGameSocket* pSock, const void* pData, int len)
{
	SC_CardDuelResult* packet = (SC_CardDuelResult*)pData;
	KNetMsgFacade::onCardDuelResult(packet->_atk,packet->_def,packet->_val1,packet->_val2);
}

void KPacketGate_SC::Process_SC_BattleUseAbilityResult(KGameSocket* pSock, const void* pData, int len)
{
	SC_UseAbilityResult* packet = (SC_UseAbilityResult*)pData;
	KNetMsgFacade::onUseAbilityResult(packet->_src,packet->_des,packet->_abilityId,packet->_val);
}

void KPacketGate_SC::Process_SC_BattleDrawCard(KGameSocket* pSock, const void* pData, int len)
{
	KMsgInputStream si(pData,len);
	int Num;
	if(!si.ReadInt(Num))
		return;
	KCardInstList drawList;
	for(int i=0;i<Num;i++){
		int realId;
		if(!si.ReadInt(realId))
			return;
		KCardInst* card = GameRoot::getSingleton().BattleCtrl()->GetCard(realId);
		if(card) drawList.push_back(card);
	}
	KNetMsgFacade::onBattleDrawCard(&drawList);
}

void KPacketGate_SC::Process_SC_BattleSummonCard(KGameSocket* pSock, const void* pData, int len)
{
	SC_Battle_SummonCard* packet = (SC_Battle_SummonCard*)pData;
	KNetMsgFacade::onBattleSummonCard(packet->_src,packet->_des);
}

void KPacketGate_SC::Process_SC_SyncAvailQuests(KGameSocket* pSock, const void* pData, int len)
{

}

void KPacketGate_SC::Process_SC_BagOperation(KGameSocket* pSock, const void* pData, int len)
{
	KNetMsgFacade::onBagOperation(pData,len);
}