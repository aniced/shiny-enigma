local class = Util.class()
-- Scene_Base doesn't really make sense here.
-- Neither does Scene_Selectable because all scenes have a selectable interface.

function class:init()
	self.update = coroutine.wrap(function ()
		while true do
			self:update_general()
			self:update_cursor()
			if self.on_update then self:on_update() end
			coroutine.yield()
		end
	end)
	self.items = {help_lines = 0, page_lines = 19}
	self.scroll_top = 1
	self.cursor = 1
end

function class:update_general()
	if Input.action() then
		Graphics.stop_transition()
		-- the update will continue processing the input
	end
end

function class:update_cursor()
	if #self.items == 0 then
		-- nothing to scroll
		return
	end
	local n = self.cursor -- new cursor index
	if Input.repeated(81) then n = n + 1 end
	if Input.repeated(82) then n = n - 1 end
	if Input.repeated(75) then n = n - 10 end
	if Input.repeated(78) then n = n + 10 end
	if Input.repeated(74) then n = 1 end
	if Input.repeated(77) then n = #self.items end
	if n > #self.items then
		n = #self.items
	elseif n < 1 then
		n = 1
	end
	self.cursor_changed = self.cursor ~= n
	self.cursor = n
	if self.cursor < self.scroll_top then
		self.scroll_top = self.cursor
	elseif self.cursor >= self.scroll_top + self.items.page_lines then
		self.scroll_top = self.cursor - self.items.page_lines + 1
	end

	local item = self.items[self.cursor]
	if item.update then
		item.update()
	end
end

function class:on_paint()
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

function class:draw_line(i, line, style)
	if line then
		line:draw(i, style)
	else
		Line.styles.null.draw_background(i)
	end
end

function class:provide_help(str)
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

return class
