local self = {
	glyphs = {}
}

function self.get_glyph(ch)
	if self.glyphs[ch] then
		return self.glyphs[ch]
	else
		local g = Graphics.render_text(0, ch)
		self.glyphs[ch] = g
		return g
	end
end

function self.draw_text(point, text, color)
	local x, y = point.x, point.y
	for ch in text:gmatch(".") do
		local g = self.get_glyph(ch)
		g:set_color(color)
		Graphics.copy({x = x, y = y}, g, g:get_rect())
		x = x + 15
	end
end

return self
