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
 * ����ҿ�����ص�������������ҵĶ���������
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
	 * ���ǵ�¼
	 */
	void Login(const void* pData, int len);
	/**
	 * ͬ�����������ʱ��
	 */
	void SetServerTime(const void* pData, int len);
	/**
	 * ��ȡ����ָ��
	 */
	static KWorldObjAbout::KPlayer* RealPlayer()
	{
		return KMainPlayer::Instance()->m_pRealPlayer;
	}
	/**
	 * ��ȡ����ID
	 */
	UINT64 GetMainPlayerID();

	void UpdateMainPlayerAttr(const void* pAttrPool, int nSize);







	
	


	/**
	 * ��Ҳ�������ͼʹ��һ����Ʒ
	 */
	void TryUseItem(int bagType, int bagPos, float x = 0, float y = 0, int nIndex = -1);

	/**


	
	 * �������������������
	 */
	void Breathe();



	//�ж�һ��id�Ƿ�������id
	bool IsMainPlayer(UINT64 id) const;


	
	
	
	/**
	 * ��Ҽ��̲���
	 */
	void KeyboardNotify(int keyVal, int pressInfo);

public:	// һЩ�ڲ��ص�

	/**
	 * ִ��һЩ��ʼ���Ĳ���
	 */
	void Init();

	
	
	/**
	 * ���ǵ�¼���
	 */
	void OnLoginOver();


	bool IsDead() const;
	


	/**
	 * ���类�ɵ�
	 */
	void OnWorldClear();

public:// quest about
	
	void initPlayer(UINT64 playerId);
private:
	KWorldObjAbout::KPlayer* m_pRealPlayer;

	// ���߲��
	
public:
	struct KPeriodRecord
	{
		int m_val;			// ֵ
		DWORD m_expireTime;	// ����ʱ��
	};

	typedef System::Collections::KMapByTree_pooled<int, KPeriodRecord> PeriodDataMap;
	PeriodDataMap m_periodData;	// GameServerͬ����CustomEventData
	int GetPlayerPeriodAttrData(int id, int defval);	// �õ���ҵ�ʱЧ����,���򷵻أ����򷵻�defval
	void SetPlayerPeriodAttrData(int id, int val, DWORD expireTime);		// gameserverͬ������
	void InitPlayerPeriodAttrData(const void* pBuff, int bSize);	// ��ʼ����ҵ�CustomEventData
	
public:

	// ��ȡ����ĳ������ĳ��ƷID��һ����Ʒ����λ��
	int GetOnePiledItemPos(int nBagID, DWORD dwItemID);
	
	KItemAbout::KBagManager* GetBagManagerPtr();



	void ChatToUser(const char* pDstPlayerName, const char* pMsg);


	


	/**
	 * ���Դ洢�˻���Ϣ
	 */
	struct AccountInfo
	{
		int m_GMLevel;	// GM�ȼ�
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
