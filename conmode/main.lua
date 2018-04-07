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
	SceneManager.on.paint(SceneManager.peek())
end

SceneManager.call(Scene_Hello.new())
