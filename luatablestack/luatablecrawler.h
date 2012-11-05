#pragma once

#include "luavalue.h"

#include <boost/shared_ptr.hpp>
#include <map>

struct lua_State;

class TableCrawler {
public:
	TableCrawler(lua_State* L,int p);
public:
	boost::shared_ptr<LuaTable> GetTable(); //calls Crawl first
private:
	void Crawl();
	void GetTable(lua_State *L, int index,boost::shared_ptr<LuaTable> T);
private:
	typedef std::map<const void*,boost::shared_ptr<LuaTable> > TableReferences;
	lua_State* L;
	int pos;
	boost::shared_ptr<LuaTable> T;
	TableReferences tables;
};