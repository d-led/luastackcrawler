#pragma once
#include "luavalue.h"

struct lua_State;
LuaMultiValue GetScalarValue(lua_State* L,int pos);
