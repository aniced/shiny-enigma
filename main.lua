require 'widget'
require 'label'
require 'textbox'

fullscreen = false
w = Textbox.new()
w:set_text("type here")

function on_paint()
	Graphics.copy(Window.get_rect(), 0, {x = 255, y = 255, w = 1, h = 1})
	Graphics.draw_9patch({x = 50, y = 50, w = 80, h = 25}, {texture = 0, x = 0, y = 0, w = 4, h = 4, t = 1, r = 2, b = 2, l = 1})
	w:draw()
end

function on_keydown(k)
	io.write(string.format("%s: %d\n", "Hello", k))
	w:on_keydown(k)
	if k == 96 then
		Window.resize(320, 240)
	elseif k == 41 then
		os.exit()
	elseif k == 68 then
		fullscreen = not fullscreen
		Window.set_fullscreen(fullscreen and "borderless" or "windowed")
	end
end
