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
		local help_lines = self.lines.help_lines or 0
		for i = -help_lines, -1 do
			self:draw_line(20 + i, self.lines[i])
		end
		for i = 1, 19 - help_lines do
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

function Scene:provide_help(str)
	local _, count = str:gsub("\n", "\n")
	count = count + 1
	self.lines.help_lines = count
	local i = 0
	for text in str:gmatch("[^\n]+") do
		i = i + 1
		self.lines[-count + i - 1] = Line.new(text, Line.styles.help)
	end
end
