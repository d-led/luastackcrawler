#pragma once
#include <boost/variant.hpp>
#include <string>
#include <vector>

class LuaTable {
};

class LuaFunction {
};

class LuaThread {
};

class LuaUserdata {
};

template <typename T>
class LuaValue
{
	T value;
	bool valid;
public:
	LuaValue(T v,bool ok):value(v),valid(ok){}
};

typedef boost::variant<
	LuaValue<bool>,
	LuaValue<double>,
	LuaValue<std::string>,
	LuaValue<LuaTable>,
	LuaValue<LuaFunction>,
	LuaValue<LuaThread>,
	LuaValue<LuaUserdata>
	 >LuaMultiValue;

struct LuaType {
	enum Type {
		NIL=0,
		BOOLEAN,
		NUMBER,
		STRING,
		TABLE,
		FUNCTION,
		THREAD,
		USERDATA
	};
};

LuaType::Type GetType(LuaMultiValue& v);

class LuaStack {
public:
private:
	std::vector<LuaMultiValue> Values;
};

struct lua_State;
void CrawlStack(lua_State* L,LuaStack& S);