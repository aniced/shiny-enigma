Widget = require 'widget'
Widget_TextBase = require 'widget_textbase'
Widget_Label = require 'widget_label'
Widget_Textbox = require 'widget_textbox'
Widget_Button = require 'widget_button'

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
	t = Graphics.render_text(0, s)
	t:set_color({r = 0, g = 0, b = 0})
	Graphics.copy({x = 0, y = Graphics.h - (8 - i) * WLH}, t, t:get_rect())
end

Input.text_start()

function on.update()
	w:update()
	if first_frame then
		first_frame = false
	end
	if Input.pressed(96) then
		x = x + 1
	end
	if Input.repeated(97) then
		y = y + 10
	end
	if Input.triggered(41) then
		os.exit()
	end
	if Input.triggered(68) then
		fullscreen = not fullscreen
		Window.set_fullscreen(fullscreen and "borderless" or "windowed")
	end
end

function on.paint()
	if x >= 60 then x = 0 end
	Graphics.copy(Graphics, 0, {x = 255, y = 255, w = 1, h = 1})
	Graphics.copy({x = x, y = y, w = 10, h = 10}, 0, {x = 0, y = 0, w = 1, h = 1})
	w:draw()
	draw_debug_line(1, string.format("set FPS = %d", Graphics.get_fps()))
	draw_debug_line(3, Input.mods().ctrl and "ctrl" or "---")
	draw_debug_line(4, string.format("{[0] = %q, n = %d}", arg[0], #arg))
	draw_debug_line(5, package.path)
	draw_debug_line(6, Util.rtp("main.lua"))
end
