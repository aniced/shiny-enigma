Scene_Hello = Util.class(Scene)

function Scene_Hello:init()
	Scene.init(self)
	self.items[0] = Line.new("Hello!")
	for i = 1, 30 do
		self.items[i] = Line.new(tostring(math.sqrt(i)))
	end
	self:provide_help("Space = Fire a message")
	function self.on:update()
		if Input.triggered(44) then
			self.items[1] = Line.new(SceneManager.call(Scene_Message.new()))
		end
	end
end
