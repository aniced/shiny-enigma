Widget_TextBase = Util.class(Widget)

function Widget_TextBase:init(text)
	Widget.init(self)
	self.color = {r = 0, g = 0, b = 0}
	self:set_text(text or "")
	self.rect.h = Widget.WLH
end

function Widget_TextBase:set_text(text)
	self.text = text
	if self.text:len() == 0 then
		self.texture = nil
	else
		self.texture = Graphics.render_text(0, self.text)
		self.texture:set_color(self.color)
	end
end

function Widget_TextBase:set_color(color)
	self.color = color
	if self.texture then
		self.texture:set_color(self.color)
	end
end
