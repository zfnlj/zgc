
#include "KPlayer.h"
#include "../Item/KItemCreate.h"
#include "../Item/KItem.h"
#include "../Item/KBag.h"
#include "../Item/KBagManager.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include "../common/KPlayerRecordAssist.h"
#include "../sqlite/KUserSql.h"
#include "../sqlite/KUserQuestSql.h"
#include "../StaticTable/KGameStatic.h"
#include "CommonLogic/WorldObject/KAttrDefines.h"
#include "../PlayerCard/KTowerAssist.h"

using namespace KItemAbout;
using namespace KAttributeAbout;

namespace KWorldObjAbout
{
	KPlayer::KPlayer(void):m_questOperation(this)
	{
		m_nFlag = wokPlayer;
		m_name[0] = 0;
		m_tribeID = 0;
		m_pBagManager = NULL;
	}

	KPlayer::~KPlayer(void)
	{
		if(m_pBagManager)
		{
			KBagManager::Free(m_pBagManager);
		}
		m_pBagManager = NULL;
	
	}
// 	bool KPlayer::InitPlayerData(const KPlayerInitParam& initParam)
// 	{
// 		float fx = initParam.playerBase.CT_X / 100.0f;
// 		float fy = initParam.playerBase.CT_Y / 100.0f;
// 
// 		m_id = initParam.charID;
// 		m_nFlag = wokPlayer;
// 
// 		m_fX = fx;
// 		m_fY = fy;
// 
// 		return true;
// 	}

	// 处理收到的玩家物品信息
// 	int KPlayer::ItemOperator(const SC_BagOperation* pBO)
// 	{
// 		ASSERT_RETURN(pBO, 0);
// 		switch( pPacRecv->s_bySubCmd )
// 		{
// 		case enumIOR_ADD:
// 			break;
// 		case enumIOR_DELETE:
// 			break;
// 		case enumIOR_MOVE_RESULT:
// 			break;
// 		case enumIOR_UPDATE_ATTRIBUTE:
// 			break;
// 		case enumIOR_UPDATE_ATTRIBUTE_STR:
// 			break;
// 		case enumIOR_EQUIP:
// 			{
// 				const SC_BagEquipmentOn* pBEON = (const SC_BagEquipmentOn*)pBO;
// 				
// 				KBag* pBagEquip = m_BagManager.FindBag(enum_item_BagEquip);
// 				if(NULL == pBagEquip)
// 				{
// 					pBagEquip = m_BagManager.InitOneBagEquip(enum_item_BagEquip);
// 					ASSERT_RETURN(pBagEquip, 0);
// 
// 					pBagEquip->m_nMasterID = m_id;
// 					// TODO: 随便写了一个
// 					pBagEquip->m_nCellMaxCount = 40;
// 					pBagEquip->m_nCellUnLockCount = 40;
// 				}
// 				m_BagManager.EquipmentOn(enum_item_BagEquip, pBEON->bytePos, pBEON->dwItemID, pBEON->byteTrengthenLevel);
// 			}
// 			break;
// 		case enumIOR_UNEQUIP:
// 			{
// 				const SC_BagEquipmentOff* pBEOFF = (SC_BagEquipmentOff *)pBO;
// 				m_BagManager.EquipmentOff(enum_item_BagEquip, pBEOFF->bytePos);
// 			}
// 			break;
// 		}
// 		return 0;
// 	}

	//*****************************************************************************************/
	
	bool KPlayer::Initialize()
	{
		m_questManager.m_pPlayer = this;
		m_pBagManager = KBagManager::Alloc();
		ASSERT_RETURN(m_pBagManager, false);
		m_pBagManager->SetMasterID(m_id);
		m_playerRecord.init();
		m_questRecord.init();
		m_cardDepot.init(&m_playerRecord,this);
		return true;
	}

	void KPlayer::Breathe(int nInterval)
	{
		KCharacter::Breathe(nInterval);
	}

	bool KPlayer::IsKindOf(eWorldObjectKind flag) const
	{
		return (KCharacter::IsKindOf(flag) || flag == wokPlayer);
	}


	int KPlayer::FillExtraAppearData( const void* data, DWORD len )
	{
		return KCharacter::FillExtraAppearData(data, len);
	}


	const char* KPlayer::GetName() const
	{
		if (m_customName.size() > 0) return m_customName.c_str();
		return m_name;
	}

	KItemAbout::KBagManager* KPlayer::GetBagManagerPtr()
	{
		return m_pBagManager;
	}

	void KPlayer::OnLeaveGame(bool onClearWorld /*= false*/)
	{
		KCharacter::OnLeaveGame(onClearWorld);
	}

	void KPlayer::GetAppearData(char* pAppearData, int nBuffLen)
	{
	}

	
	const char* KPlayer::MoneyToRichString() const
	{
		static char buffer[256];
		KMoneyAbout::GoldShellToRichString(m_money.m_money, buffer, sizeof(buffer));
		return buffer;
	}

	const char* KPlayer::GetMoney() const
	{
		static char buffer[64];
		strcpy_k(buffer, sizeof(buffer), m_money.m_money.toString().c_str());
		return buffer;
	}

	void KPlayer::SyncFromRecord()
	{
		m_questManager.m_pPlayer = this;
		m_money.Set(m_playerRecord._money);
		KPlayerRecordAssist::syncBagFromRecord(this,&m_playerRecord);
		KPlayerRecordAssist::syncQuestFromRecord(&m_questManager,&m_questRecord);
	}

	void KPlayer::syncToRecord()
	{
		if(m_playerRecord._mUpdateMask&tb_player_record::_CRI){
			m_playerRecord._money = m_money.m_money;
			KUserSql::UpdateCriVal(GetName(),&m_playerRecord);
			m_playerRecord.updateUnMask(tb_player_record::_CRI);
		}
		
		if(m_playerRecord._mUpdateMask&tb_player_record::_NORMALITEM){
			KPlayerRecordAssist::syncBagToRecord(this,&m_playerRecord);
			m_playerRecord.updateUnMask(tb_player_record::_NORMALITEM);
		}
		if(m_playerRecord._mUpdateMask&tb_player_record::_CARDSTORE){
			KUserSql::UpdateDeckStore(GetName(),&m_playerRecord);
			m_playerRecord.updateUnMask(tb_player_record::_CARDSTORE);
		}

		if(m_playerRecord._mUpdateMask&tb_player_record::_HERODATA){
			KUserSql::UpdateHeroData(GetName(),&m_playerRecord);
			m_playerRecord.updateUnMask(tb_player_record::_HERODATA);
		}
		if(m_playerRecord._mUpdateMask&tb_player_record::_CURDECK){							
			KUserSql::UpdateIntegerVal(GetName(),"curDeck",m_playerRecord._curDeck);
			m_playerRecord.updateUnMask(tb_player_record::_CURDECK);						
		}

	#define CARD_DECK_UPDATE(mask,i)								\
		if(m_playerRecord._mUpdateMask&mask){						\
			KUserSql::UpdateCardDeck(GetName(),&m_playerRecord,i);\
			m_playerRecord.updateUnMask(mask);						\
		}
		CARD_DECK_UPDATE(tb_player_record::_CARDDECK0,0)
		CARD_DECK_UPDATE(tb_player_record::_CARDDECK1,1)
		CARD_DECK_UPDATE(tb_player_record::_CARDDECK2,2)
		CARD_DECK_UPDATE(tb_player_record::_CARDDECK3,3)
		CARD_DECK_UPDATE(tb_player_record::_CARDDECK4,4)


	#define  QUEST_RECORD_ID_UPDATE( mask,fieldName,val)			\
		if(m_questRecord._mUpdateMask&mask){							\
			KUserQuestSql::UpdateIntegerVal(GetName(),fieldName,val);\
			m_questRecord.updateUnMask(mask);						\
		}

		
		

		QUEST_RECORD_ID_UPDATE(tb_playerquest_record::_QID0,"Qid_0",m_questRecord._qid[0])
		QUEST_RECORD_ID_UPDATE(tb_playerquest_record::_QID1,"Qid_1",m_questRecord._qid[1])
		QUEST_RECORD_ID_UPDATE(tb_playerquest_record::_QID2,"Qid_2",m_questRecord._qid[2])

	#define  QUEST_RECORD_STATE_UPDATE( mask,i)			\
		if(m_questRecord._mUpdateMask&mask){				\
			KUserQuestSql::UpdateQuestState(GetName(),i,&m_questRecord);\
			m_questRecord.updateUnMask(mask);				\
		}
		QUEST_RECORD_STATE_UPDATE(tb_playerquest_record::f_Qstate0,0)
		QUEST_RECORD_STATE_UPDATE(tb_playerquest_record::f_Qstate1,1)
		QUEST_RECORD_STATE_UPDATE(tb_playerquest_record::f_Qstate2,2)

		if(m_questRecord._mUpdateMask&tb_playerquest_record::_QHISTORY){
			KUserQuestSql::UpdateQuestHistory(GetName(),&m_questRecord);
			m_questRecord.updateUnMask(tb_playerquest_record::_QHISTORY);
		}

		if(m_questRecord._mUpdateMask&tb_playerquest_record::_QDAILY){
			KUserQuestSql::UpdateQuestDaily(GetName(),&m_questRecord);
			m_questRecord.updateUnMask(tb_playerquest_record::_QDAILY);
		}

	}
	
	int KPlayer::getLevel()
	{ 
		return KGameStaticMgr::getSingleton().PlayerExpToLevel(m_playerRecord.GetExp());
	}	

	void KPlayer::SetQuestGiftSelectPos(int pos)
	{
		SetAttrValue(ca_selectPos,pos);
	}

	void KPlayer::IncreaseCriVal(int money,int exp,int fateStone)
	{
		m_money.Increase(money);
		KPlayerRecordAssist::AddCriVal(&m_playerRecord,money,exp,fateStone);
	}

	KQuestNew* KPlayer::RndQueryAdventureQuest()
	{
		//int num = KPlayerRecordAssist::getDailyAwardNum(&m_questRecord);
		return m_questManager.RndQueryAdventureQuest(true);
	}

	int KPlayer::GetDailyStageLev()
	{
		return m_playerRecord._gameData.GetDailyStageLev();
	}

	bool KPlayer::IsShowTower()
	{
		return KTowerAssist::_isShow(&m_playerRecord);
	}

	int KPlayer::GetCurHeroId()
	{
		KHeroDef hero;
		m_cardDepot.PickCurHero(hero);
		return hero._cardId;
	}
}

