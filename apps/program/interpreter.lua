Interpreter = {}

function Interpreter.new(max_depth)
	local self = {}

	-- max_depth is decreased recursively, e.g.,
	-- if the parent Interpreter has a max_depth of 100,
	-- the child Interpreter created by it will have a max_depth of 99,
	-- the grandchild 98, and so on.
	if max_depth < 0 then
		error("Interpreter: too much recursion")
	end

	function self.run()
	end

	return self
end
