luastackcrawler
===============

A walkable wrapper of the Lua stack,including "unwinding" the tables.
Can be especially useful in conjunction with LuaBridge for supporting variable-length parameter lists and tables as parameters.

Usage
-----

Use the visual studio project to compile in Visual Studio and start the test.
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

Dependencies
------------

 * [lua](http://www.lua.org/) the language, Lua 5.1.4 from [luaforwindows](http://code.google.com/p/luaforwindows/), but it should work for every proper lua which is supported by LuaBridge
 * [LuaBridge](https://github.com/vinniefalco/LuaBridge) for the declarative bindings to Lua
 * [BOOST library](http://www.boost.org/) for boost::variant, boost::container and some more

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
