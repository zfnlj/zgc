/********************************************************************
created:	2012/9/19
manager:	Zhang Liuxi
decr:		
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once

#include "Inc/Lua/KLuaWrap.h"
#include <System/Memory/KStepObjectPool.h>
#include "System/Misc/KStream.h"
#include <System/KMacro.h>
#include "../ObjectFactory/KObjectFactory.h"
#include "KStoreDef.h"
#include "System/Collections/KString.h"

namespace KWorldObjAbout
{
	class KCharacter;
}

namespace KStoreAbout
{
	/**
	** ��������
	**/
	enum PRICE_TYPE
	{
		enumPT_none = -1,
		enumPT_money,			// ��Ǯ ����T1Param1
		enumPT_cash,            //�ֽ�
		enumPT_bluePower,		// ����֮�� ����T1Param1
		enumPT_item,			// ���� ����IDΪT1Param2�ĵ���T1Param1��

		enumPT_count,
	};

	struct KStoreProductPrice
	{
		int nMoneyType;		// ��������
		int nTParam1;		// ����1
		int nTParam2;		// ����2

		BeginDefLuaClass(KStoreProductPrice)
			DefMemberVar(nMoneyType);
			DefMemberVar(nTParam1);
			DefMemberVar(nTParam2);
		EndDef
	};

	class KStoreProduct : public System::Memory::KPortableStepPool<KStoreProduct, 36>
	{
	public:
		KStoreProduct();
		void Initialize();
		bool AddProductPrice(int nMoneyType, int nTParam1, int nTParam2);

		bool CanSee(const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;
		bool CanBuy(const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;

		bool Serilize(KDataOutputStream& os, const KWorldObjAbout::KCharacter* pBuyer, const KWorldObjAbout::KCharacter* pSeller) const;

		int GetNumOfMoney() const;
		int GetNumOfBluePower() const;
		DWORD GetNumOfItem(int& nNum) const;
		int GetMoneyType() const;

	public:
		int m_nID;									// ��Ʒ����
		int m_nStoreID;								// �̵�ID
		int m_nLabelID;								// ��ǩID
		DWORD m_dwItemID;							// ��ƷID
		int m_nPileCount;							// һ������ٸ�
		KStoreProductPrice m_Prices[PRICE_TYPE_MAX]; // �۸��б�
		int m_nLimitSaleCount;						// һ�������ĸ���
		System::Collections::KString<64> m_strCanBuyScript;				// �ܲ�����ű�
		System::Collections::KString<64> m_strCanSeeScript;				// ��Ʒ�Ƿ񿴼��ű�

	public:
	/*	BeginDefLuaClass(KStoreProduct)
			DefMemberVar(m_nID);
			DefMemberVar(m_nStoreID);
			DefMemberVar(m_nLabelID);
			DefMemberVar(m_dwItemID);
			DefMemberVar(m_nPileCount);
			DefMemberVar(m_nLimitSaleCount);

			DefMemberFunc(AddProductPrice);
			DefMemberFunc(CanSee);
			DefMemberFunc(CanBuy);
			DefMemberFunc(GetMoneyType);
		EndDef*/
	};

};



