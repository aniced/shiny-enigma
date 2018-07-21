WLH = 24
require 'c/item'
require 'c/scenemanager'
require 'c/scene'
require 'c/scene_hello'
require 'c/scene_message'

Window.set_resizable(false)
Graphics.set_animation_enabled(#arg > 3)

function on.update()
	SceneManager.update()
end

function on.paint()
	SceneManager.peek().on_paint()
end

SceneManager.set_root(Scene_Hello.new())
