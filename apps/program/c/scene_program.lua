Scene_Program = {}

function Scene_Program.new()
	local self = Scene.new()

	self.program = Program.new()
	self.program.display_name = "Untitled"

	self.title = function ()
		return "Program - " .. self.program.display_name
	end

	function self.on_update()
	end

	return self
end
