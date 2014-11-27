
#pragma once

#include "KWorldObj.h"
#include "Inc/Lua/KLuaWrap.h"
#include "IAttrChangeSensor.h"
#include "CommonLogic/WorldObject/ICharacter.h"

namespace KAttributeAbout
{
	class IAttrGroupHolder;
}

namespace KAIAbout
{
	class KAIController;
}

namespace KMoveAbout
{
	class KForceMoveComputer;
}

namespace KWorldObjAbout
{
	/**
	 * ��ɫ
	 * ���еĽ�ɫ��Ϊһ��Ŀ͹������ɫ������������ԣ���Ѫ/���ۣ������ƶ����ɻ�þ��飬����������װ��װ������ʹ�ü��ܣ��ɱ�һ�㼼������
	 */
	class KCharacter
		: public ICharacter
		, public KWorldObj
	{
	public:
		KCharacter(void);
		virtual ~KCharacter(void);

	public:
		// �õ�id
		virtual UINT64 GetID() const { return KWorldObj::GetID(); }

		// �õ�worldID
		virtual DWORD GetWorldID() const;

		// �õ�λ��
		virtual float GetX() const { return KWorldObj::GetX(); }
		virtual float GetY() const { return KWorldObj::GetY(); }
		virtual float GetZ() const { return KWorldObj::GetZ(); }
		virtual KCharacter* GetKCharacter(){return this;}

		virtual void Breathe(int nInterval);
		virtual bool IsKindOf(eWorldObjectKind flag) const;

		virtual void OnEnterGame();

		virtual const char* GetName(void) const;

	public:
		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;
		// iconitem���
	public:
		virtual int GetDisplayID() const { return 0; }

	protected:
		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val);

	protected:
		KAttributeAbout::IAttrGroupHolder* m_pAGH;
	public:
		// ������֪ͨ�����Ա仯���½ӿ�
		virtual void UpdateAttrValue(int idx, int val);

		// ״̬���
	protected:
		// ��һ��״̬
		int m_lastState;
		// ��ǰ״̬
		int m_state;
	public:
		// �õ���ǰ״̬
		int GetActiveStateID() const {return m_state;};
		// �õ���һ��״̬
		int GetLastStateID() const {return m_lastState;};
		// ת��״̬
		void ToState(int state);
	public:
		/**
		 * ��ϵ�ж����ж��Լ���Ŀ��Ĺ�ϵ
		 * ��ϵ�Ѻ÷���>=1����������0���жԷ���<=-1
		 */
		virtual int RelationshipTo(const ICharacter* pChar) const;

	public:
		/**
		 * �趨�ⲿ�����Ըı���Ӧ��
		 */
		IAttrChangeSensor* SetAttrChangeSensor(IAttrChangeSensor* pACS)
		{
			IAttrChangeSensor* result = m_pAttrChangeSensor;
			m_pAttrChangeSensor = pACS;
			return result;
		}
	private:
		IAttrChangeSensor* m_pAttrChangeSensor;

	public:
		void SetData(INT index, DWORD data);
		DWORD GetData(INT index);

	public:
		// ��������ͬ��������
		virtual int FillExtraAppearData(const void* data, DWORD len);


	protected:
		// �Զ����������
		System::Collections::KDString<1> m_customName;
	public:
		void SetCustomName(const char* name);

	public:
		// �����ൽLua
		BeginDefLuaClass(KCharacter)
			InheritFrom(ICharacter)

			DefMemberFunc(RelationshipTo)
			DefMemberFunc(GetName);
			DefMemberFunc(SetData);
			DefMemberFunc(GetData);
			DefMemberFunc(GetAttrValue);
			DefMemberFunc(GetID);
		EndDef
	};
}
