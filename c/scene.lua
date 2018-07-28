-- Scene_Base doesn't really make sense here.
-- Neither does Scene_Selectable because all scenes have a selectable interface.
Scene = {}

function Scene.new()
	local self = Object.new()
	self.is_scene = true

	self.items = {}
	self.help = {}
	self.cursor = 1
	self.column_count = 1
	self.scroll_top_row = 1

	function self.get_page_row_count()
		return 19 - #self.help
	end

	function self.get_page_item_count()
		return self.column_count * self.get_page_row_count()
	end

	function self.get_cursor_row()
		return math.ceil(self.cursor / self.column_count)
	end

	function self.get_scroll_top_item()
		return (self.scroll_top_row - 1) * self.column_count + 1
	end

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
		if Input.repeated(79) then n = n + 1 end
		if Input.repeated(80) then n = n - 1 end
		if Input.repeated(81) and n <= #self.items - self.column_count then
			n = n + self.column_count
		end
		if Input.repeated(82) and n > self.column_count then
			n = n - self.column_count
		end
		if Input.repeated(75) then n = n - self.get_page_item_count() end
		if Input.repeated(78) then n = n + self.get_page_item_count() end
		if Input.repeated(74) then n = 1 end
		if Input.repeated(77) then n = #self.items end
		if n > #self.items then
			n = #self.items
		elseif n < 1 then
			n = 1
		end
		self.cursor_changed = self.cursor ~= n
		self.cursor = n
		if self.cursor < self.get_scroll_top_item() then
			self.scroll_top_row = self.get_cursor_row()
		elseif self.cursor >= self.get_scroll_top_item() + self.get_page_item_count() then
			self.scroll_top = self.get_cursor_row() - self.get_page_row_count() + 1
		end

		local item = self.items[self.cursor]
		if item.update then
			item.update()
		end
	end

	function self.on_paint()
		self.draw_title_item()
		for i = 1, #self.help do
			self.draw_help_item(i)
		end
		for i = self.get_scroll_top_item(), self.get_scroll_top_item() + self.get_page_item_count() - 1 do
			local style = "normal"
			if i == self.cursor then style = "selected" end
			self.draw_item(i, style)
		end
	end

	function self.draw_title_item()
		Item.draw(self.title, self.get_title_item_rect(), "title")
	end

	function self.get_title_item_rect()
		return {x = 0, y = 0, w = 640, h = WLH}
	end

	function self.draw_item(i, style)
		local item = self.items[i]
		local rect = self.get_item_rect(i)
		if item then
			item.draw(rect, style)
		else
			Item.styles["null"].draw_background(rect)
		end
	end

	function self.get_item_rect(i)
		local w = 640 / self.column_count
		return {
			x = ((i - 1) % self.column_count) * w,
			y = (math.ceil(i / self.column_count) - self.scroll_top_row + 1) * WLH,
			w = w,
			h = WLH
		}
	end

	function self.draw_help_item(i)
		local help = self.items[self.cursor].help or self.help
		Item.draw(help[i], self.get_help_item_rect(i), "help")
	end

	function self.get_help_item_rect(i)
		return {
			x = 0,
			y = 480 - WLH * (#self.help + i - 1),
			w = 640,
			h = WLH
		}
	end

	return self
end
