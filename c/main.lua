WLH = 24
Util.dofile 'item'
Util.dofile 'scenemanager'
Util.dofile 'scene'
Util.dofile 'scene_hello'
Util.dofile 'scene_message'

Window.set_resizable(false)
Graphics.set_animation_enabled(#arg > 3)

function on.update()
	SceneManager.update()
end

function on.paint()
	SceneManager.peek().on_paint()
end

SceneManager.set_root(Scene_Hello.new())
