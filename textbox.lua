Textbox = {}
Textbox.__index = Textbox
setmetatable(Textbox, Widget)

function Textbox.new()
	local self = setmetatable(Widget.new(), Textbox)
	self:set_text("")
	return self
end

function Textbox:set_text(text)
	self.text = text
	if self.text:len() == 0 then
		self.texture = nil
	else
		self.texture = Graphics.render_text(self.text)
		self.texture:set_color({r = 0, g = 0, b = 0})
	end
end

function Textbox:on_keydown(k)
	if k >= 4 and k <= 29 then
		self:set_text(string.char(93 + k))
	end
end

function Textbox:draw()
	Graphics.draw_rect(self.rect)
	if self.texture then
		Graphics.copy(
			{x = self.rect.x, y = self.rect.y},
			self.texture,
			self.texture:get_rect()
		)
	end
end
