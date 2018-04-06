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
	local text, pos, i = nil, 1, 0
	local _, count = str:gsub("\n", "\n")
	count = count + 1
	while true do
		local pos_new
		text, pos_new = str:match("^(.-)\n()", pos)
		if not text then break end
		i = i + 1
		self.lines[-count + i - 1] = Line.new(text, Line.styles.help)
		pos = pos_new
	end
	text = str:sub(pos)
	self.lines[-1] = Line.new(text, Line.styles.help)
	self.lines.help_lines = count
end
