#include "KAttrGroupHolder.h"

namespace KAttributeAbout
{
	KCharAttrGroupHolder::KCharAttrGroupHolder()
	{
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_commonAttrHolder;
		m_attrHolderList[1] = &m_2ParamAttrHolder;
	}

	void KCharAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
	}

	int KCharAttrGroupHolder::GetAttrValue( int idx ) const
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			return m_attrHolderList[pos]->GetAttrValue(idx);
		}
		return 0;
	}

	void KCharAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			m_attrHolderList[pos]->SetAttrValue(idx, val, param);
		}
	}

	void KCharAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
		}
	}

namespace{
	class KAttrVisitor
	{
	public:
		IAllExistAttrVisitor* pVisit;
		void operator()(int n)
		{
			pVisit->Visit(n);
		}
	};
}
	void KCharAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		KAttrVisitor v;
		v.pVisit = &visitor;
		m_commonAttrHolder.VisitAll(v);
		m_2ParamAttrHolder.VisitAll(v);
	}

	KCommonAttrGroupHolder::KCommonAttrGroupHolder()
	{
	}

	KCommonAttrGroupHolder::~KCommonAttrGroupHolder()
	{
		m_attrHolder.clear();
	}

	int KCommonAttrGroupHolder::GetAttrValue( int idx ) const
	{
		HOLDER_TYPE::const_iterator it = m_attrHolder.find(idx);
		if (it != m_attrHolder.end())
		{
			return it->second;
		}
		return 0;
	}

	void KCommonAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		m_attrHolder[idx] = val;
	}

	void KCommonAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		m_attrHolder[idx] += val;
	}

	void KCommonAttrGroupHolder::Reset()
	{
		m_attrHolder.clear();
	}

	void KCommonAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		HOLDER_TYPE::iterator it = m_attrHolder.begin();
		for (; it != m_attrHolder.end(); it++)
		{
			visitor.Visit(it->first);
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	KPlayerAttrGroupHolder::KPlayerAttrGroupHolder()
	{
		m_pCharAttrGroupHolder = KCharAttrGroupHolder::Alloc();
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_PlayerAttrHolder;
		m_attrHolderList[1] = &m_PlayerParamAttrHolder;
		m_attrHolderList[2] = &m_PlayerCustomAttrHolder;
	}

	KPlayerAttrGroupHolder::~KPlayerAttrGroupHolder()
	{
		if (m_pCharAttrGroupHolder)
		{
			KCharAttrGroupHolder::Free(m_pCharAttrGroupHolder);
			m_pCharAttrGroupHolder = NULL;
		}
	}

	void KPlayerAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
		m_pCharAttrGroupHolder->Reset();
	}

	int KPlayerAttrGroupHolder::GetAttrValue( int idx ) const
	{
		if (idx < part1_ca1_start)
		{
			return m_pCharAttrGroupHolder->GetAttrValue(idx);
		}
		else
		{
			int pos = (idx - COMMON_OBJ_ATTRIBUTE_SIZE) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				return m_attrHolderList[pos]->GetAttrValue(idx);
			}
		}
		return 0;
	}

	void KPlayerAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca1_start)
		{
			m_pCharAttrGroupHolder->SetAttrValue(idx, val, param);
		}
		else
		{
			int pos = (idx - COMMON_OBJ_ATTRIBUTE_SIZE) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->SetAttrValue(idx, val, param);
			}
		}
	}

	void KPlayerAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca1_start)
		{
			m_pCharAttrGroupHolder->IncreaseAttrValue(idx, val, param);
		}
		else
		{
			int pos = (idx - COMMON_OBJ_ATTRIBUTE_SIZE) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
			}
		}
	}

	void KPlayerAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		m_pCharAttrGroupHolder->VisitAllExistAttr(visitor);

		KAttrVisitor v;
		v.pVisit = &visitor;
		m_PlayerAttrHolder.VisitAll(v);
		m_PlayerParamAttrHolder.VisitAll(v);
	}

	///////////////////////////////////////////////////////////////////////////////

	KWorldPlayerAttrHolder::KWorldPlayerAttrHolder()
	{
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_WorldPlayerAttrHolder;
	}

	void KWorldPlayerAttrHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
	}

	int KWorldPlayerAttrHolder::GetAttrValue( int idx ) const
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			return m_attrHolderList[pos]->GetAttrValue(idx);
		}
		return 0;
	}

	void KWorldPlayerAttrHolder::SetAttrValue( int idx, int val, int param )
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			m_attrHolderList[pos]->SetAttrValue(idx, val, param);
		}
	}

	void KWorldPlayerAttrHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		int pos = idx / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{
			m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
		}
	}

	void KWorldPlayerAttrHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		KAttrVisitor v;
		v.pVisit = &visitor;
		m_WorldPlayerAttrHolder.VisitAll(v);
	}

	///////////////////////////////////////////////////////////////////////////////
	KNpcAttrGroupHolder::KNpcAttrGroupHolder()
	{
		m_pCharAttrGroupHolder = KCharAttrGroupHolder::Alloc();
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_NpcAttrHolder;
		m_attrHolderList[1] = &m_NpcParamAttrHolder;
	}

	KNpcAttrGroupHolder::~KNpcAttrGroupHolder()
	{
		if (m_pCharAttrGroupHolder)
		{
			KCharAttrGroupHolder::Free(m_pCharAttrGroupHolder);
			m_pCharAttrGroupHolder = NULL;
		}
	}

	void KNpcAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
	}

	int KNpcAttrGroupHolder::GetAttrValue( int idx ) const
	{
		if (idx < part1_ca2_end)
		{
			return m_pCharAttrGroupHolder->GetAttrValue(idx);
		}
		else if(idx >= part2_ca1_start && idx < part2_ca2_end)
		{
			int pos = (idx - part2_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				return m_attrHolderList[pos]->GetAttrValue(idx);
			}
		}
		
		return 0;
	}

	void KNpcAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->SetAttrValue(idx, val, param);
		}
		else if(idx >= part2_ca1_start && idx < part2_ca2_end)
		{
			int pos = (idx - part2_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->SetAttrValue(idx, val, param);
			}
		}
	}

	void KNpcAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->IncreaseAttrValue(idx, val, param);
		}
		else if(idx >= part2_ca1_start && idx < part2_ca2_end)
		{
			int pos = (idx - part2_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
			}
		}
	}

	void KNpcAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		m_pCharAttrGroupHolder->VisitAllExistAttr(visitor);

		KAttrVisitor v;
		v.pVisit = &visitor;
		m_NpcAttrHolder.VisitAll(v);
		m_NpcParamAttrHolder.VisitAll(v);
	}

	///////////////////////////////////////////////////////////////////////////////

	KBattlePetAttrGroupHolder::KBattlePetAttrGroupHolder()
	{
		m_pCharAttrGroupHolder = KCharAttrGroupHolder::Alloc();
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_BettlePetAttrHolder;
		m_attrHolderList[1] = &m_BettlePetParamAttrHolder;
	}

	KBattlePetAttrGroupHolder::~KBattlePetAttrGroupHolder()
	{
		if (m_pCharAttrGroupHolder)
		{
			KCharAttrGroupHolder::Free(m_pCharAttrGroupHolder);
			m_pCharAttrGroupHolder = NULL;
		}
	}

	void KBattlePetAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
		m_pCharAttrGroupHolder->Reset();
	}

	int KBattlePetAttrGroupHolder::GetAttrValue( int idx ) const
	{
		if (idx < part1_ca2_end)
		{
			return m_pCharAttrGroupHolder->GetAttrValue(idx);
		}
		else if(idx >= part3_ca1_start && idx < part3_ca2_end)
		{
			int pos = (idx - part3_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				return m_attrHolderList[pos]->GetAttrValue(idx);
			}
		}
		
		return 0;
	}

	void KBattlePetAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->SetAttrValue(idx, val, param);
		}
		else if(idx >= part3_ca1_start && idx < part3_ca2_end)
		{
			int pos = (idx - part3_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->SetAttrValue(idx, val, param);
			}
		}
	}

	void KBattlePetAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->IncreaseAttrValue(idx, val, param);
		}
		else if(idx >= part3_ca1_start && idx < part3_ca2_end)
		{
			int pos = (idx - part3_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
			}
		}
	}

	void KBattlePetAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		m_pCharAttrGroupHolder->VisitAllExistAttr(visitor);

		KAttrVisitor v;
		v.pVisit = &visitor;
		m_BettlePetAttrHolder.VisitAll(v);
		m_BettlePetParamAttrHolder.VisitAll(v);
	}
	///////////////////////////////////////////////////////////////////////////////

	KBuildingAttrGroupHolder::KBuildingAttrGroupHolder()
	{
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_commonAttrHolder;
		m_attrHolderList[1] = &m_2ParamAttrHolder;
		m_level = 0;
		m_size = 0;
	}

	void KBuildingAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
		m_level = 0;
		m_size = 0;
	}

	int KBuildingAttrGroupHolder::GetAttrValue( int idx ) const
	{
		switch(idx)
		{
		case ca_Level:
			return m_level;
		case ca_Size:
			return m_size;
		}
		int pos = (idx - part4_ca1_start) / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{

		}
		else
		{
			return 0;
		}
		return m_attrHolderList[pos]->GetAttrValue(idx);
	}

	void KBuildingAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		switch(idx)
		{
		case ca_Level:
			m_level = val;
			return;
		case ca_Size:
			m_size = val;
			return;
		}

		int pos = (idx - part4_ca1_start) / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{

		}
		else
		{
			return;
		}
		m_attrHolderList[pos]->SetAttrValue(idx, val, param);
	}

	void KBuildingAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		switch(idx)
		{
		case ca_Level:
			m_level += val;
			return;
		case ca_Size:
			m_size += val;
			return;
		}
		int pos = (idx - part4_ca1_start) / COMMON_ATTRIBUTE_GROUP_SIZE;
		if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
		{

		}
		else
		{
			return;
		}
		m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
	}

	void KBuildingAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		KAttrVisitor v;
		v.pVisit = &visitor;
		visitor.Visit(ca_Level);
		visitor.Visit(ca_Size);
		m_commonAttrHolder.VisitAll(v);
		m_2ParamAttrHolder.VisitAll(v);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	KTransportAttrGroupHolder::KTransportAttrGroupHolder()
	{
		m_pCharAttrGroupHolder = KCharAttrGroupHolder::Alloc();
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_commonAttrHolder;
		m_attrHolderList[1] = &m_2ParamAttrHolder;
	}

	KTransportAttrGroupHolder::~KTransportAttrGroupHolder()
	{
		if (m_pCharAttrGroupHolder)
		{
			KCharAttrGroupHolder::Free(m_pCharAttrGroupHolder);
			m_pCharAttrGroupHolder = NULL;
		}
	}

	void KTransportAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
		m_pCharAttrGroupHolder->Reset();
	}

	int KTransportAttrGroupHolder::GetAttrValue( int idx ) const
	{
		if (idx < part1_ca2_end)
		{
			return m_pCharAttrGroupHolder->GetAttrValue(idx);
		}
		else if(idx >= part5_ca1_start && idx < part5_ca2_end)
		{
			int pos = (idx - part5_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				return m_attrHolderList[pos]->GetAttrValue(idx);
			}
		}

		return 0;
	}

	void KTransportAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->SetAttrValue(idx, val, param);
		}
		else if(idx >= part5_ca1_start && idx < part5_ca2_end)
		{
			int pos = (idx - part5_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->SetAttrValue(idx, val, param);
			}
		}
	}

	void KTransportAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->IncreaseAttrValue(idx, val, param);
		}
		else if(idx >= part5_ca1_start && idx < part5_ca2_end)
		{
			int pos = (idx - part5_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
			}
		}
	}

	void KTransportAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		m_pCharAttrGroupHolder->VisitAllExistAttr(visitor);

		KAttrVisitor v;
		v.pVisit = &visitor;
		m_commonAttrHolder.VisitAll(v);
		m_2ParamAttrHolder.VisitAll(v);
	}
	
	///////////////////////////////////////////////////////////////////////////////

	KTribeBattlePetAttrGroupHolder::KTribeBattlePetAttrGroupHolder()
	{
		m_pCharAttrGroupHolder = KCharAttrGroupHolder::Alloc();
		memset(m_attrHolderList, 0, sizeof(m_attrHolderList));
		m_attrHolderList[0] = &m_TribeBettlePetAttrHolder;
		m_attrHolderList[1] = &m_TribeBettlePetParamAttrHolder;
	}

	KTribeBattlePetAttrGroupHolder::~KTribeBattlePetAttrGroupHolder()
	{
		if (m_pCharAttrGroupHolder)
		{
			KCharAttrGroupHolder::Free(m_pCharAttrGroupHolder);
			m_pCharAttrGroupHolder = NULL;
		}
	}

	void KTribeBattlePetAttrGroupHolder::Reset()
	{
		for(int i=0; i<MAX_ATTR_GROUP; i++)
		{
			KAttributeAbout::IAttrHolder* pAttrHolder = m_attrHolderList[i];
			if(pAttrHolder)
			{
				pAttrHolder->Reset();
			}
		}
		m_pCharAttrGroupHolder->Reset();
	}

	int KTribeBattlePetAttrGroupHolder::GetAttrValue( int idx ) const
	{
		if (idx < part1_ca2_end)
		{
			return m_pCharAttrGroupHolder->GetAttrValue(idx);
		}
		else if(idx >= part6_ca1_start && idx < part6_ca2_end)
		{
			int pos = (idx - part6_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				return m_attrHolderList[pos]->GetAttrValue(idx);
			}
		}

		return 0;
	}

	void KTribeBattlePetAttrGroupHolder::SetAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->SetAttrValue(idx, val, param);
		}
		else if(idx >= part6_ca1_start && idx < part6_ca2_end)
		{
			int pos = (idx - part6_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->SetAttrValue(idx, val, param);
			}
		}
	}

	void KTribeBattlePetAttrGroupHolder::IncreaseAttrValue( int idx, int val, int param )
	{
		if (idx < part1_ca2_end)
		{
			m_pCharAttrGroupHolder->IncreaseAttrValue(idx, val, param);
		}
		else if(idx >= part6_ca1_start && idx < part6_ca2_end)
		{
			int pos = (idx - part6_ca1_start + 1) / COMMON_ATTRIBUTE_GROUP_SIZE;
			if (pos >= 0 && pos < sizeof(m_attrHolderList) / sizeof(m_attrHolderList[0]) && m_attrHolderList[pos])
			{
				m_attrHolderList[pos]->IncreaseAttrValue(idx, val, param);
			}
		}
	}

	void KTribeBattlePetAttrGroupHolder::VisitAllExistAttr( IAllExistAttrVisitor& visitor )
	{
		m_pCharAttrGroupHolder->VisitAllExistAttr(visitor);

		KAttrVisitor v;
		v.pVisit = &visitor;
		m_TribeBettlePetAttrHolder.VisitAll(v);
		m_TribeBettlePetParamAttrHolder.VisitAll(v);
	}
	
	///////////////////////////////////////////////////////////////////////////////

}
