#include "luastack.h"


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

size_t LuaStack::size() const
{
	return Values.size();
}
