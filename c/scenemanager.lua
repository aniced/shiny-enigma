local self = {
	stack = nil,
	transition_speed = 10,
}
SceneManager = self

function self.peek()
	return self.stack[#self.stack]
end

function self.update()
	self.peek().update()
end

function self.check_scene(obj)
	if type(obj) ~= "table" then
		error("not an object")
	elseif not obj.is_scene then
		error("not a scene")
	end
	return obj
end

function self.set_root(scene)
	print("Scene root ", scene)
	self.check_scene(scene)
	self.stack = {scene}
	Graphics.fade_in()
end

function self.call(scene)
	print("Scene call ", scene)
	self.check_scene(scene)
	Graphics.freeze()
	table.insert(self.stack, scene)
	Graphics.transition(self.transition_speed)
	if coroutine.running() then
		coroutine.yield()
		return unpack(self.back_value)
	end
end

function self.back(...) -- ‘return’ is a keyword
	print("Scene back ", ...)
	Graphics.freeze()
	table.remove(self.stack)
	self.back_value = {...}
	Graphics.transition(self.transition_speed)
end
