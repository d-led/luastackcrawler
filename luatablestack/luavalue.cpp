#include "luavalue.h"
#include <boost/lexical_cast.hpp>
#include <vector>
#include <algorithm>

class type_inferrer_visitor : public boost::static_visitor<LuaType::Type>
{
public:
    LuaType::Type operator()(bool const& v) const
    {
		return LuaType::BOOLEAN;
    }
    
    LuaType::Type operator()(double const& v) const
    {
		return LuaType::NUMBER;
    }

    LuaType::Type operator()(std::string const& v) const
    {
		return LuaType::STRING;
    }

    LuaType::Type operator()(boost::shared_ptr<LuaTable> const& v) const
    {
		return LuaType::TABLE;
    }

    LuaType::Type operator()(LuaFunction const& v) const
    {
		return LuaType::FUNCTION;
    }

    LuaType::Type operator()(LuaThread const& v) const
    {
		return LuaType::THREAD;
	}

	LuaType::Type operator()(LuaUserdata const& v) const
    {
		return LuaType::USERDATA;
    }

	LuaType::Type operator()(LuaNil const& v) const
    {
		return LuaType::NIL;
    }
};

void LuaTable::Append(LuaMultiValue const& key,LuaMultiValue const& value)
{
	entries.push_back(std::make_pair(key,value));
}

const void* LuaTable::lua_ptr() const
{
	return lua_table_ptr;
}

LuaTable::EntryContainer::const_iterator LuaTable::begin() const
{
	return entries.begin();
}

LuaTable::EntryContainer::const_iterator LuaTable::end() const
{
	return entries.end();
}

LuaType::Type GetType(LuaMultiValue const& v)
{
	return boost::apply_visitor( type_inferrer_visitor(), v );
}

std::string ToString(LuaType::Type t)
{
	if (t==LuaType::NIL) return "NIL";
	if (t==LuaType::BOOLEAN) return "BOOLEAN";
	if (t==LuaType::NUMBER) return "NUMBER";
	if (t==LuaType::STRING) return "STRING";
	if (t==LuaType::TABLE) return "TABLE";
	if (t==LuaType::FUNCTION) return "FUNCTION";
	if (t==LuaType::THREAD) return "THREAD";
	if (t==LuaType::USERDATA) return "USERDATA";
	return "";
}

static std::vector<const void*> done; //hack around static_visitor
static bool Done(const void* v)
{
	return std::find(done.begin(),done.end(),v)!=done.end();
}
static void AddDone(const void* v)
{
	done.push_back(v);
}
static void ClearDone()
{
	done.clear();
}

static std::string PrintTable(LuaTable const& T);

class tostring_visitor : public boost::static_visitor<std::string>
{
public:
    std::string operator()(bool const& v) const
    {
		return (v?"true":"false");
    }
    
    std::string operator()(double const& v) const
    {
		return boost::lexical_cast<std::string>(v);
    }

    std::string operator()(std::string const& v) const
    {
		return v;
    }

    std::string operator()(boost::shared_ptr<LuaTable> const& v) const
    {
		if (!v) return "table[nil]";
		bool already_done=Done(v.get());
		if (!already_done) AddDone(v.get());
		return (!already_done) ? PrintTable(*v) : "table*";
    }

    std::string operator()(LuaFunction const& v) const
    {
		return "function";
    }

    std::string operator()(LuaThread const& v) const
    {
		return "thread";
	}

	std::string operator()(LuaUserdata const& v) const
    {
		return "userdata";
    }

	std::string operator()(LuaNil const& v) const
    {
		return "nil";
    }
};

static std::string _ToString(LuaMultiValue const& v)
{
	return boost::apply_visitor( tostring_visitor(), v );
}

static std::string PrintTable(LuaTable const& T)
{
	std::stringstream s;
	s<<"{";
 for (LuaTable::EntryContainer::const_iterator it=T.begin(); it!=T.end();) {
		s
			<<_ToString(it->first)
			<<"->"
			<<_ToString(it->second);
		if (++it!=T.end()) s<<" ";
	}
	s<<"}";
	return s.str();
}

std::string ToString(LuaMultiValue const& v)
{
	std::string res=_ToString(v);
	ClearDone();
	return res;
}