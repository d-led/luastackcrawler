#include <lua.hpp>
#include <iostream>

#include "stackcrawlertest_lib.h"

#ifdef _MSC_VER
#define STACKCRAWLERTEST __declspec(dllexport)
#else
#define STACKCRAWLERTEST
#endif

extern "C" STACKCRAWLERTEST int luaopen_stackcrawlertest (lua_State* L) {
	register_stackcrawlertest(L);
	return 1;
}
