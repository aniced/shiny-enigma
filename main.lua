require 'widget'
require 'widget_textbase'
require 'widget_label'
require 'widget_textbox'

fullscreen = false
w = Widget_Textbox.new()
w:set_text("type here")
w:set_color({r = 128, g = 0, b = 128})

x = 0
y = 100
function draw_debug_line(i, s)
	local h = Graphics.get_rect().h
	t = Graphics.render_text(0, s)
	t:set_color({r = 0, g = 0, b = 0})
	Graphics.copy({x = 0, y = h - (8 - i) * Widget.WLH}, t, t:get_rect())
end

function on_paint()
	if x >= 60 then x = 0 end
	Graphics.copy(Graphics.get_rect(), 0, {x = 255, y = 255, w = 1, h = 1})
	Graphics.copy({x = x, y = y, w = 10, h = 10}, 0, {x = 0, y = 0, w = 1, h = 1})
	Graphics.draw_9patch({x = 50, y = 50, w = 80, h = 25}, {texture = 0, x = 0, y = 0, w = 4, h = 4, t = 1, r = 2, b = 2, l = 1})
	w:draw()
	local m = Input.mouse_get_point()
	draw_debug_line(1, string.format("set FPS = %d", Graphics.get_fps()))
	draw_debug_line(2, string.format("M(%d, %d)", m.x, m.y))
	draw_debug_line(3, Rect.point_in_rect(Input.mouse_get_point(), {x = 50, y = 50, w = 80, h = 25}) and "Good" or "Not")
end

Input.text_start()
function on_update()
	w:update()
	if Input.key_pressed(96) then
		x = x + 1
	end
	if Input.key_repeated(97) then
		y = y + 10
	end
	if Input.key_triggered(41) then
		os.exit()
	end
	if Input.key_triggered(68) then
		fullscreen = not fullscreen
		Window.set_fullscreen(fullscreen and "borderless" or "windowed")
	end
end
