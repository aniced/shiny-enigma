local self = {
	stack = nil,
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
end

function self.call(scene)
	print("Scene call ", scene)
	Graphics.freeze()
	table.insert(self.stack, scene)
	Graphics.transition()
	if coroutine.running() then
		return coroutine.yield()
	end
end

function self.back(...) -- ‘return’ is a keyword
	print("Scene back ", ...)
	Graphics.freeze()
	table.remove(self.stack)
	self.update(...)
	Graphics.transition()
end

return self
