Widget = {}

-- WLH = Window Line Height
WLH = 24

function Widget.new()
	local self = {}
	self.rect = {x = 0, y = 0, w = 100, h = 100}
	self.margin = {t = 3, r = 3, b = 3, l = 3}
	self.padding = {t = 1, r = 1, b = 1, l = 1}

	function self.update()
	end

	function self.draw()
		local rect = self.rect
		Graphics.draw_rect(rect)
		Graphics.draw_line(
			rect,
			{x = rect.x + rect.w, y = rect.y + rect.h}
		)
		Graphics.draw_line(
			{x = rect.x, y = rect.y + rect.h},
			{x = rect.x + rect.w, y = rect.y}
		)
	end

	return self
end
