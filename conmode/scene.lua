Scene = Util.class()

function Scene:init()
	self.on = {}
	self.update = coroutine.wrap(function ()
		while true do
			self.on.update()
			coroutine.yield()
		end
	end)
end
