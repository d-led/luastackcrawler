local OS=os.get()

local cmd = {
 dir =     { linux = "ls", windows = "dir", macosx = "ls" },
 libdirs = { linux = { "./Lua/lib"}, windows = "./Lua/lib", macosx = { "" } },
 includedirs = { linux = { "./Lua/include"}, windows = "./Lua/include", macosx = { "" } },
 links = { linux = "lua", windows = "lua5.1", macosx = "lua" }
}

local cfg={}

for i,v in pairs(cmd) do
 cfg[i]=cmd[i][OS]
end

local function concat( array1, array2 )
	local res = {}
	for _,v in ipairs(array1) do
		res[#res+1] = v
	end
	for _,v in ipairs(array2) do
		res[#res+1] = v
	end
	return res
end

-- Apply to current "filter" (solution/project)
function DefaultConfig()
	location "Build"
	configuration "Debug"
		defines { "DEBUG", "_DEBUG" }
		objdir "Build/obj"
		targetdir "simpletest"
		targetprefix ""
		flags { "Symbols" }
	configuration "Release"
		defines { "RELEASE" }
		objdir "Build/obj"
		targetdir "simpletest"
		targetprefix ""
		flags { "Optimize" }
	configuration "*" -- to reset configuration filter
end

function CompilerSpecificConfiguration()
	configuration {"xcode*" }

	configuration {"gmake"}
		buildoptions { "-v -stdlib=libc++ -std=c++11 -fPIC" }

	configuration {"codeblocks" }

	configuration { "vs*"}

	configuration { "macosx" }
		postbuildcommands { "rename ./simpletest/stackcrawlertest.dylib ./simpletest/stackcrawlertest.so" }

	configuration { "*" }
end

----------------------------------------------------------------------------------------------------------------

newaction {
   trigger     = "test",
   description = "run lua test",
   execute     = function ()
      os.execute("cd simpletest && lua -l stackcrawlertest json_test.lua && cd ..")
   end
}

----------------------------------------------------------------------------------------------------------------

-- A solution contains projects, and defines the available configurations
local sln=solution "stackcrawlertest"
	location "Build"
		sln.absbasedir=path.getabsolute(sln.basedir)
		configurations { "Debug", "Release" }
		platforms { "native","x32", "x64" }
   print 'project'
		libdirs ( cfg.libdirs )
		includedirs (concat ( cfg.includedirs, { 
			[[./LuaBridge/Source/LuaBridge]],
			[[./luatablestack]],
			[[./picojson]],
			[[./picojson_serializer]]
		} ) )
		vpaths {
			["Headers"] = {"**.h","**.hpp"},
			["Sources"] = {"**.c", "**.cpp"},
		}

----------------------------------------------------------------------------------------------------------------
   local dll=project "stackcrawlertest"
   location "Build"
		kind "SharedLib"
		DefaultConfig()
		language "C++"
		files {
			"./pugilua/*.h",
			"./luatablestack/*.cpp",
			"./simpletest/*.cpp",
			"./simpletest/*.h"
		}
		links(cfg.links)
		CompilerSpecificConfiguration()
