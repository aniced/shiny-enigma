Widget_TextBase = {}

function Widget_TextBase.new(text)
	local self = Widget.new()
	self.color = {r = 0, g = 0, b = 0}
	self.rect.h = WLH

	function self.set_text(text)
		rawset(self, "text", text)
		if self.text:len() == 0 then
			self.texture = nil
		else
			self.texture = Texture.render_text(0, self.text)
			self.texture.set_color(self.color)
		end
	end
	self.set_text(text or "")

	function self.set_color(color)
		self.color = color
		if self.texture then
			self.texture.set_color(self.color)
		end
	end

	return self
end
