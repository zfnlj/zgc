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
* ��ɫ�ӿ�
*/
namespace KWorldObjAbout
{

class ICharacter
{
public:
	virtual ~ICharacter() {}
	template <typename T> T* toImpl() { return (T*)this; }

public:
	// �õ�id
	virtual KObjectID GetID() const { return 0; }

	// �õ�worldID
	virtual DWORD GetWorldID() const { return 0; }
	virtual KWorld* GetWorld() { return NULL; }

	// �õ�λ��
	virtual float GetX() const { return 0.0f; }
	virtual float GetY() const { return 0.0f; }
	virtual float GetZ() const { return 0.0f; }

	// �õ�ָ��������ֵ
	virtual int GetAttrValue(int idx) const { return 0; }

	// �õ�����
	virtual bool IsKindOf(eWorldObjectKind flag) const {return false;}
	
	// ����Ƿ���buff
	virtual bool HasBuff(int id, KObjectID adder) const {return false;}
	
	// �õ���ǰ״̬id
	virtual int GetActiveStateID() const{ return 0; }
	
	// �õ��ϴε�״̬id
	virtual int GetLastStateID() const{ return 0; }
	
	// �õ���ǰ���ƶ��ٶ�
	virtual float GetWalkSpeed() const{return 0.0f;}
	
	// �õ���ǰ����ĽǶ�
	virtual float GetAngle() const {return 0.0f;}
	
	// �õ����ǵĽǶ�
	virtual float GetAngleZ() const {return 0.0f;}
	
	// ��ĳ�˳��
	virtual bool BeHated(KObjectID nID) { return false; }
	
	// ���ĳ�˳��
	virtual bool NotBeHated(KObjectID nID) { return false; }
	
	/**
	* ��ϵ�ж����ж��Լ���Ŀ��Ĺ�ϵ
	* ��ϵ�Ѻ÷���>=1����������0���жԷ���<=-1
	*/
	virtual int RelationshipTo(const ICharacter* pOther) const {return 0;}
	
	// �Ƿ������״̬
	virtual bool IsRiding() const {return false;}
	
	// �Ƿ�����
	virtual bool IsDead() const { return GetActiveStateID() == KAttributeAbout::enumCS_Death; }

public:
	BeginDefLuaClass(ICharacter)
		DefMemberFunc(GetID);
	EndDef
};
};