#include "luatablecrawler.h"
#include "luavalue.h"

#include <lua.hpp>

TableCrawler::TableCrawler(lua_State* L,int p):L(L),pos(p)
{
}

static void GetTable(lua_State *L, int index,std::shared_ptr<LuaTable> T)
{
	// Push another reference to the table on top of the stack (so we know
	// where it is, and this function can work for negative, positive and
	// pseudo indices
	lua_pushvalue(L, index);
	// stack now contains: -1 => table
	lua_pushnil(L);
	// stack now contains: -1 => nil; -2 => table
	while (lua_next(L, -2))
	{
		// stack now contains: -1 => value; -2 => key; -3 => table
		// copy the key so that lua_tostring does not modify the original
		lua_pushvalue(L, -2);
		// stack now contains: -1 => key; -2 => value; -3 => key; -4 => table
		std::string key;
		if (lua_istable(L,-1)) {
			std::shared_ptr<LuaTable> tablekey(new LuaTable);
			GetTable(L,-1,tablekey);
		}
		else  key= lua_tostring(L, -1);
		std::string value;
		if (lua_istable(L,-2)) {
			std::shared_ptr<LuaTable> tablevalue(new LuaTable);
			GetTable(L,-2,tablevalue);
		}
		else value = lua_tostring(L, -2);
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
}

void TableCrawler::Crawl()
{
}

std::shared_ptr<LuaTable> TableCrawler::GetTable() const
{
	return T;
}