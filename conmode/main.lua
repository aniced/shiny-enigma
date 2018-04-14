WLH = 24
Typesetting = require 'typesetting'
Line = require 'conmode/line'
SceneManager = require 'conmode/scenemanager'
Scene = require 'conmode/scene'
Scene_Hello = require 'conmode/scene_hello'
Scene_Message = require 'conmode/scene_message'

Window.set_resizable(false)
Graphics.set_animation_enabled(#arg > 3)

function on.update()
	SceneManager.update()
end

function on.paint()
	SceneManager.on.paint(SceneManager.peek())
end

SceneManager.set_root(Scene_Hello.new())
