/********************************************************************
created:	2011/3/10
manager:	zaki
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include <System/KType.h>
#include "Inc/Lua/KLuaWrap.h"
#include "Inc/KCommonDefine.h"
#include "KStateDefine.h"

class KWorld;

/**
* 角色接口
*/
namespace KWorldObjAbout
{

class ICharacter
{
public:
	virtual ~ICharacter() {}
	template <typename T> T* toImpl() { return (T*)this; }

public:
	// 得到id
	virtual KObjectID GetID() const { return 0; }

	// 得到worldID
	virtual DWORD GetWorldID() const { return 0; }
	virtual KWorld* GetWorld() { return NULL; }

	// 得到位置
	virtual float GetX() const { return 0.0f; }
	virtual float GetY() const { return 0.0f; }
	virtual float GetZ() const { return 0.0f; }

	// 得到指定的属性值
	virtual int GetAttrValue(int idx) const { return 0; }

	// 得到类型
	virtual bool IsKindOf(eWorldObjectKind flag) const {return false;}
	
	// 检查是否有buff
	virtual bool HasBuff(int id, KObjectID adder) const {return false;}
	
	// 得到当前状态id
	virtual int GetActiveStateID() const{ return 0; }
	
	// 得到上次的状态id
	virtual int GetLastStateID() const{ return 0; }
	
	// 得到当前的移动速度
	virtual float GetWalkSpeed() const{return 0.0f;}
	
	// 得到当前面向的角度
	virtual float GetAngle() const {return 0.0f;}
	
	// 得到仰角的角度
	virtual float GetAngleZ() const {return 0.0f;}
	
	// 被某人仇恨
	virtual bool BeHated(KObjectID nID) { return false; }
	
	// 解除某人仇恨
	virtual bool NotBeHated(KObjectID nID) { return false; }
	
	/**
	* 关系判定，判定自己与目标的关系
	* 关系友好返回>=1，中立返回0，敌对返回<=-1
	*/
	virtual int RelationshipTo(const ICharacter* pOther) const {return 0;}
	
	// 是否在骑乘状态
	virtual bool IsRiding() const {return false;}
	
	// 是否死亡
	virtual bool IsDead() const { return GetActiveStateID() == KAttributeAbout::enumCS_Death; }

public:
	BeginDefLuaClass(ICharacter)
		DefMemberFunc(GetID);
	EndDef
};
};