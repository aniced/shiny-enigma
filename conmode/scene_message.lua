Scene_Message = Util.class(Scene)

function Scene_Message:init()
	Scene.init(self)
	self.lines[0] = Line.new("Message")
	self.lines.help_lines = 1
	self.lines[-1] = Line.new("Space = Return", Line.styles.help)
	self.on.update = function ()
		if Input.triggered(44) then
			SceneManager.back("123")
		end
	end
end
