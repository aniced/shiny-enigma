SceneManager = {
	stack = {},
	on = nil
}

function SceneManager.peek()
	return SceneManager.stack[#SceneManager.stack]
end

function SceneManager.push(scene)
	table.insert(SceneManager.stack, scene)
	SceneManager.on = scene.on
end

function SceneManager.pop()
	table.remove(SceneManager.stack)
	SceneManager.on = SceneManager.peek().on
end
