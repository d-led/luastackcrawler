#pragma once
#include "luasctackcrawler.h"
#include <lua.hpp>
#include <LuaBridge.h>
#include <boost/shared_ptr.hpp>

namespace luabridge {
	template <>
	struct Stack <boost::shared_ptr<LuaTable> >
	{
		static boost::shared_ptr<LuaTable> get (lua_State* L, int index)
		{
			LuaStack S;
			CrawlStack(L,S,index);
			if (S.begin()!=S.end() &&
				GetType(*S.begin())==LuaType::TABLE) {
					lua_pop(L,1);
					return boost::get<boost::shared_ptr<LuaTable> >(*S.begin());
			}
			lua_pop(L,1);
			return boost::shared_ptr<LuaTable>(new LuaTable);
		}
	};

	template <>
	struct Stack <LuaStack>
	{
		static LuaStack get (lua_State* L, int index)
		{
			LuaStack S;
			int top = lua_gettop(L);
			for (int i = index; i <= top; i++) {  /* repeat for each level */
				CrawlStack(L,S,i);
			}
			lua_pop(L,S.size());
			return S;
		}
	};
}