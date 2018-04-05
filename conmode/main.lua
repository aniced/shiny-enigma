require 'conmode/scenemanager'
require 'conmode/scene'
require 'conmode/scene_hello'

WLH = 24
Window.set_resizable(false)

function draw_line(i, s)
	t = Graphics.render_text(0, s)
	t:set_color({r = 255, g = 255, b = 255})
	Graphics.copy({x = 0, y = i * WLH}, t, t:get_rect())
end

function on.update()
	SceneManager.on.update()
end

function on.paint()
	Graphics.set_color({r = 0, g = 128, b = 0, a = 255})
	Graphics.fill_rect({x = 0, y = 0, w = 640, h = 480})
	SceneManager.on.paint()
end

SceneManager.push(Scene_Hello.new())
