require 'label'

function on_paint()
	Graphics.copy({x = 0, y = 0}, 0, {x = 0, y = 0, w = 256, h = 256});
	Graphics.copy({x = 0, y = 0}, t, t:get_rect());
	Graphics.draw_rect({x = 50, y = 50, w = 100, h = 100})
	Graphics.draw_line({x = 30, y = 20}, {x = 100, y = 60}, {x = 120, y = 40})
end

function on_keydown(k)
	io.write(string.format("%s: %d\n", "Hello", k))
	if k == 96 then
		Window.resize(320, 240)
	elseif k == 41 then
		os.exit()
	end
end

io.write("Lua initialized\n")
t = Graphics.render_text("fuck")
t:set_color({r = 0, g = 0, b = 255})
Window.set_title("")
