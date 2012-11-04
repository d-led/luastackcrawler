#include "luastack.h"
#include <lua.hpp>

LuaType::Type GetType(LuaMultiValue& v)
{
	return LuaType::NIL;
}

void CrawlStack(lua_State* L,LuaStack& S)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {

		case LUA_TSTRING:  /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;

		case LUA_TBOOLEAN:  /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;

		case LUA_TNUMBER:  /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;

		case LUA_TTABLE:  /* numbers */
			break;

		default:  /* other values */
			printf("%s", lua_typename(L, t));
			break;

		}
		printf("  ");  /* put a separator */
	}
	 printf("\n");  /* end the listing */
}