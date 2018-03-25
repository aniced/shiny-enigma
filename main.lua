require 'widget'
require 'widget_textbase'
require 'widget_label'
require 'widget_textbox'

fullscreen = false
w = Widget_Textbox.new()
w:set_text("type here")
w:set_color({r = 128, g = 0, b = 128})

x = 0
function on_paint()
	if x >= 60 then x = 0 end
	Graphics.copy(Window.get_rect(), 0, {x = 255, y = 255, w = 1, h = 1})
	Graphics.copy({x = x, y = 100, w = 10, h = 10}, 0, {x = 0, y = 0, w = 1, h = 1})
	Graphics.draw_9patch({x = 50, y = 50, w = 80, h = 25}, {texture = 0, x = 0, y = 0, w = 4, h = 4, t = 1, r = 2, b = 2, l = 1})
	w:draw()
end

function on_update()
	if Input.key_pressed(96) then
		x = x + 1
	end
	if Input.key_pressed(41) then
		os.exit()
	end
	--if Input.key_trigger(68) then
	if false then
		fullscreen = not fullscreen
		Window.set_fullscreen(fullscreen and "borderless" or "windowed")
	end
end
