function draw_line(i, s)
	local h = Graphics.get_rect().h
	t = Graphics.render_text(0, s)
	t:set_color({r = 0, g = 0, b = 0})
	Graphics.copy({x = 0, y = h - (8 - i) * WLH}, t, t:get_rect())
end

function on.update()
end

function on.paint()
	Graphics.set_color({r = 0, g = 128, b = 0, a = 255})
	Graphics.fill_rect({x = 0, y = 0, w = 640, h = 480})
end
