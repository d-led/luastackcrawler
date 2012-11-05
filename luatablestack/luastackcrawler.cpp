#include "luasctackcrawler.h"
#include "luatablecrawler.h"
#include "luavalueutil.h"

#include <lua.hpp>

void CrawlStack(lua_State* L,LuaStack& S)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		if (t==LUA_TTABLE) {
			TableCrawler C(L,i);
			boost::shared_ptr<LuaTable> v=C.GetTable();
			S.Append(v);
		} else {
			S.Append(GetScalarValue(L,i));
		}
	}
}

void CrawlStack(lua_State* L,LuaStack& S,int level)
{
	int t = lua_type(L, level);
	if (t==LUA_TTABLE) {
		TableCrawler C(L,level);
		boost::shared_ptr<LuaTable> v=C.GetTable();
		S.Append(v);
	} else {
		S.Append(GetScalarValue(L,level));
	}
}
