local superclass = Scene
local class = Util.class(superclass)

function class:init()
	superclass.init(self)
	self.items[0] = Line.new("Message")
	self:provide_help("Space = Return\nHello, world!")
	function self.on:update()
		if Input.triggered(44) then
			SceneManager.back("123")
		end
	end
end

return class
