local testlib=assert(require"stackcrawlertest")
package.path=package.path..";../Lua/?/?.lua"
package.cpath=package.cpath..";./?.so"
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
		end)

		describe("the tables are captured, such as the reference nature of lua tables is preserved, but there is no infinite recursion",function()
			local empty_table={}

			it("should return the table along with a lua-compatible name",function ()
				assert.are.same( { { tostring(empty_table) } }, (json.decode( stack_to_json(empty_table))))
			end)
		end)

	end)

end)