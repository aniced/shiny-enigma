Class = {}

function Class.new()
	local self = Scope.new()
	self.class = Class

	self.accept[Class] = true
	-- There are no special ‘member functions,’
	-- as subroutines with a correctly typed ‘self’ parameter positioned the first
	-- can be called as an instance method as in Lua.
	self.accept[Subroutine] = true
	self.accept[Variable] = true

	return self
end
