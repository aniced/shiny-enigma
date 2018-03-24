Widget_Label = Util.class(Widget)

function Widget_Label:init(text)
	Widget.init(self)
	self.color = {r = 0, g = 0, b = 0}
	self:set_text(text or "")
end

function Widget_Label:set_text(text)
	self.text = text
	self.texture = Graphics.render_text(0, self.text)
	self.texture:set_color(self.color)
end

function Widget_Label:set_color(color)
	self.color = color
	self.texture:set_color(self.color)
end

function Widget_Label:draw()
	Graphics.copy(
		{x = self.rect.x, y = self.rect.y},
		self.texture,
		self.texture:get_rect()
	)
end
