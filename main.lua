require 'widget'
require 'label'
require 'textbox'

w = Textbox.new()
w:set_text("type here")

function on_paint()
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
