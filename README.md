luastackcrawler
===============

A walkable wrapper of the Lua stack,including "unwinding" the tables.
Can be especially useful in conjunction with LuaBridge for supporting variable-length parameter lists and tables as parameters.

Usage
-----

Use the visual studio project to compile in Visual Studio and start the test. Since the dependencies are portable, it is expected, that
the crawler is also portable. This will be tested in the future.

Example of usage:

c++:
````cpp
#include "luastack.h"

static void PrintStack(LuaStack const& S)
{
	for (auto it=S.begin(); it!=S.end(); ++it)
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

...

luabridge::getGlobalNamespace(L).addFunction("trystack",trystack);
````

lua:
````lua
t={}
t[1]="1"
t[2]="2"
trystack("1",t)
````

output:
````
1 {1->1 2->2}
````

Of course, the `ToString` function is meant only for demonstration purposes. By means of the iterators, the stack and the tables can be walked through.
One has to keep in mind, that tables might have keys and values that can create circular references, which are still present in the C++ LuaStack by means of shared pointers to the tables.
The `CrawlStack` and the `ToString` functions keep a temporary registry of visited tables in order to avoid infinite recurrence.

LuaBridge extension
-------------------
The header file `luabridge_extensions.h` allows convenient use the `LuaStack` and `LuaTable` types as input parameters to the lua-bound stack. These classes are not Lua-bound and can be compiled
without Lua, thus keeping the Lua-bound classes separate from Lua. The objects are generated within the extension by crawling the stack using `CrawlStack`. `LuaTable`s are passed in boost::shared_ptr, since they may contain circular references, which are resolved, but preserved during the crawling process.

````cpp
void tryextension(std::string b,boost::shared_ptr<LuaTable> T,std::string e)
{
	std::cout<<b<<" "<<ToString(T)<<" "<<e<<std::endl;
}
````

`LuaStack` is greedy, and thus should be used as the last input parameter, just as `lua_State` in LuaBridge.

````cpp
void trystackextension(std::string b,LuaStack S) {
	std::cout<<b<<" ";
	PrintStack(S);
}
````

Dependencies
------------

 * [lua](http://www.lua.org/) the language, Lua 5.1.4 from [luaforwindows](http://code.google.com/p/luaforwindows/), but it should work for every proper lua which is supported by LuaBridge
 * [LuaBridge](https://github.com/vinniefalco/LuaBridge) for the declarative bindings to Lua
 * [BOOST library](http://www.boost.org/) for `boost::variant`, portable `shared_ptr` and some more

License
-------

This code is distributed under the MIT License:

Copyright (c) 2012 Dmitry Ledentsov

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
