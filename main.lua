require 'binaryfilereader'
require 'binaryfilewriter'
require 'widget'
require 'widget_textbase'
require 'widget_label'
require 'widget_textbox'
require 'widget_button'

fullscreen = false
w = Widget_Button.new()
w.rect.x = 100
w.rect.y = 100
w.rect.w = 80
w:set_text("CommandButton1")
w:set_color({r = 0, g = 128, b = 128})

x = 0
y = 100
function draw_debug_line(i, s)
	local h = Graphics.get_rect().h
	t = Graphics.render_text(0, s)
	t:set_color({r = 0, g = 0, b = 0})
	Graphics.copy({x = 0, y = h - (8 - i) * WLH}, t, t:get_rect())
end

function on.paint()
	if x >= 60 then x = 0 end
	Graphics.copy(Graphics.get_rect(), 0, {x = 255, y = 255, w = 1, h = 1})
	Graphics.copy({x = x, y = y, w = 10, h = 10}, 0, {x = 0, y = 0, w = 1, h = 1})
	w:draw()
	local m = Input.mouse_get_point()
	draw_debug_line(1, string.format("set FPS = %d", Graphics.get_fps()))
	draw_debug_line(2, string.format("M(%d, %d)", m.x, m.y))
	draw_debug_line(3, Input.key_get_mods().ctrl and "ctrl" or "---")
	draw_debug_line(4, string.format("{[0] = %q, n = %d}", arg[0], #arg))
	draw_debug_line(5, package.path)
	draw_debug_line(6, Util.rtp("main.lua"))
end

Input.text_start()

function on.update()
	-- only for first frame
	if Input.key_get_mods().ctrl then
		on.update = on_update_c
	else
		on.update = on_update_g
	end
end

function on_update_c()
	os.exit()
end

function on_update_g()
	w:update()
	if first_frame then
		first_frame = false
	end
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
