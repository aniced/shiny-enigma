Scene_Hello = Util.class(Scene)

function Scene_Hello:init()
	Scene.init(self)
	self.x = 0
	self.on.update = function ()
		self.x = self.x + 1
		if self.x > 60 then
			self.x = 0
		end
		if Input.triggered(44) then
			Window.set_title(SceneManager.call(Scene_Message.new()))
		end
	end
end
