#pragma once

#include "luavalue.h"

#include <memory>
#include <map>

struct lua_State;

class TableCrawler {
public:
	TableCrawler(lua_State* L,int p);
public:
	std::shared_ptr<LuaTable> GetTable(); //calls Crawl first
private:
	void Crawl();
	void GetTable(lua_State *L, int index,std::shared_ptr<LuaTable> T);
private:
	typedef std::map<const void*,std::shared_ptr<LuaTable> > TableReferences;
	lua_State* L;
	int pos;
	std::shared_ptr<LuaTable> T;
	TableReferences tables;
};