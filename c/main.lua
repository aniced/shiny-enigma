WLH = 24
Util.dofile 'item'
Util.dofile 'scenemanager'
Util.dofile 'scene'
Util.dofile 'scene_hello'
Util.dofile 'scene_message'
Util.dofile 'scene_form'
Util.dofile 'scene_listeditor'

Window.set_resizable(false)
Graphics.set_animation_enabled(#arg > 3)
Graphics.set_frame_persistent(true)
Graphics.set_frame_opacity(0.125)

function on.update()
	SceneManager.update()
end

function on.paint()
	SceneManager.peek().on_paint()
end

SceneManager.set_root(Scene_Hello.new())
