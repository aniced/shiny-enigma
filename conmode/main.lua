WLH = 24
Typesetting = require 'typesetting'
require 'conmode/line'
require 'conmode/scenemanager'
require 'conmode/scene'
require 'conmode/scene_hello'
require 'conmode/scene_message'

Window.set_resizable(false)
Graphics.set_animation_enabled(#arg > 3)

function on.update()
	SceneManager.update()
end

function on.paint()
	SceneManager.on.paint(SceneManager.peek())
end

SceneManager.set_root(Scene_Hello.new())
