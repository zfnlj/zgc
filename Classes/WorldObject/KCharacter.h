
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
	 * 角色
	 * 所有的角色作为一般的客观世界角色，自有相关属性，有血/蓝槽，可以移动，可获得经验，可升级，可装备装备，可使用技能，可被一般技能作用
	 */
	class KCharacter
		: public ICharacter
		, public KWorldObj
	{
	public:
		KCharacter(void);
		virtual ~KCharacter(void);

	public:
		// 得到id
		virtual UINT64 GetID() const { return KWorldObj::GetID(); }

		// 得到worldID
		virtual DWORD GetWorldID() const;

		// 得到位置
		virtual float GetX() const { return KWorldObj::GetX(); }
		virtual float GetY() const { return KWorldObj::GetY(); }
		virtual float GetZ() const { return KWorldObj::GetZ(); }
		virtual KCharacter* GetKCharacter(){return this;}

		virtual void Breathe(int nInterval);
		virtual bool IsKindOf(eWorldObjectKind flag) const;

		virtual void OnEnterGame();

		virtual const char* GetName(void) const;

	public:
		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;
		// iconitem相关
	public:
		virtual int GetDisplayID() const { return 0; }

	protected:
		// 设定属性值
		virtual void SetAttrValue(int idx, int val);

	protected:
		KAttributeAbout::IAttrGroupHolder* m_pAGH;
	public:
		// 服务器通知的属性变化更新接口
		virtual void UpdateAttrValue(int idx, int val);

		// 状态相关
	protected:
		// 上一个状态
		int m_lastState;
		// 当前状态
		int m_state;
	public:
		// 得到当前状态
		int GetActiveStateID() const {return m_state;};
		// 得到上一个状态
		int GetLastStateID() const {return m_lastState;};
		// 转换状态
		void ToState(int state);
	public:
		/**
		 * 关系判定，判定自己与目标的关系
		 * 关系友好返回>=1，中立返回0，敌对返回<=-1
		 */
		virtual int RelationshipTo(const ICharacter* pChar) const;

	public:
		/**
		 * 设定外部的属性改变响应器
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
		// 处理额外的同步包数据
		virtual int FillExtraAppearData(const void* data, DWORD len);


	protected:
		// 自定义姓名相关
		System::Collections::KDString<1> m_customName;
	public:
		void SetCustomName(const char* name);

	public:
		// 定义类到Lua
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
