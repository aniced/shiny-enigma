Scene_Message = Util.class(Scene)

function Scene_Message:init()
	Scene.init(self)
	self.on.update = function ()
		if Input.triggered(44) then
			SceneManager.back("123")
		end
	end
	self.on.paint = function ()
		Graphics.draw_line(0, "Message")
	end
end
