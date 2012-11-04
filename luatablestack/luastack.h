#pragma once
#include <string>
#include <vector>

#include "luavalue.h"

class LuaStack {
public:
	void Append(LuaMultiValue const& V);
private:
	std::vector<LuaMultiValue> Values;
};

struct lua_State;
void CrawlStack(lua_State* L,LuaStack& S);