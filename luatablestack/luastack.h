#pragma once
#include <string>
#include <vector>

#include "luavalue.h"

class LuaStack {
public:
	void Append(LuaMultiValue const& V);
public:
	std::vector<LuaMultiValue>::const_iterator begin() const;
	std::vector<LuaMultiValue>::const_iterator end() const;
	size_t size() const;
private:
	std::vector<LuaMultiValue> Values;
};
