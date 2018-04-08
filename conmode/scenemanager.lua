SceneManager = {
	stack = {},
	on = nil
}

function SceneManager.peek()
	return SceneManager.stack[#SceneManager.stack]
end

function SceneManager.update(...)
	SceneManager.peek().update(...)
end

function SceneManager.set_root(scene)
	print("Scene root ", scene)
	SceneManager.stack = {scene}
	SceneManager.on = scene.on
end

function SceneManager.call(scene)
	print("Scene call ", scene)
	Graphics.freeze()
	table.insert(SceneManager.stack, scene)
	SceneManager.on = scene.on
	Graphics.transition()
	if coroutine.running() then
		return coroutine.yield()
	end
end

function SceneManager.back(...) -- ‘return’ is a keyword
	print("Scene back ", ...)
	Graphics.freeze()
	table.remove(SceneManager.stack)
	SceneManager.on = SceneManager.peek().on
	SceneManager.update(...)
	Graphics.transition()
end
