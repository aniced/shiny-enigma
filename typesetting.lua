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

function self.draw_text(point, text, initial_color)
	local i, x, y = 1, point.x, point.y
	local color = initial_color
	while i <= #text do
		local ch = text:sub(i, i)
		if ch == "\n" then
			x = point.x
			y = y + WLH
			i = i + 1
		elseif ch == "\b" then
			i = i + 1
			local param, t = nil, {}
			ch, param, i = text:match("^([cit])%[(%-?[0-9]-)%]()", i)
			param = tonumber(param)
			function t.c()
				color = {
					r = bit.band(param, 255),
					g = bit.band(bit.rshift(param, 8), 255),
					b = bit.rshift(param, 16)
				}
			end
			function t.t()
				x = param * WLH
				if x < 0 then
					x = x + Graphics.get_size()
				end
			end
			t[ch]()
		else
			local g = self.get_glyph(ch)
			g:set_color(color)
			local rect = g:get_rect()
			Graphics.copy({x = x, y = y}, g, rect)
			x = x + rect.w
			i = i + 1
		end
	end
end

return self
