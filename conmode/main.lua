WLH = 24
require 'conmode/line'
require 'conmode/scenemanager'
require 'conmode/scene'
require 'conmode/scene_hello'
require 'conmode/scene_message'

Window.set_resizable(false)

function on.update()
	SceneManager.update()
end

function on.paint()
	Graphics.set_color({r = 0, g = 128, b = 0, a = 255})
	Graphics.fill_rect({x = 0, y = 0, w = 640, h = 480})
	SceneManager.on.paint()
end

SceneManager.call(Scene_Hello.new())
