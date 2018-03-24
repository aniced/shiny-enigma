Widget_Label = Util.class(Widget_TextBase)

function Widget_Label:init(text)
	Widget_TextBase.init(self)
end

function Widget_Label:draw()
	Graphics.copy(
		{x = self.rect.x, y = self.rect.y},
		self.texture,
		self.texture:get_rect()
	)
end
