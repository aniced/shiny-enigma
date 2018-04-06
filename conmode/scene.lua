Scene = Util.class()

function Scene:init()
	self.on = {}
	self.update = coroutine.wrap(function ()
		while true do
			self.on.update()
			coroutine.yield()
		end
	end)
	self.on.paint = function ()
		for i = 0, 19 do
			Graphics.draw_line(i)
		end
	end
end
