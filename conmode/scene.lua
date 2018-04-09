Scene = Util.class()
-- Scene_Base doesn't really make sense here.
-- Neither does Scene_Selectable because all scenes have a selectable interface.

function Scene:init()
	self.on = {}
	self.update = coroutine.wrap(function ()
		while true do
			self.on.update_general(self)
			self.on.update_cursor(self)
			self.on.update(self)
			coroutine.yield()
		end
	end)
	function self.on:update_general()
		if Input.action() then
			Graphics.stop_transition()
		end
	end
	function self.on:update_cursor()
		if #self.items == 0 then
			-- nothing to scroll
			return
		end
		local d = 0 -- change to the cursor index
		if Input.repeated(81) then d = d + 1 end
		if Input.repeated(82) then d = d - 1 end
		if Input.repeated(75) then d = d - 10 end
		if Input.repeated(78) then d = d + 10 end
		if cursor_delta == 0 then return end
		self.cursor = self.cursor + d
		if self.cursor > #self.items then
			self.cursor = #self.items
		elseif self.cursor < 1 then
			self.cursor = 1
		end
		if self.cursor < self.scroll_top then
			self.scroll_top = self.cursor
		elseif self.cursor >= self.scroll_top + self.items.page_lines then
			self.scroll_top = self.cursor - self.items.page_lines + 1
		end
	end
	function self.on:paint()
		self:draw_line(0, self.items[0], "title")
		for i = -self.items.help_lines, -1 do
			self:draw_line(20 + i, self.items[i], "help")
		end
		for i = 1, 19 - self.items.help_lines do
			local j = self.scroll_top + i - 1
			local style = "normal"
			if j == self.cursor then style = "selected" end
			self:draw_line(i, self.items[j], style)
		end
	end
	self.items = {help_lines = 0, page_lines = 19}
	self.scroll_top = 1
	self.cursor = 1
end

function Scene:draw_line(i, line, style)
	if line then
		line:draw(i, style)
	else
		Line.styles.null.draw_background(i)
	end
end

function Scene:provide_help(str)
	local _, count = str:gsub("\n", "\n")
	count = count + 1
	self.items.help_lines = count
	self.items.page_lines = 19 - count
	local i = 0
	for text in str:gmatch("[^\n]+") do
		i = i + 1
		self.items[-count + i - 1] = Line.new(text)
	end
end
