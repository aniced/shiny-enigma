require 'widget'
require 'label'
require 'textbox'

w = Textbox.new()
w:set_text("type here")

function on_paint()
	Graphics.copy(Window.get_rect(), 0, {x = 255, y = 255, w = 1, h = 1})
	w:draw()
end

function on_keydown(k)
	io.write(string.format("%s: %d\n", "Hello", k))
	w:on_keydown(k)
	if k == 96 then
		Window.resize(320, 240)
	elseif k == 41 then
		os.exit()
	end
end
