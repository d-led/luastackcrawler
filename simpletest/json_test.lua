local testlib=assert(require"stackcrawlertest")

describe("testing library is loaded correctly", function()
  assert.is.not_nil(testlib)
end)

describe("C++ LuaStackCrawler JSON output", function()
	
	describe("LuaStack wrapper is a greedy wrapper for the lua function call stack", function()

		it("should return simple values",function()
			print(stack_to_json(1))
		end)

	end)

end)