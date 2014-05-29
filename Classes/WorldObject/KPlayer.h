
#pragma once

#include "KCharacter.h"
#include "Inc/KCommonStruct.h"
#include <System/Memory/KStepObjectPool.h>
#include "../Quest/KPlayerQuestManager.h"
#include "CommonLogic/KMoney.h"
#include "../Quest/KQuestOperation.h"
#include "../PlayerCard/KPlayerCardDepot.h"
namespace KItemAbout
{
	class KBagManager;
}


class KMainPlayer;
namespace KWorldObjAbout
{
	class KBattlePet;
	class KPlayer
		: public KCharacter
		, public ::System::Memory::KPortableMemoryPool<KPlayer, 64>
	{
		friend class KMainPlayer;
	public:
		KPlayer(void);
		virtual ~KPlayer(void);
		/**
		** 初始化
		**/
		bool Initialize();

	public:
// 		int ItemOperator(SC_ItemOperation* pRecvData, int nLen);	//处理收到的玩家物品信息
// 		KItemDisplayData GetEquipmentDisplayInfo(int nPos);		//获取装备的显示信息
		KItemAbout::KBagManager* GetBagManagerPtr();

	private:	//物品相关
		KItemAbout::KBagManager* m_pBagManager;

	public:
		virtual int FillExtraAppearData(const void* data, DWORD len);
		/**
		** 获得玩家当前出战战兽ID
		**/
	public:
		virtual void Breathe(int nInterval);
		virtual bool IsKindOf(eWorldObjectKind flag) const;
		virtual const char* GetName() const;
		virtual void OnLeaveGame(bool onClearWorld = false);

		KQuestNew* GetActiveQuest(int qid) { return m_questManager.GetQuest(qid);}
		bool IsQuestEverDone(int qid) { return m_questManager.IsQuestEverDone(qid);}
		void AddExp(int val);
		int GetLev() { return 1;}
		tb_player_record* GetPlayerRecord(){ return &m_playerRecord;}
		tb_playerquest_record* GetQuestRecord(){ return &m_questRecord;}
		void SyncFromRecord();

		void syncToRecord();

		//quest 
		bool AcceptQuest(int qid){return m_questOperation.AcceptQuest(qid);};
		bool AcceptQuestForce(int qid){return m_questOperation.AcceptQuestForce(qid);};
		bool AcceptQuestForce(KQuestNew* pQuest){return m_questOperation._AcceptQuestForce(pQuest);};
		bool CancelQuest(KQuestNew* pQuest){ return m_questOperation.CancelQuest(pQuest);}
		KQuestHolderBase& GetQuestHolder(){ return m_questOperation;}

		void SetQuestGiftSelectPos(int pos);
	private:

	public:
		char m_name[MAX_NAME_DEF];


		/**
		 * 检查是否有足够的钱，如果给负数永远返回true
		 */
		bool HasMoney(int val) const
		{
			return m_money.HasMoney(val);
		}
		bool HasMoneyStr(const char* val) const
		{
			return m_money.HasMoney(val);
		}
		void IncreaseMoney(int val);
		// 将当前的money转成字符串。返回值如果需要保存的话要立刻copy一份保存
		const char* MoneyToRichString() const;

		const char* GetMoney() const;

	
			// 记录玩家的钱数
		KMoneyAbout::KMoney m_money;
		tb_player_record m_playerRecord;
		tb_playerquest_record m_questRecord;
		KPlayerQuestManager m_questManager;
		KQuestHolderBase m_questOperation;
		KPlayerCardDepot m_cardDepot;
private:
		// 部落id
		UINT64 m_tribeID;
	public:
		UINT64 GetTribeID() const { return m_tribeID; }
		void SetTribeID(UINT64 val) { m_tribeID = val; }
		int getLevel();
	public:
		virtual void GetAppearData(char* pAppearData, int nBuffLen);
		// 得到指定技能的等级
	friend class KMainPlayer;
	public:
		BeginDefLuaClass(KPlayer)
			InheritFrom(KCharacter);
		//DefMemberFunc(IsToting);
		//DefMemberFunc(GetTribeID);
		//DefMemberFunc(GetTribeGroupID);
		//DefMemberFunc(GetCurrentBpID);
		//DefMemberFunc(GetBagManagerPtr);
		//DefMemberFunc(GetSkillLevel);
		//DefMemberFunc(IsStrategyBuffInUse);
		EndDef
	};
	
}
