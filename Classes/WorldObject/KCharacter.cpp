
#include "KCharacter.h"
#include <iostream>
#include "Attribute/KAttrGroupHolder.h"

#pragma warning(disable:4355)	//  warning C4355: 'this' : used in base member initializer list

#if !defined(max)
#	define max(a,b) (((a)<(b))?(b):(a))
#endif
#if !defined(min)
#	define min(a,b) (((a)<(b))?(a):(b))
#endif

using namespace KAttributeAbout;

namespace KWorldObjAbout
{
	KCharacter::KCharacter(void)
		: KWorldObj()
	{
		m_nFlag = wokCharacter;
		m_state = KAttributeAbout::enumCS_Stand;
		m_lastState = KAttributeAbout::enumCS_Stand;
		m_pAttrChangeSensor = NULL;
		m_pAGH = KCommonAttrGroupHolder::Alloc();
	}

	KCharacter::~KCharacter(void)
	{
		if (m_pAGH)
		{
			KCommonAttrGroupHolder::Free((KCommonAttrGroupHolder*)m_pAGH);
			m_pAGH = NULL;
		}
	}

	void KCharacter::Breathe(int nInterval)
	{
	}

	void KCharacter::OnEnterGame()
	{
		KWorldObj::OnEnterGame();
		// TODO...
	}

	int KCharacter::GetAttrValue(int idx) const
	{
		return m_pAGH->GetAttrValue(idx);
	}

	void KCharacter::SetAttrValue(int idx, int val)
	{
		m_pAGH->SetAttrValue(idx, val, 0);
	}

	bool KCharacter::IsKindOf(eWorldObjectKind flag) const
	{
		return (KWorldObj::IsKindOf(flag) || flag == wokCharacter);
	}

	const char* KCharacter::GetName(void) const
	{
		return "Unknown, must be God";
	}

	void KCharacter::ToState( int state )
	{
		if (m_state != state)
		{
			m_lastState = m_state;
			m_state = state;
		}
	}

	void KCharacter::UpdateAttrValue( int idx, int val )
	{
		// TODO: 处理属性变化更新事件
		int oldVal = GetAttrValue(idx);
		int newVal = val;
		SetAttrValue(idx, val);
		if (oldVal == newVal) return;
		/*switch(idx)
		{
		case ca_Level:
			if (oldVal < newVal)
			{
				KPlayActionParam pap;
				strcpy_s(pap.name, sizeof(pap.name), "char_upgrade");
				g_DynamicWorld.SendCharactorMsg(LOGIC_CHARACTOR_PLAY_ACTION, m_id, (unsigned long)&pap);
			}
			break;
		}*/

		// 所有的属性变更消息
		//g_DynamicWorld.SendCharactorMsg(LOGIC_CHARACTER_ATTR_CHANGE, m_id, idx);

		if (m_pAttrChangeSensor)
		{
			m_pAttrChangeSensor->OnAttrChange(idx, oldVal, newVal);
		}
	}

	int KCharacter::RelationshipTo( const ICharacter* pChar ) const
	{
		//return KRelationshipAbout::Relationship(this, (const KCharacter*)pChar);
		return 0;
	}

	DWORD KCharacter::GetWorldID() const
	{
		ASSERT(false);
		return 0;
	}

	void KCharacter::SetData(INT index, DWORD data)
	{
	/*	if( NULL == m_pUserData ) m_pUserData = KAIAbout::KCharUserData::Alloc();
		m_pUserData->Set(index, data);*/
	}

	DWORD KCharacter::GetData(INT index)
	{
		//if( NULL == m_pUserData ) m_pUserData = KAIAbout::KCharUserData::Alloc();
		//return m_pUserData->Get(index);
		return 0;
	}
	int KCharacter::FillExtraAppearData( const void* data, DWORD len )
	{
		//KMsgInputStream is(data, len);
		//if (len > 0)
		//{
		//	BYTE magicNumber;
		//	while(is.ReadByte(magicNumber))
		//	{
		//		switch(magicNumber)
		//		{
		//		case enumECAMN_SkillParent:
		//			{
		//				UINT64 skillParent;
		//				is.ReadUint64(skillParent);
		//				SetSkillParent(skillParent);
		//			}
		//			break;
		//		default:
		//			// 无法识别的magic number，回退一个byte，不再处理
		//			if (is.m_pos > 0)
		//			{
		//				return is.m_pos - 1;
		//			}
		//		}
		//	}
		//}
		//return is.m_pos;
		return 0;
	}


}