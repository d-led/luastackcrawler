#pragma once
#include <boost/variant.hpp>
#include <boost/container/vector.hpp>
#include <memory>
#include <string>
#include <functional>

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
	T Value() const { return value; }
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
public:
	void Append(LuaMultiValue const& key,LuaMultiValue const& value);
public:
	typedef boost::container::vector<std::pair<LuaMultiValue,LuaMultiValue> > EntryContainer;
public:
	EntryContainer::const_iterator begin() const;
	EntryContainer::const_iterator end() const;
private:
	EntryContainer entries;
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

LuaType::Type GetType(LuaMultiValue const& v);


typedef std::function<std::string (LuaTable const&)> TablePrinter;
std::string ToString(LuaType::Type t);
std::string ToString(LuaMultiValue const& v);