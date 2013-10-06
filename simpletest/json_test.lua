local testlib=assert(require"stackcrawlertest")
package.path=package.path..";../Lua/?/?.lua"
local json=assert(require"json")

describe("testing libraries are loaded correctly", function()
  assert.is.not_nil(testlib)
  assert.is.not_nil(json)
end)

describe("C++ LuaStackCrawler JSON output", function()
	
	describe("LuaStack wrapper is a greedy wrapper for the lua function call stack", function()

		it("should return simple values",function()
			local t = json.decode(stack_to_json(1))
			assert.are.same( {1} , t )
		end)

	end)

end)