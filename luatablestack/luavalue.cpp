#include "luavalue.h"
#include <boost/lexical_cast.hpp>
#include <vector>
#include <algorithm>

class type_inferrer_visitor : public boost::static_visitor<LuaType::Type>
{
public:
    LuaType::Type operator()(LuaValue<bool> const& v) const
    {
		return v.Valid() ? LuaType::BOOLEAN : LuaType::NIL;
    }
    
    LuaType::Type operator()(LuaValue<double> const& v) const
    {
		return v.Valid() ? LuaType::NUMBER : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<std::string> const& v) const
    {
		return v.Valid() ? LuaType::STRING : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<std::shared_ptr<LuaTable> > const& v) const
    {
		return v.Valid() ? LuaType::TABLE : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<LuaFunction> const& v) const
    {
		return v.Valid() ? LuaType::FUNCTION : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<LuaThread> const& v) const
    {
		return v.Valid() ? LuaType::THREAD : LuaType::NIL;
	}

	LuaType::Type operator()(LuaValue<LuaUserdata> const& v) const
    {
		return v.Valid() ? LuaType::USERDATA : LuaType::NIL;
    }

	LuaType::Type operator()(LuaValue<LuaNil> const& v) const
    {
		return LuaType::NIL;
    }
};

void LuaTable::Append(LuaMultiValue const& key,LuaMultiValue const& value)
{
	entries.push_back(std::make_pair(key,value));
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
    std::string operator()(LuaValue<bool> const& v) const
    {
		return v.Valid() ? (v.Value()?"true":"false") : "invalid value";
    }
    
    std::string operator()(LuaValue<double> const& v) const
    {
		return v.Valid() ? boost::lexical_cast<std::string>(v.Value()) : "invalid value";
    }

    std::string operator()(LuaValue<std::string> const& v) const
    {
		return v.Valid() ? v.Value() : "invalid value";
    }

    std::string operator()(LuaValue<std::shared_ptr<LuaTable> > const& v) const
    {
		bool already_done=Done(v.Value().get());
		if (!already_done) AddDone(v.Value().get());
		return v.Valid() ? (!already_done ? PrintTable(*v.Value()) : "table*") : "invalid value";
    }

    std::string operator()(LuaValue<LuaFunction> const& v) const
    {
		return v.Valid() ? "function" : "invalid value";
    }

    std::string operator()(LuaValue<LuaThread> const& v) const
    {
		return v.Valid() ? "thread" : "invalid value";
	}

	std::string operator()(LuaValue<LuaUserdata> const& v) const
    {
		return v.Valid() ? "userdata" : "invalid value";
    }

	std::string operator()(LuaValue<LuaNil> const& v) const
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
	for (auto it=T.begin(); it!=T.end();) {
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