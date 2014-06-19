#include "VirtualService.h"
#include "UI/BattleFieldScene.h"
#include "StaticTable/KGameStatic.h"
#include "common/KClientTabfileLoader.h"
#include "GameLogic/KDynamicWorld.h"
#include "KAction/KActionStaticMgr.h"
#include "KNet/KClientGateListener.h"
#include "StaticTable/StaticData.h"
#include "Store/KStoreCreate.h"
#include "Item/KItemCreate.h"
#include "sqlite/KUserSql.h"
#include "sqlite/KUserQuestSql.h"
#include "StaticTable/StaticData.h"
#include "CommonLogic/KDBDataStruct.h"
#include "WorldObject/KMainPlayer.h"
#include "WorldObject/KPlayer.h"
#include "KNet/KNetMsgFacade.h"
#include "Quest/KQuestManager.h"
#include "GameRoot.h"
#include "common/KVirtualServerAssist.h"
#include "common/KPlayerRecordAssist.h"
using namespace KStoreAbout;
using namespace KItemAbout;
IMPLEMENT_SIMPLE_SINGLETON(VirtualService)

void VirtualService::init()
{
	DEF_STORE_PRODUCT->Initialize();
	KStoreAbout::KStoreCreateInfo* store = DEF_STORE_PRODUCT->GetStoreCreateInfo(1);
	KItemAbout::KItemCreate::Instance()->LoadItemCreateInfo();
	LoadPlayerData();

	char szTmp[1024];
	MAP_STORE_PRODUCT::const_iterator itorProductTmp = store->m_mapStoreProduct.begin();
	for(;itorProductTmp!=store->m_mapStoreProduct.end();++itorProductTmp)
	{
		const KStoreProduct* pProduct = itorProductTmp->second;
		if(NULL == pProduct) continue;
		KDataOutputStream os(szTmp, sizeof(szTmp));
		if(pProduct->Serilize(os, NULL, NULL)){
			KNetMsgFacade::onStoreAddProduct(os.m_pBuf, os.m_pos);
		}
	}
	m_zgThread.Initialize();
	RegEvt();
}

void VirtualService::RegEvt()
{
	KMainPlayer::Instance()->RegEvt();
}

void VirtualService::LoadPlayerData()
{
	KMainPlayer::Instance()->initPlayer(1);
    std::string name="zfn";
    std::string  pwd="111";
	STATIC_DATA_INIT(name.c_str(),pwd.c_str());
	if(KUserSql::LoadUserData(STATIC_DATA_STRING("player"),KMainPlayer::RealPlayer()->GetPlayerRecord())){
		KUserQuestSql::LoadUserQuestData(STATIC_DATA_STRING("player"),KMainPlayer::RealPlayer()->GetQuestRecord());
	}else{
		KUserSql::InsertUserData(STATIC_DATA_STRING("player"));
		KUserQuestSql::InsertUserQuestData(STATIC_DATA_STRING("player"));
	}
	strcpy(KMainPlayer::RealPlayer()->m_name,STATIC_DATA_STRING("player"));
	
	//KUserSql::InsertUserData(STATIC_DATA_STRING("player"));
	
	//KUserQuestSql::UpdateQuest(STATIC_DATA_STRING("player"),1,27,buf,20);

	//初始化背包
	char buffer[1024] = {0};
	KDataOutputStream os(buffer, sizeof(buffer));
	os.WriteByte(KItemAbout::enum_item_BagNormal);					// 请注意：ID要放在最前面
	os.WriteUint64(1);								// Master id
	os.WriteShort(10);									// 最大格子数
	os.WriteShort(10);									// 未锁定格子数
	KNetMsgFacade::onInitOneBag(os.m_pBuf, os.m_pos);

	KMainPlayer::RealPlayer()->SyncFromRecord();
	KQuestManager::GetInstance()->syncAvailQuests();
}

void VirtualService::SendPlayerAllInitPacket()
{
	SendMoney();
}

void VirtualService::SendMoney()
{

}

bool VirtualService::StartQuestBattle()
{
	{ //TMP
		KQuestNew* pQuest = KQuestManager::GetInstance()->GetQuest(10001);
		GameRoot::getSingleton().BattleCtrl().PlayQuestBattle(pQuest);
		return true;
	}
	bool ret = false;
	KPlayerQuestManager& playerQuestManager = KMainPlayer::RealPlayer()->m_questManager;
	int num = playerQuestManager.m_quests.size();
	for(int i=0; i<num; i++)
	{
		KQuestNew* pQuest = playerQuestManager.m_quests[i];
		if(pQuest->GetQuestStatus()==KQ_QuestRuning && pQuest->m_battleField>0){
			GameRoot::getSingleton().BattleCtrl().PlayQuestBattle(pQuest);
			ret = true;
			break;
		}
	}
	return ret;
}

bool VirtualService::AddItem(int itemId,int count)
{
	if(KCreateInfo_ItemBase::IsCardItem(itemId)){
		bool bNew=true;
		return KPlayerRecordAssist::addStoreCard(KMainPlayer::RealPlayer()->GetPlayerRecord(),itemId,count,bNew);
	}else{
		return KVirtualServerAssist::AddItem(itemId,count);
	}
}

bool VirtualService::SubmitQuest(int qid)
{ 
	bool ret = KMainPlayer::RealPlayer()->GetQuestHolder().CanSubmit(qid);
	if(ret){
		KNetMsgFacade::onQuestFinished(qid);
	}
	return ret;
}