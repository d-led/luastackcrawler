#include <lua.hpp>
#include <LuaBridge.h>

#include <iostream>
#include <sstream>
#include <algorithm>

#include "../luatablestack/luatablecrawler.h"
#include "../luatablestack/luabridge_extensions.h"

#include "picojson_luavalue_serializer.h"

void report_errors(lua_State *L, int status)
{
	if ( status!=0 ) {
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

static std::string PrintStack(LuaStack const& S)
{
	std::stringstream test_output;
	for (std::vector<LuaMultiValue>::const_iterator it=S.begin(); it!=S.end(); ++it)
	{
		test_output<<ToString(*it);
		test_output<<" ";
	}
	std::string teststring=test_output.str();
	std::cout<<teststring<<std::endl;
	return teststring;
}

std::string trystack(lua_State* L) {
	LuaStack S;
	CrawlStack(L,S);
	return PrintStack(S);
}

std::string trystack2(int a,lua_State* L) {
	LuaStack S;
	CrawlStack(L,S);
	return PrintStack(S);
}

std::string tryextension(std::string b,boost::shared_ptr<LuaTable> T,std::string e)
{
	std::string res=b+" "+ToString(T)+" ";
	std::cout<<res<<std::endl;
	return res;
}

std::string trystackextension(std::string b,LuaStack S) {
	SerializableLuaStack s(S);
	std::cout<<picojson::convert::to_string(s)<<std::endl;
	std::cout<<b<<" ";
	return PrintStack(S);
}

void tryluaref(luabridge::LuaRef ref)
{
	std::cout<<__FUNCTION__<<"--------------"<<std::endl;
	std::cout
		<<"nil:"<<ref.isNil()<<std::endl
		<<"function:"<<ref.isFunction()<<std::endl
		<<"userdata:"<<ref.isUserdata()<<std::endl
		<<"lightuserdata:"<<ref.isLightUserdata()<<std::endl
		<<"number:"<<ref.isNumber()<<std::endl
		<<"string:"<<ref.isString()<<std::endl
		<<"table:"<<ref.isTable()<<std::endl
		<<"thread:"<<ref.isThread()<<std::endl
		;
}

static bool value_is_number(std::pair<LuaMultiValue,LuaMultiValue> const& entry) {
	  return GetType(entry.first) == LuaType::NUMBER;	
}

int ArraySize(boost::shared_ptr<LuaTable> T)
{
 if (!T)
  return 0;
 return std::count_if(T->begin(),T->end(),value_is_number);
}

int main() 
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	int s = luaL_loadfile(L, "test.lua");

	luabridge::getGlobalNamespace(L)
		.addFunction("trystack",trystack)
		.addFunction("trystack2",trystack2)
		.addFunction("tryextension",tryextension)
		.addFunction("trystackextension",trystackextension)
		.addFunction("tryluaref",tryluaref)
  .addFunction("ArraySize",ArraySize)
		;

	if ( s==0 ) {
		s = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	report_errors(L, s);

	lua_close(L);
}
