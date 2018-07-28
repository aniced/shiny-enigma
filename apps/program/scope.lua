Scope = {}

-- This class is abstract.
function Scope.new()
	local self = Object.new()
	self.class = Scope

	self.display_name = ""

	-- display_order is an array of number, string and nil.
	--     number = an object in Program#objects
	--     string = some commentary text
	--     nil = a spacing separator
	self.display_order = {}
	-- accept is a boolean hash table.
	-- Different objects accept different kinds of child objects.
	-- An object has local variables if it accpets variables as its children.
	-- (accept[Variable] = true)
	-- The variable IDs are still globally allocated, though.
	self.accept = {}

	-- insert_object([pos_in_display_order,] object_id)
	function self.insert_object(arg1, arg2)
		local pos, value
		if not arg2 then
			pos = #self.display_order + 1
			value = arg1
		else
			pos = arg1
			value = arg2
		end
		if self.accept[value.class] then
			table.insert(self.display_order, pos, value)
		else
			error("not an accepted child object type")
		end
	end

	return self
end
