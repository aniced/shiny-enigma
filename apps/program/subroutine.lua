Subroutine = {}

function Subroutine.new()
	local self = Scope.new()
	self.class = Subroutine
	-- ‘Subroutine’ is just a fancy name of functions.

	self.accept[Class] = true
	self.accept[Subroutine] = true
	self.accept[Variable] = true

	-- Instructions aren't considered child objects
	-- because instructions aren't considered objects.
	self.instructions = {}

	return self
end
