
#include "KMainPlayer.h"
#include "KPlayer.h"
//#include "Attribute/KAttrDefine.h"
//#include "Attribute/KStateDefine.h"
#include "System/Algorithm/AngleComputer.h"
#include "../Block/KBlockBoxManager.h"
#include "../Block/KBlock.h"
#include "../Item/KItemCreate.h"
#include "System/Algorithm/Distance.h"
#include <math.h>
#include "../Item/KBagManager.h"
#include "../Item/KBag.h"
#include "../sqlite/KUserSql.h"
#include "../sqlite/KUserQuestSql.h"
#include "../common/KPlayerRecordAssist.h"
#include "../GameLogic/KDynamicWorld.h"
#include "../KNet/KNetMsgFacade.h"

//#include "Business/KBusinessManager.h"
//#include "Store/KStoreManager.h"


using namespace KAttributeAbout;
using namespace KUserAssistAbout;
using namespace KSkillAbout;
using namespace KBlockAbout;
using namespace KAIAbout;
using namespace KItemAbout;
using namespace KBusinessAbout;
using namespace KStoreAbout;

#if !defined(max)
#	define max(a,b) (((a)<(b))?(b):(a))
#endif
#if !defined(min)
#	define min(a,b) (((a)<(b))?(a):(b))
#endif

KMainPlayer::KMainPlayer(void)
{
	Reset();
}

KMainPlayer::~KMainPlayer(void)
{
	this->Reset();
}

void KMainPlayer::RegEvt()
{
	KDynamicWorld::getSingleton().RegEvent(KEventAbout::enumWorldEvent_QuestSession,this,0,0);
	m_pRealPlayer->m_questManager.regAllEvent();
}

void KMainPlayer::Reset()
{

	m_pRealPlayer = NULL;
	//m_pRealPlayer->m_questManager.Reset();

	m_periodData.clear();

	if (KBlockBoxManager::GetInstancePtr())
	{
		KBlockBoxManager::GetInstancePtr()->Reset();
	}
}

void KMainPlayer::initPlayer(UINT64 playerId)
{
	m_pRealPlayer = KPlayer::Alloc();
	ASSERT_RETURN_VOID(m_pRealPlayer);

	m_pRealPlayer->m_id = playerId;
	m_pRealPlayer->Initialize();
	
}

void KMainPlayer::Login( const void* pData, int len )
{
	//ASSERT(pData);
	//SC_PlayerLogin* pPL = (SC_PlayerLogin*)pData;
	//if (pPL->m_byResult != SC_PlayerLogin::E_RESULT::SUCCESS)
	//{
	//	// TODO: 出错了，出提示，返回登录界面？
	//	return;
	//}
	//ASSERT(m_pRealPlayer);
	//m_pRealPlayer->SetAttrChangeSensor(this);


	//m_questManager.m_pPlayer = m_pRealPlayer;
}

void KMainPlayer::SetServerTime(const void* pData, int len)
{
	//ASSERT(pData);
	//SC_SyncServerTime* pTime = (SC_SyncServerTime*)pData;
	//TIME_T curTime = SECOND_NOW;
	//m_tServerMistiming = curTime - pTime->tServerTime;
	return ;
}

UINT64 KMainPlayer::GetMainPlayerID()
{
	if (!m_pRealPlayer) return 0;
	return m_pRealPlayer->GetID();
}

void KMainPlayer::UpdateMainPlayerAttr(const void* pAttrPool, int nDataSize)
{
	//KPlayer* pPlayer = m_pRealPlayer;
	//if(m_pRealPlayer == NULL)
	//	return;
	//int nPos = 0;
	//int attrValue;
	//const int nMaxSize = 1024;
	//int attrID[nMaxSize], attrSize[nMaxSize];
	//SC_MainPlayerAttr packet;

	//packet.nSize = nDataSize;
	//memcpy(packet.dataBuffer, pAttrPool, nDataSize);
	//int nCount = KAttributeAbout::KAttrSyncMap::Instance()->GetAllSyncAttr(attrID, attrSize, nMaxSize);
	//for(int i = 0; i < nCount; i++)
	//{
	//	attrValue = packet.GetValue(nPos, attrSize[i]);
	//	int nOldVal = m_pRealPlayer->GetAttrValue(attrID[i]);
	//	m_pRealPlayer->SetAttrValue(attrID[i], attrValue);
	//	// 主角等级发生变化
	//	if(nOldVal != attrValue && KAttributeAbout::ca_Level == attrID[i])
	//	{
	//		KBlockBoxManager::Instance()->OnPlayerLevelChanged();
	//	}
	//}
}

void KMainPlayer::Breathe()
{
	if (!m_pRealPlayer) return;

	KBlockAbout::KBlockBoxManager::Instance()->Breathe();
}

bool KMainPlayer::IsMainPlayer( UINT64 id ) const
{
	ASSERT_RETURN(m_pRealPlayer, false);
	return m_pRealPlayer->m_id == id;
}

void KMainPlayer::KeyboardNotify( int keyVal, int pressInfo )
{
}

void KMainPlayer::Init()
{
	m_accountInfo.Init();
	m_pRealPlayer->m_playerRecord.clearMask();
}

void KMainPlayer::OnLoginOver()
{
	KBlockBoxManager::Instance()->Build(NULL, 0);
	// 	GetMainPlayer()->m_itemPtr = &m_itemMain;
}

void KMainPlayer::TryUseItem(int bagType, int bagPos, float x, float y, int nIndex)
{
	// TODO: 考虑需要二次施法的情形
	KBagManager* pBagMgr = GetBagManagerPtr();
	ASSERT_RETURN_VOID(pBagMgr);
	const KItem* pItem = pBagMgr->GetItem(bagType, bagPos);
	if(NULL == pItem)
	{
		return;
	}

	const KCreateInfo_ItemBase* pCIIB = pBagMgr->GetItemCreateInfo(pItem->GetID());
	const KSkillStaticData* pSSD = NULL;
	DWORD dwItemID = 0;
}

int KMainPlayer::GetOnePiledItemPos( int nBagID, DWORD dwItemID )
{
	KBagManager* pBagMgr = m_pRealPlayer->GetBagManagerPtr();
	ASSERT_RETURN(pBagMgr, -1);
	return pBagMgr->GetFirstPos(nBagID, dwItemID);
}

bool KMainPlayer::IsDead() const
{
	ASSERT_RETURN(m_pRealPlayer, true);
	return m_pRealPlayer->IsDead();
}

KItemAbout::KBagManager* KMainPlayer::GetBagManagerPtr()
{
	ASSERT_RETURN(m_pRealPlayer, NULL);
	return m_pRealPlayer->GetBagManagerPtr();
}

void KMainPlayer::OnWorldClear()
{
	if (m_pRealPlayer)
	{
		m_pRealPlayer->SetAttrChangeSensor(NULL);
		m_pRealPlayer = NULL;
	}
}

void KMainPlayer::ChatToUser(const char* pDstPlayerName, const char* pMsg)
{
	//ASSERT_RETURN_VOID(pDstPlayerName && pMsg);
	//int nMsgUserFlag = KChatAbout::enumCPMUT_PetsWorkshop;

	//char buffer[2048];
	//KDataOutputStream os(buffer, sizeof(buffer));
	//os.WriteByte(nMsgUserFlag);
	//os.WriteString(pDstPlayerName);
	//
	//os.WriteString(pMsg);
	//g_DynamicWorld.SendToGatewayServer(s_nCS_ChatToUser, os.m_pBuf, os.m_pos);
}

void KMainPlayer::OnAttrChange( int idx, int oldVal, int newVal )
{
}


int KMainPlayer::GetPlayerPeriodAttrData( int id, int defval )
{
	/*PeriodDataMap::iterator pit = m_periodData.find(id);
	if ( pit != m_periodData.end() )
	{
		KPeriodRecord& record = pit->second;
		if (SECOND_NOW < record.m_expireTime)
			return record.m_val;
	}*/

	return defval;
}

void KMainPlayer::SetPlayerPeriodAttrData( int id, int val, DWORD expireTime )
{
	PeriodDataMap::iterator pit = m_periodData.find(id);
	if ( pit != m_periodData.end() )
	{
		KPeriodRecord& record = pit->second;
		record.m_val = val;
		record.m_expireTime = expireTime;
	}
	else
	{
		KPeriodRecord record;
		record.m_val = val;
		record.m_expireTime = expireTime;
		m_periodData.insert(id, record);
	}

	return;
}

void KMainPlayer::InitPlayerPeriodAttrData(const void* pBuff, int bSize )
{
	if (!pBuff || !bSize)
	{
		return;
	}
	KMsgInputStream is( (CHAR*)pBuff, bSize);
	int dataSize = 0;
	int dataId = 0;
	int dataVal = 0;
	DWORD expireTime = 0;
	is.ReadInt(dataSize);
	for (int i = 0; i < dataSize; i++)
	{
		ASSERT_RETURN_VOID(is.ReadInt(dataId));
		ASSERT_RETURN_VOID(is.ReadInt(dataVal));
		ASSERT_RETURN_VOID(is.ReadDword(expireTime));
		if (dataId > 0)
		{
			this->SetPlayerPeriodAttrData(dataId, dataVal, expireTime);
		}
	}
	return;
}

bool KMainPlayer::OnEventImp(KEventAbout::KEventID id, const KEventAbout::KEventData* pData, uint_r customData)
{
	if (id == KEventAbout::enumWorldEvent_QuestSession){
		KEventAbout::KEDQuestSession* pQuestSession = (KEventAbout::KEDQuestSession*)pData;
		KPlayerRecordAssist::UpdataQuestSession(&m_pRealPlayer->m_questRecord,pQuestSession->m_pQuest);
	}
	return false;
}

void KMainPlayer::AddMoney(int val)
{
	KNetMsgFacade::onAddMoney(val);
}