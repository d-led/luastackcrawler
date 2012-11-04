#include <lua.hpp>
#include <LuaBridge.h>

#include <iostream>

void report_errors(lua_State *L, int status)
{
	if ( status!=0 ) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // remove error message
	}
}

int main() 
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	int s = luaL_loadfile(L, "test.lua");

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}
	report_errors(L, s);
	lua_close(L);
}