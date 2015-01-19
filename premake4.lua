include 'premake'

lua = assert(dofile 'premake/recipes/lua.lua')
boost = assert(dofile 'premake/recipes/boost.lua')

OS = os.get()

----------------------------------------------------------------------------------------------------------------

newaction {
   trigger     = "test",
   description = "run lua test",
   execute     = function ()
      os.execute("cd test && busted json_test.lua && cd ..")
   end
}

----------------------------------------------------------------------------------------------------------------

make_solution 'stackcrawlertest'

includedirs { 
	'LuaBridge/Source/LuaBridge',
	'luatablestack',
	'picojson',
	'picojson_serializer',
	lua.includedirs[OS]
}

----------------------------------------------------------------------------------------------------------------
make_shared_lib('stackcrawlertest',{
	'luatablestack/*.*',
	'test/*.*'
})

use_standard 'c++0x'

includedirs {
	lua.includedirs[OS],
	boost.includedirs[OS]
}

libdirs {
	lua.libdirs[OS]
}

links {
	lua.links[OS],
	boost.links[OS]
}

boost:set_libdirs()

targetprefix ""
