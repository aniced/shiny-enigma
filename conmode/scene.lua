-- Scene_Base doesn't really make sense here.
-- Neither does Scene_Selectable because all scenes have a selectable interface.
Scene = {}

function Scene.new()
	local self = {}

	self.items = {}
	self.help_items = {}
	self.scroll_top = 1
	self.cursor = 1

	self.update = coroutine.wrap(function ()
		while true do
			self.update_general()
			self.update_cursor()
			if self.on_update then self.on_update() end
			coroutine.yield()
		end
	end)

	function self.update_general()
		if Input.action() then
			Graphics.stop_transition()
			-- the update will continue processing the input
		end
	end

	function self.update_cursor()
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
		elseif self.cursor >= self.scroll_top + 19 - #self.help_items then
			self.scroll_top = self.cursor - 18 + #self.help_items
		end

		local item = self.items[self.cursor]
		if item.update then
			item.update()
		end
	end

	function self.on_paint()
		self.draw_line(0, self.items[0], "title")
		local help_lines = #self.help_items
		local help_items = self.items[self.cursor].help_items or self.help_items
		for i, v in ipairs(help_items) do
			self.draw_line(19 - help_lines + i, v, "help")
		end
		for i = 1, 19 - help_lines do
			local j = self.scroll_top + i - 1
			local style = "normal"
			if j == self.cursor then style = "selected" end
			self.draw_line(i, self.items[j], style)
		end
	end

	function self.draw_line(i, line, style)
		if line then
			line.draw(i, style)
		else
			Line.styles.null.draw_background(i)
		end
	end

	return self
end
