Scene_Hello = Util.class(Scene)

function Scene_Hello:init()
	Scene.init(self)
	self.lines[0] = Line.new("Hello!")
	for i = 1, 30 do
		self.lines[i] = Line.new(tostring(math.sqrt(i)))
	end
	self:provide_help("Space = Fire a message")
	self.on.update = function ()
		if Input.triggered(44) then
			self.lines[1] = Line.new(SceneManager.call(Scene_Message.new()), Line.styles.selected)
		end
	end
end
