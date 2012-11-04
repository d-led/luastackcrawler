#include "luastack.h"
#include "luatablecrawler.h"

#include <lua.hpp>

void LuaStack::Append(LuaMultiValue const& V)
{
	Values.push_back(V);
}

static void iterate_and_print(lua_State *L, int index)
{
	// Push another reference to the table on top of the stack (so we know
	// where it is, and this function can work for negative, positive and
	// pseudo indices
	lua_pushvalue(L, index);
	// stack now contains: -1 => table
	lua_pushnil(L);
	// stack now contains: -1 => nil; -2 => table
	printf("{");
	bool first=true;
	while (lua_next(L, -2))
	{
		// stack now contains: -1 => value; -2 => key; -3 => table
		// copy the key so that lua_tostring does not modify the original
		lua_pushvalue(L, -2);
		// stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
		std::string key;
		if (lua_istable(L,-1)) iterate_and_print(L,-1);
		else  key= lua_tostring(L, -1);
		std::string value;
		if (lua_istable(L,-2)) iterate_and_print(L,-2);
		else value = lua_tostring(L, -2);
		if (!first) printf(",");
		if (first) first=false;
		printf("%s => %s", key.c_str(), value.c_str());
		// pop value + copy of key, leaving original key
		lua_pop(L, 2);
		// stack now contains: -1 => key; -2 => table
	}
	// stack now contains: -1 => table (when lua_next returns 0 it pops the key
	// but does not push anything.)
	// Pop table
	lua_pop(L, 1);
	// Stack is now the same as it was on entry to this function
	printf("}");
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