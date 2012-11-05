#include <lua.hpp>
#include <LuaBridge.h>

#include <iostream>
#include <sstream>

#include "../luatablestack/luastack.h"

void report_errors(lua_State *L, int status)
{
	if ( status!=0 ) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

static void PrintStack(LuaStack const& S)
{
 for (std::vector<LuaMultiValue>::const_iterator it=S.begin(); it!=S.end(); ++it)
	{
		std::cout<<ToString(*it)<<" ";
	}
	std::cout<<std::endl;
}

void trystack(lua_State* L) {
	LuaStack S;
	CrawlStack(L,S);
	PrintStack(S);
}

void trystack2(int a,lua_State* L) {
	LuaStack S;
	CrawlStack(L,S);
	PrintStack(S);
}

int main() 
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	int s = luaL_loadfile(L, "test.lua");

	luabridge::getGlobalNamespace(L)
  .addFunction("trystack",trystack)
  .addFunction("trystack2",trystack2);

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	report_errors(L, s);

	lua_close(L);
}