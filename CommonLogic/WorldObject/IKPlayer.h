#pragma once
#include <System/KPlatform.h>
#include <KCommonDefine.h>
#include <KCommonStruct.h>
#include "../KGlobalFunction.h"
#include <lua/KLuaWrap.h>
#include "../EventSystem/KEventSystem.h"

class IKPlayer : public KBaseLuaClass<IKPlayer>
{
public:
	virtual ~IKPlayer() { }

	// 真实对象
	virtual void* GetPlayer()			{ ASSERT(FALSE); return NULL; }
	template <typename T> T* ToPlayer()	{ return (T*)this->GetPlayer(); }

	// 基本信息
	virtual KObjectID GetID() const		{ ASSERT(FALSE); return 0; }
	virtual string_64 GetName() const	{ ASSERT(FALSE); return string_64(); }
	virtual int GetMapID() const		{ ASSERT(FALSE); return 0; }
	virtual float GetX(void) const		{ ASSERT(FALSE); return 0.0f; }
	virtual float GetY(void) const		{ ASSERT(FALSE); return 0.0f; }
	virtual float GetZ(void) const		{ ASSERT(FALSE); return 0.0f; }

	// 事件相关
	virtual bool regEvent(int evt, KEventHandler* handler)		{ ASSERT(FALSE); return false; }
	virtual bool unregEvent(int evt, KEventHandler* handler)	{ ASSERT(FALSE); return false; }
	virtual void fireEvent(int evt, const void* data, int len)	{ ASSERT(FALSE); }

	// 网络包发送
	virtual bool sendToClient(int cmd, const void* data, int len)		{ ASSERT(FALSE); return true; }
	virtual bool sendToWorldServer(int cmd, const void* data, int len)	{ ASSERT(FALSE); return true; }

public:
	BeginDefLuaClass(IKPlayer)
		DefMemberFunc(GetID)
		DefMemberFunc(GetName)
		DefMemberFunc(GetMapID)
		DefMemberFunc(GetX)
		DefMemberFunc(GetY)
		DefMemberFunc(GetZ)
		DefMemberFunc(regEvent)
		DefMemberFunc(unregEvent)
		DefMemberFunc(fireEvent)
		DefMemberFunc(sendToClient)
		DefMemberFunc(sendToWorldServer)
	EndDef;
};	luaClassPtr2Lua(IKPlayer);
