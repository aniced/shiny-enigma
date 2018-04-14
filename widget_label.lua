local superclass = Widget_TextBase
local class = Util.class(superclass)

function class:init(text)
	Widget_TextBase.init(self)
end

function class:draw()
	Graphics.copy(
		{x = self.rect.x, y = self.rect.y},
		self.texture,
		self.texture:get_rect()
	)
end

return class
