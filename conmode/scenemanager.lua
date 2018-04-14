local self = {
	stack = {},
	on = nil,
}

function self.peek()
	return self.stack[#self.stack]
end

function self.update(...)
	self.peek().update(...)
end

function self.set_root(scene)
	print("Scene root ", scene)
	self.stack = {scene}
	self.on = scene.on
end

function self.call(scene)
	print("Scene call ", scene)
	Graphics.freeze()
	table.insert(self.stack, scene)
	self.on = scene.on
	Graphics.transition()
	if coroutine.running() then
		return coroutine.yield()
	end
end

function self.back(...) -- ‘return’ is a keyword
	print("Scene back ", ...)
	Graphics.freeze()
	table.remove(self.stack)
	self.on = self.peek().on
	self.update(...)
	Graphics.transition()
end

return self
