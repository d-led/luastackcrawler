#include "stackcrawlertest_lib.h"

#include <LuaBridge.h>
#include <string>


void register_stackcrawlertest (lua_State* L) {
	luabridge::getGlobalNamespace(L)
	;
}
