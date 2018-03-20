require 'widget'
require 'label'

w = Label.new("Hi, there!")
w:set_color{r = 255, g = 0, b = 255}

function on_paint()
	w:draw()
end

function on_keydown(k)
	io.write(string.format("%s: %d\n", "Hello", k))
	if k == 96 then
		Window.resize(320, 240)
	elseif k == 41 then
		os.exit()
	end
end

Window.set_fullscreen("borderless")
