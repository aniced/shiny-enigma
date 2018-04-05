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

function SceneManager.call(scene)
	print("Scene call ", scene)
	table.insert(SceneManager.stack, scene)
	SceneManager.on = scene.on
	if coroutine.running() then
		return coroutine.yield()
	end
end

function SceneManager.back(...) -- ‘return’ is a keyword
	print("Scene back ", ...)
	table.remove(SceneManager.stack)
	SceneManager.on = SceneManager.peek().on
	SceneManager.update(...)
end
