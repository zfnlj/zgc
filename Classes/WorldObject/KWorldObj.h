
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
		// 得到id
		virtual UINT64 GetID() const { return m_id; }
	public:
		virtual void Breathe(int nInterval) {}

		virtual bool IsKindOf(eWorldObjectKind flag) const { return flag == wokWorldObj; }

		virtual void OnEnterGame();
		virtual void OnLeaveGame(bool onClearWorld = false);
	public:
		UINT64	m_id;
		eWorldObjectKind m_nFlag;
		int		m_nPosID;			// 从零开始
	protected:
		float m_fX;				// 全世界中的X坐标
		float m_fY;				// 全世界中的Y坐标
		float m_fZ;				// 全世界中的Z坐标，高度

		bool m_bValid;			// 是否有效，为实现在遍历中能对该结构进行删除操作而设置
	public:
		/**
		 * 更新缓冲的Appear数据
		 */
		virtual void GetAppearData(char* pAppearData, int nBuffLen) {}
		// 无条件瞬移，在当前所有移动之后
	};
}
