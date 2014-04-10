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
	* ����C++��غ�����Lua�ű�
	*/
	class KToLua
		: public System::Patterns::KSingleton<KToLua>
	{
	public:
		// ����C++�Ľӿڵ��ű�
		KToLua();

	private:
		// ������CPP���Ķ���
		void ToLuaCPPTable();
		// ������
		void ToLuaClass();
		// ֱ�ӽ�û��lua��������ർ��
		void ToLuaClassDirectly();
		// ����һЩ����Ķ���
		void ToLuaExtra();
		// ����ö��
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
	 * ����ɫ�����͵�����lua
	 */
	void CharToLuaType(const char* name, const KWorldObjAbout::ICharacter* pChar);
	/**
	 * ��WoldObj�����͵�����lua
	 */
	void WorldObjToLuaType(const char* name, const KWorldObjAbout::KWorldObj* pObj);
	/**
	 * ����ɫ������ѹ��luaջ
	 */
	void PushCharToLuaStateByType(lua_State* l, const KWorldObjAbout::ICharacter* pChar);
}
