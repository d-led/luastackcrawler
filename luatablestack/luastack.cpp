#include "luastack.h"
#include "luavalueutil.h"
#include "luatablecrawler.h"

#include <lua.hpp>

void LuaStack::Append(LuaMultiValue const& V)
{
	Values.push_back(V);
}

std::vector<LuaMultiValue>::const_iterator LuaStack::begin() const
{
	return Values.begin();
}

std::vector<LuaMultiValue>::const_iterator LuaStack::end() const
{
	return Values.end();
}

void CrawlStack(lua_State* L,LuaStack& S)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		if (t==LUA_TTABLE) {
			TableCrawler C(L,i);
			boost::shared_ptr<LuaTable> v=C.GetTable();
			S.Append(MakeLuaValue(v));
		} else {
			S.Append(GetScalarValue(L,i));
		}
	}
}