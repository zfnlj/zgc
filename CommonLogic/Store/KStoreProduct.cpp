#include "KStoreProduct.h"
#include "../WorldObject/ICharacter.h"


using namespace KWorldObjAbout;

namespace KStoreAbout
{

	//////////////////////////////////////////////////////////////////////////
	KStoreProduct::KStoreProduct()
	{
	}

	void KStoreProduct::Initialize()
	{
		m_nID = 0;						// ��Ʒ����
		m_nStoreID = 0;					// �̵�ID
		m_nLabelID = 0;					// ��ǩID
		m_dwItemID = 0;					// ��ƷID
		m_nPileCount = 0;				// һ������ٸ�
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			m_Prices[i].nMoneyType = enumPT_none;
			m_Prices[i].nTParam1 = -1;
			m_Prices[i].nTParam2 = -1;
		}
		m_nLimitSaleCount = 0;			// һ�������ĸ���
		m_strCanBuyScript.clear();		// �ܲ�����ű�
		m_strCanSeeScript.clear();		// ��Ʒ�Ƿ񿴼��ű�
	}

	bool KStoreProduct::AddProductPrice(int nMoneyType, int nTParam1, int nTParam2)
	{
		ASSERT_RETURN(nMoneyType > enumPT_none && nMoneyType < enumPT_count, false);

		KStoreProductPrice price;
		price.nMoneyType = nMoneyType;
		price.nTParam1 = nTParam1;
		price.nTParam2 = nTParam2;
		int i = 0;
		for(i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_none == m_Prices[i].nMoneyType) break;
			if(m_Prices[i].nMoneyType == nMoneyType) return false;
		}

		if(i < PRICE_TYPE_MAX)
		{
			m_Prices[i].nMoneyType = nMoneyType;
			m_Prices[i].nTParam1 = nTParam1;
			m_Prices[i].nTParam2 = nTParam2;
			return true;
		}

		return false;
	}

#define EXPORT_TO_LUA(name, val)	\
	LuaWraper.SetObjectVal("______", val);	\
	sprintf_k(buff, sizeof(buff), "%s=______", name);	\
	LuaWraper.doString(buff);
	
	bool KStoreProduct::CanSee(const KCharacter* pBuyer, const KCharacter* pSeller) const
	{
		char buff[1024];
        if(m_strCanSeeScript.GetCount() > 0)
		{
			LuaWraper.doString("CPP.ctx={}");
			EXPORT_TO_LUA("CPP.ctx.Buyer", pBuyer);
			EXPORT_TO_LUA("CPP.ctx.Seller", pSeller);
			return LuaWraper.Call<bool, const KCharacter*, const KCharacter*>(m_strCanSeeScript.c_str(), pBuyer, pSeller);
		}

		return true;
	}

	bool KStoreProduct::CanBuy(const KCharacter* pBuyer, const KCharacter* pSeller) const
	{
		char buff[1024];
		if(m_strCanBuyScript.GetCount() > 0)
		{
			LuaWraper.doString("CPP.ctx={}");
			EXPORT_TO_LUA("CPP.ctx.Buyer", pBuyer);
			EXPORT_TO_LUA("CPP.ctx.Seller", pSeller);
			return LuaWraper.Call<bool, const KCharacter*, const KCharacter*>(m_strCanBuyScript.c_str(), pBuyer, pSeller);
		}

		return true;
	}
#undef EXPORT_TO_LUA

	bool KStoreProduct::Serilize( KDataOutputStream& os, const KCharacter* pBuyer, const KCharacter* pSeller ) const
	{
		ASSERT_RETURN(os.WriteInt(m_nID), false);
		ASSERT_RETURN(os.WriteInt(m_nStoreID), false);
		ASSERT_RETURN(os.WriteInt(m_nLabelID), false);
		ASSERT_RETURN(os.WriteDword(m_dwItemID), false);
		ASSERT_RETURN(os.WriteInt(m_nPileCount), false);
		for(int i=0;i<PRICE_TYPE_MAX;i++)
		{
			BYTE* pp = (BYTE*)(&m_Prices[i]);
			ASSERT_RETURN(os.WriteByteArray(pp, sizeof(m_Prices[i])), false);
		}
		ASSERT_RETURN(os.WriteInt(m_nLimitSaleCount), false);
		bool bCanBuy = true;//CanBuy(pBuyer, pSeller);
		ASSERT_RETURN(os.WriteByte(bCanBuy ? 1 : 0), false);
		bool bCanSee = true;//CanSee(pBuyer, pSeller);
		ASSERT_RETURN(os.WriteByte(bCanSee ? 1 : 0), false);

		return true;
	}

	int KStoreProduct::GetNumOfMoney() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_money == m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nTParam1;	// ��Ǯ��
			}
		}
		return 0;
	}

	int KStoreProduct::GetNumOfBluePower() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_bluePower == m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nTParam1;	// ����֮����
			}
		}
		return 0;
	}

	DWORD KStoreProduct::GetNumOfItem( int& nNum ) const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_item == m_Prices[i].nMoneyType)
			{
				nNum = m_Prices[i].nTParam2;	// ��Ʒ����
				return m_Prices[i].nTParam1;	// ��ƷID
			}
		}
		return 0;
	}

	int KStoreProduct::GetMoneyType() const
	{
		for(int i=0;i<sizeof(m_Prices)/sizeof(m_Prices[0]);i++)
		{
			if(enumPT_none != m_Prices[i].nMoneyType)
			{
				return m_Prices[i].nMoneyType;	// ��������
			}
		}
		return enumPT_none;
	}
};