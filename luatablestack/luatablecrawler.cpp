#include "luatablecrawler.h"
#include "luavalue.h"

TableCrawler::TableCrawler(lua_State* L,int p):L(L),pos(p)
{
}

std::shared_ptr<LuaTable> TableCrawler::GetTable() const
{
	return T;
}