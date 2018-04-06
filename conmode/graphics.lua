Graphics.line_styles = {
	normal = {
		color = {r = 255, g = 255, b = 255},
		draw_background = function (rect)
			Graphics.copy(rect, 0, {x = 32, y = 0, w = 8, h = 8})
		end,
	},
	null = {
		color = {r = 0, g = 0, b = 0},
		draw_background = function (rect)
			Graphics.copy(rect, 0, {x = 40, y = 0, w = 8, h = 8})
		end,
	},
}

Graphics.lines = {}

function Graphics.get_line_rect(i)
	return {x = 0, y = i * WLH, w = Graphics.get_rect().w, h = WLH}
end

function Graphics.set_line(i, text, style)
	style = style or Graphics.line_styles.normal
	local line = {
		style = style,
		texture = Graphics.render_text(0, text),
	}
	line.texture:set_color(style.color)
	line.dest_rect = {
		x = 0,
		y = (i + 0.5) * WLH - line.texture:get_rect().h / 2,
	}
	Graphics.lines[i] = line
end

function Graphics.draw_line(i)
	local line = Graphics.lines[i]
	local rect = Graphics.get_line_rect(i)
	if line then
		line.style.draw_background(rect)
		Graphics.copy(line.dest_rect, line.texture, line.texture:get_rect())
	else
		Graphics.line_styles.null.draw_background(rect)
	end
end

Graphics.set_line(1, "Hello, world!", Graphics.line_styles.normal)
