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
		self:draw_line(0, self.lines[0])
		for i = 1, 16 do
			self:draw_line(i, self.lines[self.scroll_top + i - 1])
		end
	end
	self.lines = {}
	self.scroll_top = 1
end

function Scene:draw_line(i, line)
	if line then
		line:draw(i)
	else
		Line.styles.null.draw_background(i)
	end
end
