Line = Util.class()

Line.styles = {
	normal = {
		color = {r = 255, g = 255, b = 255},
		draw_background = function (i)
			Graphics.tile(Line.get_rect(i), 0, {x = 32, y = 0, w = 8, h = 8})
		end,
	},
	null = {
		color = {r = 0, g = 0, b = 0},
		draw_background = function (i)
			Graphics.tile(Line.get_rect(i), 0, {x = 40, y = 0, w = 8, h = 8})
		end,
	},
	selected = {
		color = {r = 45, g = 54, b = 9},
		draw_background = function (i)
			local patch = {
				texture = 0,
				x = 17, y = 0, w = 5, h = 5,
				t = 2, r = 2, b = 2, l = 2
			}
			Graphics.draw_9patch(Line.get_rect(i), patch)
		end,
	},
}

function Line.get_rect(i)
	return {x = 0, y = i * WLH, w = Graphics.get_rect().w, h = WLH}
end

function Line:init(text, style)
	self:set_style(style)
	self:set_text(text or "")
end

function Line:set_style(style)
	self.style = style or Line.styles.normal
	if self.texture then
		self.texture:set_color(self.style.color)
	end
end

function Line:set_text(text)
	if text and #text ~= 0 then
		self.texture = Graphics.render_text(0, text)
		self.texture:set_color(self.style.color)
	else
		self.texture = nil
	end
end

function Line:draw(i)
	local rect = Line.get_rect(i)
	self.style.draw_background(i)
	if self.texture then
		Graphics.copy({
			x = 0,
			y = (i + 0.5) * WLH - self.texture:get_rect().h / 2,
		}, self.texture, self.texture:get_rect())
	end
end
