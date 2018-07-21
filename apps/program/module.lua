Module = {}

function Module.new()
	local self = Scope.new()
	self.class = Module

	-- Modules are a way to organize things such as subroutines and classes.
	-- A module serve as a container, like namespaces in languages such as C++.
	-- However, as name conflicts don't occur here, modules are logical.
	-- A module can contain other modules.
	self.accept[Module] = true
	self.accept[Class] = true
	self.accept[Subroutine] = true
	self.accept[Variable] = true

	return self
end
