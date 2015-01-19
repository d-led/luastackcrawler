#include "stackcrawlertest_lib.h"

#include <LuaBridge.h>
#include <string>

#include "../luatablestack/luastack.h"
#include "../luatablestack/luabridge_extensions.h"

#include "picojson_luavalue_serializer.h"

namespace {

	std::string stack_to_json(LuaStack S) {
		SerializableLuaStack s(S);
		return s.Value().serialize();
	}

}

void register_stackcrawlertest (lua_State* L) {
	luabridge::getGlobalNamespace(L)
		.addFunction("stack_to_json",&stack_to_json)
	;
}
