#include "luastack.h"
#include "luatablecrawler.h"

#include <lua.hpp>

void LuaStack::Append(LuaMultiValue const& V)
{
	Values.push_back(V);
}

void CrawlStack(lua_State* L,LuaStack& S)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		if (t==LUA_TTABLE) {
			std::shared_ptr<LuaTable> v;
			TableCrawler C(L,i);
			v=C.GetTable();
			S.Append(MakeLuaValue(v));
		} else {
			S.Append(GetScalarValue(L,i));
		}
	}
}