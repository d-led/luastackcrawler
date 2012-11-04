#pragma once
#include <boost/variant.hpp>
#include <boost/container/vector.hpp>
#include <memory>

class LuaNil {
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
	bool Valid() const { return valid; }
};

template <typename T>
LuaValue<T> MakeLuaValue(T v)
{
	return LuaValue<T>(v,true);
}

class LuaTable;
typedef boost::variant<
	LuaValue<LuaNil>,
	LuaValue<bool>,
	LuaValue<double>,
	LuaValue<std::string>,
	LuaValue<std::shared_ptr<LuaTable> >,
	LuaValue<LuaFunction>,
	LuaValue<LuaThread>,
	LuaValue<LuaUserdata>
	 >LuaMultiValue;

class LuaTable {
	boost::container::vector<std::pair<LuaMultiValue,LuaMultiValue> > entries;
};

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

struct lua_State;
LuaMultiValue GetScalarValue(lua_State* L,int pos);