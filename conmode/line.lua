Line = Util.class()

Line.styles = {
	normal = {
		color = {r = 255, g = 255, b = 255},
		draw_background = function (i)
			Graphics.copy(Line.get_rect(i), 0, {x = 32, y = 0, w = 8, h = 8})
		end,
	},
	null = {
		color = {r = 0, g = 0, b = 0},
		draw_background = function (i)
			Graphics.copy(Line.get_rect(i), 0, {x = 40, y = 0, w = 8, h = 8})
		end,
	},
}

function Line.get_rect(i)
	return {x = 0, y = i * WLH, w = Graphics.get_rect().w, h = WLH}
end

function Line:init(text, style)
	self.style = style or Graphics.line_styles.normal
	self:set_text(text)
end

function Line:set_text(text)
	if text then
		self.texture = Graphics.render_text(0, text or "")
		self.texture:set_color(style.color)
	else
		self.texture = nil
	end
end

function Line:draw(i)
	local rect = Graphics.get_line_rect(i)
	self.style.draw_background(rect)
	if self.texture then
		Graphics.copy({
			x = 0,
			y = (i + 0.5) * WLH - self.texture:get_rect().h / 2,
		}, self.texture, self.texture:get_rect())
	end
end
