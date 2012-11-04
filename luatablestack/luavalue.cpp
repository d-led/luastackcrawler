#include "luavalue.h"

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