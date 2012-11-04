#include "luavalue.h"
#include <lua.hpp>

class type_inferrer_visitor : public boost::static_visitor<LuaType::Type>
{
public:
    LuaType::Type operator()(LuaValue<bool>& v) const
    {
		return v.Valid() ? LuaType::BOOLEAN : LuaType::NIL;
    }
    
    LuaType::Type operator()(LuaValue<double>& v) const
    {
		return v.Valid() ? LuaType::NUMBER : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<std::string>& v) const
    {
		return v.Valid() ? LuaType::STRING : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<std::shared_ptr<LuaTable> >& v) const
    {
		return v.Valid() ? LuaType::TABLE : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<LuaFunction>& v) const
    {
		return v.Valid() ? LuaType::FUNCTION : LuaType::NIL;
    }

    LuaType::Type operator()(LuaValue<LuaThread>& v) const
    {
		return v.Valid() ? LuaType::THREAD : LuaType::NIL;
	}

	LuaType::Type operator()(LuaValue<LuaUserdata>& v) const
    {
		return v.Valid() ? LuaType::USERDATA : LuaType::NIL;
    }

	LuaType::Type operator()(LuaValue<LuaNil>& v) const
    {
		return LuaType::NIL;
    }
};

LuaType::Type GetType(LuaMultiValue& v)
{
	return boost::apply_visitor( type_inferrer_visitor(), v );
}

LuaMultiValue GetScalarValue(lua_State* L,int pos)
{
	int t = lua_type(L, pos);
	switch (t) {

	case LUA_TSTRING:  /* strings */
		{
			std::string v;
			v=lua_tostring(L,pos);
			return MakeLuaValue(v);
		}
		break;

	case LUA_TBOOLEAN:  /* booleans */
		{
			bool v;
			v=lua_toboolean(L,pos)?true:false;
			return MakeLuaValue(v);
		}
		break;

	case LUA_TNUMBER:  /* numbers */
		{
			double v;
			v=lua_tonumber(L,pos);
			return MakeLuaValue(v);
		}
		break;

	default:  /* other values */
		return MakeLuaValue(LuaNil());
		break;
	}
}