Scene_Program = {}

function Scene_Program.new()
	local self = Scene.new()

	self.program = Program.new()
	self.program.display_name = "Untitled"

	self.items[0] = Item.new(function ()
		return "Program - " .. self.program.display_name
	end)

	return self
end
