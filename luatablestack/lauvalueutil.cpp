#include "luavalueutil.h"
#include <lua.hpp>

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