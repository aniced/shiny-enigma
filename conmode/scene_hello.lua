Scene_Hello = Util.class(Scene)

function Scene_Hello:init()
	Scene.init(self)
	self.items[0] = Line.new("Hello!")
	for i = 1, 30 do
		self.items[i] = Line.new(string.format(
			"%d\bt[4]%f\bt[-1]\bc[0]|",
			i, math.sqrt(i))
		)
	end
	self:provide_help("Space = Fire a message; Esc = Exit")
	function self.on:update()
		if Input.triggered(44) then
			self.items[1] = Line.new(SceneManager.call(Scene_Message.new()))
		end
		if Input.triggered(41) then
			os.exit()
		end
	end
end
