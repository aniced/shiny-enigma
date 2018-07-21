Subroutine = {}

function Subroutine.new()
	local self = Scope.new()
	self.class = Subroutine

	self.instructions = {}

	return self
end
