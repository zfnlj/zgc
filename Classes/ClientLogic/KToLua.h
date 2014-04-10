/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "System/Patterns/KSingleton.h"

#include "Inc/Lua/KLuaWrap.h"

namespace KWorldObjAbout
{
	class ICharacter;
	class KCharacter;
	class KWorldObj;
}

namespace KScriptAbout
{
	/**
	* 导出C++相关函数到Lua脚本
	*/
	class KToLua
		: public System::Patterns::KSingleton<KToLua>
	{
	public:
		// 导出C++的接口到脚本
		KToLua();

	private:
		// 导出到CPP表格的东西
		void ToLuaCPPTable();
		// 导出类
		void ToLuaClass();
		// 直接将没有lua定义过的类导出
		void ToLuaClassDirectly();
		// 导出一些特殊的东西
		void ToLuaExtra();
		// 导出枚举
		void ToLuaEnum();
	};

	template<typename T>
	void ObjToLua(const char* name, T obj)
	{
		LuaWraper.SetObjectVal("__temp__", obj);
		static char buff[1024];
		sprintf_k(buff, sizeof(buff), "%s=__temp__", name);
		LuaWraper.doString(buff);
	}

	/**
	 * 将角色按类型导出到lua
	 */
	void CharToLuaType(const char* name, const KWorldObjAbout::ICharacter* pChar);
	/**
	 * 将WoldObj按类型导出到lua
	 */
	void WorldObjToLuaType(const char* name, const KWorldObjAbout::KWorldObj* pObj);
	/**
	 * 将角色按类型压入lua栈
	 */
	void PushCharToLuaStateByType(lua_State* l, const KWorldObjAbout::ICharacter* pChar);
}
