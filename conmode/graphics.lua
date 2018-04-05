function Graphics.draw_line(i, s)
	t = Graphics.render_text(0, s)
	t:set_color({r = 255, g = 255, b = 255})
	Graphics.copy({x = 0, y = i * WLH}, t, t:get_rect())
end
