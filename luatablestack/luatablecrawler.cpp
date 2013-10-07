#include "luatablecrawler.h"
#include "luavalue.h"
#include "luavalueutil.h"
#include <lua.hpp>

TableCrawler::TableCrawler(lua_State* L,int p):
	L(L),
	pos(p)
{
}

/// http://stackoverflow.com/a/6142700/847349 modified
void TableCrawler::GetTable(lua_State *L, int index,boost::shared_ptr<LuaTable> T)
{
	const void* ref=lua_topointer(L,index);
	tables[ref]=T;
	T->lua_table_ptr=ref;

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
		////////////////////////////////////////////////////////////////
		LuaMultiValue key=LuaNil();
		if (lua_istable(L,-1)) {
			const void* kref=lua_topointer(L,-1);
			TableReferences::iterator f=tables.find(kref);
			if (f!=tables.end()) {
				key=f->second;
			} else {
				boost::shared_ptr<LuaTable> tablekey(new LuaTable);
				GetTable(L,-1,tablekey);
				key=tablekey;
			}
		} else {
			key = GetScalarValue(L, -1);
		}
		////////////////////////////////////////////////////////////////
		LuaMultiValue value=LuaNil();
		if (lua_istable(L,-2)) {
			const void* kref=lua_topointer(L,-2);
			TableReferences::const_iterator f=tables.find(kref);
			if (f!=tables.end()) {
				value=f->second;
			} else {
				boost::shared_ptr<LuaTable> tablevalue(new LuaTable);
				GetTable(L,-2,tablevalue);
				value=tablevalue;
			}
		} else {
			value = GetScalarValue(L, -2);
		}
		// evaluate before popping
		T->Append(key,value);
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
	GetTable(L,pos,T);
}

boost::shared_ptr<LuaTable> TableCrawler::GetTable()
{
	T.reset(new LuaTable);
	Crawl();
	return T;
}