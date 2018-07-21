Program = {}

function Program.new()
	local self = {}
	self.class = Program

	-- All objects share the same set of IDs.
	self.next_id = math.random(1, 999)
	function self.new_id()
		local id = self.next_id
		self.next_id = self.next_id + 1
		return id
	end
	self.objects = {}
	function self.add_object(object)
		local id = self.new_id()
		self.objects[id] = object
		return id
	end

	self.entry_subroutine_id = self.add_object(Subroutine.new())

	local main_module = Module.new()
	main_module.display_order[1] = self.entry_subroutine_id
	self.main_module_id = self.add_object(main_module)

	return self
end
