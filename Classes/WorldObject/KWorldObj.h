
#pragma once

#include "System/KType.h"
#include "Inc/KCommonDefine.h"

namespace KSceneAbout
{
	class KDynamicWorld;
	class KVillageStaticData;
}

struct TargetSelectCond;

namespace KWorldObjAbout
{
	class KCharacter;

	class KWorldObj
	{
	public:
		KWorldObj(void);
		virtual ~KWorldObj(void) { }

		bool operator < (const KWorldObj& o) { return m_id < o.m_id; }
		bool operator > (const KWorldObj& o) { return m_id > o.m_id; }
		bool operator == (const KWorldObj& o) { return m_id == o.m_id; }

	public:
		virtual void ChangePosition(float fx, float fy, float fz) { m_fX = fx; m_fY = fy; m_fZ = fz; }

		void SetValid(bool b = true) { m_bValid = b; }
		bool IsValid(void) const { return m_bValid; }

		virtual float GetX(void) const { return m_fX; }
		virtual float GetY(void) const { return m_fY; }
		virtual float GetZ(void) const { return m_fZ; }
		// �õ�id
		virtual UINT64 GetID() const { return m_id; }
	public:
		virtual void Breathe(int nInterval) {}

		virtual bool IsKindOf(eWorldObjectKind flag) const { return flag == wokWorldObj; }

		virtual void OnEnterGame();
		virtual void OnLeaveGame(bool onClearWorld = false);
	public:
		UINT64	m_id;
		eWorldObjectKind m_nFlag;
		int		m_nPosID;			// ���㿪ʼ
	protected:
		float m_fX;				// ȫ�����е�X����
		float m_fY;				// ȫ�����е�Y����
		float m_fZ;				// ȫ�����е�Z���꣬�߶�

		bool m_bValid;			// �Ƿ���Ч��Ϊʵ���ڱ������ܶԸýṹ����ɾ������������
	public:
		/**
		 * ���»����Appear����
		 */
		virtual void GetAppearData(char* pAppearData, int nBuffLen) {}
		// ������˲�ƣ��ڵ�ǰ�����ƶ�֮��
	};
}
