Widget = Util.class()

function Widget:init()
	self.rect = {x = 0, y = 0, w = 100, h = 100}
end

function Widget:draw()
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
