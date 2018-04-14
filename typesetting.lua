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
	local color = {
		r = initial_color.r, g = initial_color.g,
		b = initial_color.b, a = initial_color.a
	}
	while i <= #text do
		local ch = text:match(utf8.charpattern, i)
		if ch == "\n" then
			x = point.x
			y = y + WLH
			i = i + 1
		elseif ch == "\b" then
			i = i + 1
			local param, t = nil, {}
			ch, param, i = text:match("^(" .. utf8.charpattern .. ")%[(.-)%]()", i)
			function t.c()
				for k, v in param:gmatch("([rgb])%d+") do
					color[k] = tonumber(v)
				end
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
			i = utf8.offset(text, 2, i)
		end
	end
end

return self
