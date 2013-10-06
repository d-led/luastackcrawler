local testlib=assert(require"stackcrawlertest")
package.path=package.path..";../Lua/?/?.lua"
print(package.path)
local json=assert(require"json")

describe("testing libraries are loaded correctly", function()
  assert.is.not_nil(testlib)
  assert.is.not_nil(json)
end)

describe("C++ LuaStackCrawler JSON output", function()
	
	describe("LuaStack wrapper is a greedy wrapper for the lua function call stack", function()

		it("should return simple values",function()
			assert.are.same( {1}  ,( json.decode( stack_to_json(1) ) ) )
			assert.are.same( {1.1},( json.decode( stack_to_json(1.1) ) ) )
			assert.are.same( {"1"},( json.decode( stack_to_json("1") ) ) )
			local t={}
			print (t)
			print(stack_to_json( {a=t,b=t} ))
		end)

	end)

end)