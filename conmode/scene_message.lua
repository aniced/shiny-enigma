Scene_Message = Util.class(Scene)

function Scene_Message:init()
	Scene.init(self)
	self.lines[0] = Line.new("Message")
	self.on.update = function ()
		if Input.triggered(44) then
			SceneManager.back("123")
		end
	end
end
