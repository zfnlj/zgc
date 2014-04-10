/********************************************************************
created:	2011/4/13
manager:	All logic programmer
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once

#include "System/Patterns/KSingleton.h"
#include <System/Collections/KTree.h>

#include "Inc/KStruct_ClientItem.h"
#include "IAttrChangeSensor.h"

#include "CommonLogic/KDBDataStruct.h"
#include "../EventSystem/KEventInterface.h"

namespace KWorldObjAbout
{
	class KCharacter;
	class KPlayer;
	class KTransport;
	class KGroundItem;
	class KStall;
}

namespace KItemAbout
{
	class KBagManager;
}

namespace KSkillAbout
{
	class KSkillStaticData;
	struct TargetInfo;
}

namespace KUserAssistAbout
{
	class KUserCommandBuffer;
	class KMouseCmdAnalyzer;
}


/**
 * 主玩家控制相关等区别于其他玩家的东东在这里
 */
class KMainPlayer
	: public System::Patterns::KSingleton<KMainPlayer>
	, public IAttrChangeSensor
	, public KEventAbout::IEventInterface
	
{

public:
	KMainPlayer(void);
	~KMainPlayer(void);
public:
	virtual void OnAttrChange(int idx, int oldVal, int newVal);

public:
	void Reset();

public:
	/**
	 * 主角登录
	 */
	void Login(const void* pData, int len);
	/**
	 * 同步与服务器的时间
	 */
	void SetServerTime(const void* pData, int len);
	/**
	 * 获取主角指针
	 */
	static KWorldObjAbout::KPlayer* RealPlayer()
	{
		return KMainPlayer::Instance()->m_pRealPlayer;
	}
	/**
	 * 获取主角ID
	 */
	UINT64 GetMainPlayerID();

	void UpdateMainPlayerAttr(const void* pAttrPool, int nSize);







	
	


	/**
	 * 玩家操作，试图使用一个物品
	 */
	void TryUseItem(int bagType, int bagPos, float x = 0, float y = 0, int nIndex = -1);

	/**


	
	 * 呼吸，处理辅助操作相关
	 */
	void Breathe();



	//判定一个id是否是主角id
	bool IsMainPlayer(UINT64 id) const;


	
	
	
	/**
	 * 玩家键盘操作
	 */
	void KeyboardNotify(int keyVal, int pressInfo);

public:	// 一些内部回调

	/**
	 * 执行一些初始化的操作
	 */
	void Init();

	
	
	/**
	 * 主角登录完成
	 */
	void OnLoginOver();


	bool IsDead() const;
	


	/**
	 * 世界被干掉
	 */
	void OnWorldClear();

public:// quest about
	
	void initPlayer(UINT64 playerId);
private:
	KWorldObjAbout::KPlayer* m_pRealPlayer;

	// 行走插件
	
public:
	struct KPeriodRecord
	{
		int m_val;			// 值
		DWORD m_expireTime;	// 过期时间
	};

	typedef System::Collections::KMapByTree_pooled<int, KPeriodRecord> PeriodDataMap;
	PeriodDataMap m_periodData;	// GameServer同步的CustomEventData
	int GetPlayerPeriodAttrData(int id, int defval);	// 得到玩家的时效数据,有则返回，无则返回defval
	void SetPlayerPeriodAttrData(int id, int val, DWORD expireTime);		// gameserver同步数据
	void InitPlayerPeriodAttrData(const void* pBuff, int bSize);	// 初始化玩家的CustomEventData
	
public:

	// 获取背包某栏处里某物品ID的一个物品所在位置
	int GetOnePiledItemPos(int nBagID, DWORD dwItemID);
	
	KItemAbout::KBagManager* GetBagManagerPtr();



	void ChatToUser(const char* pDstPlayerName, const char* pMsg);


	


	/**
	 * 用以存储账户信息
	 */
	struct AccountInfo
	{
		int m_GMLevel;	// GM等级
		AccountInfo()
		{
			Init();
		}
		void Init()
		{
			m_GMLevel = 0;
		}
	} m_accountInfo;
	int GetGMLevel() const
	{
		return m_accountInfo.m_GMLevel;
	}

	void RegEvt();
	virtual bool OnEventImp(KEventAbout::KEventID id, const KEventAbout::KEventData* pData, uint_r customData);
public:
	BeginDefLuaClass(KMainPlayer)
	EndDef
};
