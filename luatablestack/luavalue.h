#pragma once
#include <boost/variant.hpp>
#include <boost/container/vector.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/functional.hpp>

class LuaNil {
};

class LuaFunction {
};

class LuaThread {
};

class LuaUserdata {
};

class LuaTable;
typedef boost::variant<
	LuaNil,
	bool,
	double,
	std::string,
	boost::shared_ptr<LuaTable>,
	LuaFunction,
	LuaThread,
	LuaUserdata
	 >LuaMultiValue;

class TableCrawler;
class LuaTable {
public:
	void Append(LuaMultiValue const& key,LuaMultiValue const& value);
public:
	typedef boost::container::vector<std::pair<LuaMultiValue,LuaMultiValue> > EntryContainer;
public:
	const void* lua_ptr() const;
public:
	EntryContainer::const_iterator begin() const;
	EntryContainer::const_iterator end() const;
private:
	friend class TableCrawler;
	EntryContainer entries;
	const void* lua_table_ptr;
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

std::string ToString(LuaType::Type t);
std::string ToString(LuaMultiValue const& v);