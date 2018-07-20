Scope = {}

-- This class is abstract.
function Scope.new()
	local self = {}

	self.name = ""

	-- Tables in …_display_order are arrays of number, string and nil.
	--     number = an object in objects
	--     string = some commentary text
	--     nil = a spacing separator
	-- An object has local variables if it has a field called variables_display_order.
	-- The variable IDs are still globally allocated, though.
	self.variables_display_order = {}
	self.subroutines_display_order = {}
	self.classes_display_order = {}

	return self
end
