local self = {
	glyphs = {}
}
Typesetting = self

function self.get_glyph(ch)
	if self.glyphs[ch] then
		return self.glyphs[ch]
	else
		local g = Texture.render_text(0, ch)
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
		elseif ch == "\f" then
			i = i + 1
			local param, f = nil, {}
			ch, param, i = text:match("^(" .. utf8.charpattern .. ")%[(.-)%]()", i)
			-- \fc[r255g255b255] = change text color to white
			function f.c()
				for k, v in param:gmatch("([rgb])(%d+)") do
					color[k] = tonumber(v)
				end
			end
			-- \ft[-1] = tab to the last column
			function f.t()
				x = param * WLH
				if x < 0 then
					x = x + Graphics.w
				end
			end
			-- \fi[16] = icon of a filled lozenge
			function f.i()
				local i = tonumber(param)
				Graphics.copy(
					{x = x, y = y + (WLH - 16) / 2},
					Texture.stock[1],
					{x = (i % 16) * 16, y = math.floor(i / 16) * 16, w = 16, h = 16}
				)
				x = x + 16
			end
			f[ch]()
		else
			local g = self.get_glyph(ch)
			g.set_color(color)
			local rect = g.get_rect()
			Graphics.copy({x = x, y = y}, g, rect)
			x = x + rect.w
			i = utf8.offset(text, 2, i)
		end
	end
end
