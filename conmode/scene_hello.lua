local superclass = Scene
local class = Util.class(superclass)

function class:init()
	superclass.init(self)
	self.items[0] = Line.new("你好！")
	for i = 1, 30 do
		self.items[i] = Line.new(string.format(
			"%d\bt[4]%f\bt[-1]\bc[r0g0b0]|",
			i, math.sqrt(i))
		)
	end
	self:provide_help("Space = Fire a message; Esc = Exit")
end

function class:on_update()
	if Input.triggered(44) then
		self.items[1] = Line.new(SceneManager.call(Scene_Message.new()))
	end
	if Input.triggered(41) then
		os.exit()
	end
end

return class
