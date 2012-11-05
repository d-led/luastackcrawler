#pragma once

#include "luastack.h"

struct lua_State;
void CrawlStack(lua_State* L,LuaStack& S);
void CrawlStack(lua_State* L,LuaStack& S,int level);